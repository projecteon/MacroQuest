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


local events = {
  mqEvents:new("slain", "You have been slain by #*#", diedEvent),
  mqEvents:new("died", "You died.", diedEvent),
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