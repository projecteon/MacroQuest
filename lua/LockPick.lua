--- @type Mq
local mq = require('mq')
local moveUtils = require('lib/MoveUtils')

local function TryDoAbility(ability)  
    if mq.TLO.Me.Skill(ability)() == false then
        print(string.format('You do not have the skill <%s>', ability))
    end

    if mq.TLO.Me.Ability(ability)() == false then
        print(string.format('Ability is not mapped to action button <%s>', ability))
    end
  
    if mq.TLO.Me.AbilityReady(ability)() then
        mq.cmdf('/doability "%s"', ability)
        mq.delay(2)
    end
end

local function DoLockPick()
    print('|- doLockPick ==>')

    if mq.TLO.FindItemCount('=Lockpicks')() < 1 then
        print('|- No Lockpicks set found.')
        print('|- doLockPick <==')
        mq.exit()
    end

    if mq.TLO.Me.Skill('Pick Lock')() == nil then
        print('|- You do not have the skill: Pick Lock')
        print('|- doLockPick <==')
        mq.exit()
    end

    if mq.TLO.Me.Skill('Pick Lock')() == mq.TLO.Skill('Pick Lock').SkillCap() then
        print('|- You maxed the skill: Pick Lock')
        print('|- doLockPick <==')
        mq.exit()
    end

    if mq.TLO.Zone.Name() ~= "Befallen" then
        print('|- This macro only works in Befallen.')
        print('|- doLockPick <==')
        mq.exit()
    end

    if not mq.TLO.Me.Invis() then
        TryDoAbility("Hide")
        TryDoAbility("Sneak")
    end

    if not mq.TLO.Me.Invis() then
        print('|- Could not Hide.')
        print('|- doLockPick <==')
        mq.exit()
    end
    
    local maxMoveTime = 5
    -- First locked door in befallen, need min skill of 16 before starting
    -- befallen#1=10,-83.41,-267.94,-13.99,DOOR1
    mq.cmd('/doortarget id 10')
    if mq.TLO.DoorTarget.Distance() > 20 then
        moveUtils.MoveToLoc(mq.TLO.DoorTarget.Y(), mq.TLO.DoorTarget.X(), maxMoveTime, 15)
    end

    if (mq.TLO.DoorTarget.Distance() > 35) then
        local logText = string.format("|- Aborting Lockpick training: I could not moveto < 20 units of %s within %ss", mq.TLO.DoorTarget.Name(), maxMoveTime)
        print(logText)
        print('|- doLockPick <==')
        mq.exit()
    end

    -- Lockpicks - 13010
    mq.cmd(string.format("/itemnotify %s leftmouseup", mq.TLO.FindItem("=Lockpicks").InvSlot()))
    mq.delay(2000)

    if mq.TLO.Cursor.Name() ~= "Lockpicks" then
        print('|- Could not pick up <Lockpicks> on cursor.')
        print('|- doLockPick <==')
        mq.exit()
    end

    while mq.TLO.Me.Skill("Pick Lock")() ~= mq.TLO.Skill("Pick Lock").SkillCap() do
        mq.cmd("/click left door")
    end

    mq.cmd("/beep")
    print('|- You maxed the skill: Pick Lock')
    print('|- doLockPick <==')
end

DoLockPick()