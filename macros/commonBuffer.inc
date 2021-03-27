| ################################################################################
| # Create Buffer Ini Section
| ################################################################################	
Sub InitBufferIni(string iniFileName)
    /declare options            string  local "BuffOptions"
    /if (!${Defined[minBuffMana]})      /declare minBuffMana int     outer 80 
    /if (!${Defined[buff1]})            /declare buff1       string  outer "Buff 1" 
        
    /if (${Ini[${iniFileName},${options},Buff1].Length}) {
       /varset buff1 "${Ini[${iniFileName},${options},Buff1]}"       
    } else {
        /ini "${iniFileName}" "${options}" "Buff1" "${buff1}"
        /return 0
    }  

    /if (${Ini[${iniFileName},${options},MinMana].Length}) {
       /varset minBuffMana ${Ini[${iniFileName},${options},MinMana]}     
    } else {
        /ini "${iniFileName}" "${options}" "MinMana" "${minBuffMana}"
        /return 0
    }

/return 1

Sub CheckGroupBuffs
 /if (${Me.PctMana} < ${minBuffMana}) {
     /return
 }

 /call CheckGroupBuff ${buff1}
/return

Sub CheckGroupBuff(string buffSpell)
    /if (!${Me.Buff[${buffSpell}].ID} || ${Me.Buff[${buffSpell}].Duration} < 50) {
        /call CastBuff "${buffSpell}" ${Me.ID} "${Me.Name}"
    }

    /if (${Group.Members}) {
        /declare x int local
        /for x 1 to ${Group.Members} {
           /if (${Spawn[${Group.Member[${x}]}].ID} && ${NetBots[${Group.Member[${x}].Name}]}) {
                /if (!${NetBots[${Group.Member[${x}].Name}].Buff[${buffSpell}].ID}) {
                    /call CastBuff "${buffSpell}" ${Group.Member[${x}].ID} "${Group.Member[${x}].Name}"
                } else /if (${NetBots[${Group.Member[${x}].Name}].Buff[${buffSpell}].Duration} < 50) {
                    /call CastBuff "${buffSpell}" ${Group.Member[${x}].ID} "${Group.Member[${x}].Name}"
                }
            }
            /next x
        }
    }
/return

Sub CastBuff(string buffSpell, int buffTargetId, string buffTargetName)
    /if (!${Defined[DontStop]}) /declare DontStop bool local FALSE
    /if (${Me.Casting.ID}) {
        /return
    }

	|- Is spell ready?
	/if (${Me.SpellReady[${buffSpell}]}) {
		|- Do we have mana
		/if (${Me.CurrentMana} < ${Spell[${buffSpell}].Mana}) {		
			/delay 1s
			/gsay !!! NO MANA TO BUFF ${buffTargetName} !!!
			/bca !!! NO MANA TO BUFF ${buffTargetName} !!!
			/return
		} else {			
            /if (${Target.ID} != ${buffTargetId}) {
                /target id ${buffTargetId}
                /delay 2s ${Target.ID} == ${buffTargetId}
                /if (${Target.ID} != ${buffTargetId}) {
                    /return
                }
            }

			|- If right target and in range, cast the spell
			/if (${Target.ID} == ${buffTargetId} && (${Target.Distance} <= ${Spell[${buffSpell}].Range}) && ${Target.Type.NotEqual[Corpse]}) {	
                /if (${Me.Sitting}) {
                    /stand
                }
				/gsay <<< ${buffSpell} -> ${buffTargetName} >>>			
				/bca <<< ${buffSpell} -> ${buffTargetName} >>>
				/call cast "${buffSpell}" gem${Me.Gem[${buffSpell}]}
				/bca MANA: ${Me.PctMana}
			}
		}
	}
/return