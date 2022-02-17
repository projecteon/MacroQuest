| ################################################################################
| # Create DeBuffer Ini Section
| https://www.redguides.com/community/resources/mq2cast.290/
| ################################################################################
Sub InitDebufferIni(string iniFileName)
  /declare options            string  local "DeBuffOptions"
  /if (!${Defined[debuffMinRefreshTimer]})   /declare debuffMinRefreshTimer   int     outer 10
  /if (!${Defined[Debug_DeBuffs]})           /declare Debug_DeBuffs           bool    outer FALSE

  /call InitSpell
  /call IniSpellArray "${iniFileName}" "DeBuffs" "DeBuff" DeBuffs

  /if (${Ini[${iniFileName},Debug,Debug_DeBuffs].Length}) {
    /varset Debug_DeBuffs ${Bool[${Ini[${iniFileName},Debug,Debug_DeBuffs]}]}
  }
/return 1

Sub DoDeBuffs
  /if (${Debug} || ${Debug_DeBuffs}) /echo |- DoDeBuffs ==>

  /call CheckDeBuffs

  /if (${Debug} || ${Debug_DeBuffs}) /echo <== DoDeBuffs -|
/return

Sub CheckDeBuffs
  /if (${Debug} || ${Debug_DeBuffs}) /echo |- CheckDeBuffs ==>

  /if (!${Defined[DeBuffs]}) {
    /if (${Debug} || ${Debug_DeBuffs}) /echo No DeBuffs defined.
    /if (${Debug} || ${Debug_DeBuffs}) /echo <== CheckDeBuffs -|
    /return
  }

  /declare k  int local
  /for k 1 to ${Math.Calc[${DeBuffs.Size}/${spellPropsDimentions}]}
    /call TryDoDebuff "${DeBuffs[${k},${spellNameProp}]}" ${DeBuffs[${k},${spellMinTargetHpProp}]} ${DeBuffs[${k},${spellMinManaProp}]} ${debuffMinRefreshTimer}
  /next k

  /if (${Debug} || ${Debug_DeBuffs}) /echo <== CheckDeBuffs -|
/return

Sub TryDoDebuff(string debuffSpell, int minPctHp, int minMana, int remainingTime)
  /if (${Debug} || ${Debug_DeBuffs}) /echo |- TryDoDebuff ==>

  /call GetMainAssist
  /declare mainAssist string	local ${Macro.Return}

  /if (${mainAssist.Equal[NULL]}) {
    /if (${Debug} || ${Debug_Heals}) /echo |- No main assist defined
    /if (${Debug} || ${Debug_DeBuffs}) /echo <== TryDoDebuff -|
    /return 0
  }

  /declare targetId int local ${NetBots[${mainAssist}].TargetID}
  /if (!${targetId}) {
    /if (${Debug} || ${Debug_DeBuffs}) /echo <== TryDoDebuff -|
    /return 0
  }

  /if (${Me.Casting.ID} || ${Me.Moving} || !${Me.Gem[${debuffSpell}]} || ${minMana} > ${Me.PctMana} || ${Me.CurrentMana} < ${Spell[${debuffSpell}].Mana}) {
    /if (${Debug} || ${Debug_DeBuffs}) /echo <== TryDoDebuff -|
    /return 0
  }

  /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
  /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
  /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
  /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

  /if ((!${isNPC} && !${isPet}) || ${targetHP} > ${minPctHp} || !${hasLineOfSight} || ${SpellTimer[${targetId},any,${debuffSpell}]} > ${remainingTime}) {
    /if (${Debug} || ${Debug_DeBuffs}) /echo <== TryDoDebuff -|
    /return 0
  }

  /target id ${targetId}
  /delay 2s ${Target.ID}==${targetId}
  /if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}!=${targetId}) {
    /if (${Debug} || ${Debug_DeBuffs}) /echo <== TryDoDebuff -|
    /return 0
  }

  /if (!${Cast.Ready[${debuffSpell}]} || ${Target.Distance} > ${Spell[${debuffSpell}].Range}) {
    /if (${Debug} || ${Debug_DeBuffs}) /echo <== TryDoDebuff -|
    /return 0
  }

  /if (${Me.Sitting}) {
    /stand
  }

  /call cast "${debuffSpell}" gem${Me.Gem[${debuffSpell}]} 0 CancelDebuff 3
  /if (${castReturn.Equal[CAST_IMMUNE]}) {
    /declare targetName string  local ${Spawn[${targetId}].Name}
    /bca ${debuffSpell} " immune - " ${Target.Name}
    | /ini "${mesmerizeSettingsFileName}" "Immune" "${targetName}" "TRUE"
  } else /if (${castReturn.Equal[CAST_RESISTED]}) {
    /bca ${debuffSpell} " resited 3 times, retrying next run - " ${Target.Name}
  } else {
    /bca ${Target.Name} debuffed with ${debuffSpell}
  }

  /if (${Debug} || ${Debug_DeBuffs}) /echo <== TryDoDebuff -|
/return 1

Sub CancelDebuff(int spellId)
  /if (!${Target.ID}) /call Interrupt
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
/return
