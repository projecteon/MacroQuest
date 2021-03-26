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

| ################################################################################
| # Cast heal if NetBots says its needed
| ################################################################################
Sub CheckNetBotsHeals
	/if (${Heal.WorstNetBots.Type.NotEqual[PC]}) /return
	|- Casters(wis & int) then Plate, then Melee(bard here)
	|- Add them to the array if they are in need of a heal
	/if (${Heal.WorstNetBots.Class.Name.Equal[Shaman]} || ${Heal.WorstNetBots.Class.Name.Equal[Cleric]} || ${Heal.WorstNetBots.Class.Name.Equal[Wizard]} || ${Heal.WorstNetBots.Class.Name.Equal[Magician]} || ${Heal.WorstNetBots.Class.Name.Equal[Enchanter]} || ${Heal.WorstNetBots.Class.Name.Equal[Necromancer]} || ${Heal.WorstNetBots.Class.Name.Equal[Druid]}) {
		/if (${NetBots[${Heal.WorstNetBots.Name}].PctHPs} <= ${CasterPct}) {
			/call CastHeal "${CasterHeal}" "${Heal.WorstNetBots.ID}" "${Heal.WorstNetBots.Name}"
		}
	} else /if (${Heal.WorstNetBots.Class.Name.Equal[Warrior]} || ${Heal.WorstNetBots.Class.Name.Equal[Shadow Knight]} || ${Heal.WorstNetBots.Class.Name.Equal[Paladin]}) {
		/if (${NetBots[${Heal.WorstNetBots.Name}].PctHPs} <= ${PlatePct}) {
			/call CastHeal "${PlateHeal}" "${Heal.WorstNetBots.ID}" "${Heal.WorstNetBots.Name}"
		}
	} else /if (${Heal.WorstNetBots.Class.Name.Equal[Ranger]} || ${Heal.WorstNetBots.Class.Name.Equal[Rogue]} || ${Heal.WorstNetBots.Class.Name.Equal[Bard]} || ${Heal.WorstNetBots.Class.Name.Equal[Berserker]} || ${Heal.WorstNetBots.Class.Name.Equal[Beastlord]} || ${Heal.WorstNetBots.Class.Name.Equal[Monk]}) {		
		/if (${NetBots[${Heal.WorstNetBots.Name}].PctHPs} <= ${MeleePct}) {
			/call CastHeal "${MeleeHeal}" "${Heal.WorstNetBots.ID}" "${Heal.WorstNetBots.Name}"
		}
	}	
/return

Sub CheckGroupHeals
    /if (${Me.PctHPs} <= ${mainHealPct}) {
        /call CastHeal "${mainHeal}" ${Me.ID} "${Me.Name}"
    }

    /if (!${Group.Members}) {
        /return
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
	/if (${Target.PctHPs} >= ${mainHealPct}) /call Interrupt
	/if (${Target.Distance} > ${healDist}) /call Interrupt
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
	| /if (${Me.AltAbilityReady[Divine Arbitration]}) /call DivineArbitration
	| /doevents StartCast
/return