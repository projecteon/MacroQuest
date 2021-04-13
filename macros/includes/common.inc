| ################################################################################
| # Create General Ini Section
| ################################################################################	
Sub InitGenralIni(string iniFileName)
    /declare options string  local "General"
    /if (!${Defined[mainAssist]})  	/declare mainAssist       string	outer "Main Assist" 
    /if (!${Defined[assistPct]})  	/declare assistPct        int	    outer 100 
    /if (!${Defined[Debug]})  		/declare Debug       				outer FALSE 
    
    | Use main assist setting to check if ini file is created
    /if (${Ini[${iniFileName},${options},MainAssist].Length}) {
        /varset mainAssist "${Ini[${iniFileName},${options},MainAssist]}"    
    } else {
        /ini "${iniFileName}" "${options}" "MainAssist" "${mainAssist}"      
        /return 0   
    }    				
    
    /if (${Ini[${iniFileName},${options},AssistPct].Length}) {
        /varset assistPct "${Ini[${iniFileName},${options},AssistPct]}"    
    } 

/return 1


| ################################################################################
| # Checks if NPC is inside camp given radius.
| # Radius default: 60
| ################################################################################	
Sub Is_NPC_In_Camp(int radiusCheck)
	/if (!${Defined[radiusCheck]})  /declare radiusCheck int local 80
	
	/if (${SpawnCount[NPC los radius ${radiusCheck}]} < 1) {
		/return 0
	} 

/return 1

|------------------------------------------------------------|
|- Reliably targets specified Target.IDs.					-|
|------------------------------------------------------------|
|- @param newTargetID: The ID of the spawn you wish to target.	-|
|------------------------------------------------------------|
| SUB TrueTarget(newTargetID)
| 	| Check if we already have target
| 	/if (${Target.ID} != ${newTargetID}) {    
| 		/return	
| 	}
| 	| Check if the target is in zone.
| 	/if (${SpawnCount[id ${newTargetID}]}) {
| 			/squelch /target id ${newTargetID}
| 			/delay 3s ${Target.ID} == ${newTargetID}
| 	} else {
| 		/echo TrueTarget has no spawncount
| 	}
| /RETURN