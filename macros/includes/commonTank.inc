Sub AmIOfftank(mainTanks)
    /if (${Debug}) /echo  |- AmIOfftank ==>
    /declare iAmOfftank bool local FALSE

    /if (!${Defined[MainTanks]}) {
        /if (${Debug}) /echo No MainTanks defined.        
        /goto :endAmIOfftank
    }
    
    /declare currentMainTank string local
    /declare mainTank string local
    /declare k  int local
    /for k 1 to ${mainTanks.Size}
        /varcalc i ${k}+1
        /varset mainTank ${mainTanks[${k}]}
        /if (${NetBots[${mainTank}].ID} && ${NetBots[${mainTank}].InZone}) {
            /if (${i} <= ${mainTanks.Size} && ${NetBots[${mainTanks[${i}]}].InZone} && ${mainTanks[${i}].NotEqual[${Me}]}) {
                /goto :endAmIOfftank
            } else /if (${mainTanks[i].Equal[${Me}]}) {      
                /varset iAmOfftank TRUE
                /goto :endAmIOfftank
            }
        }
    /next k 

    :endAmIOfftank
    /if (${Debug}) /echo <== AmIOfftank -|
/return ${iAmOfftank}

Sub TargetOfftankAdd(string currentAssist, string offtank, int otRadius)
    /if (${Debug} || ${Debug_Tank}) /echo |- TargetOfftankAdd ==>

    /if (${Me.Name.NotEqual[${offtank}]}) {
        /if (${Debug} || ${Debug_Tank}) /echo I am not offtank
        /goto :endOfftankAdd
    }

    /declare otTargetId    int local
    /declare maTargetId    int local ${NetBots[${currentAssist}].TargetID}
    /declare otTargetCount int local ${SpawnCount[npc los targetable radius ${otRadius} notid ${maTargetId}]}

    /if (!${otTargetCount}) /goto :endOfftankAdd

    /if (${Debug} || ${Debug_Tank}) /echo |- TargetOfftankAdd - # of offtank targets: ${otTargetCount}
    
    /varset otTargetId ${NearestSpawn[1,npc los targetable radius ${otRadius} notid ${maTargetId}].ID}
    
    /if (${Target.ID} == ${otTargetId}) /goto :endOfftankAdd

    /if (${Debug} || ${Debug_Tank}) /echo |- TargetOfftankAdd - offtank target id: ${otTargetId}

    /call EnsureTarget ${otTargetId}

    :endOfftankAdd
    /if (${Debug} || ${Debug_Tank}) /echo <== TargetOfftankAdd -|
/return

#event Assist "<#1#> offtank nearets target"
SUB event_OfftankNearest(line, sender)
    /if (${Debug} || ${Debug_Tank}) /echo |- event_OfftankNearest ==>
    
    /call AmIOfftank ${mainTanks}
    /declare iAmOffTank  bool local ${Macro.Return}

    /if (!${iAmOffTank}) /return
    
    /call GetMainTank
    /declare mainTank string local ${Macro.Return}
    /call TargetOfftankAdd ${mainTank} ${Me} ${campRadius}
    /if (${Target.ID}) {
        /attack on
	    /killthis
    }

    /if (${Debug} || ${Debug_Tank}) /echo <== event_OfftankNearest -|
/return

Sub DoTankEvents
    /if (${Debug} || ${Debug_Tank}) /echo |- DoTankEvents ==>

    /doevents Assist
    
    /if (${Debug} || ${Debug_Tank}) /echo <== DoTankEvents -|
/return