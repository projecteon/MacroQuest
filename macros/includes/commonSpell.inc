Sub InitSpell
    /if (!${Defined[spellPropsDimenstions]})        /declare spellPropsDimenstions      int     outer 4 
    /if (!${Defined[spellNameProp]})                /declare spellNameProp              int     outer 1 
    /if (!${Defined[spellGem]})                     /declare spellGem                   int     outer 2 
    /if (!${Defined[spellMinManaProp]})             /declare spellMinManaProp           int     outer 3 
    /if (!${Defined[spellClassRestrictionProp]})    /declare spellClassRestrictionProp  int     outer 4 

    /if (!${Defined[SpellProps]}) {
		/declare SpellProps[${spellPropsDimenstions}] string outer 0
        | Dont set spellPropName, its default value and cannot be named
		| /varset SpellProps[${spellNameProp}] CastName
		/varset SpellProps[${spellGem}] Gem
		/varset SpellProps[${spellMinManaProp}] MinMana
		/varset SpellProps[${spellClassRestrictionProp}] Classes
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

    :TrimKeys
        /if (${keys.Right[1].Equal[|]}) /varset keys ${keys.Left[${Math.Calc[${keys.Length}-1]}]}
        
        /if (${keys.Right[1].Equal[|]}) /goto :TrimKeys

        /declare numberOfKeys int local ${Math.Calc[${keys.Count[|]}+1]}
        /if (${Debug}) /echo |- numberOfKeys > ${numberOfKeys}

        /if (${numberOfKeys} < 1) {
            /if (${Debug}) /echo No keys found, exiting.
            /goto :endIniSpellArray
        }

        /if (${Defined[${arrayName}]}) /deletevar ${arrayName}
        /declare ${arrayName}[${numberOfKeys},${spellPropsDimenstions}] string outer

        /declare value string local
        /declare key string local
        /for k 1 to ${Math.Calc[${keys.Count[|]}+1]}
            /varset key ${keys.Arg[${k},|]}
            /for s 1 to ${spellPropsDimenstions}
                /if (${Debug}) /echo |- parsing ${Ini[${iniFile},${section},${key}]} >  ${SpellProps[${s}]}

                /call getValue ${Ini[${iniFile},${section},${key}]} ${SpellProps[${s}]}
                /varset value ${Macro.Return}
                
                /if (${Debug}) /echo |- ${key} > [${k},${s}] > ${value}
 
                /varset ${arrayName}[${k},${s}] ${value}
            /next s
        /next k

    :endIniSpellArray
    /if (${Debug}) /echo <== IniSpellArray
/return

Sub getValue(string iniValue, string key)
    /if (!${Bool[${iniValue}]}) /return

    /declare numSubValues               int     local ${Math.Calc[${iniValue.Count[/]}+1]}
    /declare subValue                   string  local
    /declare k                          int     local
    
    /for k 1 to ${numSubValues}
        /if (!${Defined[key]} || !${Bool[${key}]}) /return ${iniValue.Arg[${k},/]}

        /varset subValue ${iniValue.Arg[${k},/]}
        /if (${subValue.Arg[1,|].Equal[${key}]}) /return ${subValue.Arg[2,|]}
    /next k
/return