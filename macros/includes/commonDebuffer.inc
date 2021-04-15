| ################################################################################
| # Create DeBuffer Ini Section
| https://www.redguides.com/community/resources/mq2cast.290/
| ################################################################################	
Sub InitDebufferIni(string iniFileName)
    /declare options            string  local "DeBuffOptions"
    /if (!${Defined[debuffDimensions]})         /declare debuffDimensions       int     outer 4 
    /if (!${Defined[debuffNameDimension]})      /declare debuffNameDimension    int     outer 1 
    /if (!${Defined[debuffPctHpDimension]})    /declare debuffPctHpDimension    int     outer 2 
    /if (!${Defined[debuffManaDimension]})     /declare debuffManaDimension     int     outer 3 
    /if (!${Defined[debuffTimeDimension]})     /declare debuffTimeDimension     int     outer 4 

    /call IniArray "${iniFileName}" "DeBuffs" "DeBuff" ${debuffDimensions} DeBuffs

/return 1

Sub doDeBuffs
    /call CheckDeBuffs
/return

Sub CheckDeBuffs
    /if (!${Defined[DeBuffs]}) /return
    /declare k  int local
    /for k 1 to ${Math.Calc[${DeBuffs.Size}/${debuffDimensions}]}
        /call tryDoDebuff "${DeBuffs[${k},${debuffNameDimension}]}" ${DeBuffs[${k},${debuffPctHpDimension}]} ${DeBuffs[${k},${debuffManaDimension}]} ${DeBuffs[${k},${debuffTimeDimension}]}
    /next k 
/return

Sub tryDoDebuff(string debuffSpell, int minPctHp, int minMana, int remainingTime)
    /if (${Me.Casting.ID} || ${Me.Moving} || !${Me.Gem[${debuffSpell}]} || ${minMana} > ${Spell[${debuffSpell}].Mana}) {
        /return 0
    }

    /declare targetId int local ${NetBots[${mainAssist}].TargetID}
    /if (!${targetId}) {
        /return 0
    }
    
    /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
    /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
    /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
    /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

    /if ((!${isNPC} && !${isPet}) || ${targetHP} > ${minPctHp} || !${hasLineOfSight} || ${SpellTimer[${targetId},any,${debuffSpell}]} > ${remainingTime}) {
        /return 0
    }

    /target id ${targetId}
    /if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}!=${targetId}) {
        /return 0
    }

    /if (!${Cast.Ready[${debuffSpell}]} || ${Target.Distance} > ${Spell[${debuffSpell}].Range}) {
        /return 0
    }

    /if (${Me.Sitting}) {
        /stand
    }

    /call cast "${debuffSpell}" gem${Me.Gem[${debuffSpell}]} 0 CancelDebuff
    /bca ${Target.Name} " debuffed with " ${debuffSpell}
/return 1

| #################################################
| # Called during the cast of any spell and will
| # interuppt it if any condition pops true.
| #################################################
Sub CancelDebuff
    /if (!${Target.ID}) /call Interrupt
	/if (${Target.Type.Equal[Corpse]}) /call Interrupt
/return
