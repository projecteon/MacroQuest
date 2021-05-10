| http://mqemulator.net/forum2/viewtopic.php?t=1551
| http://mqemulator.net/forum2/viewtopic.php?t=794

#include includes\spell_routines.inc
#include includes\commonCaster.inc
#turbo

Sub InitNukeIni(string fileName)
    /declare nukeOptions        string  outer "NukeOptions"
    /declare mainNuke           string  outer 
    /declare undeadNuke         string  outer 
    /declare minNukeMana        int     outer 0
    /declare casterInitResult   int     local 1

    /call InitCasterIni "${fileName}"
    /varset casterInitResult ${Macro.Return}

    /if (${Ini[${fileName},${nukeOptions},MainNuke].Length}) {
        /varset mainNuke "${Ini[${fileName},${nukeOptions},MainNuke]}"
    } else {
        /ini "${fileName}" "${nukeOptions}" "MainNuke" "${mainNuke}"
        /varset casterInitResult 0
    }

    /if (${Ini[${fileName},${nukeOptions},UndeadNuke].Length}) {
       /varset undeadNuke ${Ini[${fileName},${nukeOptions},UndeadNuke]}       
    }

    /if (${Ini[${fileName},${nukeOptions},MinMana].Length}) {
       /varset minNukeMana ${Ini[${fileName},${nukeOptions},MinMana]}       
    }
/return ${casterInitResult}

Sub DoNuke
    /if (${Debug}) /echo |- DoNuke ==>

    /if (${Me.Casting.ID} || ${Me.Moving} || ${Me.CurrentMana} <= ${minNukeMana}) {
        /if (${Debug}) /echo <== DoNuke -|
        /return
    }

    /declare targetId int local ${NetBots[${mainAssist}].TargetID}
    /if (!${targetId}) {
        /if (${Debug}) /echo <== DoNuke -|
        /return
    }
    
    /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
    /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
    /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
    /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

    /if ((!${isNPC} && !${isPet}) || ${targetHP} > ${assistPct} || !${hasLineOfSight}) {
        /if (${Debug}) /echo <== DoNuke -|
        /return
    }

    /target id ${targetId}
    /delay 2s ${Target.ID}==${targetId}
    /if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}!=${targetId}) {
        /if (${Debug}) /echo <== DoNuke -|
        /return
    }
    
    /if (${Select[${Target.Body},Undead]} && ${Me.Gem[${undeadNuke}]}) {
        /call TryNukeTarget "${undeadNuke}"
    } else {
        /call TryNukeTarget ${mainNuke}
    }
    
    /if (${Debug}) /echo <== DoNuke -|
/return

Sub TryNukeTarget(string nuke)
    /if (!${Cast.Ready[${nuke}]} || ${Me.CurrentMana} < ${Spell[${nuke}].Mana} || ${Target.Distance} > ${Spell[${nuke}].Range}) {
        /return
    }

    /if (${Me.Sitting}) {
        /stand
    }
    
    /call cast "${nuke}" gem${Me.Gem[${nuke}]} 0 CancelNuke
/return

Sub CancelNuke
    /if (!${Target.ID}) /call Interrupt
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
	| /doevents StartCast
/return

Sub MakeNukeIni(string fileName)
    |- Build the INI file
    /echo Building Nuke INI file.....

    /ini "${fileName}" "${nukeOptions}" "MainNuke" "${mainNuke}"
    /ini "${fileName}" "${nukeOptions}" "UndeadNuke" "${undeadNuke}"
/return
	