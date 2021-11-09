|----------------------+----------------------------------------------------------------------------+
| @file     commonAutopilot.inc
| @author   Eredhrin
| @date     v1.00
|
| Uses MQ2Nav, MQ2Nav waypoints and ini file to join waypoints to full travel routes through zones.
|----------------------+----------------------------------------------------------------------------+

Sub DoAutoPilot(string route)
  /if (${Debug} || ${Debug_AutoPilot}) /echo |- DoAutoPilot ==>

  /declare zones string local ${Ini[${autoPilotIniFileName},${route}]}
  /if (${zones.Equal[NULL]}) {
    /echo Route <${route}> has no config in <${autoPilotIniFileName}>, exiting.
    /goto :endAutoPilot
  }

  /declare currentAutoPilotZone    string  local ${Zone.ShortName}
  /declare zoneWayPoints  string  local

  :doAutopilot
  /varset zoneWayPoints ${Ini[${autoPilotIniFileName},${route},${currentAutoPilotZone}]}
  /if (${Debug} || ${Debug_AutoPilot}) /echo <${currentAutoPilotZone}> zoneWayPoints <${zoneWayPoints}>
  /if (${zoneWayPoints.Equal[NULL]}) {
    /echo Arrived at destination or missing waypoints for <${currentAutoPilotZone}>, exiting.
    /goto :endAutoPilot
  } else {
    /if (${Defined[waypoints]}) /deletevar waypoints
    /call IniKeyToArray ${autoPilotIniFileName} ${route} ${currentAutoPilotZone} waypoints
    /call NavigateZoneWaypoints

    /if (${Zone.ShortName.NotEqual[${currentAutoPilotZone}]}) {
      /varset currentAutoPilotZone ${Zone.ShortName}
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

  /if (!${Defined[route]} || ${route.Equal[NULL]}) {
    /bc You must supply a route param.
  } else {
    /call DoAutoPilot ${route}
  }

  /if (${Debug} || ${Debug_AutoPilot}) /echo <== Event_AutoPilot -|
/return

Sub DoAutoPilotEvents
  /doevents AutoPilot
/return