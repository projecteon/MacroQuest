| http://mqemulator.net/forum2/viewtopic.php?t=1551
| http://mqemulator.net/forum2/viewtopic.php?t=794

#include spell_routines.inc
#turbo

Sub NukeSetup(string fileName)
    /declare nukeOptions        string  outer "NukeOptions"
    /declare mainNuke           string  outer "YOUR MAIN NUKE"
    /declare secondaryNuke      string  outer "YOUR SECONDARY NUKE"
    /declare nukePct            int     outer 97
    /declare nocast             string  outer 1

    /if (${Ini[${fileName},${nukeOptions},MainNuke].Length}) {
       /varset mainNuke "${Ini[${fileName},${nukeOptions},MainNuke]}"
    }

    /if (${Ini[${fileName},${nukeOptions},SecondaryNuke].Length}) {
       /varset secondaryNuke "${Ini[${fileName},${nukeOptions},SecondaryNuke]}"       
    }

    /if (${Ini[${fileName},${nukeOptions},NukePct].Length}) {
       /varset nukePct ${Ini[${fileName},${nukeOptions},NukePct]}       
    }

    /if (!${Ini[${fileName},${nukeOptions},MainNuke].Length}) {
        /call MakeNukeIni ${fileName}      
    }
/return

Sub Nuke(mainAssist)
    /if (${Me.Casting.ID} || ${Me.Moving}) {
        /return
    }

    /declare targetId int local ${NetBots[${mainAssist}].TargetID}
    /if (!${targetId}) {
        /return
    }

    /target id ${targetId}

    /if (!${Target.ID} || !${Target.Type.Equal[NPC]} || ${Target.PctHPs} > ${nukePct}) {
        /return
    }

    /if (${Target.ID}==${Me.ID}) {
        /squelch 
        /target clear 
        /return
    }

    /call TryNukeTarget ${mainNuke}
    /if (${Macro.Return} == ${nocast}) {
        /call TryNukeTarget ${secondaryNuke}
    }
        
/return

Sub TryNukeTarget(nuke)
    /if (${Me.Sitting}) {
        /stand
    }

    /if (!${Cast.Ready[${nuke}]} || ${Me.CurrentMana} < ${Spell[${nuke}].Mana} || ${Target.Distance} > ${Spell[${nuke}].Range}) {
        /return ${nocast}
    }
    
    /cast "${nuke}"
/return 0


Sub MakeNukeIni(string fileName)
    |- Build the INI file
    /echo Building INI file.....

    /ini "${fileName}" "${nukeOptions}" "MainNuke" "${mainNuke}"
    /ini "${fileName}" "${nukeOptions}" "SecondaryNuke" "${secondaryNuke}"
    /ini "${fileName}" "${nukeOptions}" "NukePct" "${nukePct}"
/return
	