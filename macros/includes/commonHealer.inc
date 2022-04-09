Sub InitHealIni(string iniFileName)
  /declare options            string  local "HealingOptions"
  /declare healerInitResult   int     local 1
  /if (!${Defined[groupHealSpell]})           /declare groupHealSpell                 string  outer
  /if (!${Defined[groupHealPct]})             /declare groupHealPct                   int     outer 0
  /if (!${Defined[netbotsHealSpell]})         /declare netbotsHealSpell               string  outer
  /if (!${Defined[netbotsHealPct]})           /declare netbotsHealPct                 int     outer 0
  /if (!${Defined[mainTankHeal]})             /declare mainTankHeal                   string  outer
  /if (!${Defined[mainTankHealPct]})          /declare mainTankHealPct                int     outer 90
  /if (!${Defined[mainTankEmergencyHeal]})    /declare mainTankEmergencyHeal          string  outer
  /if (!${Defined[mainTankEmergencyHealPct]}) /declare mainTankEmergencyHealPct       int     outer 0
  /if (!${Defined[healDist]})                 /declare healDist                       int     outer 200
  /if (!${Defined[hasNotifiedNoMana]})        /declare hasNotifiedNoMana              bool    outer FALSE
  /if (!${Defined[isHealingGroup]})           /declare isHealingGroup                 bool    outer FALSE
  /if (!${Defined[isHealingNetbots]})         /declare isHealingNetbots               bool    outer FALSE
  /if (!${Defined[isHealingMaintank]})        /declare isHealingMaintank              bool    outer FALSE

  /if (!${Defined[Debug_Heals]})              /declare Debug_Heals                    bool    outer FALSE

  /call InitCasterIni "${iniFileName}"
  /varset healerInitResult ${Macro.Return}

  /if (${Ini[${iniFileName},${options},GroupHealPct].Length}) {
    /varset groupHealPct ${Ini[${iniFileName},${options},GroupHealPct]}
  } else {
    /ini "${iniFileName}" "${options}" "GroupHealPct" "${groupHealPct}"
  }

  /if (${Ini[${iniFileName},${options},GroupHealSpell].Length}) {
    /varset groupHealSpell "${Ini[${iniFileName},${options},GroupHealSpell]}"
  } else {
    /ini "${iniFileName}" "${options}" "GroupHealSpell" "${groupHealSpell}"
  }

  /if (${groupHealPct} > 0 ) {
    /if (${groupHealSpell.Length} == 0) {
      /echo Could not enable group healing, heal spell missing.
      /varset healerInitResult 0
    } else /if (!${Bool[${Me.Book[${groupHealSpell}]}]}) {
      /echo Could not enable group healing, you do not know the spell <${groupHealSpell}>.
      /varset healerInitResult 0
    }
  }

  /if (${Ini[${iniFileName},${options},NetbotsHealPct].Length}) {
    /varset netbotsHealPct ${Ini[${iniFileName},${options},NetbotsHealPct]}
  } else {
    /ini "${iniFileName}" "${options}" "NetbotsHealPct" "${netbotsHealPct}"
  }

  /if (${Ini[${iniFileName},${options},NetbotsHealSpell].Length}) {
    /varset netbotsHealSpell "${Ini[${iniFileName},${options},NetbotsHealSpell]}"
  } else {
    /ini "${iniFileName}" "${options}" "NetbotsHealSpell" "${netbotsHealSpell}"
  }

  /if (${netbotsHealPct} > 0 ) {
    /if (${netbotsHealSpell.Length} == 0) {
      /echo Could not enable netbots healing, heal spell missing.
      /varset healerInitResult 0
    } else /if (!${Bool[${Me.Book[${netbotsHealSpell}]}]}) {
      /echo Could not enable netbots healing, you do not know the spell <${netbotsHealSpell}>.
      /varset healerInitResult 0
    }
  }

  /if (${Ini[${iniFileName},${options},MainTankHeal].Length}) {
    /varset mainTankHeal "${Ini[${iniFileName},${options},MainTankHeal]}"
  } else {
    /ini "${iniFileName}" "${options}" "MainTankHeal" "${mainTankHeal}"
  }

  /if (${Ini[${iniFileName},${options},MainTankHealPct].Length}) {
    /varset mainTankHealPct ${Ini[${iniFileName},${options},MainTankHealPct]}
  } else {
    /ini "${iniFileName}" "${options}" "MainTankHealPct" "${mainTankHealPct}"
  }

  /if (${mainTankHealPct} > 0 ) {
    /if (${mainTankHeal.Length} == 0) {
      /echo Could not enable maintank healing, heal spell missing.
      /varset healerInitResult 0
    } else /if (!${Bool[${Me.Book[${mainTankHeal}]}]}) {
      /echo Could not enable maintank healing, you do not know the spell <${mainTankHeal}>.
      /varset healerInitResult 0
    }
  }

  /if (${Ini[${iniFileName},${options},MainTankEmergencyHeal].Length}) {
    /varset mainTankEmergencyHeal "${Ini[${iniFileName},${options},MainTankEmergencyHeal]}"
  } else {
    /ini "${iniFileName}" "${options}" "MainTankEmergencyHeal" "${mainTankEmergencyHeal}"
    /varset healerInitResult 0
  }

  /if (${Ini[${iniFileName},${options},MainTankEmergencyHealPct].Length}) {
    /varset mainTankEmergencyHealPct ${Ini[${iniFileName},${options},MainTankEmergencyHealPct]}
  } else {
    /ini "${iniFileName}" "${options}" "MainTankEmergencyHealPct" "${mainTankEmergencyHealPct}"
    /varset healerInitResult 0
  }

  /if (${mainTankEmergencyHealPct} > 0 ) {
    /if (${mainTankEmergencyHeal.Length} == 0) {
      /echo Could not enable maintank emergency healing, heal spell missing.
      /varset healerInitResult 0
    } else /if (!${Bool[${Me.Book[${mainTankEmergencyHeal}]}]}) {
      /echo Could not enable maintank emergency healing, you do not know the spell <${mainTankEmergencyHeal}>.
      /varset healerInitResult 0
    }
  }

  /if (${Ini[${iniFileName},${options},HealingDist].Length}) {
    /varset healDist ${Ini[${iniFileName},${options},HealingDist]}
  } else {
    /ini "${iniFileName}" "${options}" "HealingDist" "${healDist}"
    /varset healerInitResult 0
  }

  /if (${Ini[${iniFileName},Debug,Debug_Heals].Length}) {
    /varset Debug_Heals ${Bool[${Ini[${iniFileName},Debug,Debug_Heals]}]}
  }
/return ${healerInitResult}

Sub CheckHeals
  /if (${Debug} || ${Debug_Heals}) /echo |- CheckHeals ==>

  /if (${Me.Casting.ID}) {
    /if (${Debug} || ${Debug_Heals}) /echo <== CheckHeals -|
    /return
  }

  /call CheckMainTankHeal ${mainTankHeal} ${mainTankHealPct}

  /if (${Me.PctHPs} <= ${defaultHealPct}) {
    /call CastHeal ${defaultHeal} ${Me.ID} "${Me.Name}"
  }

  /if (${groupHealPct} > 0) /call CheckHealGroup ${groupHealSpell} ${groupHealPct}
  /if (${netbotsHealPct} > 0) /call CheckHealNetBots ${netbotsHealSpell} ${netbotsHealPct}

  /if (${Debug} || ${Debug_Heals}) /echo <== CheckHeals -|
/return

Sub CheckMainTankHeal(string healSpell, int healPct)
  /if (${Debug} || ${Debug_Heals}) /echo |- CheckMaintankHeal ==>

  /call GetMainTank
  /declare mainTankToHeal string	local ${Macro.Return}

  /if (${mainTankToHeal.Equal[NULL]}) {
    /if (${Debug} || ${Debug_Heals}) /echo |- No main tank defined
    /goto :endCheckMainTankHeal
  }

  /if (!${NetBots[${mainTankToHeal}].InZone}) {
    /goto :endCheckMainTankHeal
  }

  /if (${NetBots[${mainTankToHeal}].PctHPs} <= ${healPct}) {
    /varset isHealingMaintank TRUE
    /call CastHeal "${healSpell}" ${NetBots[${mainTankToHeal}].ID} ${mainTankToHeal}
    /varset isHealingMaintank FALSE
  }

  :endCheckMainTankHeal
  /if (${Debug} || ${Debug_Heals}) /echo <== CheckMaintankHeal -|
/return

Sub CheckHealGroup(string healSpell, int healPct)
  /if (${Debug} || ${Debug_Heals}) /echo |- CheckHealGroup ==> <${healSpell}> <${healPct}>

  /if (!${Group.Members}) {
      /return
  }

  /declare lowestMemberName   string  local
  /declare lowestMemberId     int     local 0
  /declare lowestMemberPctHPs int     local 100
  /declare groupHealDist      int     local ${healDist}

  /if (${Spell[${healSpell}].Range} < ${groupHealDist}) {
    /varset groupHealDist ${Spell[${healSpell}].Range}
  }

  /declare x int local
  /for x 1 to ${Group.Members}
    /if (${Spawn[${Group.Member[${x}]}].ID} && ${Group.Member[${x}].PctHPs} <= ${healPct} && ${Group.Member[${x}].Distance} <= ${groupHealDist}) {
      /if (${Group.Member[${x}].PctHPs} < ${lowestMemberPctHPs}) {
        /varset lowestMemberName "${Group.Member[${x}].Name}"
        /varset lowestMemberId ${Group.Member[${x}].ID}
        /varset lowestMemberPctHPs ${Group.Member[${x}].PctHPs}
      }
    }
  /next x

  /if (${Debug} || ${Debug_Heals}) /echo ${lowestMemberId}>0 && ${lowestMemberPctHPs} <= ${healPct}
  /if (${lowestMemberId}>0 && ${lowestMemberPctHPs} <= ${healPct}) {
    /varset isHealingGroup TRUE
    /call CastHeal "${healSpell}" ${lowestMemberId} ${lowestMemberName}
    /varset isHealingGroup FALSE
  }

  /if (${Debug} || ${Debug_Heals}) /echo <== CheckHealGroup -|
/return

Sub CheckHealNetBots(string healSpell, int healPct)
  /if (${Debug} || ${Debug_Heals}) /echo |- CheckHealNetBots ==> <${healSpell}> <${healPct}>

  /if (${NetBots.Counts} < 2) {
    /if (${Debug} && ${NetBots.Counts} < 2) /echo Not enough Nebots clients, current: ${NetBots.Counts}
    /if (${Debug} || ${Debug_Heals}) /echo <== CheckHealNetBots -|
    /return
  }

  /declare lowestMemberName   string  local
  /declare lowestMemberId     int     local 0
  /declare lowestMemberPctHPs int     local 100
  /declare netbotsHealDist    int     local ${healDist}

  /if (${Spell[${healSpell}].Range} < ${netbotsHealDist}) {
    /varset netbotsHealDist ${Spell[${healSpell}].Range}
  }

  /declare isExpired bool local false
  /declare netbotsClient string local
  /declare x int local
  /for x 1 to ${NetBots.Counts}
    /varset netbotsClient "${NetBots.Client[${x}]}"
    /if (${NetBots[${netbotsClient}].InZone} && ${NetBots[${netbotsClient}].PctHPs} <= ${healPct} && ${Spawn[${NetBots[${netbotsClient}].ID}].Distance} <= ${netbotsHealDist} && (!${doHealGroup} || !${NetBots[${netbotsClient}].InGroup})) {
      /call CheckTimerExpired "${netbotsClient}_nbh_timer"
      /varset isExpired ${Macro.Return}
      /if (${isExpired}) {
        /if (${NetBots[${netbotsClient}].PctHPs} < ${lowestMemberPctHPs}) {
          /varset lowestMemberName "${NetBots[${netbotsClient}].Name}"
          /varset lowestMemberId ${NetBots[${netbotsClient}].ID}
          /varset lowestMemberPctHPs ${NetBots[${netbotsClient}].PctHPs}
        }
      }
    }
  /next x

  /if (${lowestMemberId}>0 && ${lowestMemberPctHPs} <= ${healPct}) {
    /if (${Debug} || ${Debug_Heals}) /echo |- CheckHealNetBots > ${lowestMemberName} > ${lowestMemberPctHPs}
    /varset isHealingNetbots TRUE
    /call CreateTimer "${lowestMemberName}_nbh_timer" 50
    /call CastHeal "${healSpell}" ${lowestMemberId} ${lowestMemberName}
    /varset isHealingNetbots FALSE
  }

  /if (${Debug} || ${Debug_Heals}) /echo <== CheckHealNetBots -|
/return

Sub CastHeal(string healSpell, int healTargetId, string healTargetName)
  /if (${Debug} || ${Debug_Heals}) /echo |- CastHeal ==>

  /if (!${Defined[DontStop]}) /declare DontStop bool local FALSE

  /if (!${Me.Gem[${healSpell}]}) {
    /echo !!! SPELL NOT MEMORIZED ${healSpell} - SHUTDOWN !!!
    /bca !!! SPELL NOT MEMORIZED ${healSpell} - SHUTDOWN !!!
    /endmac
  }

	|- Is spell ready?
  /if (!${Me.SpellReady[${healSpell}]}) /delay 3s ${Me.SpellReady[${healSpell}]}

  /if (${Me.SpellReady[${healSpell}]}) {
		|- Do we have mana
		/if (${Me.CurrentMana} < ${Spell[${healSpell}].Mana}) {
      /if (${hasNotifiedNoMana}) {
          /if (${Debug} || ${Debug_Heals}) /echo <== CastHeal -|
          /return
      }

      /varset hasNotifiedNoMana TRUE
			/delay 1s
			/if (${Group.Members}) /gsay !!! NO MANA TO HEAL ${healTargetName} !!!
			/bca !!! NO MANA TO HEAL ${healTargetName} !!!
      /if (${Debug} || ${Debug_Heals}) /echo <== CastHeal -|
			/return
		} else {
      /varset hasNotifiedNoMana FALSE
      /call EnsureTarget ${healTargetId}
      /if (${Target.ID} != ${healTargetId} && ${Target.Type.NotEqual[Corpse]}) {
        /if (${Debug} || ${Debug_Heals}) /echo |- Invalid target
        /if (${Debug} || ${Debug_Heals}) /echo <== CastHeal -|
        /return
      }

			|- If target in range, cast the spell
			/if ((${Target.Distance} <= ${Spell[${healSpell}].Range})) {
        /if (${Me.Sitting}) {
          /stand
        }

				/if (${Group.Members}) /gsay <<< ${healSpell} -> ${healTargetName} >>>
				/bca <<< ${healSpell} -> ${healTargetName} >>>
				/call cast "${healSpell}" gem${Me.Gem[${healSpell}]} 0 CancelHeal 3
				/bca MANA: ${Me.PctMana}
			}
		}
	}

  /if (${Debug} || ${Debug_Heals}) /echo <== CastHeal -|
/return

Sub CancelHeal(int spellId)
  | check main tank emergency heal
  /if (${Spell[${mainTankHeal}].ID}==${spellId} && ${Bool[${mainTankEmergencyHeal}]} && ${Me.Gem[${mainTankEmergencyHeal}]}) {
    /if (${Target.PctHPs} <= ${mainTankEmergencyHealPct} && ${mainTankEmergencyHeal.NotEqual[${mainTankHeal}]} && ${castEndTime} > ${Spell[${mainTankEmergencyHeal}].MyCastTime}) {
      /call Interrupt
      /call CastHeal ${mainTankEmergencyHeal} ${Target.ID} ${Target.Name}
    }
  } else /if (${isHealingGroup} && ${Target.PctHPs} > ${groupHealPct}) {
     /call Interrupt
  } else /if (${isHealingNetbots} && ${Target.PctHPs} > ${netbotsHealPct}) {
    /call Interrupt
  } else /if (${isHealingMaintank} && ${Target.PctHPs} > ${mainTankHealPct}) {
    /call Interrupt
  } else /if (${Target.PctHPs} > ${mainTankEmergencyHealPct} && ${spellId} == ${Spell[${mainTankEmergencyHeal}].ID}) {
    | /call Interrupt
  } else /if (${Target.Distance} > ${healDist}) {
    /call Interrupt
  } else /if (${Target.Type.Equal[Corpse]}) {
    /call Interrupt
  }
/return