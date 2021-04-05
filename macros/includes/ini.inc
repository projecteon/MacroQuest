
Sub iniArray(string INIFile, string section)
    /declare Keys       string  local
    /declare k          int     local
    /declare s          int     local

    /varset Keys ${Ini[${INIFile},${section}]}
    :TrimKeys
        /if (${Keys.Right[1].Equal[|]}) /varset Keys ${Keys.Left[${Math.Calc[${Keys.Length}-1]}]}
        
        /if (${Keys.Right[1].Equal[|]}) /goto :TrimKeys

        /declare numberOfBuffs int local ${Math.Calc[${Keys.Count[|]}+1]}
        /if (${numberOfBuffs} < 1) /return

        /declare Buffs[${numberOfBuffs},${iniBuffDimensions}] string outer
        /declare key string local
        /for k 1 to ${Math.Calc[${Keys.Count[|]}+1]}
            /varset key ${Keys.Arg[${k},|]}
            /for s 1 to ${iniBuffDimensions}
                /varset Buffs[${k},${s}] ${Ini[${INIFile},${section},${key}].Token[${s},/]}
            /next s
        /next k
/return