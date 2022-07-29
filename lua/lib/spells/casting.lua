--- @type Mq
local mq = require('mq')
local types = require('lib/spells/types')
local state = require('lib/spells/state')
local events = require('lib/spells/events')
local log = require('lib/log')

for key, value in pairs(events) do
  value:Register()
end

local castReturnTypes = types.CastReturn

local function doCastEvents()
  for key, value in pairs(events) do
    value:DoEvent()
  end
end

local function flushCastEvents()
  for key, value in pairs(events) do
    value:Flush()
  end
end

local function interrupt() 
  log.WriteLog("|- interrupt ==>")
  mq.cmd("/stopcast")
  state.castReturn = castReturnTypes.Cancelled
  mq.delay("1s", function () return not mq.TLO.Me.Casting.ID() end)
  log.WriteLog("|- interrupt <==")
  return state.castReturn
end

local function whileCasting(cancelCallback, spellId)
  log.WriteLog("|- whileCasting ==>")
  local currentTarget = mq.TLO.Target.ID()
  local currentTargetType = mq.TLO.Target.Type()
  while(mq.TLO.Me.Casting.ID()) do
    if(cancelCallback) then
      cancelCallback(spellId)
    end

    if(currentTarget and mq.TLO.Spawn(currentTarget).Type() ~= currentTargetType) then
      interrupt()
    end
  end

  doCastEvents()
  log.WriteLog("|- whileCasting <==")
end

local function castAltAbility(altAbilityName, cancelCallback, maxResists)
  repeat
    if (not mq.TLO.Me.AltAbilityReady(altAbilityName)()) then
      return castReturnTypes.NotReady
    end

    mq.cmdf("/alt activate %s", mq.TLO.Me.AltAbility(altAbilityName).ID())
    mq.delay(5, function() return mq.TLO.Me.Casting.ID() or false end)
    doCastEvents()
    whileCasting(cancelCallback)
  until (state.castReturn ~= castReturnTypes.Restart and
         state.castReturn ~= castReturnTypes.Stunned and
         state.castReturn ~= castReturnTypes.Interrupted and
         (state.castReturn == castReturnTypes.Resisted and state.resistCounter > maxResists)) or 
        state.giveUpTimer:IsComplete()

  return state.castReturn
end

local function castSpell(spellName, cancelCallback, maxResists)
  log.WriteLog("|- castSpell ==>")
  local spellBookPosition = mq.TLO.Me.Book(spellName)()
  if(not spellBookPosition) then
    return castReturnTypes.Unknown
  end

  local spellId = mq.TLO.Me.Book(spellBookPosition).ID()
  local spell = mq.TLO.Spell(spellId)
  if(mq.TLO.Me.CurrentMana() < mq.TLO.Spell(spellId).Mana()) then
    return castReturnTypes.OutOfMana
  end

  local spellGemPosition = mq.TLO.Me.Gem(spellName)
  if(not spellGemPosition) then
    return castReturnTypes.NotMemmed
  end

  state.recastTime = spell.RecastTime()
  state.recoveryTime = spell.RecoveryTime()

  log.WriteLog("|- cast loop")
  repeat
    log.WriteLog("|- delay <"..state.retryTimer:TimeRemaining()..">")
    mq.delay(state.retryTimer:DelayTime(), function() return mq.TLO.Me.SpellReady(spellName)() end)
    mq.cmdf("/cast %s", spellName)
    doCastEvents()
    whileCasting(cancelCallback, spellId)

    if(state.castReturn ~= castReturnTypes.Success 
        and state.castReturn == castReturnTypes.Recover
        and state.giveUpTimer:IsComplete()) then
          log.WriteLog("|- castSpell <==")
          return castReturnTypes.NotReady
    end
  until (state.castReturn ~= castReturnTypes.Collapse and
         state.castReturn ~= castReturnTypes.Fizzle and
         state.castReturn ~= castReturnTypes.Restart and
         state.castReturn ~= castReturnTypes.Stunned and
         state.castReturn ~= castReturnTypes.Interrupted and
         (state.castReturn == castReturnTypes.Resisted and state.resistCounter > maxResists)) or
        state.retryTimer:TimeRemaining() > state.giveUpTimer:TimeRemaining() or 
        state.giveUpTimer:IsComplete()
  
  log.WriteLog("|- castSpell <==")
  return state.castReturn
end

local function cast(spellName, cancelCallback, giveUpTimer, maxResists)
  log.WriteLog("|- cast ==>")
  flushCastEvents()
  state.Reset(giveUpTimer)

  if (mq.TLO.Window("SpellBookWnd").Open()) then
    mq.cmd("/keypress spellbook")
  end

  if (mq.TLO.Me.Ducking()) then
    mq.cmd("/keypress duck")
  end
  
  if(mq.TLO.Me.AltAbility(spellName)()) then
    state.castReturn = castAltAbility(spellName)
  elseif (mq.TLO.Me.Book(spellName)()) then
    state.castReturn = castSpell(spellName, cancelCallback, maxResists)
  end

  log.WriteLog("|- cast <==")
  return state.castReturn
end

return function() return cast, interrupt end