Sub InitHealIni(string iniFileName)
    /declare options            string  local "HealingOptions"
    /declare healerInitResult   int     local 1
    /if (!${Defined[defaultHeal]})              /declare defaultHeal                    string  outer  
    /if (!${Defined[defaultHealPct]})           /declare defaultHealPct                 int     outer 90 
    /if (!${Defined[mainTankHeal]})             /declare mainTankHeal                   string  outer  
    /if (!${Defined[mainTankHealPct]})          /declare mainTankHealPct                int     outer 90 
    /if (!${Defined[mainTankEmergencyHeal]})    /declare mainTankEmergencyHeal          string  outer  
    /if (!${Defined[mainTankEmergencyHealPct]}) /declare mainTankEmergencyHealPct       int     outer 0 
    /if (!${Defined[healDist]})                 /declare healDist                       int     outer 200 
    /if (!${Defined[hasNotifiedNoMana]})        /declare hasNotifiedNoMana              bool    outer FALSE
    /if (!${Defined[doHealGroup]})              /declare doHealGroup                    bool    outer FALSE 
    /if (!${Defined[doHealNetBots]})            /declare doHealNetBots                  bool    outer FALSE 
    /if (!${Defined[Debug_Heals]})              /declare Debug_Heals                    bool    outer FALSE
        
    /call InitCasterIni "${iniFileName}"
    /varset healerInitResult ${Macro.Return}
    
    /if (${Ini[${iniFileName},${options},DeafultHeal].Length}) {
       /varset defaultHeal "${Ini[${iniFileName},${options},DeafultHeal]}"       
    } else {
        /ini "${iniFileName}" "${options}" "DeafultHeal" "${defaultHeal}"
        /varset healerInitResult 0
    }

    /if (${Ini[${iniFileName},${options},DeafultHealPct].Length}) {
       /varset defaultHealPct ${Ini[${iniFileName},${options},DeafultHealPct]}       
    } else {
        /ini "${iniFileName}" "${options}" "DeafultHealPct" "${defaultHealPct}"
        /varset healerInitResult 0
    }
    
    /if (${Ini[${iniFileName},${options},MainTankHeal].Length}) {
       /varset mainTankHeal "${Ini[${iniFileName},${options},MainTankHeal]}"       
    } else {
        /ini "${iniFileName}" "${options}" "MainTankHeal" "${mainTankHeal}"
        /varset healerInitResult 0
    }

    /if (${Ini[${iniFileName},${options},MainTankHealPct].Length}) {
       /varset mainTankHealPct ${Ini[${iniFileName},${options},MainTankHealPct]}       
    } else {
        /ini "${iniFileName}" "${options}" "MainTankHealPct" "${mainTankHealPct}"
        /varset healerInitResult 0
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

    /if (${Ini[${iniFileName},${options},HealingDist].Length}) {
       /varset healDist ${Ini[${iniFileName},${options},HealingDist]}

        /if (${Spell[${defaultHeal}].Range} < ${healDist}) {
            /varset healDist ${Spell[${defaultHeal}].Range}
        }       
    } else {
        /ini "${iniFileName}" "${options}" "HealingDist" "${healDist}"  
        /varset healerInitResult 0
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

    /if (${doHealGroup}) /call CheckHealGroup ${defaultHeal} ${defaultHealPct}
    /if (${doHealNetBots}) /call CheckHealNetBots ${defaultHeal} ${defaultHealPct}
    
    /if (${Debug} || ${Debug_Heals}) /echo <== CheckHeals -|
/return

Sub CheckMainTankHeal(string healSpell, int healPct)
    /if (${Debug} || ${Debug_Heals}) /echo |- CheckMaintankHeal ==>

    /if (!${Defined[mainTank]}) {
        /if (${Debug} || ${Debug_Heals}) /echo |- No main tank defined
        /goto :endCheckMainTankHeal
    }

    /if (!${NetBots[${mainTank}].InZone}) {
        /goto :endCheckMainTankHeal
    }
    
    /if (${NetBots[${mainTank}].PctHPs} <= ${healPct}) { 
        /call CastHeal "${healSpell}" ${NetBots[${mainTank}].ID} ${mainTank}
    }

    :endCheckMainTankHeal
    /if (${Debug} || ${Debug_Heals}) /echo <== CheckMaintankHeal -|
/return

Sub CheckHealGroup(string healSpell, int healPct)
    /if (${Debug} || ${Debug_Heals}) /echo |- CheckHealGroup ==>

    /if (!${Group.Members}) {
        /return
    }

    /declare lowestMemberName   string local 
    /declare lowestMemberId     int local 0
    /declare lowestMemberPctHPs int local 100

    /declare x int local
    /for x 1 to ${Group.Members} {
        /if (${Spawn[${Group.Member[${x}]}].ID} && ${Group.Member[${x}].PctHPs} <= ${healPct} && ${Group.Member[${x}].Distance} <= ${healDist}) {
            /if (${Group.Member[${x}].PctHPs} < ${lowestMemberPctHPs}) {
                /varset lowestMemberName "${Group.Member[${x}].Name}"
                /varset lowestMemberId ${Group.Member[${x}].ID}
                /varset lowestMemberPctHPs ${Group.Member[${x}].PctHPs}
            }
        }
        /next x
    }

    /if (${lowestMemberId}>0 && ${lowestMemberPctHPs} <= ${healPct}) {
        /call CastHeal "${healSpell}" ${lowestMemberId} ${lowestMemberName}
    }
    
    /if (${Debug} || ${Debug_Heals}) /echo <== CheckHealGroup -|
/return

Sub CheckHealNetBots(string healSpell, int healPct)
    /if (${Debug} || ${Debug_Heals}) /echo |- CheckHealNetBots ==>

    /if (${NetBots.Counts} < 2) {
        /if (${Debug} && ${NetBots.Counts} < 2) /echo Not enough Nebots clients, current: ${NetBots.Counts} 
        /if (${Debug} || ${Debug_Heals}) /echo <== CheckHealNetBots -|
        /return
    }

    /declare isExpired bool local false
    /declare netbotsClient string local
    /declare x int local
    /for x 1 to ${NetBots.Counts} {
        /varset netbotsClient "${NetBots.Client[${x}]}"
        /if (${NetBots[${netbotsClient}].InZone} && ${NetBots[${netbotsClient}].PctHPs} <= ${healPct} && ${Spawn[${NetBots[${netbotsClient}].ID}].Distance} <= ${healDist} && (!${doHealGroup} || !${NetBots[${netbotsClient}].InGroup})) {            
            /call CheckTimerExpired "${netbotsClient}_nbh_timer"            
            /varset isExpired ${Macro.Return}
            /if (${isExpired}) {
                /if (${Debug} || ${Debug_Heals}) /echo |- CheckHealNetBots > ${NetBots[${netbotsClient}].Name} > ${NetBots[${netbotsClient}].PctHPs}
                /call CastHeal "${healSpell}" ${NetBots[${netbotsClient}].ID} "${NetBots[${netbotsClient}].Name}"
                /call CreateTimer "${netbotsClient}_nbh_timer" 30
            }
        }
        /next x
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
                /return 0
            }

            /varset hasNotifiedNoMana TRUE
			/delay 1s
			/if (${Group.Members}) /gsay !!! NO MANA TO HEAL ${healTargetName} !!!
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

				/if (${Group.Members}) /gsay <<< ${healSpell} -> ${healTargetName} >>>			
				/bca <<< ${healSpell} -> ${healTargetName} >>>
				/call cast "${healSpell}" gem${Me.Gem[${healSpell}]} 0 CancelHeal 3
				/bca MANA: ${Me.PctMana}
			}
		}
	}

    /if (${Debug} || ${Debug_Heals}) /echo <== CastHeal -|
/return 1

Sub iniHeal(string INIFile, string section, string key, array)
    /declare s int local
    /for s 1 to ${iniHealDimensions}
        /varset ${array}[${k},${s}] ${Ini[${INIFile},${section},${key}].Token[${s},/]}
    /next s
/return

Sub CancelHeal(int spellId)
    /if (${Target.Name.Equal[${mainTank}]} && ${Spell[${mainTankHeal}].ID}==${spellId} && ${Bool[${mainTankEmergencyHeal}]} && ${Me.Gem[${mainTankEmergencyHeal}]}) {
        /if (${Target.PctHPs} <= ${mainTankEmergencyHealPct} && ${mainTankEmergencyHeal.NotEqual[${mainTankHeal}]} && ${castEndTime} > ${Spell[${mainTankEmergencyHeal}].MyCastTime}) { 
            /call Interrupt
            /call CastHeal ${mainTankEmergencyHeal} ${Target.ID} ${mainTank}
        }
    }

    | Following line has issue with NetBots update report, cancels heals when it shouldnt
	| /if (${Target.PctHPs} > ${defaultHealPct}) /call Interrupt
	/if (${Target.Distance} > ${healDist}) /call Interrupt
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
	| /if (${Me.AltAbilityReady[Divine Arbitration]}) /call DivineArbitration
	| /doevents StartCast
/return