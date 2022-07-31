--- @type Mq
local mq = require('mq')
local log = require('lib/log')
local ensureTarget = require('lib/target')

local function consentNetbots()
	log.WriteLog("|- consentNetbots ==>")

  for i = 1,mq.TLO.NetBots.Counts(),1 do 
    mq.delay(2)
    mq.cmdf("/consent %s", mq.TLO.NetBots.Client[i]())
  end

  log.WriteLog("|- consentNetbots <==")
end

local function waitToZone()
	log.WriteLog("|- waitToZone ==>")
  
  local me = mq.TLO.Me.Name()
  repeat
    mq.delay(100)
  until mq.TLO.Spawn(me.."'s").ID()

  mq.delay(500)

  log.WriteLog("|- waitToZone <==")
end

local function doLoot()
	log.WriteLog("|- doLoot ==>")

  local me = mq.TLO.Me.Name()
  if ensureTarget(mq.TLO.Spawn(me.."'s").ID()) then
    log.WriteLog("|- Target distance <"..mq.TLO.Target.Distance()..">")
    if mq.TLO.Target.Distance() < 100 then
      while mq.TLO.Target.Distance() > 15 do
        mq.cmd("/corpse")
        mq.delay(20)
      end 
      
      mq.cmd("/loot")
      mq.delay("5s", function() return mq.TLO.Window("LootWnd") and mq.TLO.Window("LootWnd").Open() end)
      mq.delay("5s", function() return mq.TLO.Corpse.Items() ~= nil end)
      mq.delay(500)
      if not mq.TLO.Window("LootWnd") or not mq.TLO.Corpse.Items then
        log.WriteLog("|- Could not open loot window")
      else
        mq.cmd("/notify LootWnd LootAllButton leftmouseup")
        mq.delay("30s", function() return not mq.TLO.Window("LootWnd").Open() end)
        end
    end
  end

  log.WriteLog("|- doLoot <==")
end

local function  memorizeSpells()
	log.WriteLog("|- memorizeSpells ==>")

  -- mq.cmd("/loadspells default");

  log.WriteLog("|- memorizeSpells <==")
end

local function doWait4Rez()
	log.WriteLog("|- doWait4Rez ==>")

  mq.cmd("/bc Ready for rezz.")
  mq.cmd("/consent guild")
  repeat
    mq.delay(10)
  until mq.TLO.Window("ConfirmationDialogBox").Open()

  mq.cmd("/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup")
  waitToZone()
  doLoot()
  memorizeSpells()

  log.WriteLog("|- doWait4Rez <==")
end


local function createAliases()
  mq.bind("/wait4rez", doWait4Rez)
  mq.bind("/waitforrez", doWait4Rez)
  mq.bind("/dead", doWait4Rez)
  mq.bind("/lootCorpse", doLoot)
end

createAliases()

return function() return doWait4Rez, doLoot end