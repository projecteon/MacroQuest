Sub InitSpell
  /if (!${Defined[spellPropsDimentions]})         /declare spellPropsDimentions       int     outer 6
  /if (!${Defined[spellNameProp]})                /declare spellNameProp              int     outer 1
  /if (!${Defined[spellGem]})                     /declare spellGem                   int     outer 2
  /if (!${Defined[spellMinManaProp]})             /declare spellMinManaProp           int     outer 3
  /if (!${Defined[spellClassRestrictionProp]})    /declare spellClassRestrictionProp  int     outer 4
  /if (!${Defined[spellMinTargetHpProp]})         /declare spellMinTargetHpProp       int     outer 5
  /if (!${Defined[itemCastProp]})                  /declare itemCastProp              int     outer 6

  /if (!${Defined[spellProps]}) {
		/declare spellProps[${spellPropsDimentions}] string outer
		/varset spellProps[${spellNameProp}] CastName
		/varset spellProps[${spellGem}] Gem
		/varset spellProps[${spellMinManaProp}] MinMana
		/varset spellProps[${spellClassRestrictionProp}] Classes
		/varset spellProps[${spellMinTargetHpProp}] MinTargetHp
		/varset spellProps[${itemCastProp}] ItemCast
	}
/return

Sub IniSpellArray(string iniFile, string section, string keyName, arrayName)
  /if (${Debug}) /echo |- IniSpellArray ==>

  /declare keys       string  local
  /declare k          int     local
  /declare s          int     local

  /varset keys ${Ini[${iniFile},${section}]}


  /if (${Debug}) /echo |- ${iniFile} > ${section} > ${keys}
  /if (${keys.Equal[NULL]}) {
    /if (${Debug}) /echo <keys> is NULL, exiting.
    /goto :endIniSpellArray
  }

  :trimKeys
    /if (${keys.Right[1].Equal[|]}) /varset keys ${keys.Left[${Math.Calc[${keys.Length}-1]}]}

    /if (${keys.Right[1].Equal[|]}) /goto :trimKeys

    /if (${Debug}) /echo |- numberOfKeys > ${Math.Calc[${keys.Count[|]}+1]}

    /declare numberOfValidKeys int local 0
    /declare key string local
    /for k 1 to ${Math.Calc[${keys.Count[|]}+1]}
      /varset key ${keys.Arg[${k},|]}
      /if (${key.Left[-2].Equal[${keyName}]} || ${key.Left[-1].Equal[${keyName}]}) {
        /varcalc numberOfValidKeys ${numberOfValidKeys}+1
      }
    /next k

    /if (${numberOfValidKeys} < 1) {
      /if (${Debug}) /echo No valid keys found, exiting.
      /goto :endIniArray
    }

    /if (${Debug}) /echo |- numberOfValidKeys > ${numberOfValidKeys}

    /if (${Defined[${arrayName}]}) /deletevar ${arrayName}
    /declare ${arrayName}[${numberOfValidKeys},${spellPropsDimentions}] string outer

    /declare value string local
    /for k 1 to ${Math.Calc[${keys.Count[|]}+1]}
      /varset key ${keys.Arg[${k},|]}
      /if (${key.Left[-2].Equal[${keyName}]} || ${key.Left[-1].Equal[${keyName}]}) {
        /for s 1 to ${spellPropsDimentions}
          /if (${Debug}) /echo |- parsing ${Ini[${iniFile},${section},${key}]} > spellProps[${s}] > ${spellProps[${s}]}

          /call getValue "${Ini[${iniFile},${section},${key}]}" ${s}
          /varset value ${Macro.Return}

          /if (${Debug}) /echo |- ${key} > [${k},${s}] > ${value}

          /varset ${arrayName}[${k},${s}] ${value}
        /next s
      }
    /next k

  :endIniSpellArray
  /if (${Debug}) /echo <== IniSpellArray -|
/return

Sub getValue(string iniValue, int spellKeyIndex)
  /if (!${Bool[${iniValue}]}) /return

  | First key is always spell name, and has no prefix
  /if (${spellKeyIndex} <= 1) /return ${iniValue.Arg[1,/]}

  | if outside range, default to NULL
  /if (${spellKeyIndex} > ${spellPropsDimentions}) /return

  /declare numSubValues   int     local ${Math.Calc[${iniValue.Count[/]}+1]}
  /declare subValue       string  local
  /declare k              int     local
  /declare key            string  local ${spellProps[${spellKeyIndex}]}
  /for k 2 to ${numSubValues}
    /varset subValue ${iniValue.Arg[${k},/]}
    /if (${subValue.Arg[1,|].Equal[${key}]}) /return ${subValue.Arg[2,|]}
  /next k
/return