Sub AmIOfftank
    /if (${Debug}) /echo  |- AmIOfftank ==>
    /declare iAmOfftank bool local FALSE

    /if (!${Defined[MainTanks]}) {
        /if (${Debug}) /echo No MainTanks defined.        
        /goto :endAmIOfftank
    }
    
    /declare currentMainTank string local
    /declare mainTank string local
    /declare i  int local
    /declare k  int local
    /for k 1 to ${MainTanks.Size}
        /varcalc i ${k}+1
        /varset mainTank ${MainTanks[${k}]}
        /if (${NetBots[${mainTank}].ID} && ${NetBots[${mainTank}].InZone}) {
            /if (${i} <= ${MainTanks.Size} && ${NetBots[${MainTanks[${i}]}].InZone} && ${MainTanks[${i}].NotEqual[${Me}]}) {
                /goto :endAmIOfftank
            } else /if (${MainTanks[${i}].Equal[${Me}]}) {      
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

#Event OfftankNearest "<#1#> offtank nearets target"
Sub event_OfftankNearest(line, sender)
    /if (${Debug} || ${Debug_Tank}) /echo |- event_OfftankNearest ==>
    
    /call AmIOfftank
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

    /doevents OfftankNearest
    
    /if (${Debug} || ${Debug_Tank}) /echo <== DoTankEvents -|
/return