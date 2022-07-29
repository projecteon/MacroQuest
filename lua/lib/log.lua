local loadConfig, saveConfig = require('lib/config')()

local logConfig = {
  printDebug = true
}

logConfig = loadConfig("log", logConfig)

local log = {
  config = logConfig
}

log.WriteLog = function(logText)
  if(logConfig.printDebug) then
    local text = string.format("[%s] %s", os.date("%X"), logText)
    print(text)
  end
end

return log