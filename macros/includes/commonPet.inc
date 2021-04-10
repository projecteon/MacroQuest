Sub InitPetIni(string iniFileName)
    /declare options            string  local "PetOptions"
    /declare currentPetTargetId int     outer 0
/return 1

Sub doPet
    /if (!${Me.Pet.ID}) {
        /return
    }

    /if (${currentPetTargetId} && ${Spawn[${currentPetTargetId}].Type.Equal[Corpse]}) {
		/squelch /target clear 
    } else {
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

    /if (!${isNPC} || !${isPet} || ${targetHP} > 98 || !${hasLineOfSight} || !${SpawnCount[id ${targetId}]}) {
        /return
    }

	/target id ${targetId}
    /if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}!=${targetId}) {
        /return
    }

    /varset currentPetTargetId ${targetId}
    /pet back off
    /pet attack
/return