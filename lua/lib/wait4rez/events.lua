--- @type Mq
local mq = require('mq')
local log = require('lib/log')
local mqEvents = require('lib/mqevent')
local doWait4Rez, doLoot = require('lib/wait4rez/wait4rez')()

local function diedEvent()
	log.WriteLog("|- diedEvent ==>")
  
  mq.cmd("/beep")
  doWait4Rez()

  log.WriteLog("|- diedEvent <==")
end

local function lootEvent(line, chatSender)
	log.WriteLog("|- lootEvent ==>")
  
  doLoot()

  log.WriteLog("|- lootEvent <==")
end

local events = {
  mqEvents:new("slain", "You have been slain by #*#", diedEvent),
  mqEvents:new("died", "You died.", diedEvent),
  mqEvents:new("loot1", "[#1#] Loot Now", lootEvent),
  mqEvents:new("loot2", "<#1#> Loot Now#*#", lootEvent),
  mqEvents:new("loot3", "#1# tells you, 'Loot Now#*#", lootEvent),
  mqEvents:new("loot4", "#1# tells you, in #*#, 'Loot Now#*#", lootEvent),
}

for key, value in pairs(events) do
  value:Register()
end

local function doEvents()
  for key, value in pairs(events) do
    value:DoEvent()
  end
end

return doEvents