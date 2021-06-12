#include includes\commonLoot.inc

Sub InitGenralIni(string iniFileName)
    /declare options string  local "General"
    /if (!${Defined[mainAssist]})  	    /declare mainAssist     string	outer 
    /if (!${Defined[mainTank]})  	    /declare mainTank       string	outer 
    /if (!${Defined[assistPct]})  	    /declare assistPct      int	    outer 95 
    /if (!${Defined[campRadius]})  	    /declare campRadius     int	    outer 100 
    /if (!${Defined[Debug]})  		    /declare Debug       	bool  	outer FALSE 
    | sea furious are |32, but so are guards... needs more testing
    /if (!${Defined[aggroAnimation]})   /declare aggroAnimation string  outer |5|8|12|17|18|32|42|44|80|106|129|144|
    /declare initResult                                         int     local 1

    | Use main assist setting to check if ini file is created
    /if (${Ini[${iniFileName},${options},MainAssist].Length}) {
        /varset mainAssist "${Ini[${iniFileName},${options},MainAssist]}"    
    } else {
        /ini "${iniFileName}" "${options}" "MainAssist" "${mainAssist}"      
        /varset initResult 0   
    }
    
    /if (${Ini[${iniFileName},${options},MainTank].Length}) {
        /varset mainTank "${Ini[${iniFileName},${options},MainTank]}"    
    } else {
        /ini "${iniFileName}" "${options}" "MainTank" "${mainTank}"     
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

    /if (${Ini[${iniFileName},Debug,Debug].Length}) {
       /varset Debug ${Bool[${Ini[${iniFileName},Debug,Debug]}]}     
    } 

    /call InitLoot
/return ${initResult}


| ################################################################################
| # Checks if agro NPC is inside camp given radius.
| # Radius default: 100
| ################################################################################	
Sub CheckNPCInRange(int radiusCheck)
    /if (${Debug}) /echo |- CheckNPCInRange ==>

	/if (!${Defined[radiusCheck]})  /declare radiusCheck int local 100
	
    /if (${Debug}) /echo |- CheckNPCInRange > ${radiusCheck}
	/if (!${Defined[npcCount]})  /declare npcCount int local ${SpawnCount[npc los radius ${radiusCheck}]}
    /if (${Debug}) /echo |- CheckNPCInRange > ${npcCount}
	/if (${npcCount} < 1) {
        /if (${Debug}) /echo <== CheckNPCInRange -|
		/return 0
	} 
    
    /declare i int local 0
    /for i 1 to ${npcCount} {
        /if ( ${Bool[${aggroAnimation.Find[|${NearestSpawn[${i}, npc los targetable radius ${radiusCheck}].Animation}|]}]}) {
            /if (${Debug}) /echo |-  CheckNPCInRange > TRUE > ${NearestSpawn[${i}, npc los targetable radius ${radiusCheck}].Name} > ${NearestSpawn[${i}, npc los targetable radius ${radiusCheck}].Animation}
            /if (${Debug}) /echo <== CheckNPCInRange -|
            /return 1
        }
        
        /next i
    }

    /if (${Debug}) /echo <== CheckNPCInRange -|
/return 0

Sub EnsureTarget(targetId) 
    /if (${Debug}) /echo |- EnsureTarget ==>

    /if (${Target.ID}==${targetId}) {
        /goto :endEnsureTarget
    }

    /if (${SpawnCount[id ${targetId}]}) {
			/squelch /target id ${targetId}
			/delay 3s ${Target.ID} == ${targetId}
	} else {
		/echo EnsureTarget has no spawncount
	}
    
    :endEnsureTarget
    /if (${Debug}) /echo <== EnsureTarget -|
/return

| ################################################################################
| # Creates a timer given name and default time setting.
| # Dont use this unless you can always check the timer later 
| # fex using mob names or IDs change on respawn etc, so not safe to use 
| #     unless you want to populate global variable namespace indefinetly
| ################################################################################	
Sub CreateTimer(timerName, timerSetting)
    /if (${Debug}) /echo |- CreateTimer ==>
    
    /if (${Defined[${timerName}]}) {
        /varset ${timerName} ${timerSetting}
    } else {
        /declare ${timerName} timer outer ${timerSetting}
    }
    
    /if (${Debug}) /echo <== CreateTimer -| created ${timerName} [${${timerName}}]
/return

Sub CheckTimerExpired(timerName)
    /if (${Debug}) /echo |- CheckTimerExpired ==>
    
    /declare isExpried bool local FALSE
    /if (!${Defined[${timerName}]} || ${${timerName}} <= 0) {
        /varset isExpried TRUE 
        /if (${Defined[${timerName}]} && ${${timerName}} <= 0) /deletevar ${timerName}
    }
    
    /if (${Debug}) /echo |- ${timerName} [${${timerName}}] expired: ${isExpried}
    /if (${Debug}) /echo <== CheckTimerExpired -|
/return ${isExpried}


| ################################################################################
| # Use MQ2MoveUtils /moveto function, to move to destination.
| ################################################################################	
Sub MoveToLoc(int yLoc, int xLoc, int timeToWait, int arrivalDist)
    /if (${Debug}) /echo |- MoveToLoc ==>
    
    /if (!${Bool[${arrivalDist}]}) /varset arrivalDist 10
    /if (!${Bool[${timeToWait}]}) /varset timeToWait 30
    
    /declare moveTimer timer local ${timeToWait}
    /if (${Debug})  /echo MoveToLoc ${yLoc},${xLoc},${timeToWait},${arrivalDist}

    /if (${Math.Distance[${yLoc},${xLoc}]}<=${arrivalDist}) {
        /goto :endMoveToLoc
    }

    /if (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) /call interrupt
    
    /squelch /moveto loc ${yLoc} ${xLoc} dist ${arrivalDist}
    :retryMove
        | /call Background_Events
        /delay ${Math.Calc[${timeToWait}/5]} ${Math.Distance[${yLoc},${xLoc}]}<=${arrivalDist}
        /if (${Math.Distance[${yLoc},${xLoc}]}>${arrivalDist} && ${moveTimer}) {
            /goto :retryMove
        } 

    /moveto off

    :endMoveToLoc
    /if (${Debug}) /echo <== MoveToLoc -|
/return

Sub DoCommonEvents
    /if (${Debug}) /echo |- DoCommonEvents ==>

    /call Wait4Rez_Background_Events

    | /call CheckNPCInRange
    | /declare hasNpcInCamp bool local ${Bool[${Macro.Return}]}
    | /if (${Debug}) /echo |- CheckNPCInRange > ${hasNpcInCamp}
    | /if (${hasNpcInCamp}) /goto :endDoClearCampEvents
    
    /call DoLootEvents

    :endDoClearCampEvents
    /if (${Debug}) /echo <== DoCommonEvents -|
/return


| #####################################################################################
| # Auto inventories items from your cursor. 										
| # Alerts the user, and ends the macro, if there is no free inventory space available
| #####################################################################################	
Sub ClearCursor
    /if (${Debug}) /echo |- ClearCursor ==>

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

    /if (${Debug}) /echo <== ClearCursor -|
/return

| #####################################################################################
| # Type string by chrs 										
| #####################################################################################	
Sub TypeChrs(string inputString)
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
/return