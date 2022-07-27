--- @type Mq
local mq = require('mq')

local plugins = {}

plugins.IsPluginLoaded = function(plugin)
  return mq.TLO.Plugin(plugin).IsLoaded()
end

plugins.EnsurePluginLoaded = function(plugin)
  if (plugins.IsPluginLoaded(plugin) == false) then
    print(string.format("[PLUGINS] Loading <%s>..."), plugin)
    mq.cmd("/plugin " .. plugin)
  end
end

return plugins