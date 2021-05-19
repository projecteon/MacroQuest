Sub InitGenralIni(string iniFileName)
    /declare options string  local "General"
    /if (!${Defined[mainAssist]})  	    /declare mainAssist     string	outer 
    /if (!${Defined[mainTank]})  	    /declare mainTank       string	outer 
    /if (!${Defined[assistPct]})  	    /declare assistPct      int	    outer 100 
    /if (!${Defined[Debug]})  		    /declare Debug       	bool  	outer FALSE 
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
            /if (${Debug}) /echo |-  CheckNPCInRange > TRUE
            /if (${Debug}) /echo <== CheckNPCInRange -|
            /return 1
        }
        
        /next i
    }

    /if (${Debug}) /echo <== CheckNPCInRange -|
/return 0

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