|----------------------+----------------------------------------------------------------------+
| IDLE              | Waiting for commands
| BUILDING          | In process of building heal chain
| HEALING           | Heal chain is active and running
|----------------------+----------------------------------------------------------------------+

Sub DoHealChain
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- DoHealChain ==>

    /if (${chainState.NotEqual[HEALING]}) /call DoHealChainPreHealingEvents
    
    /while (${chainState.Equal[HEALING]}) {
        /if (!${Defined[prevHealer]})  	/declare prevHealer string local
        /if (!${Defined[currTank]})  	/declare currTank   string local
        /if (${chainHealers.Length} > 0) {
            /if (${chainNextHealTimer} < 2) {
                /call GetNextHealer ${prevHealer}
                /varset prevHealer ${Macro.Return}
                /call GetTank
                /varset currTank ${Macro.Return}

                /tell ${prevHealer} Heal <${currTank}>

                /declare numberOfHealers    int local ${Math.Calc[${chainHealers.Count[,]}+1]}
                /varset chainNextHealTimer ${Math.Calc[100/${numberOfHealers}+5]}
            }
        }
        
        /call DoHealChainHealingEvents
    }
    
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== DoHealChain -|
/return

Sub GetNextHealer(string previousHealer)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- GetNextHealer ==>

    /declare healer          string  local
    /declare numberOfHealers int local ${Math.Calc[${chainHealers.Count[,]}+1]}
    /declare e int local ${Math.Calc[${Select[${previousHealer},${chainHealers}]} + 1]}
    /if (${e} == ${numberOfHealers}) /varset e 1

    /declare k int local
    /for k ${e} to ${numberOfHealers}        
        /if (${SpawnCount[pc ${chainHealers.Arg[${k},,]}]}) /varset healer ${chainHealers.Arg[${k},,]}
    /next k

    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== GetNextHealer -|
/return ${healer}

Sub GetTank
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- GetTank ==>

    /declare tank          string  local
    /declare numberOfTanks int local ${Math.Calc[${chainTankList.Count[,]}+1]}
    /declare k int local
    /for k 1 to ${numberOfTanks}        
        /if (${SpawnCount[pc ${chainTankList.Arg[${k},,]}]}) /varset tank ${chainTankList.Arg[${k},,]}
    /next k

    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== GetTank -|
/return ${tank}

Sub AddHealerToChain(string healer)
    /if (${chainHealers.Length} == 0) {
        /varset chainHealers ${healer}
    } else {
        /varset chainHealers ${chainHealers},${healer}
    }
/return

#Event StartChain  "<#*#> Start heal chain"
Sub Event_StartChain(string eventText)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_StartChain ==>

    /if (${Raid.Members}) {
        /varset chainState HEALING
    }
    
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_StartChain -|
/return

#Event EndChain  "<#*#> Stop heal chain"
Sub Event_EndChain(string eventText)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_EndChain ==>

    /if (${Raid.Members}) {
        /varset chainState IDLE
        /varset chainHealers ""
        /varset chainTiming 0
        /varset chainTankList ""
    }
    
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_EndChain -|
/return

#Event CastHeal  "<#*#> tells you, Heal <#1#>"
Sub Event_CastHeal(string eventText, string healTarget)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_CastHeal ==>

    /if (${chainState.Equal[HEALING]}) {
        /rs <${Time.Time24}> <${Me.PctMana}m> Casting <${If[${Me.Class.ShortName.Equal[clr]},CH,KR]}> on <${Spawn[pc =${healTarget}].Name}>
        /casting "${If[${Me.Class.ShortName.Equal[clr]},Complete Heal,Karana's Renewal]}" "-targetid|${Spawn[pc =${Chain_Target}].ID}" -maxtries|3
    }
    
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_CastHeal -|
/return

#Event MakeChain  "<#*#> Build heal chain #1#"
Sub Event_MakeChain(string eventText, string tankList)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_MakeChain ==>

    /if (!${Raid.Members}) /goto :endEvent_MakeChain
    /if (${chainState.NotEqual[IDLE]}) /goto :endEvent_MakeChain

    /varset chainState BUILDING
    /varset chainHealers ""
    /varset chainTiming 0
    /varset chainTankList ${tankList}

    /rs 'Join heal chain'
    
    :endEvent_MakeChain
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_MakeChain -|
/return

#Event RecruitChain  "#1# tells the raid, 'Join heal chain'"
Sub Event_RecruitChain(string eventText, string sender)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_RecruitChain ==>

    /if (!${Raid.Members}) /goto :endEvent_RecruitChain
    /if (${chainState.Equal[HEALING]}) /goto :endEvent_RecruitChain

    /tell ${sender} 'Add Me'
    
    :endEvent_RecruitChain
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_RecruitChain -|
/return

#Event AddHealer  "#1# tells you, 'Add Me'"
Sub Event_AddHealer(string eventText, string sender)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_AddHealer ==>
    
    /if (!${Raid.Members}) /goto :endEvent_AddHealer
    /if (!${Select[${sender},${Raid.Members}]}) /goto :endEvent_AddHealer

    /if (${Select[${sender},${chainHealers}]}) {
        /rs <{sender}> is already part of the heal chain.
    } else {
        /call AddHealerToChain ${sender}
        /rs Added <${sender}> to healing chain <ChainCount>: <${Math.Calc[${chainHealers.Count[,]}+1]}>
    }

    :endEvent_AddHealer
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_AddHealer -|
/return

#Event RemoveHealer  "#1# tells you, 'Remove me'"
Sub Event_RemoveHealer(string eventText, string sender)
    /if (${Debug} || ${Debug_Heal_Chain}) /echo |- Event_RemoveHealer ==>
    
    /if (!${Raid.Members}) /goto :endEvent_RemoveHealer
    /if (!${Select[${sender},${Raid.Members}]}) /goto :endEvent_RemoveHealer
    /if (!${Select[${sender},${Raid.Members}]}) /goto :endEvent_RemoveHealer

    /if (!${Select[${sender},${chainHealers}]}) {
        /rs <{sender}> is not part of the heal chain.
    } else {
        /declare numberOfHealers    int     local ${Math.Calc[${chainHealers.Count[,]}+1]}
        /declare k                  int     local
        /declare healer             string  local
        /for k 1 to ${numberOfHealers}
            /varset healer ${chainHealers.Arg[${k},,]}
            /if (${Debug} || ${Debug_Heal_Chain}) /echo |- ${healer} > [${k}] > ${chainHealers.arg[${k},,]}
            /if (${sender.NotEqual[${healer}]}) /call AddHealerToChain ${healer}
        /next k
        
        /rs Removed <${sender}> from healing chain <ChainCount>: <${Math.Calc[${chainHealers.Count[,]}+1]}>
    }

    :endEvent_RemoveHealer
    /if (${Debug} || ${Debug_Heal_Chain}) /echo <== Event_RemoveHealer -|
/return

Sub InitHealChain(string iniFileName)
    /declare Debug_Heal_Chain     bool    outer FALSE

    /declare chainState         string  outer IDLE
    /declare chainHealers       string  outer 0
    /declare chainNextHealTimer timer   outer
    /declare chainTankList      string  outer
    /declare canJoinHealChain   bool    outer FALSE

    /declare validClasses       string  local "CLR DRU"

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
	/squelch /alias /tanklist /bc Set Heal Chain Tank List
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