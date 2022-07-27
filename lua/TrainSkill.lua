--- @type Mq
local mq = require('mq')

local function Main()
  local ability  = Lua.Arguments

  if mq.TLO.Me.Skill(ability)() == false then
    print(string.format('You do not have the skill <%s>', ability))
    mq.exit()
  end

  if mq.TLO.Me.Ability(ability)() == false then
    print(string.format('Ability is not mapped to action button <%s>', ability))
    mq.exit()
  end

  while mq.TLO.Me.Skill(ability)() ~= mq.TLO.Skill(ability).SkillCap() do
    if mq.TLO.Me.Sneaking() then
        mq.cmd('/doability Sneak')
    end

    if mq.TLO.Me.AbilityReady(ability)() then
        mq.cmdf('/doability "%s"', ability)
        mq.delay(2)
    end

    if mq.TLO.Cursor.ID() then
        mq.cmd('/autoinv')
    end

    if mq.TLO.Me.Feigning() then
        mq.cmd('/stand')
    end
  end

  print(string.format('You maxed the skill <%s>', ability))
end

Main()