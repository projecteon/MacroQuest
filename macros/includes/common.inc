#include includes\ini.inc
#include includes\commonItems.inc
#include includes\commonLoot.inc
#include includes\commonAutopilot.inc

Sub InitGenralIni(string iniFileName)
  /declare options string  local "General"
  /if (!${Defined[numInventorySlots]})  	/declare numInventorySlots  int	    outer 8
  /if (!${Defined[assistPct]})  	        /declare assistPct          int	    outer 95
  /if (!${Defined[campRadius]})  	        /declare campRadius         int	    outer 100
  /if (!${Defined[Debug]})  		          /declare Debug       	      bool  	outer FALSE
  /if (!${Defined[Debug_Common]})	        /declare Debug_Common  	    bool  	outer FALSE
  | sea furious are |32, but so are guards... needs more testing
  /if (!${Defined[aggroAnimation]})       /declare aggroAnimation     string  outer |5|8|12|17|18|32|42|44|80|106|129|144|
  /declare initResult                                                 int     local 1

  /if (${Ini[${iniFileName},Debug,Debug].Length}) {
    /varset Debug ${Bool[${Ini[${iniFileName},Debug,Debug]}]}
  }

  /if (${Ini[${iniFileName},Debug,Debug_Common].Length}) {
    /varset Debug_Common ${Bool[${Ini[${iniFileName},Debug,Debug_Common]}]}
  }

  /call IniArray2 "${iniFileName}" ${options} "MainAssist#" MainAssists
  /call IniArray2 "${iniFileName}" ${options} "MainTank#" MainTanks

  /if (!${Defined[MainAssists]} || ${MainAssists.Size} < 1) {
    /echo No main assists defined! - <${MainAssists}> - <${MainAssists.Size}>
    /varset initResult 0
  }

  /if (!${Defined[MainTanks]} || ${MainTanks.Size} < 1) {
    /echo No main tanks defined! - <${MainTanks}> - <${MainTanks.Size}>
    /varset initResult 0
  }

  /if (${Ini[${iniFileName},${options},AssistPct].Length}) {
    /varset assistPct ${Ini[${iniFileName},${options},AssistPct]}
  } else {
    /ini "${iniFileName}" "${options}" "AssistPct" "${assistPct}"
  }

  /if (${Ini[${iniFileName},${options},CampRadius].Length}) {
    /varset campRadius ${Ini[${iniFileName},${options},CampRadius]}
  } else {
    /ini "${iniFileName}" "${options}" "CampRadius" "${campRadius}"
  }

  /squelch /alias /followme /bc follow me
  /squelch /alias /movetome /bc move to me
  /squelch /alias /removebuffs /bc remove buffs with remaining duration:

  /call InitLoot ${iniFileName}
  /call InitAutopilot ${iniFileName}
/return ${initResult}


| ################################################################################
| # Get prioritized Main Assist that is in zone
| ################################################################################
Sub GetMainAssist
  /if (${Debug}) /echo  |- GetMainAssist ==>

  /if (!${Defined[MainAssists]}) {
    /if (${Debug}) /echo No MainAssists defined.
    /goto :endGetMainAssist
  }

  /declare currentMainAssist string local
  /declare mainAssist string local
  /declare k  int local
  /for k 1 to ${MainAssists.Size}
    /varset mainAssist ${MainAssists[${k}]}
    /if (${NetBots[${mainAssist}].ID} && ${NetBots[${mainAssist}].InZone}) {
      /varset currentMainAssist ${mainAssist}
      /goto :endGetMainAssist
    }
  /next k

  :endGetMainAssist
  /if (${Debug}) /echo <== GetMainAssist -|
/return ${currentMainAssist}

| ################################################################################
| # Get prioritized Main Tank that is in zone
| ################################################################################
Sub GetMainTank
  /if (${Debug}) /echo  |- GetMainTank ==>

  /if (!${Defined[MainTanks]}) {
    /if (${Debug}) /echo No MainTanks defined.
    /goto :endGetMainTank
  }

  /declare currentMainTank string local
  /declare mainTank string local
  /declare k  int local
  /for k 1 to ${MainTanks.Size}
    /varset mainTank ${MainTanks[${k}]}
    /if (${NetBots[${mainTank}].ID} && ${NetBots[${mainTank}].InZone}) {
      /varset currentMainTank ${mainTank}
      /goto :endGetMainTank
    }
  /next k

  :endGetMainTank
  /if (${Debug}) /echo <== GetMainTank -|
/return ${currentMainTank}


| ################################################################################
| # Checks if agro NPC is inside camp given radius.
| # Radius default: 100
| ################################################################################
Sub CheckNPCInRange(int radiusCheck)
  /if (${Debug} || ${Debug_Common}) /echo |- CheckNPCInRange ==>

	/if (!${Defined[radiusCheck]}) {
    /declare radiusCheck int local ${campRadius}
  } else /if (${radiusCheck} == 0) {
    /varset radiusCheck ${campRadius}
  }

	/if (!${Defined[npcCount]})  /declare npcCount int local ${SpawnCount[npc los radius ${radiusCheck}]}

  /if (${Debug} || ${Debug_Common}) {
    /echo |- radiusCheck > ${radiusCheck}
    /echo |- npcCount > ${npcCount}
  }

  /if (${npcCount} < 1) {
    /if (${Debug} || ${Debug_Common}) /echo <== CheckNPCInRange -|
    /return 0
  }

  /declare i int local 0
  /for i 1 to ${npcCount}
    /if ( ${Bool[${aggroAnimation.Find[|${NearestSpawn[${i}, npc los targetable radius ${radiusCheck}].Animation}|]}]}) {
      /if (${Debug} || ${Debug_Common}) /echo |-  CheckNPCInRange > TRUE > ${NearestSpawn[${i}, npc los targetable radius ${radiusCheck}].Name} > ${NearestSpawn[${i}, npc los targetable radius ${radiusCheck}].Animation}
      /if (${Debug} || ${Debug_Common}) /echo <== CheckNPCInRange -|
      /return 1
    }
  /next i

  /if (${Debug} || ${Debug_Common}) /echo <== CheckNPCInRange -|
/return 0

Sub EnsureTarget(int targetId)
  /if (${Debug} || ${Debug_Common}) /echo |- EnsureTarget ==>

  /if (${Target.ID}==${targetId}) {
    /goto :endEnsureTarget
  }

  /if (${SpawnCount[id ${targetId}]}) {
    /squelch /target id ${targetId}
    /delay 3s ${Target.ID} == ${targetId}
	} else {
		/echo EnsureTarget has no spawncount for target id ${targetId}
	}

  :endEnsureTarget
  /if (${Debug} || ${Debug_Common}) /echo <== EnsureTarget -|
/return

| ################################################################################
| # Creates a timer given name and default time setting.
| # Dont use this unless you can always check the timer later
| # fex using mob names or IDs change on respawn etc, so not safe to use
| #     unless you want to populate global variable namespace indefinetly
| ################################################################################
Sub CreateTimer(timerName, timerSetting)
  /if (${Debug} || ${Debug_Common}) /echo |- CreateTimer ==>

  /if (${Defined[${timerName}]}) {
    /varset ${timerName} ${timerSetting}
  } else {
    /declare ${timerName} timer outer ${timerSetting}
  }

  /if (${Debug} || ${Debug_Common}) /echo <== CreateTimer -| created ${timerName} [${${timerName}}]
/return

Sub CheckTimerExpired(timerName)
  /if (${Debug} || ${Debug_Common}) /echo |- CheckTimerExpired ==>

  /declare isExpried bool local FALSE
  /if (!${Defined[${timerName}]} || ${${timerName}} <= 0) {
    /varset isExpried TRUE
    /if (${Defined[${timerName}]} && ${${timerName}} <= 0) /deletevar ${timerName}
  }

  /if (${Debug} || ${Debug_Common}) /echo |- ${timerName} [${${timerName}}] expired: ${isExpried}
  /if (${Debug} || ${Debug_Common}) /echo <== CheckTimerExpired -|
/return ${isExpried}


| ################################################################################
| # Use MQ2MoveUtils /moveto function, to move to destination.
| ################################################################################
Sub MoveToLoc(int yLoc, int xLoc, int timeToWait, int arrivalDist)
  /if (${Debug} || ${Debug_Common}) /echo |- MoveToLoc ==>

  /if (!${Bool[${arrivalDist}]}) /varset arrivalDist 10
  /if (!${Bool[${timeToWait}]}) /varset timeToWait 30

  /declare moveTimer timer local ${timeToWait}
  /if (${Debug} || ${Debug_Common})  /echo MoveToLoc ${yLoc},${xLoc},${timeToWait},${arrivalDist}

  /if (${Math.Distance[${yLoc},${xLoc}]}<=${arrivalDist}) {
    /goto :endMoveToLoc
  }

  /if (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) /call interrupt

  :retryMove
    /squelch /moveto loc ${yLoc} ${xLoc} dist ${arrivalDist}
    /delay ${Math.Calc[${timeToWait}/5]} ${Math.Distance[${yLoc},${xLoc}]}<=${arrivalDist}
    /if (${Math.Distance[${yLoc},${xLoc}]}>${arrivalDist} && ${moveTimer}) {
      /goto :retryMove
    }

  :endMoveToLoc
  /moveto off

  /if (${Debug} || ${Debug_Common}) /echo <== MoveToLoc -|
/return

#Event MoveToMe "<#1#> move to me"
Sub Event_MoveToMe(string line, string sender)
  /if (${Debug} || ${Debug_Common}) /echo |- Event_MoveToMe ==>

  /if (!${Defined[${sender}]} || !${NetBots.Client[${sender}].ID}) {
    /if (${Debug} || ${Debug_Common}) /echo No valid <sender> of message: ${sender}
    /goto :endMoveToMe
  }

  /if (!${NetBots.Client[${sender}].InZone}) {
    /if (${Debug} || ${Debug_Common}) /echo ${sender} not in zone.
    /goto :endMoveToMe
  }

  /if (${Nav}) {
    /squelch /nav id ${Me.ID}
  } else {
    /squelch /call MoveToLoc ${Me.Y} ${Me.X}
  }

  :endMoveToMe
  /if (${Debug} || ${Debug_Common}) /echo <== Event_MoveToMe -|
/return

#Event FollowMe "<#1#> follow me"
Sub Event_FollowMe(string line, string sender)
  /if (${Debug} || ${Debug_Common}) /echo |- FollowMe ==>

  /if (!${Defined[${sender}]} || !${NetBots.Client[${sender}].ID}) {
    /if (${Debug} || ${Debug_Common}) /echo No valid <sender> of message: ${sender}
    /goto :endFollowMe
  }

  /if (!${NetBots.Client[${sender}].InZone}) {
    /if (${Debug} || ${Debug_Common}) /echo ${sender} not in zone.
    /goto :endFollowMe
  }

  /squelch /makecamp off
  /if (${NetAdvPath}) {
    /call EnsureTarget ${NetBots.Client[${sender}].ID}
    /squelch /netfollow on
  } else /if (${AdvPath} && ${AdvPath.Active}) {
    /squelch /afollow spawn ${NetBots.Client[${sender}].ID}
  } else {
    /squelch /stick id ${NetBots.Client[${sender}].ID} uw 15
  }

  :endFollowMe
  /if (${Debug} || ${Debug_Common}) /echo <== FollowMe -|
/return

| #####################################################################################
| # Auto inventories items from your cursor.
| # Alerts the user, and ends the macro, if there is no free inventory space available
| #####################################################################################
Sub ClearCursor
  /if (${Debug} || ${Debug_Common}) /echo |- ClearCursor ==>

	/declare i int local 1
  :check_cursor
	/if (${Cursor.ID}) {
    /autoinventory
    /if (${i} > 5) {
      /beep
      /bc ${Me.Name} cannot clear cursor, ending macro
      /popup ${Me.Name} cannot clear cursor, ending macro
      /endmacro
    } else {
      /delay 1s !${Cursor.ID}
    }

    /varcalc i ${i}+1
    /goto :check_cursor
  }

  /if (${Debug} || ${Debug_Common}) /echo <== ClearCursor -|
/return

| #####################################################################################
| # Type string by chrs
| #####################################################################################
Sub TypeChrs(string inputString)
  /if (${Debug} || ${Debug_Common}) /echo |- TypeChrs ==>

  /declare char       string local
  /declare loopctr    int local
  /for loopctr 1 to ${inputString.Length}
    /varset char ${inputString.Mid[${loopctr},1]}
    /if (!${char.Length}) {
      /nomodkey /keypress space chat
    } else {
      /nomodkey /keypress ${char} chat
    }
  /next loopctr

  /if (${Debug} || ${Debug_Common}) /echo <== TypeChrs -|
/return

Sub CheckAllBagsAreOpen
  /if (!${Defined[currentZoneIdForBags]}) {
    /echo |- CheckAllBagsAreOpen ==> <${Zone.ID}>
    /keypress OPEN_INV_BAGS
    /declare currentZoneIdForBags int outer ${Zone.ID}
  } else /if (${currentZoneIdForBags} != ${Zone.ID}) {
    /echo |- CheckAllBagsAreOpen ==> <${currentZoneIdForBags}> <${Zone.ID}>
    /keypress OPEN_INV_BAGS
    /varset currentZoneIdForBags ${Zone.ID}
  }
/return

Sub CheckPlugin(string pluginName)
  /if (!${Bool[${Plugin[${pluginName}]}]}) {
    /squelch /plugin ${pluginName}
    /echo ${pluginName} not detected! This macro requires it! Loading ...
    /delay 2s ${Bool[${Plugin[${pluginName}]}]}
    /if (!${Bool[${Plugin[${pluginName}]}]}) {
      /echo Unable to load plugin <${pluginName}> ending macro...
      /endmac
    }
  }
/return

| #####################################################################################
| # Moves to a given location, and clicks the nearest door object.
| #####################################################################################
#Event ClickIt "<#1#> Click It#2#"
#Event ClickIt "#1# tells the group, 'Click It#2#'"
#Event ClickIt "#1# tell your party, 'Click It#2#'"
Sub Event_ClickIt(line, chatSender, eventParams)
  /if (${Debug} || ${Debug_Common}) /echo |- EVENT_ClickIt ==>

  /call IniArray "data\doors.ini" "${Zone.ShortName}" "${Zone.ShortName}#" 1 doorList

  /if (!${Defined[doorList]}) {
    /if (${Debug} || ${Debug_Common}) /echo No door list defined for zone <${Zone.Name}>
    /goto :endClickIt
  }

  /if (${Debug} || ${Debug_Common}) /echo Number of doors in zone <${Zone.Name}> is <${doorList.Size}>

  /if (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) /call interrupt

  /declare i                  int     local
  /declare closestDoorID      int     local 0
  /declare closestDoorDist    float   local 0

  |find the closest door to me
  /for i 1 to ${doorList.Size}
    /squelch /doortarget id ${doorList[${i}].Arg[1,,]}
    /if ((${closestDoorDist}==0 || ${DoorTarget.Distance} < ${closestDoorDist}) && ${DoorTarget.DistanceZ} < 125 && ${DoorTarget.Distance} < 400) {
      /varset closestDoorDist ${DoorTarget.Distance}
      /varset closestDoorID ${doorList[${i}].Arg[1,,]}
    }
  /next i

  /if (!${Bool[${closestDoorID}]}) {
    /if (${Debug} || ${Debug_Common}) /echo Unable to find a valid door within range for zone <${Zone.Name}>
    /goto :endClickIt
  }

  /squelch /doortarget id ${closestDoorID}
  /if (${DoorTarget.Distance} > 20) /call MoveToLoc ${DoorTarget.Y} ${DoorTarget.X} 50 15
  /if (${DoorTarget.Distance} > 35) {
    /if (${Debug} || ${Debug_Common}) /echo Aborting ClickIt: I could not moveto < 20 units of ${DoorTarget.Name} within 5s
    /moveto off
    /goto :endClickIt
  }


  |add starting loc vs final loc for intrazone click
  /declare startingLoc ${Me.Loc}
  /declare clickDelay     int     local ${Math.Calc[${Math.Rand[${Me.ID}]}%7|1]}
  /declare currentZone    int     local ${Zone.ID}
  /declare clickTimer     timer   local 5s

  :retryClickDoor
  /delay ${clickDelay}
  /squelch /doortarget id ${closestDoorID}
  /squelch /click left door
  /if (${currentZone}==${Zone.ID} && ${Math.Distance[${startingLoc}]} < 10) {
    /if (${clickTimer}) {
      /goto :retryClickDoor
    } else {
      /if (${Debug} || ${Debug_Common}) /echo I have failed to click Door ${Switch.ID} ${DoorTarget.Name}
      /goto :endClickIt
    }
  }

  :endClickIt
  /if (${Debug} || ${Debug_Common}) /echo <== EVENT_ClickIt -|
/return


#Event RemoveLowDurationBuffs "<#*#> remove buffs with remaining duration: #1#"
Sub Event_RemoveLowDurationBuffs(line, int duration)
  /if (${Debug}) /echo  |- EVENT_RemoveLowDurationBuffs ==>

  /declare k  int local
  /for k 1 to 15
    /if (${Bool[${Me.Buff[${k}]}]} && ${Me.Buff[${k}].Duration.TotalSeconds} < ${duration}) {
      /if (${Debug}) /echo  Removing buff <${Me.Buff[${k}]}> from buffslot <${Math.Calc[${k}-1].Int}> with remaining duration <${Me.Buff[${k}].Duration.TotalSeconds}s>
      /notify BuffWindow Buff${Math.Calc[${k}-1].Int} leftmouseup
    }
  /next k

  /if (${Debug}) /echo <== EVENT_RemoveLowDurationBuffs -|
/return

Sub DoCommonEvents
  /if (${Debug} || ${Debug_Common}) /echo |- DoCommonEvents ==>

  /call Wait4Rez_Background_Events

  /doevents ClickIt
  /doevents FollowMe
  /doevents MoveToMe
  /doevents RemoveLowDurationBuffs

  /call CheckNPCInRange
  /declare hasNpcInCamp bool local ${Bool[${Macro.Return}]}
  /if (${Debug} || ${Debug_Common}) /echo |- CheckNPCInRange > ${hasNpcInCamp}
  /if (${hasNpcInCamp}) /goto :endDoClearCampEvents

  /call DoItemEvents
  /call DoLootEvents
  /call DoAutoPilotEvents

  :endDoClearCampEvents
  /if (${Debug} || ${Debug_Common}) /echo <== DoCommonEvents -|
/return