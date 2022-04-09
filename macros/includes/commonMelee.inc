Sub InitMeleeIni(string iniFileName)
    /declare meleeOptions       string  local "MeleeOptions"
    /declare Debug_Melee        bool    outer FALSE
	  /declare isEnraged          bool    outer FALSE

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
        /if (${Debug} || ${Debug_Melee}) /echo |- No main assist defined
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

Sub DoMeleeDps(meleeAbilitySub)
  /if (${Debug} || ${Debug_Melee}) /echo |- DoMeleeDps ==>

  /if (${Me.Combat} && !${Target.ID}) {
    /attack off
  }

	/if (${Me.Combat} && ${Target.ID} && !${Target.Type.Equal[Corpse]}) {
    /if (${Debug} || ${Debug_Melee}) /echo |- Combat <${Me.Combat}> or Attacking <${NetBots[${Me}].Attacking}> - <${Target}>
    /goto :doMeleeDps
  }

  /call GetMainAssist
  /declare mainAssist string	local ${Macro.Return}

  /if (${mainAssist.Equal[NULL]}) {
    /if (${Debug} || ${Debug_Melee}) /echo |- No main assist defined
    /if (${Me.Combat}) /attack off
    /if (${Stick.Active}) /stick off
    /varset isEnraged FALSE
    /goto :endDoMeleeDps
  }

  /declare targetId int local ${NetBots[${mainAssist}].TargetID}
  /if (!${targetId} || ${Bool[${Spawn[id ${targetId}].Type.Equal[Corpse]}]}) {
    /if (${Me.Combat}) /attack off
    /if (${Stick.Active}) /stick off
    /varset isEnraged FALSE
    /goto :endDoMeleeDps
  }

  /declare isNPC          local ${Spawn[${targetId}].Type.Equal[NPC]}
  /declare isPet          local ${Spawn[${targetId}].Type.Equal[PET]}
  /declare hasLineOfSight local ${Spawn[${targetId}].LineOfSight}
  /declare targetHP       local ${NetBots[${mainAssist}].TargetHP}

  /if ((!${isNPC} && !${isPet}) || ${targetHP} > ${assistPct} || !${hasLineOfSight} || ${Spawn[${targetId}].Distance3D} > ${campRadius}) {
    /if (${Me.Combat}) /attack off
    /if (${Stick.Active}) /stick off
    /varset isEnraged FALSE
    /goto :endDoMeleeDps
  }

  /call EnsureTarget ${targetId}
  /if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}!=${targetId}) {
    /if (${Debug} || ${Debug_Melee}) /echo |- Invalid target
    /if (${Me.Combat}) /attack off
    /stick off
    /varset isEnraged FALSE
    /goto :endDoMeleeDps
  }

  /if (!${Stick.Active} || !${Stick.StickTarget}==${targetId}) {
    /call StickToTarget ${targetId} 0.75
  }

  :doMeleeDps
  /if (${Stick.Active}) {
    /if (${Defined[meleeAbilitySub]}) {
      /echo ${meleeAbilitySub}
      /call ${meleeAbilitySub}
    }

    | rogues need hidden check, we might want to start with disc strike backstab
    /if (!${Me.Combat} && !${isEnraged}) /attack on
    /if (${isEnraged}) /attack off
  }

  :endDoMeleeDps
  /if (${Debug} || ${Debug_Melee}) /echo <== DoMeleeDps -|
/return

Sub StickToTarget(int targetId, float modifier)
  /if (${Debug} || ${Debug_Melee}) /echo |- StickToTarget ==>	<${targetId}> <${modifier}>

  | https://www.mmobugs.com/wiki/index.php/MQ2MoveUtils:v11_FAQ
  | https://www.redguides.com/community/threads/mq2moveutils-question.70706/
  | https://www.redguides.com/community/threads/mq2-vanilla-max-melee-range.54990/
  | only snaproll for rogues, others can start DPS right away?
  /if (!${Stick.Active} || ${Stick.StickTarget} != ${targetId}) {
    /declare stickDistance int local ${Math.Calc[${Spawn[id ${targetId}].MaxRangeTo}*${modifier}].Int}
    /if (${Debug} || ${Debug_Melee}) /echo Stick distance <${stickDistance}> <${Stick.Distance}> <${Stick.DistMod}> <${Stick.DistModPercent}>

    /if (${stickDistance} > 25) /varset stickDistance 25

    /call GetMainTank
    /declare mainTank string	local ${Macro.Return}

    /call AmIOfftank
    /declare iAmOffTank  bool local ${Macro.Return}

    /squelch /face fast
    /if (${mainTank.Equal[${Me.Name}]} || ${iAmOffTank}) {
      /squelch /stick front 4 uw
      /delay 5s ${Stick.Stopped}
      /if (${Debug} || ${Debug_Melee}) /echo stuck to front
    } else {
      /squelch /stick id ${targetId} snaproll ${stickDistance}
      /delay 5s ${Stick.Behind} && ${Stick.Stopped}
      /squelch /stick id ${targetId} moveback behind ${stickDistance} uw
      /if (${Debug} || ${Debug_Melee}) /echo stuck to behind
    }
  }

  /if (${Debug} || ${Debug_Melee}) /echo <== StickToTarget -|
/return

|------------------------------------------------------------|
|- Turns off attack, when a mob you're attacking enrages.	-|
|------------------------------------------------------------|
#Event EnrageOn "#1# has become ENRAGED."
Sub Event_EnrageOn(line, RageOn)
  /if (${Debug} || ${Debug_Melee}) /echo |- Event_EnrageOn ==>

  /if (${Target.ID} == ${Spawn[${RageOn}].ID}) {
    /if (${Me.Combat}) /attack off
    /varset isEnraged TRUE
  }

  /if (${Debug} || ${Debug_Melee}) /echo <== Event_EnrageOn -|
/return
|------------------------------------------------|
|- Turns attack back on, after enrage is over.	-|
|------------------------------------------------|
#Event EnrageOff "#1# is no longer enraged."
Sub Event_EnrageOff(line, RageOn)
  /if (${Debug} || ${Debug_Melee}) /echo |- Event_EnrageOff ==>

  /if (${Target.ID} == ${Spawn[${RageOn}].ID}) {
    /if (!${Me.Combat}) /attack on
    /varset isEnraged FALSE
  }

  /if (${Debug} || ${Debug_Melee}) /echo <== Event_EnrageOff -|
/return

|------------------------------------------------|
|- Adjust Melee distance if too far away msg   	-|
|------------------------------------------------|
#Event GetCloser "Your target is too far away, get closer!"
Sub Event_GetCloser
  /if (${Debug} || ${Debug_Melee}) /echo |- Event_GetCloser ==>

	/if (${Me.Combat}) {
    /if (${Debug} || ${Debug_Melee}) /echo Stick distance <${Stick.Distance}> mod <${Stick.DistMod}> modPct <${Stick.DistModPercent}>
    /doevents flush Event_GetCloser
    /squelch /face fast
    /declare stickDistance int local ${Math.Calc[${Stick.Distance}*0.75].Int}
    /if (${Stick.MoveBehind}) {
      /squelch /stick id ${Target.ID} moveback behind ${stickDistance} uw
    } else {
      /squelch /stick id ${Target.ID} moveback front ${stickDistance} uw
    }
	}

  /if (${Debug} || ${Debug_Melee}) /echo <== Event_GetCloser -|
/return


Sub DoMeleeEvents
  /doevents EnrageOn
  /doevents EnrageOff
  /doevents GetCloser
/return