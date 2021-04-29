| ################################################################################
| # Create Caster Ini Section
| # Returns 1/0 if sections had to be created
| ################################################################################	
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

| ################################################################################
| # Checks a player's mana, report's when below a specified level. 
| # This should be called in the main loop for casting classes.
| ################################################################################	
Sub Check_Mana_Status
	/if (!${Defined[_lowMana]}) /declare _lowMana            outer FALSE 
	
	/if (${Math.Calc[${Me.MaxMana}*${_minMana}/100]} >= ${Me.CurrentMana} && !${_lowMana}) { 
		/bca I am Low On Mana... 
        /varset _lowMana TRUE
	} else /if (${Math.Calc[${Me.MaxMana}*${_readyMana}/100]} <= ${Me.CurrentMana} && ${_lowMana}) { 
		/bca I am ready...      
        /varset _lowMana FALSE
	} 
/return 

Sub Medidate 
    /if (${Me.Casting.ID}) {
        /return
    }

    /call check_Manastone
    /call Is_NPC_In_Camp
    /if (${Math.Calc[${Me.CurrentMana} / ${Me.MaxMana} * 100]} < ${_meditateAt} && !${Me.Sitting} && ${Macro.Return} == 0 && ${Stick.Status.Equal[OFF]}) {
        /sit
    } else /if (${Me.Sitting} && (${Me.CurrentMana} == ${Me.MaxMana} || ${Macro.Return} > 0)) {
        /stand
    }
/return

|-----------------------------------------------------------|
|- Uses Manastone whenever nothing else is going on to     -|
|- bring mana up to 50% while HP is above 70%              -|
|-----------------------------------------------------------|
SUB check_Manastone
/if (${Debug}) /echo |- check_Manastone ==>
    /if ((!${Me.CombatState.Equal[DEBUFFED]}) && !${Me.Invis} && ${Bool[${FindItem[=Manastone]}]} && ${Me.PctMana} < 70 && ${Me.PctHPs} > 70 && ${Me.PctAggro} < 1 && !${Me.CombatState.Equal[COMBAT]} ) {
        /casting "Manastone"    
    }
/if (${Debug}) /echo <== check_Manastone -|
/RETURN