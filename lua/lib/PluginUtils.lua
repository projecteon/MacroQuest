--- @type Mq
local mq = require('mq')

local plugins = {}

plugins.IsLoaded = function(plugin)
  return mq.TLO.Plugin(plugin).IsLoaded()
end

plugins.EnsureIsLoaded = function(plugin)
  if (plugins.IsLoaded(plugin) == false) then
    print(string.format("[PLUGINS] Loading <%s>..."), plugin)
    mq.cmd("/plugin " .. plugin)
  end
end

return plugins