| http://mqemulator.net/forum2/viewtopic.php?t=1551
| http://mqemulator.net/forum2/viewtopic.php?t=794

#include includes\spell_routines.inc
#include includes\commonCaster.inc
#turbo

Sub InitNukeIni(string fileName)
    /declare nukeOptions        string  outer "NukeOptions"
    /declare mainNuke           string  outer 
    /declare undeadNuke         string  outer 
    /declare nukePct            int     outer 97
    /declare casterInitResult   int     local 1

    /call InitCasterIni "${fileName}"
    /varset casterInitResult ${Macro.Return}

    /if (${Ini[${fileName},${nukeOptions},MainNuke].Length}) {
        /varset mainNuke "${Ini[${fileName},${nukeOptions},MainNuke]}"
    } else {
        /ini "${fileName}" "${nukeOptions}" "MainNuke" "${mainNuke}"
        /varset casterInitResult 0
    }

    /if (${Ini[${fileName},${nukeOptions},NukePct].Length}) {
        /varset nukePct ${Ini[${fileName},${nukeOptions},NukePct]}       
    } else {
        /ini "${fileName}" "${nukeOptions}" "NukePct" "${nukePct}"
        /varset casterInitResult 0
    }

    /if (${Ini[${fileName},${nukeOptions},UndeadNuke].Length}) {
       /varset undeadNuke ${Ini[${fileName},${nukeOptions},UndeadNuke]}       
    }
/return ${casterInitResult}

Sub Nuke
    /if (${Me.Casting.ID} || ${Me.Moving}) {
        /return
    }

    /declare targetId int local ${NetBots[${mainAssist}].TargetID}
    /if (!${targetId}) {
        /return
    }
    
    /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
    /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
    /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
    /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

    /if ((!${isNPC} && !${isPet}) || ${targetHP} > ${nukePct} || !${hasLineOfSight}) {
        /return
    }

    /target id ${targetId}
    /if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}!=${targetId}) {
        /return
    }
    
    /if (${Select[${Target.Body},Undead]} && ${Me.Gem[${undeadNuke}]}) {
        /call TryNukeTarget "${undeadNuke}"
    } else {
        /call TryNukeTarget ${mainNuke}
    }
/return

Sub TryNukeTarget(string nuke)
    /if (${Me.Sitting}) {
        /stand
    }

    /if (!${Cast.Ready[${nuke}]} || ${Me.CurrentMana} < ${Spell[${nuke}].Mana} || ${Target.Distance} > ${Spell[${nuke}].Range}) {
        /return
    }
    
    /call cast "${nuke}" gem${Me.Gem[${nuke}]} 0 CancelNuke
/return

| #################################################
| # Called during the cast of any spell and will
| # interuppt it if any condition pops true.
| #################################################
Sub CancelNuke
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
	| /doevents StartCast
/return

Sub MakeNukeIni(string fileName)
    |- Build the INI file
    /echo Building INI file.....

    /ini "${fileName}" "${nukeOptions}" "NukePct" "${nukePct}"
    /ini "${fileName}" "${nukeOptions}" "MainNuke" "${mainNuke}"
    /ini "${fileName}" "${nukeOptions}" "UndeadNuke" "${undeadNuke}"
    /ini "${fileName}" "${nukeOptions}" "NukePct" "${nukePct}"
/return
	