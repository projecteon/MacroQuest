| ################################################################################
| # Create Healer Ini Section
| ################################################################################	
Sub InitHealIni(string iniFileName)
    /declare options            string  local "HealingOptions"
    /declare casterInitResult   int     local 1
    /if (!${Defined[mainHeal]})     /declare mainHeal       string  outer "Main Heal" 
    /if (!${Defined[mainHealPct]})  /declare mainHealPct    int     outer 90 
    /if (!${Defined[healDist]})     /declare healDist       int     outer 200 
        
    /call InitCasterIni "${iniFileName}"
    /varset casterInitResult ${Macro.Return}
    
    /if (${Ini[${iniFileName},${options},MainHeal].Length}) {
       /varset mainHeal "${Ini[${iniFileName},${options},MainHeal]}"       
    } else {
        /ini "${iniFileName}" "${options}" "MainHeal" "${mainHeal}"
        /return 0
    }

    /if (${Ini[${iniFileName},${options},MainHealPct].Length}) {
       /varset mainHealPct ${Ini[${iniFileName},${options},MainHealPct]}       
    } else {
        /ini "${iniFileName}" "${options}" "MainHealPct" "${mainHealPct}"
        /return 0
    }

    /if (${Ini[${iniFileName},${options},HealingDist].Length}) {
       /varset healDist ${Ini[${iniFileName},${options},HealingDist]}       
    } else {
        /ini "${iniFileName}" "${options}" "HealingDist" "${healDist}"   
        /return 0 
    }

    /if (${Spell[${mainHeal}].Range} < ${healDist}) {
        /varset healDist ${Spell[${mainHeal}].Range}
    }

/return ${casterInitResult}

Sub CheckGroupHeals
    /if (!${Group.Members}) {
        /return
    }

    /if (${Me.PctHPs} <= ${mainHealPct}) {
        /call CastHeal "${mainHeal}" ${Me.ID} "${Me.Name}"
    }

    /declare x int local
    /for x 1 to ${Group.Members} {
        /if (${Spawn[${Group.Member[${x}]}].ID} && ${Group.Member[${x}].PctHPs} <= ${mainHealPct} && ${Group.Member[${x}].Distance} <= ${healDist}) {
            /call CastHeal "${mainHeal}" ${Group.Member[${x}].ID} "${Group.Member[${x}].Name}"
        }
        /next x
    }
/return

Sub CastHeal(string healSpell, int healTargetId, string healTargetName)
    /if (!${Defined[DontStop]}) /declare DontStop bool local FALSE
    /if (${Me.Casting.ID}) {
        /return
    }

    /if (!${Me.Gem[${healSpell}]}) {
        /echo !!! SPELL NOT MEMORIZED ${healSpell} - SHUTDOWN !!!
        /bca !!! SPELL NOT MEMORIZED ${healSpell} - SHUTDOWN !!!
        /endmac
    }

    /echo CastHeal1 ${Me.SpellReady[${healSpell}]} ${Me.CurrentMana} ${Spell[${healSpell}].Mana}
    /echo CastHeal2 ${Target.ID} ${healTargetId} ${Target.Distance} ${Spell[${healSpell}].Range}
	|- Is spell ready?
	/if (${Me.SpellReady[${healSpell}]}) {
		|- Do we have mana
		/if (${Me.CurrentMana} < ${Spell[${healSpell}].Mana}) {		
			/delay 1s
			/gsay !!! NO MANA TO HEAL ${healTargetName} !!!
			/bca !!! NO MANA TO HEAL ${healTargetName} !!!
			/return
		} else {			
            /if (${Target.ID} != ${healTargetId}) {
                /target id ${healTargetId}
                /delay 2s ${Target.ID} == ${healTargetId}
                /if (${Target.ID} != ${healTargetId}) /return
            }

			|- If right target and in range, cast the spell
            /echo ${Target.Name}
			/if (${Target.ID} == ${healTargetId} && (${Target.Distance} <= ${Spell[${healSpell}].Range}) && ${Target.Type.NotEqual[Corpse]}) {	
                /if (${Me.Sitting}) {
                    /stand
                }
				/gsay <<< ${healSpell} -> ${healTargetName} >>>			
				/bca <<< ${healSpell} -> ${healTargetName} >>>
				/call cast "${healSpell}" gem${Me.Gem[${healSpell}]} 0 CancelHeal
				/bca MANA: ${Me.PctMana}
			}
		}
	}
/return

| #################################################
| # Called during the cast of any spell and will
| # interuppt it if any condition pops true.
| #################################################
Sub CancelHeal
	/if (${Target.PctHPs} > ${mainHealPct}) /call Interrupt
	/if (${Target.Distance} > ${healDist}) /call Interrupt
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
	| /if (${Me.AltAbilityReady[Divine Arbitration]}) /call DivineArbitration
	| /doevents StartCast
/return