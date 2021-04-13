Sub InitPetIni(string iniFileName)
    /declare petOptions         string  local "PetOptions"
    /declare petInitResult      int     local 1
    /declare currentPetTargetId int     outer 0

    | /if (${Ini[${iniFileName},${petOptions},AttackPct].Length}) {
    |     /varset assistPct ${Ini[${iniFileName},${petOptions},AttackPct]}
    | } else {
    |     /ini "${iniFileName}" "${petOptions}" "AttackPct" "${assistPct}"
    |     /varset petInitResult 0
    | }
/return 1

Sub doPet
    /if (!${Me.Pet.ID}) {
        /return
    }

    /if (${currentPetTargetId} > 0 && (${Spawn[${currentPetTargetId}].Type.Equal[Corpse]}|| !${SpawnCount[id ${currentPetTargetId}]})) {
        /varset currentPetTargetId 0
    } else /if (${currentPetTargetId} > 0) {
        /return
    }

    /declare targetId int local ${NetBots[${mainAssist}].TargetID}
    /if (!${targetId}) {
        /return
    }

    /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
    /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
    /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
    /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

    /if ((!${isNPC} && !${isPet}) || ${targetHP} > ${assistPct} || !${hasLineOfSight} || !${SpawnCount[id ${targetId}]} || ${Spawn[${targetId}].Distance} > 100) {
        /return
    }

    /if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}!=${targetId}) {
	    /target id ${targetId}
        /return
    }

    /varset currentPetTargetId ${targetId}
    /pet back off
    /pet attack
/return

Sub MakePetIni(string fileName)
    |- Build the INI file
    /echo Building Pet INI file.....

    | /ini "${fileName}" "${petOptions}" "AttackPct" "${assistPct}"
/return