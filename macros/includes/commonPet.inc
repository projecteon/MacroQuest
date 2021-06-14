Sub InitPetIni(string iniFileName)
    /declare petOptions         string  local "PetOptions"
    /declare petInitResult      int     local 1
    /declare petSpell           string  outer 
    /declare petAssistPct       int     outer 
    /declare petWeaponsSpell    string  outer 
    /declare petTaunt           bool    outer false
    /declare currentPetTargetId int     outer 0
    /declare Debug_Pet          bool    outer FALSE

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

    /if (${Ini[${iniFileName},Debug,Debug_Pet].Length}) {
       /varset Debug_Pet ${Bool[${Ini[${iniFileName},Debug,Debug_Pet]}]}     
    } 

/return 1

Sub DoPet
    /if (${Debug} || ${Debug_Pet}) /echo |- DoPet ==>

    /if (!${Me.Pet.ID}) {
        /if (${Debug} || ${Debug_Pet}) /echo |- Has no pet
        /goto :endDoPet
    }

    /if (${currentPetTargetId} > 0 && (${Spawn[${currentPetTargetId}].Type.Equal[Corpse]} || !${SpawnCount[id ${currentPetTargetId}]})) {
        /pet back off
        /varset currentPetTargetId 0
    } else /if (${currentPetTargetId} > 0) {
        /goto :endDoPet
    }

    /declare targetId int local ${NetBots[${mainAssist}].TargetID}
    /if (!${targetId}) {
        /if (${Debug} || ${Debug_Pet}) /echo <== DoPet -|
        /goto :endDoPet
    }

    /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
    /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
    /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
    /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

    /if ((!${isNPC} && !${isPet}) || (${targetHP} > ${assistPct} && ${targetHP} > ${petAssistPct}) || !${hasLineOfSight} || ${Spawn[${targetId}].Distance} > 100) {
        /if (${Debug} || ${Debug_Pet}) /echo |- Not a valid target for pet
        /goto :endDoPet
    }

    /call EnsureTarget ${targetId}

    /if (!${Target.ID} || ${Target.ID}!=${targetId}) {
        /if (${Debug} || ${Debug_Pet}) /echo |- Not correct target
        /goto :endDoPet
    }

    /varset currentPetTargetId ${targetId}
    /pet back off
    /pet attack
    
    :endDoPet
    /if (${Debug} || ${Debug_Pet}) /echo <== DoPet -|
/return

Sub SummonPet
    /if (${Debug} || ${Debug_Pet}) /echo |- SummonPet

    /if (!${Me.Gem[${petSpell}]}) {
        /echo !!! SPELL NOT MEMORIZED <<${petSpell}>> !!!
        /bca !!! SPELL NOT MEMORIZED <<${petSpell}>> !!!
        /goto :endSummonPet
    }

    |handle zone delay to prevent summoning pet if one exists
    | /if (!${Bool[${Zone.ID}]} || ${currentZone} != ${Zone.ID} || ${SpawnCount[pc = ${Me}]}==0) {
    |     /delay 3s
    |     /call check_Zone
    |     /return
    | }

    /if (!${Bool[${Me.Pet.ID}]} && !${Bool[${Me.Casting}]} && ${Me.SpellReady[${petSpell}]} && ${Me.CurrentMana} > ${Spell[${petSpell}].Mana}) {
        /call cast "${petSpell}"
    }

    /delay 1s ${Me.Pet.ID}
    /if (${Me.Pet.ID} && ${petTaunt}) {
        /pet taunt on
    } else /if (${Me.Pet.ID}) {
        /pet taunt off
    }
    
    :endSummonPet
    /if (${Debug} || ${Debug_Pet}) /echo <== SummonPet -|
/return



Sub WeaponizePet(int petID)
    /if (${Debug} || ${Debug_Pet}) /echo |- WeaponizePet ==> ${petID} 

    /if (!${Bool[${petWeaponsSpell}]}) {
        /goto :endWeaponizePet
    }

    /if (!${Me.Gem[${petWeaponsSpell}]}) {
        /echo !!! SPELL NOT MEMORIZED <<${petWeaponsSpell}>> !!!
        /bca !!! SPELL NOT MEMORIZED <<${petWeaponsSpell}>> !!!
        /goto :endWeaponizePet
    }

    /if (!${Defined[petID]}) {
        /declare petID int local ${Me.Pet.ID} 
    } else /if (${petID} == 0) {
        /varset petID ${Me.Pet.ID} 
    }

    

    /if (!${petID} && !${Spawn[pet id ${petID}].ID}) {  
        /echo No pet to weaponize, missing pet id ${petID} ${Spawn[pet id ${petID}].ID}
        /bca No pet to weaponize, missing pet id
        /goto :endWeaponizePet       
    }

    /if (${Me.Casting} || !${Me.SpellReady[${petSpell}]} || ${Me.CurrentMana} < ${Spell[${petSpell}].Mana}) {
        /goto :endWeaponizePet       
    }

    /call EnsureTarget ${petID}

    /declare k  int local
    /for k 1 to 2 {
        /if (!${Me.SpellReady[${petSpell}]}) {
            | /delay ${Spell[${petSpell}].RecastTime} | reports wrong, reports recovery time
            /delay 6s
        }

        /call cast "${petWeaponsSpell}"
        /delay 3
        /click left target
        /delay 1s ${Window[GiveWnd].Open}
        /if (${Window[GiveWnd].Open}) {
            /notify GiveWnd GVW_Give_Button LeftMouseUp
            /delay 5s !${Window[GiveWnd].Open}
        }
        /next k
    }
    
    :endWeaponizePet
    /if (${Debug} || ${Debug_Pet}) /echo <== WeaponizePet -|
/RETURN

Sub MakePetIni(string fileName)
    |- Build the INI file
    /echo Building Pet INI file.....

    | /ini "${fileName}" "${petOptions}" "AttackPct" "${assistPct}"
/return

| ################################################################################
| # Summon pet event
| ################################################################################
#Event SummonYourPet "#*#[MQ2] summon your pet#*#"
Sub Event_SummonYourPet
    /if (${Debug} || ${Debug_Pet}) /echo |- Event_SummonYourPet ==>

    /call SummonPet
    
    /if (${Debug} || ${Debug_Pet}) /echo <== Event_SummonYourPet -|
/return

| ################################################################################
| # Weaponize pet event
| ################################################################################
#Event WeaponizeThePet "#*#[MQ2] weaponize the pet:#1##*#"
#Event WeaponizeThePet "#*#[MQ2] weaponize your pet#1##*#"
Sub Event_WeaponizeThePet(eventText, int petID)
    /if (${Debug} || ${Debug_Pet}) /echo |- Event_WeaponizeThePet ==>

    /call WeaponizePet ${petID}
    
    /if (${Debug} || ${Debug_Pet}) /echo <== Event_WeaponizeThePet -|
/return

Sub DoPetEvents
    /doevents SummonYourPet
    /doevents WeaponizeThePet
/return