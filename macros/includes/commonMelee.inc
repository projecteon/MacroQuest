Sub doMelee
    /if (${Me.Combat} || ${Me.Moving}) {
        /return
    }

    /declare targetId int local ${NetBots[${mainAssist}].TargetID}
    /if (!${targetId} || ${Bool[${Spawn[id ${targetId}].Type.Equal[Corpse]}]}) {
        /attack off
        /return
    }
    
    /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
    /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
    /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
    /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

    /if ((!${isNPC} && !${isPet}) || ${targetHP} > ${assistPct} || !${hasLineOfSight}) {
        /return
    }

    /target id ${targetId}
    /delay 50 ${Target.ID}==${targetId}
    /if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}!=${targetId}) {
        /return
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
/return

Sub doRogueStrike
    /if (${Debug}) /echo |- doRogueStrike ==>	

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

    /if (${Debug}) /echo <== doRogueStrike -|
/return