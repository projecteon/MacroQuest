|----------------------+----------------------------------------------------------------------+
| @file     heal_chain.inc
| @author   Eredhrin
| @date     v1.00
|
| Handle main tank heal chain for druids and clerics.
|----------------------+----------------------------------------------------------------------+
| Heal Chain States
| IDLE              | Waiting for commands
| BUILDING          | In process of building heal chain
| HEALING           | Heal chain is active and running
|----------------------+----------------------------------------------------------------------+

Sub DoHealChain
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- DoHealChain ==>

    /if (${chainState.NotEqual[HEALING]}) /call DoHealChainPreHealingEvents

    /declare numberOfHealers    int local
    /while (${chainState.Equal[HEALING]}) {
      /if (!${Defined[prevHealer]}) /declare prevHealer string local
      /if (!${Defined[currTank]})  	/declare currTank   string local
      /if (${chainHealers.Length} > 0) {
          /if (${chainNextHealTimer} < 2) {
              /call GetNextHealer ${prevHealer}
              /varset prevHealer ${Macro.Return}
              /call GetTank
              /varset currTank ${Macro.Return}

              /if (${Bool[${prevHealer}]} && ${Bool[${currTank}]}) {
                  /tell ${prevHealer} Heal <${currTank}>
                  /varcalc numberOfHealers ${chainHealers.Count[,]}+1
                  /varcalc chainNextHealTimer 100/${numberOfHealers}+5
              } else /if (${chainModerator.Equal[${Me}]}) {
                  /if (${Debug} || ${Debug_Heal_Chain}) /echo Missing healer or tank or both to run chain: <${prevHealer}> <${currTank}>
                  /rs Stop heal chain
              }
          }
      }

      /call DoHealChainHealingEvents
    }

    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== DoHealChain -|
/return

Sub GetNextHealer(string previousHealer)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- GetNextHealer ==>

    /declare healer             string  local
    /declare numberOfHealers    int     local ${Math.Calc[${chainHealers.Count[,]}+1]}
    /declare e                  int     local ${Math.Calc[${Select[${previousHealer},${chainHealers}]}+1]}

    /if (${e} > ${numberOfHealers}) /varset e 1

    /declare k int local
    /for k ${e} to ${numberOfHealers}
        /if (${SpawnCount[pc ${chainHealers.Arg[${k},,]}]}) {
            /varset healer ${chainHealers.Arg[${k},,]}
            /break
        }
    /next k

    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== GetNextHealer -|
/return ${healer}

Sub GetTank
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- GetTank ==>

    /declare tank          string   local
    /declare numberOfTanks int      local ${Math.Calc[${chainTankList.Count[,]}+1]}
    /if (${numberOfTanks} == 1) {
        /varset tank ${chainTankList}
    } else {
        /declare k int local
        /for k 1 to ${numberOfTanks}
            /if (${SpawnCount[pc ${chainTankList.Arg[${k},,]}]}) {
                /varset tank ${chainTankList.Arg[${k},,]}
                /break
            }
        /next k
    }

    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== GetTank -|
/return ${tank}

Sub AddHealerToChain(string healer)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- AddHealerToChain ==>

    /if (${chainHealers.Length} == 0) {
        /varset chainHealers ${healer}
    } else {
        /varset chainHealers ${chainHealers},${healer}
    }

    /rs Added <${healer}> to healing chain <ChainCount>: <${Math.Calc[${chainHealers.Count[,]}+1]}>

    :endAddHealerToChain
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== AddHealerToChain -|
/return

#Event StartChain  "#*# tells the raid, 'Start heal chain'"
#Event StartChain  "#*# tells the raid,  'Start heal chain'"
Sub Event_StartChain(string eventText)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_StartChain ==>

    /if (${Raid.Members}) {
        /varset chainState HEALING
    }

    /rs Chain Heal State: <${chainState}>

    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_StartChain -|
/return

#Event EndChain  "#*# tells the raid, 'Stop heal chain'"
#Event EndChain  "#*# tells the raid,  'Stop heal chain'"
Sub Event_EndChain(string eventText)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_EndChain ==>

    /if (${Raid.Members}) {
        /varset chainState IDLE
        /varset chainHealers
        /varset chainNextHealTimer 0
        /varset chainTankList
        /varset chainModerator
    }

    /rs Chain Heal State: <${chainState}>

    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_EndChain -|
/return

#Event CastHeal  "#*# tells you, 'Heal <#1#>'"
Sub Event_CastHeal(string eventText, string healTarget)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_CastHeal ==>

    /if (${chainState.Equal[HEALING]}) {
        /rs <${Time.Time24}> <${Me.PctMana}m> Casting <${If[${Me.Class.ShortName.Equal[clr]},CH,KR]}> on <${Spawn[pc =${healTarget}].Name}>
        /cast "${If[${Me.Class.ShortName.Equal[clr]},Complete Heal,Karana's Renewal]}" "-targetid|${Spawn[pc =${healTarget}].ID}" -maxtries|3
    }

    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_CastHeal -|
/return

| ################################################################################
| # Ask toon to create and moderate heal chain
| # Takes a param of comma separated tanks as tank list
| ################################################################################
#Event MakeChain  "#*# tells you, 'Make Heal Chain #1#'"
#Event MakeChain  "[#*#(msg)] Make Heal Chain #1#"
Sub Event_MakeChain(string eventText, string tankList)
/echo  |- Event_MakeChain ==> ${tankList}
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_MakeChain ==>

    /if (!${Raid.Members}) /goto :endEvent_MakeChain
    /if (${chainState.NotEqual[IDLE]}) /goto :endEvent_MakeChain

    /varset chainState BUILDING
    /varset chainHealers
    /varset chainNextHealTimer 0
    /varset chainTankList ${tankList}
    /varset chainModerator ${Me}

    /if (${canJoinHealChain}) /call AddHealerToChain ${Me}

    /rs Chain Heal State: <${chainState}>
    /rs Join heal chain

    :endEvent_MakeChain
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_MakeChain -|
/return

#Event RecruitChain  "#1# tells the raid, 'Join heal chain'"
#Event RecruitChain  "#1# tells the raid,  'Join heal chain'"
Sub Event_RecruitChain(string eventText, string sender)
/echo  |- Event_RecruitChain ==> ${sender}
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_RecruitChain ==>

    /if (!${Raid.Members}) /goto :endEvent_RecruitChain
    /if (${chainState.Equal[HEALING]}) /goto :endEvent_RecruitChain

    /if (${canJoinHealChain}) {
        /varset chainModerator ${sender}
        /tell ${sender} Add Me
    }

    :endEvent_RecruitChain
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_RecruitChain -|
/return

#Event AddHealer  "#1# tells you, 'Add Me'"
Sub Event_AddHealer(string eventText, string sender)
/echo  |- Event_AddHealer ==> ${sender}
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_AddHealer ==>

    /if (!${Raid.Members}) /goto :endEvent_AddHealer
    /if (${Select[${sender},${Raid.Members}]}) /goto :endEvent_AddHealer

    /if (${Select[${sender},${chainHealers}]}) {
        /rs <{sender}> is already part of the heal chain.
    } else {
        /call AddHealerToChain ${sender}
    }

    :endEvent_AddHealer
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_AddHealer -|
/return

#Event RemoveHealer  "#1# tells you, 'Remove me'"
Sub Event_RemoveHealer(string eventText, string sender)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_RemoveHealer ==>

    /if (!${Raid.Members}) /goto :endEvent_RemoveHealer
    /if (!${Select[${sender},${Raid.Members}]}) /goto :endEvent_RemoveHealer

    /if (!${Select[${sender},${chainHealers}]}) {
        /rs <{sender}> is not part of the heal chain.
    } else {
        /declare removeHealer string local ${sender},
        /varset chainHealers ${chainHealers.Replace[${removeHealer},]}
        /rs Removed <${sender}> from healing chain <ChainCount>: <${Math.Calc[${chainHealers.Count[,]}+1]}>
    }

    :endEvent_RemoveHealer
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_RemoveHealer -|
/return

Sub InitHealChain(string iniFileName)
    /declare Debug_Heal_Chain     bool    outer FALSE

    /declare chainState         string  outer IDLE
    /declare chainHealers       string  outer
    /declare chainNextHealTimer timer   outer
    /declare chainTankList      string  outer
    /declare chainModerator     string  outer
    /declare canJoinHealChain   bool    outer FALSE

    /declare validClasses       string  local "clr"

    /if (${Ini[${iniFileName},Debug,Debug_Heal_Chain].Length}) {
       /varset Debug_Heal_Chain ${Bool[${Ini[${iniFileName},Debug,Debug_Heal_Chain]}]}
    }

    /if (${Select[${Me.Class.ShortName},${validClasses}]}) {
        /if (${Ini[${iniFileName},HealingOptions,HealChain].Length}) {
        /varset Debug_Heal_Chain ${Bool[${Ini[${iniFileName},HealingOptions,HealChain]}]}
        }
    }

	/squelch /alias /makehealchain /bc Make Heal Chain
	/squelch /alias /endhealchain /bc End Heal Chain
/return

Sub DoHealChainPreHealingEvents
    /doevents AddHealer
    /doevents RemoveHealer
    /doevents MakeChain
    /doevents RecruitChain
    /doevents StartChain
/return

Sub DoHealChainHealingEvents
    /doevents EndChain
    /doevents CastHeal
/return