Sub InitGenralIni(string iniFileName)
    /declare options string  local "General"
    /if (!${Defined[mainAssist]})  	    /declare mainAssist     string	outer 
    /if (!${Defined[mainTank]})  	    /declare mainTank       string	outer 
    /if (!${Defined[assistPct]})  	    /declare assistPct      int	    outer 100 
    /if (!${Defined[Debug]})  		    /declare Debug       	bool  	outer FALSE 
    | sea furious are |32, but so are guards... needs more testing
    /if (!${Defined[aggroAnimation]})   /declare aggroAnimation string  outer |5|8|12|17|18|32|42|44|80|106|129|144|
    
    | Use main assist setting to check if ini file is created
    /if (${Ini[${iniFileName},${options},MainAssist].Length}) {
        /varset mainAssist "${Ini[${iniFileName},${options},MainAssist]}"    
    } else {
        /ini "${iniFileName}" "${options}" "MainAssist" "${mainAssist}"      
        /return 0   
    }
    
    /if (${Ini[${iniFileName},${options},MainTank].Length}) {
        /varset mainTank "${Ini[${iniFileName},${options},MainTank]}"    
    } else {
        /ini "${iniFileName}" "${options}" "MainTank" "${mainTank}"      
        /return 0   
    }    				    				
    
    /if (${Ini[${iniFileName},${options},AssistPct].Length}) {
        /varset assistPct ${Ini[${iniFileName},${options},AssistPct]}    
    } 

    /if (${Ini[${iniFileName},${options},Debug].Length}) {
       /varset Debug ${Bool[${Ini[${iniFileName},${options},Debug]}]}     
    } 

/return 1


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

    /if (${SpawnCount[id ${targetId}]}) {
			/squelch /target id ${targetId}
			/delay 3s ${Target.ID} == ${targetId}
	} else {
		/echo EnsureTarget has no spawncount
	}
    
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