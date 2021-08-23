Sub InitMeleeIni(string iniFileName)
    /declare meleeOptions       string  local "MeleeOptions"
    /declare Debug_Melee        bool    outer FALSE

    /if (${Ini[${iniFileName},Debug,Debug_Melee].Length}) {
       /varset Debug_Melee ${Bool[${Ini[${iniFileName},Debug,Debug_Melee]}]}     
    } 

/return 1

Sub DoKill
    /if (${Debug} || ${Debug_Melee}) /echo |- DoKill ==>	

	/if (!${Melee.Enable} || ${NetBots[${Me}].Attacking}) {
        /if (${Debug} || ${Debug_Melee}) /echo |- Melee <${Melee.Enable}> or Attacking <${NetBots[${Me}].Attacking}>
        /goto :endDoKill
    }

    /call GetMainAssist
    /declare mainAssist string	local ${Macro.Return}

    /if (${mainAssist.Equal[NULL]}) {
        /if (${Debug} || ${Debug_Heals}) /echo |- No main assist defined
        /goto :endDoKill
    }
    
    /declare targetId int local ${NetBots[${mainAssist}].TargetID}

    /if (!${targetId}) {
        /if (${Debug} || ${Debug_Melee}) /echo |- Missing MA target from NetBots
        /goto :endDoKill
    }
    
    /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
    /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
    /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
    /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

    /if (!${isNPC} && !${isPet}) {
        /if (${Debug} || ${Debug_Melee}) /echo |- Invalid target ${Spawn[${targetId}].Type}
        /goto :endDoKill
    } 

	/if (!${hasLineOfSight}) {
        /if (${Debug} || ${Debug_Melee}) /echo |- No LOS for target: ${Spawn[${targetId}]}
        /goto :endDoKill
    }

	/if (${targetHP} > ${assistPct}) {
        /if (${Debug} || ${Debug_Melee}) /echo |- Target HP to low: ${targetHP}
        /goto :endDoKill
    }

    /call EnsureTarget ${targetId}
    /if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}!=${targetId}) {
        /if (${Debug} || ${Debug_Melee}) /echo |- Invalid target ${Target.Name}
        /goto :endDoKill
    }

    /attack on
	/killthis

    :gotoloopkeepfighting
        /if (${Target.ID} && ${NetBots[${Me}].Attacking} && ${Target.Type.Equal[NPC]}) {
            /delay 10
            /doevents
            /if (${Debug} || ${Debug_Melee}) /echo DEBUG: KillMob: Combat Loop
            /goto :gotoloopkeepfighting            
        } 

    :endDoKill
    /if (${Debug} || ${Debug_Melee}) /echo <== DoKill -|
/return

Sub DoMelee(meleeAbilitySub)    
    /if (${Debug} || ${Debug_Melee}) /echo |- DoMelee ==>	

	/if (${Me.Combat}) {
        /goto :endDoMelee
    }

    /call GetMainAssist
    /declare mainAssist string	local ${Macro.Return}

    /if (${mainAssist.Equal[NULL]}) {
        /if (${Debug} || ${Debug_Heals}) /echo |- No main assist defined
        /goto :endDoMelee
    }

    /declare targetId int local ${NetBots[${mainAssist}].TargetID}
    /if (!${targetId} || ${Bool[${Spawn[id ${targetId}].Type.Equal[Corpse]}]}) {
        /if (${Me.Combat}) /attack off
        /goto :endDoMelee
    }
    
    /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
    /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
    /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
    /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

    /if ((!${isNPC} && !${isPet}) || ${targetHP} > ${assistPct} || !${hasLineOfSight}) {
        /goto :endDoMelee
    }

    /call EnsureTarget ${targetId}
    /if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}!=${targetId}) {
        /if (${Debug} || ${Debug_Melee}) /echo |- Invalid target
        /goto :endDoMelee
    }

    /if (!${Stick.Active}) /stick id ${targetId} snaproll 90%
    /if (${Target.Distance} <= ${Stick.Distance}) {
        /if (!${Me.Combat}) /attack on

        /if (Defined[${meleeAbilitySub}]) {
            /call ${meleeAbilitySub}
        }
    }

    :endDoMelee
    /if (${Debug} || ${Debug_Melee}) /echo <== DoMelee -|
/return
