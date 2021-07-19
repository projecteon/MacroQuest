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

Sub DoMelee    
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
        /attack off
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

        /if (${Math.Abs[${Me.Heading.Degrees}-${Target.Heading.Degrees}]} < 45) {
            | /call doRogueStrike
            /if (${Me.AbilityReady[Backstab]}) {
		        /doability Backstab
            }
        }
    }

    :endDoMelee
    /if (${Debug} || ${Debug_Melee}) /echo <== DoMelee -|
/return

Sub DoRogueStrike
    /if (${Debug} || ${Debug_Melee}) /echo |- DoRogueStrike ==>	

	/if (${Me.Endurance} > ${strikeDiscEndCost} && ${Me.PctEndurance} >= ${strikeDiscMinEnd} && ${Me.CombatAbilityReady[${strikeDisc}]} && ${Me.AbilityReady[Backstab]} && !${Me.ActiveDisc.ID} && ${Me.Invis} && ${Me.Sneaking}) {
		| Use 'Assassin's Strike' type disc.
		/delay 1
		/disc ${strikeDisc}
		/delay 5 ${Bool[${Me.ActiveDisc.ID}]}
		/delay 3
		/doability Backstab
		/delay 1
		/attack on
	}

    /if (${Debug} || ${Debug_Melee}) /echo <== DoRogueStrike -|
/return