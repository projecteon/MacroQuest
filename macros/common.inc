| ################################################################################
| # Create General Ini Section
| ################################################################################	
Sub InitGenralIni(string iniFileName)
    /declare options string  local "General"
    /if (!${Defined[mainAssist]})  /declare mainAssist       string   outer "Main Assist" 
    
    | Use main assist setting to check if ini file is created
    /if (${Ini[${iniFileName},${options},MainAssist].Length}) {
        /varset mainAssist "${Ini[${iniFileName},${options},MainAssist]}"    
    } else {
        /ini "${iniFileName}" "${options}" "MainAssist" "${mainAssist}"      
        /return 0   
    }

/return 1


| ################################################################################
| # Checks if NPC is inside camp given radius.
| # Radius default: 60
| ################################################################################	
Sub Is_NPC_In_Camp(int radiusCheck)
	/if (!${Defined[radiusCheck]})  /declare radiusCheck int local 80
	
	/if (${SpawnCount[NPC radius los ${radiusCheck}]} < 1) {
		/return 0
	} 

/return 1