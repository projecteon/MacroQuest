| https://everquest.fanra.info/wiki/Pet_commands

Sub InitPetIni(string iniFileName)
  /declare petOptions         string  local "PetOptions"
  /declare petInitResult      int     local 1
  /declare petSpell           string  outer
  /declare petAssistPct       int     outer
  /declare petFocusItem       string  outer
  /declare petWeaponsSpell    string  outer
  /declare petTaunt           bool    outer false
  /declare currentPetTargetId int     outer 0
  /declare Debug_Pet          bool    outer FALSE

  /if (${Ini[${iniFileName},${petOptions},"UsePet"].Length}) {
    /declare usePet string local ${Ini[${iniFileName},${petOptions},"UsePet"]}
    /call Event_SetActivePetSpell "" ${usePet}
    /varset petInitResult ${Macro.Return}
  } else {
    /ini "${iniFileName}" "${petOptions}" "UsePet" "PetSectionName"
    /ini "${iniFileName}" "PetSectionName" "Pet-Spell" "${petSpell}"
    /ini "${iniFileName}" "PetSectionName" "Pet-Focus-Item" "${petFocusItem}"
  }

  /if (${Ini[${iniFileName},${petOptions},"Pet-AssistPct"].Length}) {
    /varset petAssistPct ${Ini[${iniFileName},${petOptions},"Pet-AssistPct"]}
  } else {
    /ini "${iniFileName}" "${petOptions}" "Pet-AssistPct" "${petAssistPct}"
  }

  /if (${Ini[${iniFileName},${petOptions},"Pet-Weapons-Spell"].Length}) {
    /varset petWeaponsSpell ${Ini[${iniFileName},${petOptions},"Pet-Weapons-Spell"]}
  } else {
    /ini "${iniFileName}" "${petOptions}" "Pet-Weapons-Spell" "${petWeaponsSpell}"
  }

  /if (${Ini[${iniFileName},${petOptions},"Pet-Taunt"].Length}) {
    /varset petTaunt ${Bool[${Ini[${iniFileName},${petOptions},"Pet-Taunt"]}]}
  } else {
    /ini "${iniFileName}" "${petOptions}" "Pet-Taunt" "${petTaunt}"
  }

  /if (${Ini[${iniFileName},Debug,Debug_Pet].Length}) {
    /varset Debug_Pet ${Bool[${Ini[${iniFileName},Debug,Debug_Pet]}]}
  }

  /call IniArray2 "${iniFileName}" ${petOptions} "Buff#" SelfPetBuffs
/return 1

Sub DoPet
  /if (${Debug} || ${Debug_Pet}) /echo |- DoPet ==>

  /if (!${Me.Pet.ID}) {
    /if (${Debug} || ${Debug_Pet}) /echo |- Has no pet
    /goto :endDoPet
  }

  /call BuffPet

  /if (${currentPetTargetId} > 0 && (${Spawn[${currentPetTargetId}].Type.Equal[Corpse]} || !${SpawnCount[id ${currentPetTargetId}]})) {
    /pet back off
    /varset currentPetTargetId 0
  } else /if (${currentPetTargetId} > 0) {
    /if (${Debug} || ${Debug_Pet}) /echo |- Current pet target id ${currentPetTargetId} ${Spawn[id ${currentPetTargetId}]}
    /goto :endDoPet
  }

  /call GetMainAssist
  /declare mainAssist string	local ${Macro.Return}

  /if (${mainAssist.Equal[NULL]}) {
    /if (${Debug} || ${Debug_Heals}) /echo |- No main assist defined
    /goto :endDoPet
  }

  /declare targetId int local ${NetBots[${mainAssist}].TargetID}
  /if (!${targetId}) {
    /if (${Debug} || ${Debug_Pet}) /echo |- No MA target
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
  /delay 5
  /pet attack
  /delay 5
  /pet attack

  :endDoPet
  /call DoPetEvents

  /if (${Debug} || ${Debug_Pet}) /echo <== DoPet -|
/return

Sub SummonPet
  /if (${Debug} || ${Debug_Pet}) /echo |- SummonPet

  /if (!${Me.Gem[${petSpell}]}) {
    /bca !!! SPELL NOT MEMORIZED <<${petSpell}>> !!!
    /goto :endSummonPet
  }

  /if (${Bool[${Me.Pet.ID}]}) /goto :endSummonPet

  /if (!${Bool[${Me.Pet.ID}]} && !${Bool[${Me.Casting}]} && ${Me.SpellReady[${petSpell}]} && ${Me.CurrentMana} > ${Spell[${petSpell}].Mana}) {
    /if (${Bool[${petFocusItem}]} && ${Bool[${FindItem[=${petFocusItem}]}]} && ${FindItem[=${petFocusItem}].ItemSlot} >=23) {
      /if (!${Me.FreeInventory}) {
        /bcaa No free inventory for current weapons
        /beep
        /goto :endSummonPet
      }

      /call CheckAllBagsAreOpen
      /delay 10
      /declare currentMainHand string local ${Me.Inventory[mainhand]}
       | 14 -> offhand
      /declare currentOffHand  string local ${Me.Inventory[14]}
      /if (${Bool[${Me.Inventory[14]}]}) {
        /call UnequipOffhand ${numInventorySlots}
        /if (${Cursor.ID} || ${Bool[${Me.Inventory[14]}]}) {
          /call ClearCursor
          /bcaa Could not unequip offhand <${Cursor.ID}> <${Me.Inventory[14]}]>
          /beep
          /goto :endSummonPet
        }
      }

      /exchange "${petFocusItem}" mainhand
      /delay 10
    }

    /call cast "${petSpell}" gem${Me.Gem[${petSpell}]} 0 EmptyCancelCast 3

    /if (${Defined[currentMainHand]} && ${Bool[${currentMainHand}]}) {
      /exchange "${currentMainHand}" mainhand
      /delay 10

      /if (${Bool[${currentOffHand}]}) {
        /exchange "${currentOffHand}" offhand
        /delay 10
      }
    }
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
    /echo  !!! Not set <<${petWeaponsSpell}>> !!!
    /goto :endWeaponizePet
  }

  /if (!${Me.Gem[${petWeaponsSpell}]}) {
    /bca !!! SPELL NOT MEMORIZED <<${petWeaponsSpell}>> !!!
    /goto :endWeaponizePet
  }

  /declare innerPetID int local ${petID}

  /if (!${Bool[${innerPetID}]}) {
    /varset innerPetID ${Me.Pet.ID}
  }

  | check is of <pcpet> in spawn filter
  /if (!${Bool[${innerPetID}]} || !${Spawn[pet id ${innerPetID}].ID}) {
    /bca No pet to weaponize, missing pet id: <${innerPetID}>
    /goto :endWeaponizePet
  }

  | check is of <pcpet> in spawn filter
  /if (!${Spawn[pcpet radius 100 id ${innerPetID}].LineOfSight}) {
    /bca Pet outside range or line of sight: <${innerPetID}> <${Spawn[pet id ${innerPetID}]}>
    /goto :endWeaponizePet
  }

  /if (${Me.Casting} || !${Me.SpellReady[${petSpell}]} || ${Me.CurrentMana} < ${Spell[${petSpell}].Mana}) {
    /goto :endWeaponizePet
  }

  /call ClearCursor
  /if (${Cursor.ID}) {
    /bca Cursor not empty <${Cursor}>, cancelling pet weaponizing
    /beep
    /goto :endWeaponizePet
  }

  /call EnsureTarget ${innerPetID}

  /declare tradeAttemps int local 1
  /declare k  int local
  /for k 1 to 2
    /if (!${Me.SpellReady[${petSpell}]}) {
      | /delay ${Spell[${petSpell}].RecastTime} | reports wrong, reports recovery time
      /delay 6s
    }

    /call cast "${petWeaponsSpell}" gem${Me.Gem[${petWeaponsSpell}]} 0 EmptyCancelCast 3
    /delay 1s ${Cursor.ID}

    :tradePet
    /varcalc tradeAttemps ${tradeAttemps} + 1
    /if (${Me.Pet.Distance}>16 && ${Me.Pet.DistanceZ} < 80) {
      /call MoveToLoc ${Me.Pet.Y} ${Me.Pet.X} 20 12
    }

    /if (!${Window[GiveWnd].Open}) {
      /click left target
      /delay 1s ${Window[GiveWnd].Open}
    }

    /if (${Window[GiveWnd].Open}) {
      /notify GiveWnd GVW_Give_Button LeftMouseUp
      /delay 5s !${Window[GiveWnd].Open}
    }

    /if (${Cursor.ID} && ${tradeAttemps} < 3) /goto :tradePet

    /if (${Cursor.ID}) {
      /bcaa Could not hand <${Cursor}> to <${Target}> <${innerPetID}>
      /beep
    }
  /next k

  :endWeaponizePet

  /if (${Debug} || ${Debug_Pet}) /echo <== WeaponizePet -|
/return

Sub BuffPet
  /if (${Debug} || ${Debug_Pet}) /echo |- BuffPet ==>

  /if (!${Defined[SelfPetBuffs]}) {
    /if (${Debug} || ${Debug_Pet}) /echo No SelfPetBuffs defined.
    /goto :endBuffPet
  }

  /if (!${doBuffsWithNpcInCamp}) {
    /call CheckNPCInRange
    /declare hasNpcInCamp bool local ${Bool[${Macro.Return}]}
    /if (${Debug}) /echo |- CheckNPCInRange > ${hasNpcInCamp}
    /if (${hasNpcInCamp}) /goto :endBuffPet
  }

  /declare buffSpell string local
  /declare buffId int local
  /declare k  int local
  /for k 1 to ${SelfPetBuffs.Size}
    /varset buffSpell ${SelfPetBuffs[${k}]}
    /if (${Me.CurrentMana} < ${Spell[${buffSpell}].Mana} || !${Spell[${buffSpell}].StacksPet}) {
      /goto :endBuffPet
    }

    /if (!${Bool[${Me.PetBuff[${buffSpell}]}]}) {
      /call EnsureTarget ${Me.Pet.ID}
      /call cast "${buffSpell}" gem${Me.Gem[${buffSpell}]} 0 EmptyCancelCast 3
    }
  /next k

  :endBuffPet
  /if (${Debug} || ${Debug_Pet}) /echo <== BuffPet -|
/return

Sub UnequipOffhand(int numberOfInventorySlots))
  /if (${Debug} || ${Debug_Pet}) /echo |- UnequipOffhand ==> InventorySlots[${numberOfInventorySlots}]

  /if (!${Me.FreeInventory}) /goto :endUnequipOffhand
  /if (${Me.LargestFreeInventory} < ${Me.Inventory[offhand].Size}) /goto :endUnequipOffhand

  /declare i int local
  /declare c int local

  /if (${Debug} || ${Debug_Pet}) /echo |- attempting to find free slot
  /for i 1 to ${numberOfInventorySlots}
    /if (!${Bool[${Me.Inventory[pack${i}]}]}) {
      /if (${Debug} || ${Debug_Pet}) /echo |- free inventory slot
      /itemnotify offhand leftmouseup
      /delay 5
      /itemnotify pack${i} leftmouseup
      /delay 5
      /goto :endUnequipOffhand
    }

    /if (${Me.Inventory[pack${i}].Container}) {
      /if (${Debug} || ${Debug_Pet}) /echo |- check bag size capacity
      /if (${Me.Inventory[pack${i}].SizeCapacity} >=  ${Cursor.Size}) {
        /for c 1 to ${Me.Inventory[pack${i}].Container}
          /if (!${Bool[${Me.Inventory[pack${i}].Item[${c}]}]}) {
            /if (${Debug} || ${Debug_Pet}) /echo |- free bag slot pack${i} slot${c}
            /itemnotify offhand leftmouseup
            /delay 5
            /itemnotify in pack${i} ${c} leftmouseup
            /delay 5
            /goto :endUnequipOffhand
          }
        /next c
      }
    }
  /next i

  :endUnequipOffhand
  /if (${Debug} || ${Debug_Pet}) /echo <== UnequipOffhand -|
/return

| ################################################################################
| # Set active pet summon spell
| ################################################################################
#Event SetActivePetSpell "<#*#> set active pet spell #1#"
#Event SetActivePetSpell "#*#[MQ2] set active pet spell #1#"
Sub Event_SetActivePetSpell(eventText, newPetSection)
  /if (${Debug} || ${Debug_Pet}) /echo |- SetActivePet ==>

  /declare setPetSpellResult  int     local 1
  /declare iniFileName        string  local "ini/${MacroQuest.Server}/${Macro.Name.Token[1,.]}_${Me.CleanName}.ini"

  /if (${Ini[${iniFileName},${newPetSection},"Pet-Spell"].Length}) {
    /varset petSpell ${Ini[${iniFileName},${newPetSection},"Pet-Spell"]}
    /if (${Debug} || ${Debug_Pet}) /echo Active pet spell: ${petSpell}
  } else {
    /varset setPetSpellResult 0
  }

  /if (${setPetSpellResult} == 1 && ${Ini[${iniFileName},${newPetSection},"Pet-Focus-Item"].Length}) {
    /varset petFocusItem ${Ini[${iniFileName},${newPetSection},"Pet-Focus-Item"]}
    /if (${Debug} || ${Debug_Pet}) /echo Active pet docus item: ${petFocusItem}
  }

  /if (${setPetSpellResult} == 0) {
    /beep
    /echo Pet spell section <${newPetSection}> with a valid <Pet-Spell> was not found in ${iniFileName}.
  }

  /if (!${Me.Book[${petSpell}]}) {
    /beep
    /echo You do not know the spell <${petSpell}>.
  }

  /if (${Bool[${petFocusItem}]} && !${Bool[${FindItem[=${petFocusItem}]}]}) {
    /beep
    /echo You do not have the focus item <${petFocusItem}> on your character.
  }

  /if (${Debug} || ${Debug_Pet}) /echo <== SetActivePet -|
/return ${setPetSpellResult}

| ################################################################################
| # Disband pet event
| ################################################################################
#Event DisbandYourPet "<#1#> disband your pet#*#"
#Event DisbandYourPet "#*#[MQ2] disband your pet#*#"
Sub Event_DisbandYourPet
  /if (${Debug} || ${Debug_Pet}) /echo |- Event_DisbandYourPet ==>

  /if (${Me.Pet.ID}) {
    /if (${Debug} || ${Debug_Pet}) /echo Telling {Pet} to get lost!
    /pet get lost
  }

  /if (${Debug} || ${Debug_Pet}) /echo <== Event_DisbandYourPet -|
/return

| ################################################################################
| # Summon pet event
| ################################################################################
#Event SummonYourPet "<#1#> summon your pet#*#"
#Event SummonYourPet "#*#[MQ2] summon your pet#*#"
Sub Event_SummonYourPet
  /if (${Debug} || ${Debug_Pet}) /echo |- Event_SummonYourPet ==>

  /call SummonPet

  /if (${Debug} || ${Debug_Pet}) /echo <== Event_SummonYourPet -|
/return

| ################################################################################
| # Summon pet event
| ################################################################################
#Event SummonYourPet "<#*#> ask for pet weapons from #1#"
#Event SummonYourPet "[#*#] ask for pet weapons from #1#"
Sub Event_BegForWeapons(eventText, string weaponSummoner)
  /if (${Debug} || ${Debug_Pet}) /echo |- Event_BegForWeapons ==>

  /if (!${Bool[${Me.Pet.ID}]}) /goto :endBegForWeapons

  /if (${NetBots[${weaponSummoner}].InZone}) {
    /bct ${weaponSummoner} "weaponize the pet:"${Me.Pet.ID}
  } else {
    /bca "Cannot ask " ${weaponSummoner} " for pet weapons"
  }

  :endBegForWeapons
  /if (${Debug} || ${Debug_Pet}) /echo <== Event_BegForWeapons -|
/return

| ################################################################################
| # Weaponize pet event
| ################################################################################
#Event WeaponizeThePet "<#*#> weaponize the pet:#1#"
#Event WeaponizeThePet "<#*#> weaponize your pet#*#"
#Event WeaponizeThePet "#*#[MQ2] weaponize the pet:#1#"
#Event WeaponizeThePet "#*#[MQ2] weaponize your pet#*#"
#Event WeaponizeThePet "[#*#] weaponize the pet:#1#"
#Event WeaponizeThePet "[#*#] weaponize your pet#*#"
Sub Event_WeaponizeThePet(eventText, int petID)
  /if (${Debug} || ${Debug_Pet}) /echo |- Event_WeaponizeThePet ==>

  /call WeaponizePet ${petID}

  /if (${Debug} || ${Debug_Pet}) /echo <== Event_WeaponizeThePet -|
/return

Sub DoPetEvents
  /doevents SetActivePetSpell
  /doevents SummonYourPet
  /doevents WeaponizeThePet
  /doevents DisbandYourPet
/return