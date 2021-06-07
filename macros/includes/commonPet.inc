Sub InitPetIni(string iniFileName)
    /declare petOptions         string  local "PetOptions"
    /declare petInitResult      int     local 1
    /declare petSpell           string  outer 
    /declare petAssistPct       int     outer 
    /declare petWeaponsSpell    string  outer 
    /declare petTaunt           bool    outer false
    /declare currentPetTargetId int     outer 0

    /if (${Ini[${iniFileName},${petOptions},"Pet Spell"].Length}) {
        /varset petSpell ${Ini[${iniFileName},${petOptions},"Pet Spell"]}
    } else {
        /ini "${iniFileName}" "${petOptions}" "Pet Spell" "${petSpell}"
    }

    /if (${Ini[${iniFileName},${petOptions},"Pet AssistPct"].Length}) {
        /varset petAssistPct ${Ini[${iniFileName},${petOptions},"Pet TaAssistPctunt"]}
    } else {
        /ini "${iniFileName}" "${petOptions}" "Pet AssistPct" "${petAssistPct}"
    }

    /if (${Ini[${iniFileName},${petOptions},"Pet Weapons Spell"].Length}) {
        /varset petWeaponsSpell ${Ini[${iniFileName},${petOptions},"Pet Weapons Spell"]}
    } else {
        /ini "${iniFileName}" "${petOptions}" "Pet Weapons Spell" "${petWeaponsSpell}"
    }

    /if (${Ini[${iniFileName},${petOptions},"Pet Taunt"].Length}) {
        /varset petTaunt ${Bool[${Ini[${iniFileName},${petOptions},"Pet Taunt"]}]}
    } else {
        /ini "${iniFileName}" "${petOptions}" "Pet Taunt" "${petTaunt}"
    }
/return 1

Sub DoPet
    /if (${Debug}) /echo |- DoPet ==>

    /if (!${Me.Pet.ID}) {
        /if (${Debug}) /echo <== DoPet -|
        /return
    }

    /if (${currentPetTargetId} > 0 && (${Spawn[${currentPetTargetId}].Type.Equal[Corpse]} || !${SpawnCount[id ${currentPetTargetId}]})) {
        /pet back off
        /varset currentPetTargetId 0
    } else /if (${currentPetTargetId} > 0) {
        /if (${Debug}) /echo <== DoPet -|
        /return
    }

    /declare targetId int local ${NetBots[${mainAssist}].TargetID}
    /if (!${targetId}) {
        /if (${Debug}) /echo <== DoPet -|
        /return
    }

    /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
    /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
    /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
    /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

    /if ((!${isNPC} && !${isPet}) || (${targetHP} > ${assistPct} && ${targetHP} > ${petAssistPct}) || !${hasLineOfSight} || ${Spawn[${targetId}].Distance} > 100) {
        /if (${Debug}) /echo <== DoPet -|
        /return
    }

    /if (!${Target.ID} || ${Target.ID}!=${targetId}) {
	    /target id ${targetId}
        /delay 1s ${Target.ID}==${targetId}
    }

    /if (!${Target.ID} || ${Target.ID}!=${targetId}) {
        /if (${Debug}) /echo <== DoPet -|
        /return
    }

    /varset currentPetTargetId ${targetId}
    /pet back off
    /pet attack
    
    /if (${Debug}) /echo <== DoPet -|
/return

Sub SummonPet
    /if (!${Me.Gem[${petSpell}]}) {
        /echo !!! SPELL NOT MEMORIZED ${petSpell} - SHUTDOWN !!!
        /bca !!! SPELL NOT MEMORIZED ${petSpell} - SHUTDOWN !!!
        /endmac
    }

    |handle zone delay to prevent summoning pet if one exists
    | /if (!${Bool[${Zone.ID}]} || ${currentZone} != ${Zone.ID} || ${SpawnCount[pc = ${Me}]}==0) {
    |     /delay 3s
    |     /call check_Zone
    |     /return
    | }

    /if (!${Me.Pet.ID} && !${Me.Casting} && ${Me.SpellReady[${petSpell}]} && ${Me.CurrentMana} > ${Spell[${petSpell}].Mana}) {
        /call cast ${petSpell}
    }

    /if (${Me.Pet.ID} && ${petTaunt}) {
        /pet taunt on
    } else {
        /pet taunt off
    }

    /call weaponizePet
/RETURN



Sub WeaponizePet
    /if (!${Me.Gem[${petWeaponsSpell}]}) {
        /echo !!! SPELL NOT MEMORIZED ${petWeaponsSpell} - SHUTDOWN !!!
        /bca !!! SPELL NOT MEMORIZED ${petWeaponsSpell} - SHUTDOWN !!!
        /endmac
    }

    /if (!${Me.Pet.ID} || ${Me.Casting} || !${Me.SpellReady[${petSpell}]} || ${Me.CurrentMana} < ${Spell[${petSpell}].Mana}) {
        /return
    }

    /if (!${Target.ID} || ${Target.ID}!=${Pet.ID}) {
	    /target id ${Pet.ID}
        /delay 1s ${Target.ID}==${Pet.ID}
    }

    /declare k  int local
    /for k 1 to 2 {
        /call cast ${petWeaponsSpell}
        /delay 3
        /click left target
        /delay 1s ${Window[GiveWnd].Open}
        /if (${Window[GiveWnd].Open}) {
            /notify GiveWnd GVW_Give_Button LeftMouseUp
            /delay 5s !${Window[GiveWnd].Open}
        }
        /next k
    }
/RETURN

Sub MakePetIni(string fileName)
    |- Build the INI file
    /echo Building Pet INI file.....

    | /ini "${fileName}" "${petOptions}" "AttackPct" "${assistPct}"
/return