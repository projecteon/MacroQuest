| ################################################################################
| # Create Healer Ini Section
| ################################################################################	
Sub InitHealIni(string iniFileName)
    /declare options            string  local "HealingOptions"
    /declare casterInitResult   int     local 1
    /if (!${Defined[mainHeal]})             /declare mainHeal                       string  outer "Main Heal" 
    /if (!${Defined[mainHealPct]})          /declare mainHealPct                    int     outer 90 
    /if (!${Defined[healDist]})             /declare healDist                       int     outer 200 
    /if (!${Defined[hasNotifiedNoMana]})    /declare hasNotifiedNoMana                      outer FALSE
    /if (!${Defined[doHealGroup]})          /declare doHealGroup                    bool    outer false 
    /if (!${Defined[doHealNetBots]})        /declare doHealNetBots                  bool    outer false 
        
    /call InitCasterIni "${iniFileName}"
    /varset casterInitResult ${Macro.Return}
    
    /if (${Ini[${iniFileName},${options},MainHeal].Length}) {
       /varset mainHeal "${Ini[${iniFileName},${options},MainHeal]}"       
    } else {
        /ini "${iniFileName}" "${options}" "MainHeal" "${mainHeal}"
        /varset casterInitResult 0
    }

    /if (${Ini[${iniFileName},${options},MainHealPct].Length}) {
       /varset mainHealPct ${Ini[${iniFileName},${options},MainHealPct]}       
    } else {
        /ini "${iniFileName}" "${options}" "MainHealPct" "${mainHealPct}"
        /varset casterInitResult 0
    }

    /if (${Ini[${iniFileName},${options},HealingDist].Length}) {
       /varset healDist ${Ini[${iniFileName},${options},HealingDist]}       
    } else {
        /ini "${iniFileName}" "${options}" "HealingDist" "${healDist}"  
        /varset casterInitResult 0
    }

    /if (${Spell[${mainHeal}].Range} < ${healDist}) {
        /varset healDist ${Spell[${mainHeal}].Range}
    }

    /if (${Ini[${iniFileName},${options},DoHealGroup].Length}) {
       /varset doHealGroup ${Bool[${Ini[${iniFileName},${options},DoHealGroup]}]}     
    } else {
        /ini "${iniFileName}" "${options}" "DoHealGroup" "${doHealGroup}"
    }

    /if (${Ini[${iniFileName},${options},DoHealNetBots].Length}) {
       /varset doHealNetBots ${Bool[${Ini[${iniFileName},${options},DoHealNetBots]}]}
    } else {
        /ini "${iniFileName}" "${options}" "DoHealNetBots" "${doHealNetBots}"
    }

/return ${casterInitResult}

Sub CheckHeals
    /if (${Me.Casting.ID}) {
        /return
    }

    /if (${Me.PctHPs} <= ${mainHealPct}) {
        /call CastHeal ${mainHeal} ${Me.ID} "${Me.Name}"
    }

    /call CheckHealGroup
    /call CheckHealNetBots
/return

Sub CheckHealGroup
    /if (!${Group.Members} || !${doHealGroup}) {
        /return
    }

    /declare x int local
    /for x 1 to ${Group.Members} {
        /if (${Spawn[${Group.Member[${x}]}].ID} && ${Group.Member[${x}].PctHPs} <= ${mainHealPct} && ${Group.Member[${x}].Distance} <= ${healDist}) {
            /call CastHeal ${mainHeal} ${Group.Member[${x}].ID} "${Group.Member[${x}].Name}"
        }
        /next x
    }
/return

Sub CheckHealNetBots
    /if (${NetBots.Counts} < 2 || !${doHealNetBots}) {
        /return
    }

    /declare x int local
    /for x 1 to ${NetBots.Counts} {
        /varset netbotsClient "${NetBots.Client[${x}]}"
        /if (${NetBots[${netbotsClient}].InZone} && ${NetBots[${netbotsClient}].PctHPs} <= ${mainHealPct} && ${Spawn[${NetBots[${netbotsClient}].ID}].Distance} <= ${healDist}) {
            /call CastHeal ${mainHeal} ${NetBots[${netbotsClient}].ID} "${NetBots[${netbotsClient}].Name}"
        }
        /next x
    }

/return

Sub CastHeal(string healSpell, int healTargetId, string healTargetName)
    /if (!${Defined[DontStop]}) /declare DontStop bool local FALSE

    /if (!${Me.Gem[${healSpell}]}) {
        /echo !!! SPELL NOT MEMORIZED ${healSpell} - SHUTDOWN !!!
        /bca !!! SPELL NOT MEMORIZED ${healSpell} - SHUTDOWN !!!
        /endmac
    }

	|- Is spell ready?
	/if (${Me.SpellReady[${healSpell}]}) {
		|- Do we have mana
		/if (${Me.CurrentMana} < ${Spell[${healSpell}].Mana}) { 		
            /if (${hasNotifiedNoMana}) {
                /return 0
            }

            /varset hasNotifiedNoMana TRUE
			/delay 1s
			/gsay !!! NO MANA TO HEAL ${healTargetName} !!!
			/bca !!! NO MANA TO HEAL ${healTargetName} !!!
			/return 0
		} else {			
            /varset hasNotifiedNoMana FALSE
            /if (${Target.ID} != ${healTargetId}) {
                /target id ${healTargetId}
                /delay 2s ${Target.ID} == ${healTargetId}
                /if (${Target.ID} != ${healTargetId}) /return 0
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
/return 1


Sub iniHeal(string INIFile, string section, string key, array)
    /declare s int local
    /for s 1 to ${iniHealDimensions}
        /varset ${array}[${k},${s}] ${Ini[${INIFile},${section},${key}].Token[${s},/]}
    /next s
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