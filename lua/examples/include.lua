local mq = require('mq')

-- Include helper function so we can give user friendly messages
function Include(...)
    local status, lib = pcall(require, ...)
    if(status) then
        return lib
    end
    return nil
end

local Write = Include('knightlinc/write')

if not Write then
    print('This script requires knightly\'s includes to function.  Go download them (Write.lua).')
else
    Write.prefix = '\ag[MyScript]\ax '
    local sql = Include('lsqlite3complete')
    if not sql then
        local PackageMan = Include('knightlinc/PackageMan')
        if not PackageMan then
            Write.Fatal('This script requires knightly\'s includes to function.  Go download them. (PackageMan.lua)')
        else
            sql = PackageMan.InstallAndLoad('lsqlite3complete')
        end
    end

    if not sql then
        Write.Fatal('LuaRock lsqlite3complete is required for this script and could not be installed.')
    else
        local running = true
        
        local server_char = 'unknown_unknown'

        while running do
            if mq.TLO.EverQuest.GameState() == 'INGAME' then
                if (mq.TLO.EverQuest.Server() .. '_' .. mq.TLO.Me.Name()) ~= server_char then
                    server_char = mq.TLO.EverQuest.Server() .. '_' .. mq.TLO.Me.Name()
                    -- Do schema stuff here
                end


            end
        end
    end
end

-- ttps://luarocks.macroquest.org/