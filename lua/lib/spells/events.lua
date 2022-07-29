--- @type Mq
local mq = require('mq')
local types = require('lib/spells/types')
local state = require('lib/spells/state')
local mqEvents = require('lib/mqevent')
local timer = require('lib/timer')

local castReturnTypes = types.CastReturn

local function beginCastEvent()
  state.castReturn = castReturnTypes.Success
end

local function collapseEvent()
  state.giveUpTimer = timer:new(200)
  state.castReturn = castReturnTypes.Collapse
end

local function feignDeathFailedEvent(line, name)
  if(mq.TLO.Me.Name() == name) then
    if(mq.TLO.Me.Standing()) then 
      mq.cmd("/stand")
    end

    state.castReturn = castReturnTypes.Restart
  end
end

local function fizzleEvent()
  local fizzleTime = math.ceil(state.recoveryTime/1000)
  state.retryTimer = timer:new(fizzleTime)
  state.castReturn = castReturnTypes.Fizzle
end

local function immuneEvent()
  local recastTime = math.ceil(state.recastTime/1000)
  state.retryTimer = timer:new(recastTime)
  state.castReturn = castReturnTypes.Immune
end

local function interruptedEvent()
  state.castReturn = castReturnTypes.Interrupted
end

local function noHoldEvent()
  state.spellNotHold = true
end

local function cannotSeeEvent()
  state.castReturn = castReturnTypes.CannotSee
end

local function noTargetEvent()
  state.castReturn = castReturnTypes.NoTarget
end

local function notReadyEvent()
  state.castReturn = castReturnTypes.NotReady
end

local function outOfManaEvent()
  state.castReturn = castReturnTypes.OutOfMana
end

local function outOfRangeEvent()
  state.castReturn = castReturnTypes.OutOfRange
end

local function recoverEvent()
  local recastTime = math.ceil(state.recastTime/1000)
  state.retryTimer = timer:new(recastTime)
  state.castReturn = castReturnTypes.Recover
end

local function resistEvent(line, spellname)
  local recastTime = math.ceil(state.recastTime/1000)
  state.retryTimer = timer:new(recastTime)
  state.resistCounter = state.resistCounter + 1
  state.castReturn = castReturnTypes.Cast_Resisted
end

local function standEvent()
  mq.cmd("/stand")
  state.castReturn = castReturnTypes.Cast_Restart
end

local function stunnedEvent()
  if (mq.TLO.Me.Stunned()) then
    mq.delay('3s', function() return mq.TLO.Me.Stunned() == false end)
  else
    mq.delay(7)
  end

  state.castReturn = castReturnTypes.Cast_Stunned
  mq.flushevents("stunned")
end

local events = {
  mqEvents:new("begincast", "You begin casting#*#", beginCastEvent),
  mqEvents:new("collapse", "Your gate is too unstable, and collapses.#*#", collapseEvent),
  mqEvents:new("feignDeathFailed", "#1# has fallen to the ground.#*#", feignDeathFailedEvent),
  mqEvents:new("fizzle", "Your spell fizzles#*#", fizzleEvent),
  mqEvents:new("immuneAttackSpeed", "Your target is immune to changes in its attack speed#*#", immuneEvent),
  mqEvents:new("immuneRunSpeed", "Your target is immune to changes in its run speed#*#", immuneEvent),
  mqEvents:new("immuneMezmerize", "Your target cannot be mesmerized#*#", immuneEvent),
  mqEvents:new("interruptCasting", "Your casting has been interrupted#*#", interruptedEvent),
  mqEvents:new("interruptSpell", "Your spell is interrupted#*#", interruptedEvent),
  mqEvents:new("noHoldSpellDid", "Your spell did not take hold#*#", noHoldEvent),
  mqEvents:new("noHoldSpellWould", "Your spell would not have taken hold#*#", noHoldEvent),
  mqEvents:new("noHoldNoGroupTarget", "You must first target a group member#*#", noHoldEvent),
  mqEvents:new("noHoldToPowerfull", "Your spell is too powerful for your intended target#*#", noHoldEvent),
  mqEvents:new("noLineOfSight", "You cannot see your target.#*#", cannotSeeEvent),
  mqEvents:new("noTarget", "You must first select a target for this spell!#*#", noTargetEvent),
  mqEvents:new("notReady", "Spell recast time not yet met.#*#", notReadyEvent),
  mqEvents:new("outOfMana", "Insufficient Mana to cast this spell!#*#", outOfManaEvent),
  mqEvents:new("outOfRange", "Your target is out of range, get closer!#*#", outOfRangeEvent),
  mqEvents:new("recoverYou", "You haven't recovered yet...#*#", recoverEvent),
  mqEvents:new("recoverSpell", "Spell recovery time not yet met#*#", recoverEvent),
  mqEvents:new("resistTarget", "Your target resisted the #1# spell#*#", resistEvent),
  mqEvents:new("standing", "You must be standing to cast a spell#*#", standEvent),
  mqEvents:new("stunned", "You are stunned#*#", stunnedEvent),
  mqEvents:new("stunnedCast", "You can't cast spells while stunned!#*#", stunnedEvent),
  mqEvents:new("stunnedSilenced", "You *CANNOT* cast spells, you have been silenced!#*#", stunnedEvent),
}

return events