local mq = require 'mq'
require 'ImGui'

-- GUI Control variables
local openGUI = true
local shouldDrawGUI = true
local terminate = false

-- ImGui main function for rendering the UI window
local uisample = function()
    openGUI, shouldDrawGUI = ImGui.Begin('Sample UI', openGUI)
    if shouldDrawGUI then
        ImGui.Text('blah')
    end
    ImGui.End()
    if not openGUI then
        terminate = true
    end
end

mq.imgui.init('uisample', uisample)

while not terminate do
    mq.delay(1000)
end


local timeCounter = os.time()
while __something__ do
    if (mq.TLO.Target.Distance() or 0) > mq.TLO.Target.MaxRangeTo() then
        -- Only print every 60 seconds
        if os.difftime(os.time(), timeCounter) > 60 then
            print(mq.TLO.Target.CleanName() .. '\ar is OOR')
            timeCounter = os.time()
        end
    else
        print(mq.TLO.Target.CleanName() .. '\ag is IR')
    end
    mq.delay(100)
end

local function isnetbots(member, check)
  local retval = mq.TLO.NetBots(member)[check]()
  return retval ~= 'NULL' and retval
end 

local netbotname = mq.TLO.Raid.Member(i).Name()
local pctHPs = isnetbots(netbotname, 'PctHPs') or 0
local pctMana = isnetbots(netbotname, 'PctMana') or 0 

local function isnetbots(member, check, nilreturn)
    local retval = mq.TLO.NetBots(member)[check]()
    if retval == nil or retval == "NULL" then
        return nilreturn
    else
        return retval
    end
end

local raidname = mq.TLO.Raid.Member(i).Name()
local pctHPs = isnetbots(raidname, 'PctHPs', 0)
local pctMana = isnetbots(raidname, 'PctMana', 0)
local targetName = isnetbots(raidname, 'Name', 'No Target')
local Attacking = isnetbots(raidname, "Attacking", false)
local Casting = isnetbots(raidname, "Casting", false)


-- /itemnotify in pack1-10 itemslot2+1 i think
-- and itemslot would be -22 to get the pack number
-- so i restack that by setting bagstart and bagend to between 1 and 10
-- so we first have to convert it to itemslot by adding 22 so the me.inventory can work with it, then when i itemnofity i substract 22 from the bag
local function restack()
  print(string.format('\agRestacking bag %d to %d', BagStart, BagEnd))
  for bag = BagStart,BagEnd do
      bag = bag+22
      for slot = 1, mq.TLO.Me.Inventory(bag).Container() do
          if mq.TLO.Me.Inventory(bag).Item(slot)() then
              mq.cmdf('/nomodkey /shiftkey /itemnotify in pack%d %d leftmouseup', bag-22, slot)
              mq.delay(500,function() return mq.TLO.Cursor() ~= nil end)
              while mq.TLO.Cursor.ID() do
                  mq.cmd('/autoinventory')
                  mq.delay(100)
              end
          end
      end
  end
  print('\agRestacking done')
end