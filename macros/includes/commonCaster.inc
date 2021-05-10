Sub InitCasterIni(string iniFileName)
    /declare options            string  local "CasterOptions"
    /declare generalInitResult  int     local 1
    
    /if (!${Defined[_minMana]})     /declare _minMana     int   outer 15 
    /if (!${Defined[_readyMana]})   /declare _readyMana  int   outer 85 
    /if (!${Defined[_meditateAt]})  /declare _meditateAt  int   outer 90 

    /call InitGenralIni "${iniFileName}"
    /varset generalInitResult ${Macro.Return}

    /if (${Ini[${iniFileName},${options},MinMana].Length}) {
        /varset _minMana ${Ini[${iniFileName},${options},MinMana]}
    } else {
        /ini "${iniFileName}" "${options}" "MinMana" "${_minMana}"
        /varset generalInitResult 0
    }

    /if (${Ini[${iniFileName},${options},ReadyMana].Length}) {
        /varset _readyMana ${Ini[${iniFileName},${options},ReadyMana]} 
    } else { 
        /ini "${iniFileName}" "${options}" "ReadyMana" "${_readyMana}"
    }

    /if (${Ini[${iniFileName},${options},MeditateAt].Length}) {
        /varset _meditateAt ${Ini[${iniFileName},${options},MeditateAt]} 
    } else {
        /ini "${iniFileName}" "${options}" "MeditateAt" "${_meditateAt}" 
    }

/return ${generalInitResult}

Sub CheckManaStatus
    /if (${Debug}) /echo |- CheckManaStatus ==>

	/if (!${Defined[_lowMana]}) /declare _lowMana outer FALSE 	
	/if (${Math.Calc[${Me.MaxMana}*${_minMana}/100]} >= ${Me.CurrentMana} && !${_lowMana}) { 
		/bca I am Low On Mana... 
        /varset _lowMana TRUE
	} else /if (${Math.Calc[${Me.MaxMana}*${_readyMana}/100]} <= ${Me.CurrentMana} && ${_lowMana}) { 
		/bca I am ready...      
        /varset _lowMana FALSE
	} 
    
    /if (${Debug}) /echo <== CheckManaStatus -|
/return 

Sub TryMedidate 
    /if (${Debug}) /echo |- TryMedidate ==>

    /if (${Me.Casting.ID}) {
        /return
    }

    /call CheckManastone
    /call CheckNPCInRange
    /if (!${Defined[hasNpcInCamp]})  /declare hasNpcInCamp bool local ${Bool[${Macro.Return}]}
    /if (${Math.Calc[${Me.CurrentMana} / ${Me.MaxMana} * 100]} < ${_meditateAt} && !${Me.Sitting} && ${hasNpcInCamp} && ${Stick.Status.Equal[OFF]}) {
        /sit
    } else /if (${Me.Sitting} && (${Me.CurrentMana} == ${Me.MaxMana} || ${hasNpcInCamp})) {
        /stand
    }
    
    /if (${Debug}) /echo <== TryMedidate -|
/return

Sub CheckManastone
    /if (${Debug}) /echo |- CheckManastone ==>

    /if ((!${Me.CombatState.Equal[DEBUFFED]}) && !${Me.Invis} && ${Bool[${FindItem[=Manastone]}]} && ${Me.PctMana} < 70 && ${Me.PctHPs} > 70 && ${Me.PctAggro} < 1 && !${Me.CombatState.Equal[COMBAT]} ) {
        /casting "Manastone"    
    }
    
    /if (${Debug}) /echo <== CheckManastone -|
/RETURN