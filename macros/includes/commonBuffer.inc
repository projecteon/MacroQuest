| ################################################################################
| # Create Buffer Ini Section
| # https://www.mmobugs.com/forums/index.php?threads/searching-an-ini-file-without-knowing-section-names.9329/
| ################################################################################	
Sub InitBufferIni(string iniFileName)
    /declare options            string  local "BuffOptions"
    /if (!${Defined[maxBuffsPerPerson]})                /declare maxBuffsPerPerson              int     outer 16 
    /if (!${Defined[iniBuffDimensions]})                /declare iniBuffDimensions              int     outer 3 
    /if (!${Defined[buffNameDimension]})                /declare buffNameDimension              int     outer 1 
    /if (!${Defined[minManaDimensions]})                /declare minManaDimensions              int     outer 2 
    /if (!${Defined[buffClassRestrictionsDimension]})   /declare buffClassRestrictionsDimension int     outer 3 
    /if (!${Defined[iniSelfBuffDimensions]})            /declare iniSelfBuffDimensions          int     outer 2
    /if (!${Defined[doBuffGroup]})                      /declare doBuffGroup                    bool    outer false 
    /if (!${Defined[doBuffNetBots]})                    /declare doBuffNetBots                  bool    outer false 
    /if (!${Defined[doBuffsWithNpcInCamp]})             /declare doBuffsWithNpcInCamp           bool    outer FALSE 
    /if (!${Defined[Debug_Group_Buffs]})                /declare Debug_Group_Buffs              bool    outer FALSE
    /if (!${Defined[Debug_NetBots_Buffs]})              /declare Debug_NetBots_Buffs            bool    outer FALSE

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

    /if (${Ini[${iniFileName},${options},DoBuffsWithNpcInCamp].Length}) {
       /varset doBuffsWithNpcInCamp ${Bool[${Ini[${iniFileName},${options},DoBuffsWithNpcInCamp]}]}
    } else {
        /ini "${iniFileName}" "${options}" "DoBuffsWithNpcInCamp" "${doBuffsWithNpcInCamp}"
    }

    /call IniArray "${iniFileName}" "Buffs" "Buff" ${iniBuffDimensions} Buffs
    /call IniArray "${iniFileName}" "SelfBuffs" "SelfBuff" ${iniSelfBuffDimensions} SelfBuffs

    /if (${Buffs.Size} < 1) {
        /varset doBuffGroup false
        /varset doBuffNetBots false
    }

    /if (${Ini[${iniFileName},Debug,Debug_Group_Buffs].Length}) {
       /varset Debug_Group_Buffs ${Bool[${Ini[${iniFileName},Debug,Debug_Group_Buffs]}]}     
    } 

    /if (${Ini[${iniFileName},Debug,Debug_NetBots_Buffs].Length}) {
       /varset Debug_NetBots_Buffs ${Bool[${Ini[${iniFileName},Debug,Debug_NetBots_Buffs]}]}     
    } 

/return 1

Sub DoBuffs
    /if (${Debug}) /echo |- DoBuffs ==>
    
    /if (${Stick.Status.Equal[ON]}) {
        /goto :endDoBuffs
    }

    /if (!${doBuffsWithNpcInCamp}) {
        /call CheckNPCInRange
        /declare hasNpcInCamp bool local ${Bool[${Macro.Return}]}
        /if (${Debug}) /echo |- CheckNPCInRange > ${hasNpcInCamp}
        /if (${hasNpcInCamp}) {
            /if (${Debug}) /echo ${hasNpcInCamp} ${campRadius}
            /goto :endDoBuffs
        }
    }

    /call CheckSelfBuffs
    /if (${doBuffGroup}) /call CheckGroupBuffs
    /if (${doBuffNetBots}) /call CheckNetBotBuffs

    :endDoBuffs
    /if (${Debug}) /echo <== DoBuffs -|
/return

Sub CheckSelfBuffs
    /if (${Debug}) /echo  |- CheckSelfBuffs ==>
    
    /if (!${Defined[SelfBuffs]}) {
        /if (${Debug}) /echo No SelfBuffs defined.        
        /goto :endCheckSelfBuffs
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

    :endCheckSelfBuffs
    /if (${Debug}) /echo <== CheckSelfBuffs -|
/return

Sub CheckGroupBuffs
    /if (${Debug} || ${Debug_Group_Buffs}) /echo  |- CheckGroupBuffs ==>
    
    /if (!${Defined[Buffs]}) {
        /if (${Debug} || ${Debug_Group_Buffs}) /echo No Buffs defined.
        /goto :endCheckGroupBuffs
    }
    
    /if (!${Group.Members}) {
        /if (${Debug} || ${Debug_Group_Buffs}) /echo Not enough in a group. 
        /goto :endCheckGroupBuffs
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
                /if (${Spawn[pc = ${Group.Member[${x}]}].ID} && (!${Defined[classRestrictions]} || ${classRestrictions.Equal[NULL]} || ${Select[${Group.Member[${x}].Class.ShortName},${classRestrictions}]})) {
                    /if (${SpellTimer[${Group.Member[${x}].ID},any,${buffSpell}]} < 5) {
                        /call CastBuff "${buffSpell}" ${Group.Member[${x}].ID} "${Group.Member[${x}].Name}"
                    } 
                }
                /next x
            }
        }
        /next k 
    }

    :endCheckGroupBuffs
    /if (${Debug} || ${Debug_Group_Buffs}) /echo <== CheckGroupBuffs -|
/return

Sub CheckNetBotBuffs
    /if (${Debug} || ${Debug_NetBots_Buffs}) /echo  |- CheckNetBotBuffs ==>
    
    /if (!${Defined[Buffs]}) {
        /if (${Debug} || ${Debug_NetBots_Buffs}) /echo No Buffs defined.
        /goto :endCheckNetBotBuffs
    }
    
    /if (${NetBots.Counts} < 2) {
        /if (${Debug} || ${Debug_NetBots_Buffs}) /echo Not enough Nebots clients, current: ${NetBots.Counts} 
        /goto :endCheckNetBotBuffs
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
                /if (${NetBots[${netbotsClient}].InZone} && (!${Defined[classRestrictions]} || ${classRestrictions.Equal[NULL]} || ${Select[${NetBots[${netbotsClient}].Class.ShortName},${classRestrictions}]})) {
                    /if (!${NetBots[${netbotsClient}].Buff.Find[${buffId} ]}) {
                        /call CastBuff "${buffSpell}" ${NetBots[${netbotsClient}].ID} "${NetBots[${netbotsClient}].Name}"
                    } else {
                        /if (!${Defined[i]}) /declare i int local
                        /for i 0 to ${NetBots[${netbotsClient}].Buff.Length}
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

    :endCheckNetBotBuffs
    /if (${Debug} || ${Debug_NetBots_Buffs}) /echo <== CheckNetBotBuffs -|
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
        /goto :endCastBuff
    }

	|- Is spell ready?
	/if (${Me.SpellReady[${buffSpell}]}) {
		|- Do we have mana
		/if (${Me.CurrentMana} < ${Spell[${buffSpell}].Mana}) {		
			/delay 1s
			/if (${Group.Members}) /gsay !!! NO MANA TO BUFF ${buffTargetName} !!!
			/bca !!! NO MANA TO BUFF ${buffTargetName} !!!
            /goto :endCastBuff
		} else {			
            /if (${Target.ID} != ${buffTargetId}) {
                /target id ${buffTargetId}
                /delay 2s ${Target.ID} == ${buffTargetId}
                /if (${Target.ID} != ${buffTargetId}) {
                    /goto :endCastBuff
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

    :endCastBuff
    /if (${Debug}) /echo <== CastBuff
/return

| #################################################
| # Called during the cast of any spell and will
| # interuppt it if any condition pops true.
| #################################################
Sub CancelBuff(int spellId)
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
/return