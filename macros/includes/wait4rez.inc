|----------------------------------------------------------------------------|
|- e3_Macro_Wait4Rez.inc v1.4 ( originally designed to work with e3.mac )	-|
|- Written by Killians of PEQ												-|
|----------------------------------------------------------------------------|
|- Based off Wait4Rez.inc by A_Druid_00 (originally written by fantum409)	-|
|----------------------------------------------------------------------------|
|- Accepts rezzes and loots corpses after being killed.						-|
|----------------------------------------------------------------------------|
|- Use,	[Wait4Rez] in /t or /echo to manually call Wait4Rez.				-|
|- Use, [Loot Now] in /t or /echo to manually skip rez acceptance,			-|
|- and begin looting.														-|
|----------------------------------------------------------------------------|

Sub InitWait4Rez(string iniFileName)
    /declare options                        string  local "Wait4Rez"
    /declare Debug_Wait4Rez                 bool    outer FALSE
    /declare wait4RezEnabled                bool    outer FALSE
    /declare wait4RezAlertCount             int     outer 1
    /declare waiting4Rez                    bool    outer FALSE
    /declare waiting4RezMemmedSpells        bool    outer FALSE
    /declare waiting4RezSpellset            string  outer

    /if (${Ini[${iniFileName},${options},Enabled].Length}) {
       /varset wait4RezEnabled ${Bool[${Ini[${iniFileName},${options},Enabled]}]}
    } else {
        /ini "${iniFileName}" "${options}" "Enabled" "${wait4RezEnabled}"
    }

    /if (${Ini[${iniFileName},${options},AlertBeepsCount].Length}) {
       /varset wait4RezAlertCount ${Ini[${iniFileName},${options},AlertBeepsCount]}
    } else {
        /ini "${iniFileName}" "${options}" "AlertBeepsCount" "${wait4RezAlertCount}"
    }

    /if (${Ini[${iniFileName},${options},SpellSet].Length}) {
       /varset waiting4RezSpellset ${Ini[${iniFileName},${options},SpellSet]}
    } else {
        /ini "${iniFileName}" "${options}" "SpellSet" "${waiting4RezSpellset}"
    }

    /call Wait4Rez_Aliases
/return 1


|------------------------------------------------|
|- Automatically engages Wait4Rez when you die.	-|
|------------------------------------------------|
#Event dead	"You have been slain by #*#"
#Event dead	"You died."
Sub Event_dead
    /if (${Debug} || ${Debug_Wait4Rez}) /echo |- Event_dead ==>

	| /call AssistOff
	/if (${wait4RezEnabled}) {
		| Alert master that a bot has died.
		/if (${wait4RezAlertCount}) {
			/declare a int local
			/for a 1 to ${wait4RezAlertCount}
				/beep
				/delay 1
			/next a
		}

    	| Wait for a Rez.
		| /docommand ${ChatToggle} I'm dead. Waiting for rez.
		/bca I'm dead. Waiting for rez.
		/varset waiting4Rez TRUE
		/varset waiting4RezMemmedSpells FALSE
		/call Wait4Rez
	}

    /if (${Debug} || ${Debug_Wait4Rez}) /echo <== Event_Dead -|
/RETURN

|--------------------------------------------|
|- Manually engages Wait4Rez on command.	-|
|--------------------------------------------|
#Event waitNow "#1# tells you, 'Wait4Rez'"
#Event waitNow "<#1#> Wait4Rez"
Sub Event_waitNow(line, chatSender)
    /if (${Debug} || ${Debug_Wait4Rez}) /echo |- Event_WaitNow ==>

    /if (!${checkEventArgs[${chatSender},${line},U,""]}) /return

    /if (${wait4RezEnabled}) {
        /consent ${chatSender}
        |If I'm not already waiting, wait for a rez.
        /if (!${waiting4Rez}) {
		    /varset waiting4Rez TRUE
		    /varset waiting4RezMemmedSpells FALSE
            /call Wait4Rez
        }
    }

    /if (${Debug} || ${Debug_Wait4Rez}) /echo <== Event_WaitNow -|
/RETURN

|--------------------------------------------------------------------------------------------------------|
|- Manual bypasses for rez acceptance and zoning features, to loot corpses that are already close by.	  -|
|--------------------------------------------------------------------------------------------------------|
#Event lootNow "[#1#] Loot Now"
#Event lootNow "<#1#> Loot Now#*#"
#Event lootNow "#1# tells you, 'Loot Now#*#"
Sub Event_lootNow(line, chatSender)
    /if (${Debug} || ${Debug_Wait4Rez}) /echo |- Event_lootNow ==>

    /if (!${checkEventArgs[${chatSender},${line},U,""]}) /return

    /if (${wait4RezEnabled}) {
        /echo Looting....
        /call Wait4Rez LOOT_NOW
    }

    /if (${Debug} || ${Debug_Wait4Rez}) /echo <== Event_lootNow -|
/RETURN

|------------------------------------------------------------------------------------|
|- Consents NetBots, loads spells, accepts rez, and loots your body after zoning.	-|
|------------------------------------------------------------------------------------|
Sub Wait4Rez(functionCall)
    /if (${Debug} || ${Debug_Wait4Rez}) /echo |- Wait4Rez${If[${Defined[functionCall]}, ${functionCall},]} ==>

	/if (${functionCall.Equal[LOOT_NOW]}) /goto :Looting

    /echo Awaiting rez: auto-accepting, and looting my corpse...
	/call ConsentNetbots

    |Load Spells.
	| /if (${Me.Class.CanCast}) /call check_Gems

    |Wait for Rez------------------------------------------------------|
	:rezmeffs
        /if (${Debug} || ${Debug_Wait4Rez}) /echo |- Wait4Rez -| Waiting to accept rez.

        /call Wait4Rez_Background_Events
        /delay 10 ${Window[ConfirmationDialogBox].Open}
        /if (!${waiting4RezMemmedSpells} && ${Bool[${waiting4RezSpellset}]}) {
            /loadspells ${waiting4RezSpellset}
            /varset waiting4RezMemmedSpells TRUE
        }

        /if (!${Window[ConfirmationDialogBox].Open}) /goto :rezmeffs

        |Click yes to rez box.
        /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
	|Wait for zone-----------------------------------------------------|
	:zoneffs
        /if (${Debug} || ${Debug_Wait4Rez}) /echo |- Wait4Rez -| Waiting to zone.

        /delay 300 ${Spawn[${Me}'s].ID}
        /if (!${Spawn[${Me}'s].ID}) /goto :zoneffs

        /delay 30
        |Loot corpse-------------------------------------------------------|
        | /docommand ${ChatToggle} Looting...
	:Looting
        /if (${Debug} || ${Debug_Wait4Rez}) /echo |- Wait4Rez -| Attempting to loot my corpse.

        /if (!${Defined[lootTotal]}) /declare lootTotal int local 0
        /call EnsureTarget ${Spawn[${Me}'s].ID}
        /if (${Target.Distance}<100 && ${Target.CleanName.Equal[${Me}'s corpse]}) {
            |Pull the corpse.
            /if (${Target.Distance}>15) /corpse

            /delay 20 ${Target.Distance}<15
            /loot
            /delay 20 ${Me.State.Equal[BIND]}
            /if (${Me.State.NotEqual[BIND]}) /goto :Looting

            |Wait for all the items on your corpse to appear in your loot window.
            /varset lootTotal -1
            /varset i 0
            :autoxplootlag
            /varcalc i ${i}+1
            /if (${lootTotal}!=${Corpse.Items}) {
                /varset lootTotal ${Corpse.Items}
                /delay 5
                /if (${i}<50) /goto :autoxplootlag
            }

            |Loot All Items.
            /notify LootWnd LootAllButton leftmouseup
            /delay 30s !${Window[LootWnd].Open}

            |Check you don't still have a corpse.
            /if (!${Me.State.Equal[BIND]}) {
                /delay 2s !${Spawn[${Me}'s].ID}
                /if (${Spawn[${Me}'s].ID}) /goto :Looting
            }
            | /docommand ${ChatToggle} Ready to die again!
        }

    /doevents flush

    /varset waiting4RezMemmedSpells FALSE
    | /if (${buffAfterRez} && ${NetBots[${NetBots.Client[1]}].CombatState}>=3) /bc buff me

    /if (${Debug} || ${Debug_Wait4Rez}) /echo <== Wait4Rez -|
/return

Sub ConsentNetbots
	/if (${Debug} || ${Debug_Wait4Rez}) /echo |- ConsentNetbots ==>

    /declare i int local 0
	/for i 1 to ${NetBots.Counts}
	    /delay 2
		/consent ${NetBots.Client[${i}]}
	/next i

    /if (${Debug} || ${Debug_Wait4Rez}) /echo <== ConsentNetbots -|
/return

Sub Wait4Rez_Aliases
	/squelch /alias /wait4rez /bc Wait4Rez
	/squelch /alias /waitforrez /bc Wait4Rez
	/squelch /alias /lootCorpses /bc Loot Now
	/squelch /alias /dead /bc Wait4Rez
/return

 Sub Wait4Rez_Background_Events
   /doevents lootNow
   /doevents waitNow
   /doevents dead
 /return