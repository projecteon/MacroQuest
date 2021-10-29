Sub IniArray(string iniFile, string section, string keyName, int arrayDimensions, arrayName)
  /if (${Debug}) /echo |- IniArray ==>

  /declare keys       string  local
  /declare k          int     local
  /declare s          int     local

  /varset keys ${Ini[${iniFile},${section}]}

  /if (${Debug}) /echo |- ${iniFile} > ${section} > ${keys}
  /if (${keys.Equal[NULL]}) {
    /if (${Debug}) /echo <keys> is NULL, exiting.
    /goto :endIniArray
  }

  :TrimKeys
    /if (${keys.Right[1].Equal[|]}) /varset keys ${keys.Left[${Math.Calc[${keys.Length}-1]}]}

    /if (${keys.Right[1].Equal[|]}) /goto :TrimKeys

    /declare numberOfKeys int local ${Math.Calc[${keys.Count[|]}+1]}
    /if (${Debug}) /echo |- numberOfKeys > ${numberOfKeys}

    /if (${numberOfKeys} < 1) {
      /if (${Debug}) /echo No keys found, exiting.
      /goto :endIniArray
    }

    /if (${Defined[${arrayName}]}) /deletevar ${arrayName}
    /declare ${arrayName}[${numberOfKeys},${arrayDimensions}] string outer

    /declare key string local
    /for k 1 to ${Math.Calc[${keys.Count[|]}+1]}
      /varset key ${keys.Arg[${k},|]}
      /for s 1 to ${arrayDimensions}
        /if (${Debug}) /echo |- ${key} > [${k},${s}] > ${Ini[${iniFile},${section},${key}].Token[${s},/]}

        /varset ${arrayName}[${k},${s}] ${Ini[${iniFile},${section},${key}].Token[${s},/]}
      /next s
    /next k

  :endIniArray
  /if (${Debug}) /echo <== IniArray
/return

Sub IniArray2(string iniFile, string section, string keyName, arrayName)
  /if (${Debug}) /echo |- IniArray2 ==>

  /declare keys       string  local
  /declare k          int     local
  /declare s          int     local

  /varset keys ${Ini[${iniFile},${section}]}

  /if (${Debug}) /echo |- ${iniFile} > ${section} > ${keys}
  /if (${keys.Equal[NULL]}) {
    /if (${Debug}) /echo <keys> is NULL, exiting.
    /goto :endIniArray
  }

  :TrimKeys
    /if (${keys.Right[1].Equal[|]}) /varset keys ${keys.Left[${Math.Calc[${keys.Length}-1]}]}

    /if (${keys.Right[1].Equal[|]}) /goto :TrimKeys

    /declare numberOfKeys int local ${Math.Calc[${keys.Count[|]}+1]}
    /if (${numberOfKeys} < 1) {
      /if (${Debug}) /echo No keys found, exiting.
      /goto :endIniArray
    }

    /if (${Debug}) /echo |- numberOfKeys > ${numberOfKeys}

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
    /declare ${arrayName}[${numberOfValidKeys}] string outer

    /varset numberOfValidKeys 1
    /for k 1 to ${Math.Calc[${keys.Count[|]}+1]}
      /varset key ${keys.Arg[${k},|]}
      /if (${key.Left[-2].Equal[${keyName}]} || ${key.Left[-1].Equal[${keyName}]}) {
        /if (${Debug}) /echo |- ${key} > ${arrayName}[${numberOfValidKeys}] > ${Ini[${iniFile},${section},${key}]}
        /varset ${arrayName}[${numberOfValidKeys}] ${Ini[${iniFile},${section},${key}]}
        /varcalc numberOfValidKeys ${numberOfValidKeys}+1
      }
    /next k

  :endIniArray
  /if (${Debug}) /echo <== IniArray2
/return

Sub IniKeyToArray(string iniFile, string section, string keyName, arrayName)
  /if (${Debug}) /echo |- IniKeyToArray ==>

  /declare values     string  local
  /declare k          int     local
  /declare key        string  local

  /varset values ${Ini[${iniFile},${section},${keyName}]}

  /if (${values.Equal[NULL]}) {
    /if (${Debug}) /echo <values> is NULL, exiting.
    /goto :endIniKeyToArray
  }

  /declare numberOfValues int local ${Math.Calc[${values.Count[/]}+1]}

  /if (${Defined[${arrayName}]}) /deletevar ${arrayName}
  /declare ${arrayName}[${numberOfValues}] string outer

  /for k 1 to ${numberOfValues}
    /varset key ${values.Arg[${k},|]}
    /if (${Debug}) /echo |- ${key} > [${k}] > ${values.Token[${k},/]}
    /varset ${arrayName}[${k}] ${values.Token[${k},/]}
  /next k

  :endIniKeyToArray
  /if (${Debug}) /echo <== IniKeyToArray
/return