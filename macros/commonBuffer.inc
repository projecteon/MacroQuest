| ################################################################################
| # Create Buffer Ini Section
| ################################################################################	
Sub InitBufferIni(string iniFileName)
    /declare options            string  local "BuffOptions"
    /if (!${Defined[iniBuffDimensions]})                /declare iniBuffDimensions              int     outer 3 
    /if (!${Defined[buffNameDimension]})                /declare buffNameDimension              int     outer 1 
    /if (!${Defined[minManaDimensions]})                /declare minManaDimensions              int     outer 2 
    /if (!${Defined[buffClassRestrictionsDimension]})   /declare buffClassRestrictionsDimension int     outer 3 
    /if (!${Defined[minBuffMana]})       /declare minBuffMana       int     outer 80 
    /if (!${Defined[buff1]})             /declare buff1             string  outer "Buff 1" 
        
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

    /call iniBuffs "${iniFileName}" "Buffs"

/return 1

Sub CheckGroupBuffs
    /if (!${Defined[Buffs]}) /return
    /declare k  int local
    /for k 1 to ${Math.Calc[${Buffs.Size}/${iniBuffDimensions}]}
        /call CheckGroupBuff ${Buffs[${k},${buffNameDimension}]} ${Buffs[${k},${minManaDimensions}]} ${Buffs[${k},${buffClassRestrictionsDimension}]}
    /next k 
/return

Sub CheckGroupBuff(string buffSpell, int minMana, string classRestrictions)
    /if (${Me.PctMana} < ${minMana} || ${Me.Casting.ID} || !${Cast.Ready[${buffSpell}]}) {
        /return
    }

    /declare buffId int local ${Spell[${buffSpell}].ID}

    /if ((!${Me.Buff[${buffSpell}].ID} || ${Me.Buff[${buffSpell}].Duration} < 10) && (${classRestrictions.Equal[NULL]} || ${Select[${Me.Class.ShortName},${classRestrictions}]})) {
        /call CastBuff "${buffSpell}" ${Me.ID} "${Me.Name}"
    }

    /if (${Group.Members}) {
        /declare x int local
        /for x 1 to ${Group.Members} {
            /if (${Spawn[${Group.Member[${x}]}].ID} && ${NetBots[${Group.Member[${x}].Name}]} && (${classRestrictions.Equal[NULL]} || ${Select[${Group.Member[${x}].Class.ShortName},${classRestrictions}]})) {
                /if (!${NetBots[${Group.Member[${x}].Name}].Buff.Find[${buffId}]}) {
                    /call CastBuff "${buffSpell}" ${Group.Member[${x}].ID} "${Group.Member[${x}].Name}"
                } else /if (${NetBots[${Group.Member[${x}].Name}].Buff[${buffSpell}].Duration} < 10) {
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
				/call cast "${buffSpell}" gem${Me.Gem[${buffSpell}]} 0 CancelBuff
				/bca MANA: ${Me.PctMana}
			}
		}
	}
/return


Sub iniBuffs(string INIFile, string section)
    /declare Keys       string  local
    /declare k          int     local
    /declare s          int     local

    /varset Keys ${Ini[${INIFile},${section}]}
    :TrimKeys
        /if (${Keys.Right[1].Equal[|]}) /varset Keys ${Keys.Left[${Math.Calc[${Keys.Length}-1]}]}
        
        /if (${Keys.Right[1].Equal[|]}) /goto :TrimKeys

        /declare numberOfBuffs int local ${Math.Calc[${Keys.Count[|]}+1]}
        /if (${numberOfBuffs} < 1) /return

        /declare Buffs[${numberOfBuffs},${iniBuffDimensions}] string outer
        /declare key string local
        /for k 1 to ${Math.Calc[${Keys.Count[|]}+1]}
            /varset key ${Keys.Arg[${k},|]}
            /for s 1 to ${iniBuffDimensions}
                /varset Buffs[${k},${s}] ${Ini[${INIFile},${section},${key}].Token[${s},/]}
            /next s
        /next k
/return

| #################################################
| # Called during the cast of any spell and will
| # interuppt it if any condition pops true.
| #################################################
Sub CancelBuff
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
/return