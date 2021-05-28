| ################################################################################
| # Create Buffer Ini Section
| # https://www.mmobugs.com/forums/index.php?threads/searching-an-ini-file-without-knowing-section-names.9329/
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

    /call IniArray "${iniFileName}" "Buffs" "Buff" ${iniBuffDimensions} Buffs
    /call IniArray "${iniFileName}" "SelfBuffs" "SelfBuff" ${iniSelfBuffDimensions} SelfBuffs

    /if (${Buffs.Size} < 1) {
        /varset doBuffGroup false
        /varset doBuffNetBots false
    }

/return 1

Sub DoBuffs
    /if (${Debug}) /echo |- DoBuffs ==>
    
    /call CheckNPCInRange
    /declare hasNpcInCamp bool local ${Bool[${Macro.Return}]}
    /if (${Debug}) /echo |- CheckNPCInRange > ${hasNpcInCamp}
    /if (${hasNpcInCamp}) /return

    /call CheckSelfBuffs
    /if (${doBuffGroup}) /call CheckGroupBuffs
    /if (${doBuffNetBots}) /call CheckNetBotBuffs

    /if (${Debug}) /echo <== DoBuffs -|
/return

Sub CheckSelfBuffs
    /if (${Debug}) /echo  |- CheckSelfBuffs ==>
    
    /if (!${Defined[SelfBuffs]}) {
        /if (${Debug}) /echo No SelfBuffs defined.
        /if (${Debug}) /echo <== CheckSelfBuffs -|
        /return
    }
    
    /declare buffSpell string local
    /declare k  int local
    /for k 1 to ${Math.Calc[${SelfBuffs.Size}/${iniSelfBuffDimensions}]}
        /varset buffSpell ${SelfBuffs[${k},${buffNameDimension}]}
        /call CanCastBuff "${buffSpell}" ${SelfBuffs[${k},${minManaDimensions}]}         
        /if (${Macro.Return} == 1 && (!${Me.Buff[${buffSpell}].ID} || ${Me.Buff[${buffSpell}].Duration} < 10)) {
            /call CastBuff "${buffSpell}" ${Me.ID} "${Me.Name}"
        }
    /next k 

    /if (${Debug}) /echo <== CheckSelfBuffs -|
/return

Sub CheckGroupBuffs
    /if (${Debug}) /echo  |- CheckGroupBuffs ==>
    
    /if (!${Defined[Buffs]}) {
        /if (${Debug}) /echo No Buffs defined.
        /if (${Debug}) /echo <== CheckGroupBuffs -|
        /return
    }
    
    /if (!${Group.Members}) {
        /if (${Debug}) /echo Not enough in a group. 
        /if (${Debug}) /echo <== CheckGroupBuffs -|
        /return
    }

    /declare buffSpell string local
    /declare classRestrictions string local
    /declare k  int local
    /declare x int local
    /for k 1 to ${Math.Calc[${Buffs.Size}/${iniBuffDimensions}]} {
        /varset buffSpell ${Buffs[${k},${buffNameDimension}]}
        /call CanCastBuff "${buffSpell}" ${Buffs[${k},${minManaDimensions}]}     
        /if (${Macro.Return} == 1 && ${Spell[${buffSpell}].TargetType.Equal[Single]}) {
            /varset classRestrictions ${Buffs[${k},${buffClassRestrictionsDimension}]}
            /for x 1 to ${Group.Members} {      
                /if (${Spawn[${Group.Member[${x}]}].ID} && (!${Defined[classRestrictions]} || ${classRestrictions.Equal[NULL]} || ${Select[${Group.Member[${x}].Class.ShortName},${classRestrictions}]})) {
                    /if (${SpellTimer[${Group.Member[${x}].ID},any,${buffSpell}]} < 5) {
                        /call CastBuff "${buffSpell}" ${Group.Member[${x}].ID} "${Group.Member[${x}].Name}"
                    } 
                }
                /next x
            }
        }
        /next k 
    }

    /if (${Debug}) /echo <== CheckGroupBuffs -|
/return

Sub CheckNetBotBuffs
    /if (${Debug}) /echo  |- CheckNetBotBuffs ==>
    
    /if (!${Defined[Buffs]}) {
        /if (${Debug}) /echo No Buffs defined.
        /if (${Debug}) /echo <== CheckNetBotBuffs -|
        /return
    }
    
    /if (${NetBots.Counts} < 2) {
        /if (${Debug}) /echo Not enough Nebots clients, current: ${NetBots.Counts} 
        /if (${Debug}) /echo <== CheckNetBotBuffs -|
        /return
    }
    
    /declare buffSpell string local
    /declare buffId int local
    /declare classRestrictions string local
    /declare netbotsClient string local
    /declare k  int local
    /declare x int local
    /for k 1 to ${Math.Calc[${Buffs.Size}/${iniBuffDimensions}]} {
        /varset buffSpell ${Buffs[${k},${buffNameDimension}]}
        /call CanCastBuff "${buffSpell}" ${Buffs[${k},${minManaDimensions}]}   
        /varset buffId ${Spell[${buffSpell}].ID}
        /if (${Macro.Return} == 1 && ${Spell[${buffSpell}].TargetType.Equal[Single]}) {
            /varset classRestrictions ${Buffs[${k},${buffClassRestrictionsDimension}]}
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

    /if (${Debug}) /echo <== CheckNetBotBuffs -|
/return

Sub CanCastBuff(string buffSpell, int minMana)
    /if (${Debug}) /echo  |- CanCastBuff ==> ${buffSpell} ${minMana}
    
    /if (${Me.PctMana} < ${minMana} || ${Me.Casting.ID} || !${Cast.Ready[${buffSpell}]} || ${Spell[${buffSpell}].SpellType.NotEqual[Beneficial]}) {
        /if (${Debug}) /echo <== CanCastBuff 0 -|
        /return 0
    }

    /if (${Debug}) /echo <== CanCastBuff 1 -|
/return 1

Sub CastBuff(string buffSpell, int buffTargetId, string buffTargetName)
    /if (${Debug}) /echo  |- CastBuff ==> ${buffSpell} -> ${buffTargetName}

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

| #################################################
| # Called during the cast of any spell and will
| # interuppt it if any condition pops true.
| #################################################
Sub CancelBuff(int spellId)
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
/return