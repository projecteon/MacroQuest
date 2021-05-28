Sub IniArray(string iniFile, string section, string keyName, int arrayDimensions, arrayName)
    /declare keys       string  local
    /declare k          int     local
    /declare s          int     local

    /varset keys ${Ini[${iniFile},${section}]}
    :TrimKeys
        /if (${keys.Right[1].Equal[|]}) /varset keys ${keys.Left[${Math.Calc[${keys.Length}-1]}]}
        
        /if (${keys.Right[1].Equal[|]}) /goto :TrimKeys

        /declare numberOfKeys int local ${Math.Calc[${keys.Count[|]}+1]}
        /if (${numberOfKeys} < 1) /return

        /if (${Defined[${arrayName}]}) /deletevar ${arrayName}
        /declare ${arrayName}[${numberOfKeys},${arrayDimensions}] string outer

        /declare key string local
        /for k 1 to ${Math.Calc[${keys.Count[|]}+1]}
            /varset key ${keys.Arg[${k},|]}
            /for s 1 to ${arrayDimensions}
                /varset ${arrayName}[${k},${s}] ${Ini[${iniFile},${section},${key}].Token[${s},/]}
            /next s
        /next k
/return