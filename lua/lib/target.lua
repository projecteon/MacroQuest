--- @type Mq
local mq = require('mq')
local log = require('lib/log')

local function ensureTarget(targetId)
	log.WriteLog("|- ensureTarget ==>")

  if mq.TLO.Target.ID() ~= targetId then
    if(mq.TLO.SpawnCount("id "..targetId)) then
      mq.cmdf("/target id %s", targetId)
      mq.delay("3s", function() return mq.TLO.Target.ID() == targetId end)
    else
      print("EnsureTarget has no spawncount for target id " .. targetId)
    end
  end

  log.WriteLog("|- ensureTarget <==")
  return mq.TLO.Target.ID() == targetId
end

return ensureTarget