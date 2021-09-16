Sub DoAutoPilot(string route)
  /if (${Debug} || ${Debug_AutoPilot}) /echo |- DoAutoPilot ==> 

  /declare zones string local ${Ini[${autoPilotIniFileName},${route}]}
  /if (${zones.Equal[NULL]}) {
    /echo Route <${route}> has no config in <${autoPilotIniFileName}>, exiting.
    /goto :endAutoPilot
  }

  /declare currentZone    string  local ${Zone.ShortName}
  /declare zoneWayPoints  string  local

  :doAutopilot
  /varset zoneWayPoints ${Ini[${autoPilotIniFileName},${route},${currentZone}]}
  /if (${Debug} || ${Debug_AutoPilot}) /echo zoneWayPoints <${zoneWayPoints}>
  /if (${zoneWayPoints.Equal[NULL]}) {
    /echo No waypoints for <${currentZone}>, exiting.
    /goto :endAutoPilot
  } else {
    /if (${Defined[waypoints]}) /deletevar waypoints
    /call IniKeyToArray ${autoPilotIniFileName} ${route} ${currentZone} waypoints
    /call NavigateZoneWaypoints

    /if (${Zone.ShortName.NotEqual[${currentZone}]}) {
      /varset currentZone ${Zone.ShortName}
      /goto :doAutopilot
    }
  }
  
  :endAutoPilot
  /if (${Debug} || ${Debug_AutoPilot}) /echo <== DoAutoPilot -|
/return

Sub NavigateZoneWaypoints
  /if (${Debug} || ${Debug_AutoPilot}) /echo number of waypoints <${waypoints.Size}>
  /declare k  int local
  /for k 1 to ${waypoints.Size}
    /if (!${Navigation.PathExists[wp ${waypoints[${k}]}]}) {
      /bc Could not find nav path to waypoint ${waypoints[${k}]}
      /return
    }

    /nav wp ${waypoints[${k}]}
    :doNaviate
    /delay 5
    /if (${Navigation.Active}) /goto :doNaviate
  /next k 
/return

Sub InitAutopilot(string iniFile)
  /declare autoPilotIniFileName string  outer "data/Autopilot Routes.ini"
  /declare Debug_AutoPilot 	    bool  	outer TRUE 

  
    /squelch /alias /autopilot /bc autopilot too
/return

| ################################################################################
| # Autopilot to navpoint
| ################################################################################
#Event AutoPilot "<#*#> autopilot too #1#"
#Event AutoPilot "#*#[MQ2] autopilot too #1#"
Sub Event_AutoPilot(eventText, route)
  /if (${Debug} || ${Debug_AutoPilot}) /echo |- Event_AutoPilot ==> 

  /if (!${Defined[route]}) {
    /bc You must supply a route param. 
  } else {
    /call DoAutoPilot ${route}
  }

  /if (${Debug} || ${Debug_AutoPilot}) /echo <== Event_AutoPilot -|
/return

Sub DoAutoPilotEvents
  /doevents AutoPilot
/return