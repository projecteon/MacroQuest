| http://mqemulator.net/forum2/viewtopic.php?t=1551
| http://mqemulator.net/forum2/viewtopic.php?t=794

#include spell_routines.inc
#include commonCaster.inc
#turbo

Sub InitNukeIni(string fileName)
     /declare nukeOptions        string  outer "NukeOptions"
    /declare mainNuke           string  outer "YOUR MAIN NUKE"
    /declare secondaryNuke      string  outer "YOUR SECONDARY NUKE"
    /declare undeadNuke         string  outer "YOUR UNDEAD NUKE"
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

    /if (${Ini[${fileName},${nukeOptions},SecondaryNuke].Length}) {
        /varset secondaryNuke "${Ini[${fileName},${nukeOptions},SecondaryNuke]}"       
    } else {
        /ini "${fileName}" "${nukeOptions}" "SecondaryNuke" "${secondaryNuke}"
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

    /target id ${targetId}

    /if (!${Target.ID} || !${Target.Type.Equal[NPC]} || ${Target.PctHPs} > ${nukePct} || !${Target.LineOfSight}) {
        /return
    }

    /if (${Target.ID}==${Me.ID}) {
        /squelch 
        /target clear 
        /return
    }
    
    /if (${Select[${Target.Body},Undead]} && ${Cast.Ready[${undeadNuke}]}) {
        /call TryNukeTarget "${undeadNuke}"
    } else {
        /if (!${Defined[_nocast]}) /declare _nocast int   outer 1 
        /call TryNukeTarget ${mainNuke}
        /if (${Macro.Return} == ${nocast}) {
            /call TryNukeTarget ${secondaryNuke}
        }
    }
/return

Sub TryNukeTarget(string nuke)
    /if (${Me.Sitting}) {
        /stand
    }

    /if (!${Cast.Ready[${nuke}]} || ${Me.CurrentMana} < ${Spell[${nuke}].Mana} || ${Target.Distance} > ${Spell[${nuke}].Range}) {
        /return ${nocast}
    }
    
    /call cast "${nuke}" gem${Me.Gem[${nuke}]} 0 CancelNuke
/return 0

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

    /ini "${fileName}" "${nukeOptions}" "MainNuke" "${mainNuke}"
    /ini "${fileName}" "${nukeOptions}" "SecondaryNuke" "${secondaryNuke}"
    /ini "${fileName}" "${nukeOptions}" "UndeadNuke" "${undeadNuke}"
    /ini "${fileName}" "${nukeOptions}" "NukePct" "${nukePct}"
/return
	