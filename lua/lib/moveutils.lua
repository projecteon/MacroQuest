--- @type Mq
local mq = require('mq')
local log = require('lib/log')
local plugins = require('lib/pluginutils')
local timer = require('lib/timer')

local function arrivedAtDestination(xLoc, yLoc, distanceDelta)
  return mq.TLO.Math.Distance(xLoc, yLoc)() <= distanceDelta
end

local moveUtils = {}

moveUtils.MoveToLoc = function(xLoc, yLoc, maxTime, arrivalDist)
  log.WriteLog("|- MoveToLoc ==>")
  if(plugins.IsLoaded("mq2moveutils") == false) then
    log.WriteLog("MoveUtils not loaded, cannot move to loc.")
    return
  end

  if(not xLoc or not yLoc) then
    local logText = string.Format("Cannot move to location <x:%s> <y:%s>", xLoc, yLoc)
    log.WriteLog(logText)
    return
  end

  local distanceDelta = arrivalDist or 10
  local maxTryTime = maxTime or 3

  log.WriteLog(string.format("|- Distance to loc <x:%s> <y:%s> => <%s>", xLoc, yLoc, mq.TLO.Math.Distance(xLoc, yLoc)()))
  if arrivedAtDestination(xLoc, yLoc, distanceDelta) then
    return
  end

  if mq.TLO.Me.Casting.ID() and mq.TLO.Me.Class.ShortName()() ~= "BRD" then
    mq.cmd("/stopcast")
  end

  local timeOut = timer:new(maxTryTime)

  local moveCmd = string.format("/moveto loc %s %s dist %s", xLoc, yLoc, distanceDelta)
  while not arrivedAtDestination(xLoc, yLoc, distanceDelta) and timeOut:IsRunning() do
    mq.cmd(moveCmd)
    mq.delay(maxTryTime * 1000 / 5, function() return arrivedAtDestination(xLoc, yLoc, distanceDelta) end)
  end

  mq.cmd("/moveto off")
  log.WriteLog("|- MoveToLoc <==")
end

return moveUtils