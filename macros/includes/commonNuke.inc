| http://mqemulator.net/forum2/viewtopic.php?t=1551
| http://mqemulator.net/forum2/viewtopic.php?t=794
Sub InitNukeIni(string fileName)
  /if (!${Defined[nukeOptions]})  		/declare nukeOptions        string  outer "NukeOptions"
  /if (!${Defined[useNuke]})  		    /declare useNuke                   outer
  /if (!${Defined[Undead]})  		        /declare Undead             string  outer
  /if (!${Defined[Fire]})  		        /declare Fire               string  outer
  /if (!${Defined[Cold]})  		        /declare Cold               string  outer
  /if (!${Defined[Magic]})  		        /declare Magic              string  outer
  /if (!${Defined[minNukeMana]})  		/declare minNukeMana        int     outer 0
  /if (!${Defined[nukeInitResult]})  		/declare nukeInitResult     int     local 1
  /if (!${Defined[Debug_Nuke]})  		    /declare Debug_Nuke       	bool  	outer FALSE

  /call InitCasterIni "${fileName}"
  /varset nukeInitResult ${Macro.Return}

  /if (${Ini[${fileName},${nukeOptions},UndeadNuke].Length}) {
    /varset Undead ${Ini[${fileName},${nukeOptions},Undead]}
  }

  /if (${Ini[${fileName},${nukeOptions},Fire].Length}) {
    /varset Fire ${Ini[${fileName},${nukeOptions},Fire]}
  }

  /if (${Ini[${fileName},${nukeOptions},Cold].Length}) {
    /varset Cold ${Ini[${fileName},${nukeOptions},Cold]}
  }

  /if (${Ini[${fileName},${nukeOptions},Magic].Length}) {
    /varset Magic ${Ini[${fileName},${nukeOptions},Magic]}
  }

  /if (${Ini[${fileName},${nukeOptions},UseNuke].Length}) {
    /varset useNuke ${Ini[${fileName},${nukeOptions},UseNuke]}
    /if (!${Bool[${${useNuke}}]}) {
      /echo Unable to set nuke spell to '${useNuke}'.
      /varset nukeInitResult 0
    }
  } else {
    /ini "${fileName}" "${nukeOptions}" "UseNuke" "${useNuke}"
    /varset nukeInitResult 0
  }

  /if (${Ini[${fileName},${nukeOptions},MinMana].Length}) {
    /varset minNukeMana ${Ini[${fileName},${nukeOptions},MinMana]}
  }

  /if (${Ini[${fileName},Debug,Debug_Nuke].Length}) {
    /varset Debug_Nuke ${Bool[${Ini[${fileName},Debug,Debug_Nuke]}]}
  }
/return ${nukeInitResult}

Sub DoNuke
  /if (${Debug} || ${Debug_Nuke}) /echo |- DoNuke ==>

  /if (${Me.Casting.ID} || ${Me.Moving} || ${Me.PctMana} <= ${minNukeMana}) {
    /if (${Debug} || ${Debug_Nuke}) /echo |- Unabel to cast right now
    /goto :endDoNuke
  }

  /call GetMainAssist
  /declare mainAssist string	local ${Macro.Return}

  /if (${mainAssist.Equal[NULL]}) {
    /if (${Debug} || ${Debug_Heals}) /echo |- No main assist defined
    /goto :endDoNuke
  }

  /declare targetId int local ${NetBots[${mainAssist}].TargetID}
  /if (!${targetId}) {
    /if (${Debug} || ${Debug_Nuke}) /echo |- No MA target
    /goto :endDoNuke
  }

  /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
  /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
  /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
  /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

  /if ((!${isNPC} && !${isPet}) || ${targetHP} > ${assistPct} || !${hasLineOfSight}) {
    /if (${Debug} || ${Debug_Nuke}) /echo |- Wrong target type
    /goto :endDoNuke
  }

  /call EnsureTarget ${targetId}
  /if (!${Target.ID} || ${Target.ID}!=${targetId}) {
    /if (${Debug} || ${Debug_Nuke}) /echo |- Invalid target
    /goto :endDoNuke
  }

  /if (${Select[${Target.Body},Undead]} && ${Me.Gem[${undeadNuke}]}) {
    /call TryNukeTarget "${undeadNuke}"
  } else {
    /call TryNukeTarget "${${useNuke}}"
  }

  :endDoNuke
  /call DoNukeEvents
  /if (${Debug} || ${Debug_Nuke}) /echo <== DoNuke -|
/return

Sub TryNukeTarget(string nuke)
  /if (${Debug} || ${Debug_Nuke}) /echo |- TryNukeTarget ==>

  /if (!${Cast.Ready[${nuke}]} || ${Me.CurrentMana} < ${Spell[${nuke}].Mana} || (${Spell[${nuke}].Range} > 0 && ${Target.Distance} > ${Spell[${nuke}].Range})) {
    /if (${Debug} || ${Debug_Nuke}) /echo Failed trying to nuke: !${Cast.Ready[${nuke}]} || ${Me.CurrentMana} < ${Spell[${nuke}].Mana} || ${Target.Distance} > ${Spell[${nuke}].Range}
    /goto :endTryNukeTarget
  }

  /if (${Me.Sitting}) {
    /stand
  }

  /call cast "${nuke}" gem${Me.Gem[${nuke}]} 0 CancelNuke

  :endTryNukeTarget
  /if (${Debug} || ${Debug_Nuke}) /echo <== TryNukeTarget -|
/return

Sub CancelNuke(int spellId)
  /if (!${Target.ID}) /call Interrupt
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
	| /doevents StartCast
/return



| ################################################################################
| # Change type of nuke
| ################################################################################
#Event ChangeNukeLineup "<#*#> nuke lineup #1#"
#Event ChangeNukeLineup "#*#[MQ2] nuke lineup #1#"
Sub Event_ChangeNukeLineup(eventText, lineup)
  /if (${Debug} || ${Debug_Nuke}) /echo |- Event_ChangeNukeLineup ==>

  /if (${Defined[${lineup}]} && ${Bool[${${lineup}}]} && ${Bool[${Me.Gem[${${lineup}}]}]}) {
    /varset useNuke ${lineup}
    /bca Nuke lineup set to <${lineup}> using <${${lineup}}>
  }

  /if (${Debug} || ${Debug_Nuke}) /echo <== Event_ChangeNukeLineup -|
/return

Sub DoNukeEvents
  /doevents ChangeNukeLineup
/return