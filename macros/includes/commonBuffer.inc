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
    /if (!${Defined[doBuffGroup]})                      /declare doBuffGroup                    bool    outer false 
    /if (!${Defined[doBuffNetBots]})                    /declare doBuffNetBots                  bool    outer false 

    /if (${Ini[${iniFileName},${options},DoBuffGroup].Length}) {
       /varset doBuffGroup ${Bool[${Ini[${iniFileName},${options},DoBuffGroup]}]}     
    } else {
        /ini "${iniFileName}" "${options}" "DoBuffGroup" "${doBuffGroup}"
    }

    /if (${Ini[${iniFileName},${options},DoBuffNetBots].Length}) {
       /varset doBuffNetBots ${Bool[${Ini[${iniFileName},${options},DoBuffNetBots]}]}
    } else {
        /ini "${iniFileName}" "${options}" "DoBuffNetBots" "${doBuffNetBots}"
    }

    /call iniBuffs "${iniFileName}" "Buffs" "Buff" ${iniBuffDimensions} Buffs
    /call iniBuffs "${iniFileName}" "SelfBuffs" "SelfBuff" ${iniSelfBuffDimensions} SelfBuffs

    /if (${Buffs.Size} < 1) {
        /varset doBuffGroup false
        /varset doBuffNetBots false
    }

/return 1

Sub doBuffs
    /call CheckSelfBuffs
    /if (${doBuffGroup}) /call CheckGroupBuffs
    /if (${doBuffNetBots}) /call CheckNetBotBuffs
/return

Sub CheckSelfBuffs
    /if (!${Defined[SelfBuffs]}) /return
    /declare k  int local
    /for k 1 to ${Math.Calc[${SelfBuffs.Size}/${iniSelfBuffDimensions}]}
        /declare buffSpell string local ${SelfBuffs[${k},${buffNameDimension}]}
        /call canCastBuff "${buffSpell}" ${SelfBuffs[${k},${minManaDimensions}]}         
        /if (${Macro.Return} == 1 && (!${Me.Buff[${buffSpell}].ID} || ${Me.Buff[${buffSpell}].Duration} < 10)) {
            /call CastBuff "${buffSpell}" ${Me.ID} "${Me.Name}"
        }
    /next k 
/return

Sub CheckGroupBuffs
    /if (!${Defined[Buffs]}) /return
    /if (!${Group.Members}) /return
    /declare k  int local
    /for k 1 to ${Math.Calc[${Buffs.Size}/${iniBuffDimensions}]} {
        /declare buffSpell string local ${Buffs[${k},${buffNameDimension}]}
        /call canCastBuff "${buffSpell}" ${Buffs[${k},${minManaDimensions}]}     
        /if (${Macro.Return} == 1 && ${Spell[${buffSpell}].TargetType.Equal[Single]}) {
            /declare classRestrictions string local ${Buffs[${k},${buffClassRestrictionsDimension}]}
            /declare x int local
            /for x 1 to ${Group.Members} {
                /if (${Spawn[${Group.Member[${x}]}].ID} && ${NetBots[${Group.Member[${x}].Name}]} && (!${Defined[classRestrictions]} || ${classRestrictions.Equal[NULL]} || ${Select[${Group.Member[${x}].Class.ShortName},${classRestrictions}]})) {
                    /if (${SpellTimer[${Group.Member[${x}].ID},any,${buffSpell}]} < 5) {
                        /echo "CheckGroupBuffs ==> CastBuff " "${buffSpell}"
                        /call CastBuff "${buffSpell}" ${Group.Member[${x}].ID} "${Group.Member[${x}].Name}"
                    } 
                }
                /next x
            }
        }
        /next k 
    }
/return

Sub CheckNetBotBuffs
    /if (!${Defined[Buffs]}) /return
    /if (${NetBots.Counts} < 2) /return
    /declare netbotsClient string local
    /declare k  int local
    /for k 1 to ${Math.Calc[${Buffs.Size}/${iniBuffDimensions}]} {
        /declare buffSpell string local ${Buffs[${k},${buffNameDimension}]}
        /call canCastBuff "${buffSpell}" ${Buffs[${k},${minManaDimensions}]}   
        /declare buffId int local ${Spell[${buffSpell}].ID}
        /if (${Macro.Return} == 1 && ${Spell[${buffSpell}].TargetType.Equal[Single]}) {
            /declare classRestrictions string local ${Buffs[${k},${buffClassRestrictionsDimension}]}
            /declare x int local
            /for x 1 to ${NetBots.Counts} {
                /varset netbotsClient "${NetBots.Client[${x}]}"
                /if (${Spawn[${NetBots[${netbotsClient}].Name}].ID} && (!${Defined[classRestrictions]} || ${classRestrictions.Equal[NULL]} || ${Select[${NetBots[${netbotsClient}].Class.ShortName},${classRestrictions}]})) {
                    /if (!${NetBots[${netbotsClient}].Buff.Find[${buffId} ]}) {
                        /call CastBuff "${buffSpell}" ${NetBots[${netbotsClient}].ID} "${NetBots[${netbotsClient}].Name}"
                    } else {
                        /if (!${Defined[i]}) /declare i int local
                        /for i 0 to ${maxBuffsPerPerson}
                            /if (${Bool[${NetBots[${netbotsClient}].Buff[${i}]}]}) {
                                /if (${NetBots[${netbotsClient}].Buff[${i}].ID} == ${buffId} && ${NetBots[${netbotsClient}].Duration[${i}]} < 5) {
                                    /call CastBuff "${buffSpell}" ${NetBots[${netbotsClient}].ID} "${NetBots[${netbotsClient}].Name}"
                                }
                            }
                        /next i
                    }
                }
                /next x
            }
        }
        /next k 
    }
/return

Sub canCastBuff(string buffSpell, int minMana)
    /if (${Me.PctMana} < ${minMana} || ${Me.Casting.ID} || !${Cast.Ready[${buffSpell}]} || ${Spell[${buffSpell}].SpellType.NotEqual[Beneficial]}) {
        /return 0
    }
/return 1

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