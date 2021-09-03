Sub InitCasterIni(string iniFileName)
    /declare options            string  local "CasterOptions"
    /declare initResult         int     local 1
    
    /if (!${Defined[_minMana]})                 /declare _minMana               int     outer 15 
    /if (!${Defined[_readyMana]})               /declare _readyMana             int     outer 85 
    /if (!${Defined[_meditateAt]})              /declare _meditateAt            int     outer 90 
    /if (!${Defined[_meditateWithNpcInCamp]})   /declare _meditateWithNpcInCamp bool    outer FALSE 
    /if (!${Defined[_manaStoneMinPctHP]})       /declare _manaStoneMinPctHP     int     outer 80 
    /if (!${Defined[_manaStoneMaxPctMana]})     /declare _manaStoneMaxPctMana   int     outer 70 
    /if (!${Defined[_evacSpell]})               /declare _evacSpell             string  outer 
    /if (!${Defined[Debug_Caster]})             /declare Debug_Caster           bool    outer FALSE

    /if (${Ini[${iniFileName},${options},MinMana].Length}) {
        /varset _minMana ${Ini[${iniFileName},${options},MinMana]}
    } else {
        /ini "${iniFileName}" "${options}" "MinMana" "${_minMana}"
        /varset initResult 0
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

    /if (${Ini[${iniFileName},${options},MeditateWithNpcInCamp].Length}) {
        /varset _meditateWithNpcInCamp ${Ini[${iniFileName},${options},MeditateWithNpcInCamp]} 
    } else {
        /ini "${iniFileName}" "${options}" "MeditateWithNpcInCamp" "${_meditateWithNpcInCamp}" 
    }

    /if (${Ini[${iniFileName},ManaStone,MinPctHP].Length}) {
        /varset _manaStoneMinPctHP ${Ini[${iniFileName},ManaStone,MinPctHP]} 
    }

    /if (${Ini[${iniFileName},ManaStone,MaxPctMana].Length}) {
        /varset _manaStoneMaxPctMana ${Ini[${iniFileName},ManaStone,MaxPctMana]} 
    }

    /if (${Ini[${iniFileName},${options},EvacSpell].Length}) {
        /varset _evacSpell ${Ini[${iniFileName},${options},EvacSpell]} 
    }

    /if (${Ini[${iniFileName},Debug,Debug_Caster].Length}) {
       /varset Debug_Caster ${Bool[${Ini[${iniFileName},Debug,Debug_Caster]}]}     
    } 

    /squelch /alias /evacuate /echo EVACUATE NOW
/return ${initResult}

Sub CheckManaStatus
    /if (${Debug} || ${Debug_Caster}) /echo |- CheckManaStatus ==>

	/if (!${Defined[_lowMana]}) /declare _lowMana outer FALSE 	
	/if (${Math.Calc[${Me.MaxMana}*${_minMana}/100]} >= ${Me.CurrentMana} && !${_lowMana}) { 
		/bca I am Low On Mana... 
        /varset _lowMana TRUE
	} else /if (${Math.Calc[${Me.MaxMana}*${_readyMana}/100]} <= ${Me.CurrentMana} && ${_lowMana}) { 
		/bca I am ready...      
        /varset _lowMana FALSE
	} 
    
    /if (${Debug} || ${Debug_Caster}) /echo <== CheckManaStatus -|
/return 

Sub TryMedidate 
    /if (${Debug} || ${Debug_Caster}) /echo |- TryMedidate ==>

    /if (${Me.Casting.ID}) {
        /goto :endTryMedidate
    }

    /call CheckManastone
    /call CheckNPCInRange
    /declare hasNpcInCamp bool local ${Bool[${Macro.Return}]}
    /if (${Debug} || ${Debug_Caster}) /echo |- hasNpcInCamp ${hasNpcInCamp}
    /if (${Me.PctMana} < ${_meditateAt} && !${Me.Sitting} && (!${hasNpcInCamp} || ${_meditateWithNpcInCamp}) && ${Stick.Status.Equal[OFF]}) {
        /sit
    } else /if (${Me.Sitting} && (${Me.CurrentMana} == ${Me.MaxMana} || (${hasNpcInCamp} && !${_meditateWithNpcInCamp}))) {
        /stand
    }
    
    :endTryMedidate
    /if (${Debug} || ${Debug_Caster}) /echo <== TryMedidate -|
/return

Sub CheckManastone
    /if (${Debug} || ${Debug_Caster}) /echo |- CheckManastone ==>

    /if (!${Me.Invis} && ${Bool[${FindItem[=Manastone]}]} && ${Me.PctMana} < ${_manaStoneMaxPctMana} && ${Me.PctHPs} > ${_manaStoneMinPctHP}) {
        /casting "Manastone"    
    }
    
    /if (${Debug} || ${Debug_Caster}) /echo <== CheckManastone -|
/return

Sub EmptyCancelCast
/return


Sub DoEvacute
    /if (${Debug} || ${Debug_Caster}) /echo |- DoEvacute ==>

    /if (!${bool[${_evacSpell}]}) {
        /goto :endDoEvac
    }

    /if (!${Me.Gem[${_evacSpell}]}) {
        /goto :endDoEvac
    }

    /if (${Me.Casting.ID}) /call Interrupt
    
    /if (!${Me.SpellReady[${_evacSpell}]}) /delay 3s ${Me.SpellReady[${_evacSpell}]}

    /call cast "${_evacSpell}" gem${Me.Gem[${_evacSpell}]} 0 CancelEvac 3
    /bca <<< EVACUATING >>>
    
    :endDoEvac
    /if (${Debug} || ${Debug_Caster}) /echo <== DoEvacute -|
/return

Sub CancelEvac
/return

| ################################################################################
| # Summon pet event
| ################################################################################
#Event Evac "#*#[MQ2] EVACUATE NOW#*#"
Sub Event_Evac
    /if (${Debug} || ${Debug_Caster}) /echo |- Event_Evac ==>

    /call DoEvacute
    
    /if (${Debug} || ${Debug_Caster}) /echo <== Event_Evac -|
/return

Sub DoCasterEvents
  /doevents Evac
/return