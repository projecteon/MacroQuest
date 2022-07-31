--- @type Mq
local mq = require('mq')
local log = require('lib/log')
local plugin = require('lib/pluginutils')
local moveUtils = require('lib/moveutils')
local ensureTarget = require('lib/target')

local function lootNearestCorpse()
	log.WriteLog("|- lootNearestCorpse ==>")

  local startX = mq.TLO.Me.X()
  local startY = mq.TLO.Me.Y()
  local isTwisting = plugin.IsLoaded("mq2twist") and mq.TLO.Twist.Twisting()
  if isTwisting then
    mq.cmd("/twist stop")
    mq.delay(20, function() return not mq.TLO.Me.Casting.ID() end)
  end

  if not mq.TLO.Me.Casting.ID() then
    local seekRadius = 50
    local searchCorpseString = string.format("npc corpse zradius 50 radius %s", seekRadius)
    local closestCorpseID = mq.TLO.NearestSpawn(1, searchCorpseString).ID()
    if mq.TLO.Spawn(closestCorpseID) and ensureTarget(closestCorpseID) then
      local target = mq.TLO.Target
      if target.Distance() > 16 and target.DistanceZ() < 80 then
        moveUtils.MoveToLoc(target.Y, target.X(), 20, 12)
      end
      if target.Distance() <= 20 and target.DistanceZ() < 40 then
        log.WriteLog("|- LOOT TARGET")
      else
        local logText = string.format("|- Corpse %s is %s|%s distance, skipping", target.Distance, target.DistanceZ)
        log.WriteLog("")
      end
    else
      log.WriteLog("|- Unable to locate or target corpse id <"..closestCorpseID..">")
    end

  else
    log.WriteLog("|- Unable to loot corpse, currently casting.")
  end 

  log.WriteLog("|- lootNearestCorpse <==")
end

local function markItemForDestroying()
end

local function markItemForSelling()
end

local function createAliases()
  mq.bind("/setdestroyitem", markItemForDestroying)
  mq.bind("/setsellitem", markItemForSelling)
  mq.bind("/doloot", lootNearestCorpse)
end

createAliases()