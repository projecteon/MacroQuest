--- @type Mq
local mq = require('mq')

--- @type ImGui
require 'ImGui'

local ICON = require('icons')

local openGUI = true
local shouldDrawGUI = true

local function DrawMainWindow()
    if not openGUI then return end
    openGUI, shouldDrawGUI = ImGui.Begin('Example Icon App', openGUI)
    if shouldDrawGUI then

        for key, value in pairs(ICON) do
            ImGui.Text(string.format('%s : %s', value, key))
        end

    end
    ImGui.End()
end

mq.imgui.init('Icon Example', DrawMainWindow)

while openGUI do
    mq.delay(1000)
end