| ################################################################################
| # Create Buffer Ini Section
| # https://www.mmobugs.com/forums/index.php?threads/searching-an-ini-file-without-knowing-section-names.9329/
| ################################################################################
Sub InitBufferIni(string iniFileName)
  /declare options            string  local "BuffOptions"
  /if (!${Defined[maxBuffsPerPerson]})                /declare maxBuffsPerPerson              int     outer 16
  /if (!${Defined[doBuffGroup]})                      /declare doBuffGroup                    bool    outer false
  /if (!${Defined[doBuffNetBots]})                    /declare doBuffNetBots                  bool    outer false
  /if (!${Defined[doBuffPets]})                       /declare doBuffPets                     bool    outer false
  /if (!${Defined[doBuffsWithNpcInCamp]})             /declare doBuffsWithNpcInCamp           bool    outer FALSE
  /if (!${Defined[Debug_Group_Buffs]})                /declare Debug_Group_Buffs              bool    outer FALSE
  /if (!${Defined[Debug_NetBots_Buffs]})              /declare Debug_NetBots_Buffs            bool    outer FALSE


  /if (!${Defined[noLevitateZones]})                  /declare noLevitateZones                string  outer "airplane"
  /if (!${Defined[inDoorZone]})                       /declare inDoorZone                     string  outer "befallen blackburrow gukbottom guktop neriaka neriakb neriakc paw permafrost qcat runnyeye soldunga soldungb soltemple akanon kaladima kaladimb kedge kurn"

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

  /if (${Ini[${iniFileName},${options},DoBuffPets].Length}) {
    /varset doBuffPets ${Bool[${Ini[${iniFileName},${options},DoBuffPets]}]}
  } else {
    /ini "${iniFileName}" "${options}" "DoBuffPets" "${doBuffPets}"
  }

  /if (${Ini[${iniFileName},${options},DoBuffsWithNpcInCamp].Length}) {
    /varset doBuffsWithNpcInCamp ${Bool[${Ini[${iniFileName},${options},DoBuffsWithNpcInCamp]}]}
  } else {
    /ini "${iniFileName}" "${options}" "DoBuffsWithNpcInCamp" "${doBuffsWithNpcInCamp}"
  }

  /call InitSpell
  /call IniSpellArray "${iniFileName}" "Buffs" "Buff" Buffs
  /call IniSpellArray "${iniFileName}" "SelfBuffs" "SelfBuff" SelfBuffs
  /call IniSpellArray "${iniFileName}" "PetBuffs" "Buff" PetBuffs

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

  /if (!${doBuffsWithNpcInCamp}) {
    /call CheckNPCInRange
    /declare hasNpcInCamp bool local ${Bool[${Macro.Return}]}
    /if (${Debug}) /echo |- CheckNPCInRange > ${hasNpcInCamp}
    /if (${hasNpcInCamp}) /goto :endDoBuffs
  }

  /call CheckSelfBuffs
  /if (${doBuffGroup}) /call CheckGroupBuffs
  /if (${doBuffNetBots}) /call CheckNetBotBuffs
  /if (${doBuffPets}) /call CheckPetBuffs

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
  /for k 1 to ${Math.Calc[${SelfBuffs.Size}/${spellPropsDimentions}]}
    /varset buffSpell ${SelfBuffs[${k},${spellNameProp}]}
    /call CheckCanCastBuff "${buffSpell}" ${SelfBuffs[${k},${spellMinManaProp}]}
    /if (${Macro.Return} == 1 && !${Me.Buff[${buffSpell}].ID} && ${Spell[${buffSpell}].Stacks} && ${Spell[${buffSpell}].NewStacks}) {
      /if (${Bool[${SelfBuffs[${k},${itemCastProp}]}]}) {
        /call ItemClickBuff "${SelfBuffs[${k},${itemCastProp}]}" "${buffSpell}" ${Me.ID} "${Me.Name}"
      } else /if (${Cast.Ready[${buffSpell}]}) {
        /call CastBuff "${buffSpell}" ${Me.ID} "${Me.Name}"
      }
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
  /for k 1 to ${Math.Calc[${Buffs.Size}/${spellPropsDimentions}]}
    /varset buffSpell ${Buffs[${k},${spellNameProp}]}
    /call CheckCanCastBuff "${buffSpell}" ${Buffs[${k},${spellMinManaProp}]}
    /if (${Macro.Return} == 1 && ${Spell[${buffSpell}].TargetType.Equal[Single]}) {
      /varset classRestrictions ${Buffs[${k},${spellClassRestrictionProp}]}
      /for x 1 to ${Group.Members}
        /if (${Spawn[pc = ${Group.Member[${x}]}].ID} && (${classRestrictions.Length} == 0 || ${Select[${Group.Member[${x}].Class.ShortName},${classRestrictions}]})) {
          /if (${SpellTimer[${Group.Member[${x}].ID},any,${buffSpell}]} < 5) {
            /call CastBuff "${buffSpell}" ${Group.Member[${x}].ID} "${Group.Member[${x}].Name}"
          }
        }
      /next x
    }
  /next k

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
  /for k 1 to ${Math.Calc[${Buffs.Size}/${spellPropsDimentions}]}
    /varset buffSpell ${Buffs[${k},${spellNameProp}]}
    /call CheckCanCastBuff "${buffSpell}" ${Buffs[${k},${spellMinManaProp}]}
    /varset buffId ${Spell[${buffSpell}].ID}
    /if (${Macro.Return} == 1 && ${Spell[${buffSpell}].TargetType.Equal[Single]}) {
      /varset classRestrictions ${Buffs[${k},${spellClassRestrictionProp}]}
      /for x 1 to ${NetBots.Counts}
        /varset netbotsClient "${NetBots.Client[${x}]}"
        /if (${NetBots[${netbotsClient}].InZone} && (${classRestrictions.Length} == 0 || ${Select[${NetBots[${netbotsClient}].Class.ShortName},${classRestrictions}]})) {
          /if (!${NetBots[${netbotsClient}].Buff.Find[${buffId} ]} && ${NetBots[${netbotsClient}].Stacks[${buffId}]}) {
            /if (${Debug} || ${Debug_NetBots_Buffs}) /echo Buff missing, recasting

            /if (${Bool[${Buffs[${k},${itemCastProp}]}]}) {
              /call ItemClickBuff "${Buffs[${k},${itemCastProp}]}" "${buffSpell}" ${NetBots[${netbotsClient}].ID} "${NetBots[${netbotsClient}].Name}"
            } else /if (${Cast.Ready[${buffSpell}]}) {
              /call CastBuff "${buffSpell}" ${NetBots[${netbotsClient}].ID} "${NetBots[${netbotsClient}].Name}"
            }
          }
        }
      /next x
    }
  /next k

  :endCheckNetBotBuffs
  /if (${Debug} || ${Debug_NetBots_Buffs}) /echo <== CheckNetBotBuffs -|
/return

Sub CheckPetBuffs
  /if (${Debug} || ${Debug_Buffs}) /echo  |- CheckPetBuffs ==>

  /if (!${Defined[PetBuffs]}) {
    /if (${Debug} || ${Debug_Buffs}) /echo No PetBuffs defined.
    /goto :endCheckPetBuffs
  }

  /if (${NetBots.Counts} < 2) {
    /if (${Debug} || ${Debug_Buffs}) /echo Not enough Nebots clients, current: ${NetBots.Counts}
    /goto :endCheckPetBuffs
  }

  /declare buffSpell string local
  /declare buffId int local
  /declare netbotsClient string local
  /declare k  int local
  /declare x int local
  /for k 1 to ${PetBuffs.Size}
    /varset buffSpell ${PetBuffs[${k}]}
    /varset buffId ${Spell[${buffSpell}].ID}
    /if (${Spell[${buffSpell}].TargetType.Equal[Single]}) {
      /for x 1 to ${NetBots.Counts}
        /varset netbotsClient "${NetBots.Client[${x}]}"
        /if (${NetBots[${netbotsClient}].InZone} && ${NetBots[${netbotsClient}].PetID}) {
          /if (!${NetBots[${netbotsClient}].PetBuff.Find[${buffId} ]}) {
            /if (${Debug} || ${Debug_Buffs}) /echo Pet buff missing, recasting
            /call CastBuff "${buffSpell}" ${NetBots[${netbotsClient}].PetID} "${netbotsClient}'s' Pet"
          }
        }
      /next x
    }
  /next k

  :endCheckPetBuffs
  /if (${Debug} || ${Debug_Buffs}) /echo <== CheckPetBuffs -|
/return

Sub CheckCanCastBuff(string buffSpell, int minMana)
  /if (${Debug}) /echo  |- CheckCanCastBuff ==> ${buffSpell} ${minMana}

  /if ((${minMana} > 0 && ${Me.PctMana} < ${minMana}) || ${Me.Casting.ID} || ${Spell[${buffSpell}].SpellType.NotEqual[Beneficial]}) {
    /if (${Debug}) /echo <== CheckCanCastBuff 0 -|
    /return 0
  }

  /if (${Select[${buffSpell},Spirit of Wolf,Spirit of Eagle,Flight of Eagles]} && ${Select[${Zone.ShortName},${inDoorZone}]}) {
    /if (${Debug}) /echo <== CheckCanCastBuff 0 > cant cast indoor -|
    /return 0
  }

  /if (${Select[${buffSpell},Spirit of Eagle,Flight of Eagles,Levitate,Levitation,Dead Man Floating,Dead Men Floating]} && ${Select[${Zone.ShortName},${noLevitateZones}]}) {
    /if (${Debug}) /echo <== CheckCanCastBuff 0 > cant cast levitate -|
      /return 0
  }

  /if (${Debug}) /echo <== CheckCanCastBuff 1 -|
/return 1

Sub ItemClickBuff(string itemClick, string buffSpell, int buffTargetId, string buffTargetName)
  /if (${Debug}) /echo  |- ItemClickBuff ==> ${itemClick} -> ${buffTargetName}

  /if (${Me.Casting.ID}) {
    /goto :endItemClickBuff
  }

  /if (${Target.ID} != ${buffTargetId}) {
    /call EnsureTarget ${buffTargetId}
    /if (${Target.ID} != ${buffTargetId}) {
      /goto :endItemClickBuff
    }
  }

  /if (${Target.ID} == ${buffTargetId} && (${Target.Distance} <= ${Spell[${buffSpell}].Range}) && ${Target.Type.NotEqual[Corpse]}) {
    /if (${Me.Sitting}) {
      /stand
    }

    /bca <<< ${itemClick} -> ${buffSpell} -> ${buffTargetName} >>>
    /casting "${itemClick}"
    /delay 30 ${Me.Casting.ID}
  }

  :endItemClickBuff
  /if (${Debug}) /echo <== ItemClickBuff
/return

Sub CastBuff(string buffSpell, int buffTargetId, string buffTargetName)
  /if (${Debug}) /echo  |- CastBuff ==> ${buffSpell} -> ${buffTargetName}

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
        /call EnsureTarget ${buffTargetId}
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