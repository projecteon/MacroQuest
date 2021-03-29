| http://mqemulator.net/forum2/viewtopic.php?t=1551
| http://mqemulator.net/forum2/viewtopic.php?t=794

#include spell_routines.inc
#turbo

Sub NukeSetup(string fileName)
    /declare nukeOptions        string  outer "NukeOptions"
    /declare mainNuke           string  outer "YOUR MAIN NUKE"
    /declare secondaryNuke      string  outer "YOUR SECONDARY NUKE"
    /declare undeadNuke         string  outer "YOUR UNDEAD NUKE"
    /declare nukePct            int     outer 97
    /declare stopNukePct        int     outer 0
    /declare nocast             string  outer 1

    /if (${Ini[${fileName},${nukeOptions},MainNuke].Length}) {
       /varset mainNuke "${Ini[${fileName},${nukeOptions},MainNuke]}"
    }

    /if (${Ini[${fileName},${nukeOptions},SecondaryNuke].Length}) {
       /varset secondaryNuke "${Ini[${fileName},${nukeOptions},SecondaryNuke]}"       
    }

    /if (${Ini[${fileName},${nukeOptions},SecondaryNuke].Length}) {
       /varset secondaryNuke "${Ini[${fileName},${nukeOptions},SecondaryNuke]}"       
    }

    /if (${Ini[${fileName},${nukeOptions},UndeadNuke].Length}) {
       /varset undeadNuke ${Ini[${fileName},${nukeOptions},UndeadNuke]}       
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
        /squelch /target clear 
        /return
    }

    /echo ${Target.Body}
    /if (${Select[${Target.Body},Undead]} && !${Cast.Ready[${undeadNuke}]}) {
        /call TryNukeTarget ${undeadNuke}
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
    
    /call cast "${nuke}" gem${Me.Gem[${nuke}]} 0 CancelNuke
/return 0

| #################################################
| # Called during the cast of any spell and will
| # interuppt it if any condition pops true.
| #################################################
Sub CancelNuke
	/if (${Target.Distance} > ${Spell[${nuke}].Range}) /call Interrupt
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
	| /if (${Me.AltAbilityReady[Divine Arbitration]}) /call DivineArbitration
	| /doevents StartCast
/return

Sub MakeNukeIni(string fileName)
    |- Build the INI file
    /echo Building INI file.....

    /ini "${fileName}" "${nukeOptions}" "MainNuke" "${mainNuke}"
    /ini "${fileName}" "${nukeOptions}" "SecondaryNuke" "${secondaryNuke}"
    /ini "${fileName}" "${nukeOptions}" "NukePct" "${nukePct}"
/return
	