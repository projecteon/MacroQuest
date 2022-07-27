-- /lua run examples/uisample
local mq = require 'mq'
require 'ImGui'
require 'mq'
mq.cmd("/bccmd connect")
-- GUI Control variables
local openGUI = true
local shouldDrawGUI = true
local terminate = false
local AlphaBool = true

-- ImGui main function for rendering the UI window
local uisample = function()
    -- Window Alpha Settings on Hover 
    if AlphaBool then
        ImGui.SetNextWindowBgAlpha(1) else ImGui.SetNextWindowBgAlpha(.1)
    end
    -- ImGui UI Window Startup Must call ImGui.End()
    openGUI, shouldDrawGUI = ImGui.Begin('My First UI', openGUI)
    -- This Is For Refreshing Client on Window Crash when you are editing ImGui
    mq.cmd("/mqoverlay resume")
    if shouldDrawGUI then
        -- Window Alpha Settings on Hover 
        if ImGui.IsWindowHovered() then
            AlphaBool = true else AlphaBool = false
        end
        -- Setup button in a row
        if ImGui.Button("Stay", 42, 20) then mq.cmd('/bc Stay') end
        ImGui.SameLine()
        ImGui.SetCursorPosX(57)
        if ImGui.Button("OTM", 42, 20) then mq.cmd('/bc Follow Panther') end
        ImGui.SameLine()
        ImGui.SetCursorPosX(105)
        if ImGui.Button("Buff", 42, 20) then mq.cmd("/g Lets Buff") end
        ImGui.SameLine()
        ImGui.SetCursorPosX(167)
        if ImGui.Button("Button4", 60, 20) then mq.cmd("/g test me") end

        
        -- Get HPs of Target
        local pctHPs = tonumber(mq.TLO.Target.PctHPs()) or 0
        local ratioHPs = pctHPs / 100
        local targetName = mq.TLO.Target.DisplayName() or 'No Target'
        local targetLvl = mq.TLO.Target.Level() or '00'
        local targetDist = mq.TLO.Target.Distance3D.Int() or '00'
        local targetClass = mq.TLO.Target.Class.ShortName() or 'NULL'
        -- Setup Progress Bar
        ImGui.PushStyleColor(ImGuiCol.PlotHistogram, 1 - ratioHPs, ratioHPs-.5, .5, 1)
        ImGui.PushStyleColor(ImGuiCol.Text, 0, 0, 0, 1)
        -- Create Color Changing Bar for Target health 
        ImGui.ProgressBar(ratioHPs, 150, 20, targetName..': '..pctHPs..'%')
        ImGui.PopStyleColor(2)
        ImGui.SetWindowFontScale(.9)
        ImGui.Text("LVL "..targetLvl.." Class: "..targetClass.." Dist "..targetDist)
        ImGui.SetWindowFontScale(1)

        ImGui.Text("")
        -- For loop for displaying Members health and Mana 
        for i=1,mq.TLO.Raid.Members() do
            -- local pctHPs = i*20 -- testing
            -- Get pctHP and Mana using tonumber force to a number Stops Crash from NULL or Nil
            local pctHPs = tonumber(mq.TLO.NetBots(mq.TLO.Raid.Member(i).Name()).PctHPs()) or 0
            local pctMana = tonumber(mq.TLO.NetBots(mq.TLO.Raid.Member(i).Name()).PctMana()) or 0
            local ratioHPs = pctHPs / 100
            local ratioMana = pctMana / 100
            local targetName = mq.TLO.Raid.Member(i).Name() or 'No Target'
            local InZone = 0
            if mq.TLO.NetBots(mq.TLO.Raid.Member(i).Name()).InZone() then InZone = 0 else
                InZone = .3
            end
            -- local targetName = mq.TLO.Group.Member(i).Name()
            -- Setup Health Bars and %
            ImGui.SetCursorPosY(i*30+75-i*5)
            ImGui.SetWindowFontScale(1)
            ImGui.PushStyleColor(ImGuiCol.PlotHistogram, 1 - ratioHPs - InZone, ratioHPs-.4 - InZone, .2 - InZone, 1)
            ImGui.PushStyleColor(ImGuiCol.Text, 0.8, 0.8, 0.8, 1)
            ImGui.ProgressBar(ratioHPs, 150, 15, targetName..': '..pctHPs..'%')
            -- Target Player if clicked on 
            if ImGui.IsItemHovered() and ImGui.IsMouseReleased(0) then
                mq.cmdf("/target %s",mq.TLO.Raid.Member(i).Name())
            end
            ImGui.PopStyleColor(2)
            -- Setup Mana Bars and %
            ImGui.SetCursorPosY(i*30+90-i*5)
            ImGui.SetWindowFontScale(.4)
            ImGui.PushStyleColor(ImGuiCol.PlotHistogram, .2 - InZone, .2 - InZone, ratioMana - InZone, 1)
            ImGui.PushStyleColor(ImGuiCol.Text, 1, 1, 1, .5)
            ImGui.ProgressBar(ratioMana, 150, 5)
            -- Target Player if clicked on s 
            if ImGui.IsItemHovered() and ImGui.IsMouseReleased(0) then
                mq.cmdf("/target %s",mq.TLO.Raid.Member(i).Name())
            end
            ImGui.PopStyleColor(2)
            -- Set Window Font to 1 so that the next Window is normal Text
            ImGui.SetWindowFontScale(1)
            -- This stops Screen flicker when pressing down the mouse or Release of Fade 
            if ImGui.IsAnyItemHovered() and ImGui.IsMouseDown(0) or  ImGui.IsAnyItemHovered() and ImGui.IsMouseReleased(0)then
                AlphaBool = true
            end
        end

        end
    ImGui.End()
    if not openGUI then
        terminate = true
    end
end

mq.imgui.init('uisample', uisample)

-- while loop waiting to close window 
while not terminate do
    mq.delay(100)
end

