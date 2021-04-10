| ################################################################################
| # Create Buffer Ini Section
| https://www.mmobugs.com/forums/index.php?threads/searching-an-ini-file-without-knowing-section-names.9329/
| ################################################################################	
Sub InitBufferIni(string iniFileName)
    /declare options            string  local "BuffOptions"
    /if (!${Defined[maxBuffsPerPerson]})                /declare maxBuffsPerPerson              int     outer 15 
    /if (!${Defined[iniBuffDimensions]})                /declare iniBuffDimensions              int     outer 3 
    /if (!${Defined[buffNameDimension]})                /declare buffNameDimension              int     outer 1 
    /if (!${Defined[minManaDimensions]})                /declare minManaDimensions              int     outer 2 
    /if (!${Defined[buffClassRestrictionsDimension]})   /declare buffClassRestrictionsDimension int     outer 3 
    /if (!${Defined[iniSelfBuffDimensions]})            /declare iniSelfBuffDimensions          int     outer 2
    /if (!${Defined[minBuffMana]})                      /declare minBuffMana                    int     outer 80 

    /if (${Ini[${iniFileName},${options},MinMana].Length}) {
       /varset minBuffMana ${Ini[${iniFileName},${options},MinMana]}     
    } else {
        /ini "${iniFileName}" "${options}" "MinMana" "${minBuffMana}"
        /return 0
    }

    /call iniBuffs "${iniFileName}" "Buffs" "Buff" ${iniBuffDimensions} Buffs
    /call iniBuffs "${iniFileName}" "SelfBuffs" "SelfBuff" ${iniSelfBuffDimensions} SelfBuffs

/return 1

Sub doBuffs
    /call CheckSelfBuffs
    /call CheckGroupBuffs
/return

Sub CheckSelfBuffs
    /if (!${Defined[SelfBuffs]}) /return
    /declare k  int local
    /for k 1 to ${Math.Calc[${SelfBuffs.Size}/${iniSelfBuffDimensions}]}
        /call tryDoBuff "${SelfBuffs[${k},${buffNameDimension}]}" ${SelfBuffs[${k},${minManaDimensions}]}
    /next k 
/return

Sub CheckGroupBuffs
    /if (!${Defined[Buffs]}) /return
    /declare k  int local
    /for k 1 to ${Math.Calc[${Buffs.Size}/${iniBuffDimensions}]}
        /call tryDoBuff "${Buffs[${k},${buffNameDimension}]}" ${Buffs[${k},${minManaDimensions}]} ${Buffs[${k},${buffClassRestrictionsDimension}]}
    /next k 
/return

Sub tryDoBuff(string buffSpell, int minMana, string classRestrictions)
    /if (${Me.PctMana} < ${minMana} || ${Me.Casting.ID} || !${Cast.Ready[${buffSpell}]} || ${Spell[${buffSpell}].SpellType.NotEqual[Beneficial]}) {
        /return
    }

    /declare buffId int local ${Spell[${buffSpell}].ID}
    /if ((!${Me.Buff[${buffSpell}].ID} || ${Me.Buff[${buffSpell}].Duration} < 10) && (!${Defined[classRestrictions]} || ${classRestrictions.Equal[NULL]} || ${Select[${Me.Class.ShortName},${classRestrictions}]})) {
        /call CastBuff "${buffSpell}" ${Me.ID} "${Me.Name}"
    }

    /if (${Spell[${buffSpell}].TargetType.Equal[Single]} && ${Group.Members}) {
        /declare x int local
        /for x 1 to ${Group.Members} {
            /if (${Spawn[${Group.Member[${x}]}].ID} && ${NetBots[${Group.Member[${x}].Name}]} && (!${Defined[classRestrictions]} || ${classRestrictions.Equal[NULL]} || ${Select[${Group.Member[${x}].Class.ShortName},${classRestrictions}]})) {
                /if (!${NetBots[${Group.Member[${x}].Name}].Buff.Find[${buffId} ]}) {
                    /call CastBuff "${buffSpell}" ${Group.Member[${x}].ID} "${Group.Member[${x}].Name}"
                } else {
                    /if (!${Defined[i]}) /declare i int local
                    /for i 0 to ${maxBuffsPerPerson}
                        /if (${Bool[${NetBots[${Group.Member[${x}].Name}].Buff[${i}]}]}) {
                            /if (${NetBots[${Group.Member[${x}].Name}].Buff[${i}].ID} == ${buffId} && ${NetBots[${Group.Member[${x}].Name}].Duration[${i}]} < 5) {
                                /call CastBuff "${buffSpell}" ${Group.Member[${x}].ID} "${Group.Member[${x}].Name}"
                            }
                        }
                    /next i
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
			/if (${Group.Members}) /gsay !!! NO MANA TO BUFF ${buffTargetName} !!!
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
				/if (${Group.Members}) /gsay <<< ${buffSpell} -> ${buffTargetName} >>>			
				/bca <<< ${buffSpell} -> ${buffTargetName} >>>
				/call cast "${buffSpell}" gem${Me.Gem[${buffSpell}]} 0 CancelBuff
				/bca MANA: ${Me.PctMana}
			}
		}
	}
/return

Sub iniBuffs(string INIFile, string section, string keyName, int arrayDimensions, arrayName)
    /declare Keys       string  local
    /declare k          int     local
    /declare s          int     local

    /varset Keys ${Ini[${INIFile},${section}]}
    :TrimKeys
        /if (${Keys.Right[1].Equal[|]}) /varset Keys ${Keys.Left[${Math.Calc[${Keys.Length}-1]}]}
        
        /if (${Keys.Right[1].Equal[|]}) /goto :TrimKeys

        /declare numberOfBuffs int local ${Math.Calc[${Keys.Count[|]}+1]}
        /if (${numberOfBuffs} < 1) /return

        /if (${Defined[${arrayName}]}) /deletevar ${arrayName}
        /declare ${arrayName}[${numberOfBuffs},${arrayDimensions}] string outer

        /declare key string local
        /for k 1 to ${Math.Calc[${Keys.Count[|]}+1]}
            /varset key ${Keys.Arg[${k},|]}
            /for s 1 to ${arrayDimensions}
                /varset ${arrayName}[${k},${s}] ${Ini[${INIFile},${section},${key}].Token[${s},/]}
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