local mq = require('mq')
local luautils = require('lib/luautils')

-- Make includes easier
local scriptPath = (debug.getinfo(1, "S").source:sub(2)):match("(.*[\\|/]).*$")
package.path = package.path .. ';' .. scriptPath .. '?.lua'
local resourcePath = mq.TLO.MacroQuest.Path('Resources')()
local luasharePath = resourcePath .. "\\luarocks\\share\\lua\\5.1\\"
local lualibPath = resourcePath .. "\\luarocks\\lib\\lua\\5.1\\"
package.path = package.path .. ';' .. luasharePath .. '?.lua'
package.cpath = package.cpath .. ';' .. lualibPath .. '?.dll'
-- End include section

local json = require "cjson"
local util = require "cjson.util"

local luaDir = mq.TLO.Lua.Dir
local serverName = mq.TLO.MacroQuest.Server
local fileName = mq.TLO.Me.Name()
local configFilePath = string.format("%s/config/%s/%s.json", luaDir, serverName, fileName)

luautils.EnsureFileExists(configFilePath)

local function tableMerge(default, loaded)
  for key, value in pairs(default) do
    local loadedValue = loaded[key]
    if type(value) == "table" then
        if type(loadedValue or false) == "table" then
            tableMerge(default[key] or {}, loadedValue or {})
        end
    elseif type(value) == type(loadedValue) then
        default[key] = loadedValue
    end
  end

  return default
end

local function LoadConfig(key, default)
  local json_text = util.file_load(configFilePath)
  local loadedConfig = json.decode(json_text)
  return tableMerge(default, loadedConfig[key] or {})
end

local function SaveConfig(config)
  local json_text = json.encode(config)
  util.file_save(configFilePath, json_text)
end

return function () return LoadConfig, SaveConfig end