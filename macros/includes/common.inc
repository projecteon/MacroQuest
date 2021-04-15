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
        /varset assistPct ${Ini[${iniFileName},${options},AssistPct]}    
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


Sub IniArray(string iniFile, string section, string keyName, int arrayDimensions, arrayName)
    /declare keys       string  local
    /declare k          int     local
    /declare s          int     local

    /varset keys ${Ini[${iniFile},${section}]}
    :TrimKeys
        /if (${keys.Right[1].Equal[|]}) /varset keys ${keys.Left[${Math.Calc[${keys.Length}-1]}]}
        
        /if (${keys.Right[1].Equal[|]}) /goto :TrimKeys

        /declare numberOfBuffs int local ${Math.Calc[${keys.Count[|]}+1]}
        /if (${numberOfBuffs} < 1) /return

        /if (${Defined[${arrayName}]}) /deletevar ${arrayName}
        /declare ${arrayName}[${numberOfBuffs},${arrayDimensions}] string outer
        /declare key string local
        /for k 1 to ${Math.Calc[${keys.Count[|]}+1]}
            /varset key ${keys.Arg[${k},|]}
            /for s 1 to ${arrayDimensions}
                /varset ${arrayName}[${k},${s}] ${Ini[${iniFile},${section},${key}].Token[${s},/]}
            /next s
        /next k
/return