| # @file - common.inc
| # @author - Reno
| # 
| # General include file used by almost every player.

| ***********
| * Includes
| ***********

|- Autoaccetps rez and loots corpse
#include Wait4Rez.inc

|- Casts any spell/item you may need
#include Spell_Routines.inc 

|- Need this because I use it everywhere
| #include DataStructures.inc

|- God, I hope this works
| #include CHRot.inc

| *********
| * Events
| *********

|- The event to trigger the call for rez and corpse looting
#Event ImDead           "You have been slain by#*#" 

|- Event to autojoin groups, one for titanium and one for SoD
#Event Invited          "#*#To join the group, click on the 'FOLLOW' option, or 'DISBAND' to cancel#*#"
#Event Invited          "#*#To join the group, click on the 'FOLLOW' option, or 'DECLINE' to cancel#*#"

|- Raid invites
#Event RaidInvited		"#*#invites you to join a raid.#*#"
#Event RaidInvite		"#1# tells the guild, '#*#raid invite#*#'"

|- These event's are used to trigger a chat event via BCS
#Event IRC              "<#1#> #*# #2#"
#Event IRC				"[#1#(msg)] #2#"

|- Follow and stop following events
#Event Follow           "<#1#> #*# #*#follow#*#"
#Event Follow           "[#1#(msg)] #*#follow#*#"
#Event Stop				"[#1#(msg)] #*#stop#*#"
#Event Stop             "<#1#> #*# #*#stop#*#"

|- Event for hailing an npc
#Event HailIt			"<#1#> #*# Hail"

|- Event for barking text in say
#Event SayIt			"<#1#> #*# Bark #2#"

|- Event for PEQ'n
#Event PEQZone			"<#1#> #*# Zone #2#"

|- Event for buffing all of my groups
#Event BuffAll			"<#*#> #*# Full Buff"

|- Event for Forming a group
#Event FormGroup		"[#1#(msg)] Group#*#"
#Event FormGroup		"<#*#> #*# Form Group"

|- Events for controlling Vet AA's
#Event Devoted			"<#*#> #*# Devoted#*#"
#Event Faithful			"<#*#> #*# Faithful#*#"
#Event Jester			"<#*#> #*# Jester#*#"
#Event ExRecovery		"<#*#> #*# ExRecovery#*#"
#Event Servant			"<#*#> #*# Servant#*#"
#Event StRecovery		"<#*#> #*# StRecovery#*#"
#Event Resolute			"<#*#> #*# Resolute#*#"
#Event Devoted			"[#1#(msg)] Devoted#*#"
#Event Faithful			"[#1#(msg)] Faithful#*#"
#Event Jester			"[#1#(msg)] Jester#*#"
#Event ExRecovery		"[#1#(msg)] ExRecovery#*#"
#Event Servant			"[#1#(msg)] Servant"
#Event StRecovery		"[#1#(msg)] StRecovery#*#"
#Event Resolute			"[#1#(msg)] Resolute#*#"

|- Event for clicking off a buff
#Event ClickOff			"<#*#> #*# Remove #1#"
#Event ClickOff			"[#*#(msg)] Remove #1#"

|- Click the nearest door
#Event ClickDoor		"<#1#> #*# ClickDoor"

|- Event for using bard Melody
#Event Melody			"[#1#(msg)] #*#Melody #2# #3# #4# #5#"
#Event Melody			"<#1#> #*# #*#Melody #2# #3# #4# #5#"

|- Event for controlling melee combat
#Event Combat			"<#1#> #*# #*#Combat #2# #3#"
#Event Combat			"[#1#(msg)] #*#Combat #2# #3#"
#Event Combat			"#1# tells the group#*#Combat #2#"

|- Event for getting BCS conencted users
#Event ConnectedNames	"#*#Names: #1#"
#Event BCSUser			"#*#has joined the server#*#"
#Event BCSUser			"#*#has left the server#*#"

|- Event to click yes on a confirmation window
#Event ClickYes			"<#1#> #*# #*#Click Yes#*#"

|- Event for DPS burn
#Event BurnDmg			"<#1#> #*# BURN #2#"

|- Find items
#Event FindItem			"<#*#> #*# Find Item #1#"

|- Event for counting toons near a player
#Event CountBotsNearMe	"[#*#(msg)] #*#Count Bots#*#"

|- Event for toggling ranged attack on Berz or Ranger
#Event ToggleRanged     "<#*#> #*# Use Ranged #1#"

#Event Rez				"<#1#> #*# rez"
#Event Rez				"#1# tells you, 'rez'"
#Event Rez				"#1# tells the group, 'rez'"

| #################################################################
| # Will move to and hail the Chat Sender's target.
| #
| # @PARAM - string Line - The line that triggered the event.
| # @PARAM - string ChatSentBy - Person who sent the command.
| #################################################################
sub Event_HailIt(Line,ChatSentBy)

	/if (!${Spawn[${ChatSentBy}].ID}) {
		/return
	}
	
	/assist ${ChatSentBy}
	/delay 2s ${Target.Type.Equal[NPC]}
	
	/if (${Target.Type.Equal[NPC]}) {
		/squelch /stick hold 8
		/delay 2s (${Target.Distance} <= 10)
		/keypress h
		/squelch /stick off
	}
		
/return

| #################################################################
| # Initiates sticking to the chat sender.
| #
| # @PARAM - string Line - The line that triggered the event.
| # @PARAM - string ChatSentBy - Person who sent the command.
| #################################################################
sub Event_Follow(Line,ChatSentBy)
	|- Who do we follow?
	/if (!${Defined[FollName]}) /declare FollName string outer 
	/varset FollName ${ChatSentBy}
	/if (!${Defined[Underwater]}) /declare Underwater outer FALSE
	
	|- Not in zone!
	/if (!${Spawn[${FollName}].ID}) {
		/return
	}
	
	|- Get off the horse
	/if (${Me.Mount.ID}) /dismount 

	|- Get the target
	/target pc ${FollName} 
	/delay 2s ${Target.Name.Equal[${FollName}]} 
	/if (${Target.Name.Equal[${FollName}]}) { 
		|- Let's stick to it then
		/call DoStick hold 12
	} 
		
/return

| ######################################################
| # Stops any and all current sticking.
| # 
| # @PARAM - string Line - The line that triggered the event.
| # @PARAM - string Sender - The person who initiated the stop.
| ######################################################
sub Event_Stop(Stop, Sender)
	/if (!${Defined[UseMount]}) /declare UseMount outer FALSE
	
	|- They better be in zone
	/if (${Spawn[${Sender}].ID}) {
		/squelch /stick off 
		/if (${UseMount} && !${Me.Mount.ID}) {
			/call GetMountItem
			/declare MountItem string local ${Macro.Return}
			/if (${MountItem.NotEqual[NULL]}) /call cast ${MountItem} item
		}
	}
	
/return

| ############################################
| # Handles autofollow functionality. Reverts
| # to stick uw if player is underwater.
| #
| # @PARAM - string StickType - The type of stick to used. I.E. behind, moveback
| # @PARAM - int Distance - The distance to stick at.
| ############################################
sub DoStick(string StickType, int Distance)	
	|- OMG! Water!
	/if (${Me.Underwater}) {
		/squelch /stick off
		/squelch /stick uw ${StickType} ${Distance}
	|- No water 
	} else {
		/squelch /stick off
		/squelch /stick ${StickType} ${Distance}
	}
	
/return

| ###################################################
| # Will echo the chat sender's command in /say.
| # Sender MUST be in zone.
| #
| # @PARAM - SayIt - The line that triggered the event
| # @PARAM - ChatSentBy - Person who sent the command
| # @PARAM - Message - The message to be echoed
| ###################################################
sub Event_SayIt(SayIt,ChatSentBy,Message)
	/if (${SpawnCount[pc ${ChatSentBy}]} > 0) {
		/assist ${ChatSentBy}
		/delay 1s 
		/say ${Message}
	}
/return

| ########################################################
| # Ports BCS text to existing MQ2 chat system, Event_Chat.
| #
| # @PARAM - IRCSay - The line that triggered the event.
| # @PARAM - IRCSender - The player who sent the command.
| # @PARAM - IRCCommand - The command issued.
| ########################################################
Sub Event_IRC(IRCSay,IRCSender,IRCCommand)
	/call Event_Chat "IRC" "${IRCSender}" "${IRCCommand}"
/return

| ##############################################################
| # On death will bark for rez, wait for rez box, click accept
| # and auto loot corpse upon succesful landing at corpse
| # NOTE: Uses Wait4Rez.inc that I have modified. While it is
| #       entirely possible that it will continue to work with 
| #       a basic wait4rez, it might not
| ##############################################################
sub Event_ImDead
	/call wait4rez
/return

| ##
| # Finds and returns an item in inventory that can summon a mount
| #
| ##
sub GetMountItem
	/declare ItemName string local NULL
	
	/if (${FindItemCount[bridle]} > 0) /varset ItemName "${FindItem[bridle].Name}"

/return ${ItemName}

| ################################################################################
| # Checks a player's mana, report's when below a specified level. This should be
| # called in the main loop for casting classes.
| ################################################################################	
Sub Check_Mana 
	/if (!${Defined[LowMana]}) /declare LowMana       outer FALSE 
	/if (!${Defined[MinMana]}) /declare MinMana       int   outer 15 
	
	/if (${Math.Calc[${Me.MaxMana}*${MinMana}/100]} >= ${Me.CurrentMana} && !${LowMana}) { 
		/varset LowMana TRUE 
		/bca I am LOM... 
	} else /if (${Math.Calc[${Me.MaxMana}*${BuffMana}/100]} <= ${Me.CurrentMana} && ${LowMana}) { 
		/bca I am GTG...      
		/varset LowMana FALSE  
	} 
/return 

| #################################
| # Auto-accepts any group invite
| #################################
Sub Event_Invited 
   /invite 
/return

| ############################################################
| # Zones all BCS conencted clients to specified zone.
| #
| # @PARAM - PEQ - The line that triggered the event.
| # @PARAM - Sender - Player who issued command.
| # @PARAM - ZoneToGo - Zone to move to.
| ############################################################
sub Event_PEQZone(Line,Sender,ZoneToGo)
	/say #peqzone ${ZoneToGo}
	
/return

| ############################################################
| # Will place melee into close quarter combat routines. This
| # gets called during an assist call.
| #
| # @PARAM - string Combat - THe line that triggered the event.
| # @PARAM - string MainAssist - Player who initiated the combat.
| # @PARAM - string ToTarget - The mob you want to attack.
| # @PARAM - int Distance - Range to stay away from target(optional).
| ############################################################	
sub Event_Combat(Combat,MainAssist,ToTarget,Distance)
	/if (!${Defined[MeleeDistance]}) /declare MeleeDistance int outer 9
	/if (!${Defined[Attacking]}) /declare Attacking outer FALSE
	/if (!${Defined[DoWisCombat]}) /declare DoWisCombat outer FALSE
	/if (!${Defined[RangedAttack]}) /declare RangedAttack outer FALSE
	/if (!${Defined[AttackTarget]}) /declare AttackTarget int outer

	/if (!${Spawn[${MainAssist}].ID}) /return
	
	/if (${Defined[Distance]}) /varset MeleeDistance ${Distance}
	
	/assist ${MainAssist}
	/delay 2s ${Target.Name.Equal[${ToTarget}]}
	/if (!${Target.Name.Equal[${ToTarget}]}) /return	
	/if (${Me.State.Equal[Feign Death]}) /return
	
	/varset Attacking TRUE
	/varset AttackTarget ${Target.ID}

	/if (${Me.Class.Name.Equal[Shaman]} || ${Me.Class.Name.Equal[Enchanter]}) {
		/call DebuffCycle
	}
	
	/if ((${Me.Class.Name.Equal[Berserker]} || ${Me.Class.Name.Equal[Ranger]}) && (${RangedAttack})) {
		/if (!${Defined[RangedDistance]}) /declare RangedDistance int outer 35
		/if (!${Defined[StickOnRange]}) /declare StickOnRange outer bool FALSE
		/squelch /stick off
		/if (${StickOnRange}) {
			/call DoStick moveback ${RangedDistance}
		}

		/autofire
		
	} else /if (${Me.Class.Name.Equal[Berserker]} || ${Me.Class.Name.Equal[Ranger]} || ${Me.Class.Name.Equal[Rogue]} || ${Me.Class.Name.Equal[Paladin]} || ${Me.Class.Name.Equal[Shadow Knight]} || ${Me.Class.Name.Equal[Monk]} || ${Me.Class.Name.Equal[Bard]} || ${Me.Class.Name.Equal[Beastlord]}) {
		/attack on
		/call DoStick "behind moveback" ${MeleeDistance}
	} else /if (${Me.Class.Name.Equal[Warrior]}) {
		/attack on
		/call DoStick moveback 8	
	} else {
		/if ((${Me.Class.Name.Equal[Cleric]} || ${Me.Class.Name.Equal[Shaman]}) && ${Me.PctMana} > 70 && ${DoWisCombat}) {
			/attack on
			/call DoStick behind ${MeleeDistance}
		} else {
			/squelch /stick off
		}
	}
	
	/if (${SpawnCount[${Me.Pet}]} > 0) {
		/delay 1s
		/pet attack
	}
	
/return

| ##
| # Toggles ranged attacking on or off
| #
| # @PARAM - string Line - The line that triggered the event.
| # @PARAM - bool StickTo - Do we stick to our combat targets.
| ##
Sub Event_ToggleRanged(string Line, string StickTo)
	/if (!(${Me.Class.Name.Equal[Berserker]} || ${Me.Class.Name.Equal[Ranger]})) /return
	/if (!${Defined[RangedDistance]}) /declare RangedDistance int outer 35
	/if (!${Defined[RangedAttack]}) /declare RangedAttack outer FALSE
	/if (!${Defined[StickOnRange]}) /declare StickOnRange bool outer FALSE
	
	/varset StickOnRange ${StickTo}
	
	/if (${RangedAttack}) {
		/varset RangedAttack FALSE
	} else {
		/varset RangedAttack TRUE
	}
/return

| ##
| #
| #
| ##
Sub GetAttackTarget
	/if (!${Attacking}) /return
	/if (${Attacking} && ${SpawnCount[id ${AttackTarget}]} < 1) {
		/varset Attacking FALSE
		/return
	}	

	/if (${Spawn[id ${AttackTarget}].Distance} > 210) /return
	
	:GetAttackTarget
	/target id ${AttackTarget}
	/delay 3s ${Target.ID} == ${AttackTarget}
	
	/if (${Target.ID} != ${AttackTarget} && ${Spawn[id ${AttackTarget}].Name.Find['s_corpse]}) {
		/goto :GetAttackTarget
	} else {
		/varset Attacking FALSE
	}

/return

| ###################################################
| # Called by every class in the main to handle 
| # combat routines for melee classes. 
| ###################################################
Sub DoUpdates
	/if (!${Defined[AmCamping]}) /declare AmCamping outer FALSE
	/if (!${Target.ID} && ${Attacking}) /varset Attacking FALSE
	/if (!${Target.Type.Equal[NPC]} && !${Target.Type.Equal[Pet]} && ${Attacking}) {
		/varset Attacking FALSE
		| /gsay I AM HERE! --> ${Target.Type}
		/attack off
	}
	
	/if (!${Attacking}) /return
	
	|- Ranger stuff
	/if (${Me.Class.Name.Equal[Ranger]}) {
		/if (${Me.Combat} && ${Target.Distance} < 25 && ${Me.AbilityReady[Kick]}) /doability "Kick"
		/if (${RangedAttack} && ${Target.PctHPs} > 0) /face fast nolook
		/return
	}
	
	|- Monk stuff
	/if (${Me.Class.Name.Equal[Monk]}) {
		/if (${Me.Combat} && ${Target.Distance} < 35 && ${Me.AbilityReady[Flying Kick]}) /doability "Flying Kick"
		/if (${Me.Combat} && ${Me.PctHPs} < 20) /doability "Feign Death"
		/return
	}
	
	|- Berserker stuff
	/if (${Me.Class.Name.Equal[Berserker]}) {
	    /if (${Me.Combat}  && !${Me.Moving} && ${Me.AbilityReady[Frenzy]}) /doability Frenzy
	   
		/if ((${Me.Combat} || (${RangedAttack} && ${Attacking})) && !${Me.Moving} && (${Me.Endurance} > 200) && (!${Defined[timerRageVolley]} || ${timerRageVolley} == 0)) {
			/disc Rage Volley
			
			/if (!${Defined[timerRageVolley]}) {
				/declare timerRageVolley timer outer
			}
			
			/varset timerRageVolley   135
		}
		 
		/return
	}
	
	|- SK and Paladin bash (2hand bash must be trained or you will get spammed)
	/if (${Me.Class.Name.Equal[Shadow Knight]} || ${Me.Class.Name.Equal[Paladin]}) {
		/if (${Me.Combat} && ${Target.Distance} < 25 && ${Me.AbilityReady[Bash]}) /doability "Bash"
		/return
	}
	
	|- Beastlord
	/if (${Me.Class.Name.Equal[Beastlord]}) {
		/if (${Me.Combat} && ${Target.Distance} < 25 && ${Me.AbilityReady[Kick]}) /doability "Kick"
		/return
	}
	
	|- Warrior
	/if (${Me.Class.Name.Equal[Warrior]}) {
		/if (${Me.Combat} && ${Target.Distance} < 25 && ${Me.AbilityReady[${If[${Me.Inventory[Offhand].Type.Equal[Shield]},Bash,Kick]}]}) /doability ${If[${Me.Inventory[Offhand].Type.Equal[Shield]},Bash,Kick]}		
		/return
	}
	
	|- Rogue stuff
	/if (${Me.Class.Name.Equal[Rogue]}) {
	    /if (${Attacking} && !${Me.Moving}) {
			/if (${Me.AbilityReady["Backstab"]}) /doability "Backstab"
			/if (${Me.AbilityReady["Hide"]}) { 
			   /attack off 
			   /delay 2 
			   /doability "Hide" 
			   /attack on
			} 
		}
		/return
   }
   
/return

| Below is written by Cantus, and stolen from his Common.inc with his permission.
| ***********************************************
| * InMyGroup()									*
| *												*
| * A utility function that accepts a character	*
| * name as a @PARAMeter, and determines whether	*
| *	or not that character is in the calling		*
| * character's group (and in the same zone).	*
| *												*
| * ${Macro.Return} will be set to TRUE or FALSE*
| ***********************************************
| ##
| # A utility function that accepts a character name as a parameter, and 
| # determines whether or not that character is in the calling character's 
| # group (and in the same zone). Written By Cantus.
| # 
| # @PARAM - string szCharName - The name of the character we are looking for.
| # @RETURN - TRUE if player is in group or FALSE if not.
| ##
Sub InMyGroup(string szCharName)

	/declare GroupMember	int	local 0

	/for GroupMember 0 to ${Group.Members}

		/if ( ${Select[${Group.Member[${GroupMember}].Type},PC]} == 1 ) {

			/if ( ${Group.Member[${GroupMember}].CleanName.Equal[${szCharName}]} ) {

				/return TRUE
			}
		}

	/next GroupMember

/return FALSE

| #################################################
| # Buffs every group via existing buff routines.
| #################################################
Sub Event_BuffAll
	/if (${Group.Leader.Name.Equal[${Me.CleanName}]}) {
		/bct Elena buff
		/bct Asystole buff
		/bct Mezzem buff
		/bct Allannon buff
	}
/return

| #################################################
| # Form's a group using predefined names
| # NOTE: Halfass and needs work to allow for 
| #       differening situations and group members
| #       Maybe base something off BCS clients?
| #################################################
Sub Event_FormGroup
	|- Reno
	/if (${Me.CleanName.Equal[Rude]}) {
		/invite Reno
		/invite Ultimate
		/invite Aeaeni
		/invite Elena
		/invite Asystole
	|- Geko
	} else /if (${Me.CleanName.Equal[Geko]}) {
		/invite Leeeroy
		/invite Pyrosanon
		/invite Finder
		/invite Xtravagent
		/invite Furry
	|- Xtravagent
	} else /if (${Me.CleanName.Equal[Xcess]}) {
		/invite	Tseng
		/invite Xorbitant
		/invite Mezzem
		/invite Narga
		/invite Allannon
	}
/return

| #################################################
| # Lesson of the Devoted AA activation
| #################################################
Sub Event_Devoted()
	/aa act Lesson of the Devoted	
/return

| #################################################
| # Infusion of the Faithful AA Activation
| #################################################
Sub Event_Faithful()
	/aa act Infusion of the Faithful	
/return

| #################################################
| # Chaotic Jester AA Activation
| #################################################
Sub Event_Jester()
	/aa act Chaotic Jester	
/return

| #################################################
| # Expedient Recovery AA Activation
| #################################################
Sub Event_ExRecovery()
	/aa act Expedient Recovery	
/return

| #################################################
| # Steadfast Servant AA Activation
| #################################################
Sub Event_Servant()
	| I hate Plane of Air
	| /aa act Steadfast Servant
/return

| #################################################
| # Staunch Recovery AA Activation
| #################################################
Sub Event_StRecovery()
	/aa act Staunch Recovery
	
/return

| #################################################
| # Intensity of the Resolute AA Activation
| #################################################
Sub Event_Resolute()
	/aa act Intensity of the Resolute
	
/return

| #############################################################
| # Clicks off a specified buff.
| #
| # @PARAM - string Line - The line that triggered the event.
| # @PARAM - string BuffToRemove - The buff to be removed.
| #############################################################
Sub Event_ClickOff(string Line, string BuffToRemove)
    /if (${Me.Buff[${BuffToRemove}].ID}) /nomodkey /notify BuffWindow Buff${Math.Calc[${Me.Buff[${BuffToRemove}].ID}-1].Int} leftmouseup
/return

| #########################################################
| # This sub will move a PC to the nearest door and click.
| # It currently is set for my use from Chase view camera 
| # angles. I have First Person view bound to the 'n' key
| # and Chase view bound to the 'o' key. If you do not want
| # to use these views, feel free to modify the binds as 
| # see fit.
| # NOTE: You MUST toggle to First Person before it can 
| #       click on the door.
| #########################################################
Sub Event_ClickDoor	
	/declare StartZone int local ${Zone.ID}
	/if (${Stick.Status.Equal[ON]}) /squelch /stick off
	/if (${Target.ID}) /squelch /target clear
	
	|- Target the door
	/squelch /doortarget

	/if (${DoorTarget.Distance} > 30) /return
	
	|- This is the loop to move us to the door
	:MoveToDoor
	/if (${DoorTarget.Distance} > 10) {
		/squelch /face fast door
		/nomodkey /keypress forward hold
		/delay 3
		/nomodkey /keypress forward
		/goto :MoveToDoor
	|- Now that we are at the door
	} else {		
		|- This is the key bind for your First Person camera View. I use 'n'
		/nomodkey /keypress N
		
		:DidNotClickDoor
		/squelch /face fast door
		/keypress esc
		/target pc ${Me.CleanName}
		/delay 3s ${Target.CleanName.Equal[${Me.CleanName}]}
		
		|- Center screen
		/nomodkey /keypress e
		
		|- This is the keybind to Use CenterScreen
		/nomodkey /keypress u
		
		/if (${Target.CleanName.NotEqual[${Me.CleanName}]}) {
			/goto :DidNotClickDoor
			/gsay Going back to :DidNotClickDoor
		}
		
		|- Wait til we have zoned
		/delay 5m ${Zone.ID} != ${StartZone}
		
		|- Move back to Chase View. I use 'o'
		/nomodkey /keypress O
	}
	
/return

| #################################################
| # Plays melody with specified songs
| #
| # @PARAM - string ChatSender - The sender of the request
| # @PARAM - string SongOne - The first song.
| # @PARAM - string SongTwo - The second song.
| # @PARAM - string SongThree - The third song.
| # @PARAM - string SongFour - The fourth song.
| #################################################
Sub Event_Melody(string ChatSender, string SongOne, string SongTwo, string SongThree, string SongFour)
	/melody ${SongOne} ${SongTwo} ${SongThree} ${SongFour} 
	
/return

| ######################################################################
| # This function controls the use, and reuse, of any combat ability
| # NOTE: Helped with busted skill timers in Titanium, with SoD it
| #       appears that it may no longer be needed.
| #
| # @PARAM - string Ability - The string name of the ability you want to use
| ######################################################################
Sub CombatAbility(string Ability)
	/if (!${Defined[timerCombatAbility]} || ${timerCombatAbility} == 0) {
	
		/doability ${Ability}
		
		/if (!${Defined[timerCombatAbility]} ) {
			/declare timerCombatAbility    timer    outer
		}
		
		/varset timerCombatAbility   40
	}

/return

| ##
| # Builds a dynamic array named "BCUsers" that contains the names of all players 
| # logged into BoxChat.
| #
| # @PARAM - string Line - The line that triggered the event.
| # @PARAM - string users - The string containing the names of all the BCS connected users.
| ##
Sub Event_ConnectedNames(string Line, string users)
/if (!${Defined[BCUsers]}) /declare BCUsers string outer "BCUsers"
/declare i int local 1
/declare end int local ${users.Count[ ]}
/varcalc end ${end}+1
/data ${Vector[${BCUsers}].Erase}	

	| /if (${end} > 8) /endmacro
	/for i 1 to ${end}
		/if (${users.Arg[${i}, ].Length} > 0) {
			/if (${users.Arg[${i}, ].Find[.]} > 0) {
				/data ${Vector[${BCUsers}].Insert[${users.Arg[${i}, ].Replace[.,]}]}
			} else {
				/data ${Vector[${BCUsers}].Insert[${users.Arg[${i}, ]}]}
			}
		}
	/next i

/return

| ##
| # Tells us to rebuild the BCUser dynamic array because something has changed.
| # NOTE: Just deletes and rebuilds from scratch, would be better to break into
| #       adds and deletes. Would save some cpu time.
| ##
Sub Event_BCSUser
	/if (${Defined[BCUsers]}) /bccmd names
/return

| ############################
| # Clicks yes button on a 
| # popup window.
| ############################
Sub Event_ClickYes
	/notify ConfirmationDialogBox Yes_Button leftmouseup
/return

| #####################################################
| # Returns the position number
| # of a specified group member.
| #
| # @PARAM - string Name - The name of the group member.
| ######################################################
Sub GetGroupPosition(string Name)
	/declare iGroup int local 0
	/for iGroup 0 to ${Group.Members}
		/if (${Name.Equal[${Group.Member[${iGroup}]}]}) {
			/return ${iGroup}
		} 
	/next iGroup
	
/return 0

| ##
| # Will count and echo the number of bots in zone 
| # and within a certain radius
| #
| # @PARAM - string Line - The line that triggered the event.
| # @PARAM - int Radius - The radius to check for bots. Defaults to 75 if not set.
| ##
Sub Event_CountBotsNearMe(string Line, int Radius)
	/if (!${Defined[Radius]}) /declare Radius int local 75
	
	/if (!${Defined[BCUsers]}) {
		/bccmd names
		/delay 2s
		/doevents ConnectedNames
	}
	
	/declare BotsInZone int local 0
	/declare BotsInRange int local 0
	/declare MissingNames string local 
	
	/declare i int local 0
	/declare size int local 0
	/varcalc size ${Vector[${BCUsers}].Size} - 1
	
	/for i 0 to ${size}
		/if (!${Defined[Bot]}) /declare Bot string local 
		/varset Bot ${Vector[${BCUsers}].At[${i}]}
		/if (!${SpawnCount[pc ${Bot}]} > 0) /next i
		
		/varcalc BotsInZone ${BotsInZone} + 1
		
		/if (${SpawnCount[pc ${Bot} radius ${Radius}]} > 0) {
			/varcalc BotsInRange ${BotsInRange} + 1
		} else {
			/varset MissingNames ${MissingNames} ${Bot}
		}
	/next i
	
	/echo In Zone[${BotsInZone}] :: In Range[${BotsInRange}] :: Missing[${Math.Calc[${BotsInZone} - ${BotsInRange}]}]
	/if (${BotsInRange} < ${BotsInZone}) {
		/echo Missing Toons:[${MissingNames}]
	} else {
		/echo ALL TOONS PRESENT!
	}		
/return

| ################################################################## 
| # Handles all DPS increasing functions for each class.
| #
| # @PARAM - string Line - The chat line that triggered the event.
| # @PARAM - string MainAssist - The player to assist off of.
| # @PARAM - string ToTarget - The NPC to be damaged.
| ##################################################################
Sub Event_BurnDmg(string Line, string MainAssist, string ToTarget)
	|- Verify target
	/if (!${Target.Name.Equal[${ToTarget}]}) {
		/assist ${MainAssist}
		/delay 2s ${Target.Name.Equal[${ToTarget}]}
	}	
	/if (!${Target.Name.Equal[${ToTarget}]}) /return
	
	|- WIZARD
	/if (${Me.Class.Name.Equal[Wizard]}) {
		/call cast "Call of Xuzl" alt
		/return
	|- SHAMAN
	} else /if (${Me.Class.Name.Equal[Shaman]}) {
		/call Cast "Spirit Call" alt	
		/return
	|- RANGER
	} else /if (${Me.Class.Name.Equal[Ranger]}) {
		/call Cast "Guardian of the Forest" alt
		/if (${FindItem[Heartwood Blade].ID}) /call cast "Heartwood Blade" item
		/return
	|- MONK
	} else /if (${Me.Class.Name.Equal[Monk]}) {
		/disc Innerflame
		/return
	|- ROGUE
	} else /if (${Me.Class.Name.Equal[Rogue]}) {
		/disc Twisted Chance
		/return
	|- BERSERKER
	} else /if (${Me.Class.Name.Equal[Berserker]}) {
		/disc Ancient: Cry of Chaos
		/delay 1s
		/disc Cleaving Anger
		/return
	|- SHADOW KNIGHT
	} else /if (${Me.Class.Name.Equal[Shadow Knight]}) {
		/return
	|- MAGICIAN
	} else /if (${Me.Class.Name.Equal[Magician]}) {
		/call Cast "Host of the Elements" alt
		/return
	|- WARRIOR
	} else /if (${Me.Class.Name.Equal[Warrior]}) {
		/return
	}
	
/return

| ###############################################
| # Checks to see if an NPC is in camp. Sets the
| # variable CampClear to TRUE or FALSE.
| #
| # @PARAM - int Radius - The distance to check
| ###############################################
Sub NPCInCamp(int Radius)
	/if (!${Defined[Radius]}) /declare Radius int local 60
	/if (!${Defined[CampClear]}) /declare CampClear outer FALSE
	
	/if (${SpawnCount[NPC radius ${Radius}]} < 1) {
		/varset CampClear TRUE
	} else {
		/varset CampClear FALSE
	}

/return

| ###############################################
| # Called during the Main Loop for all casters.
| # This sub manages the sitting and differing 
| # things that a caster does when not in combat.
| # I.E. Sitting, Mana regen, etc
| ###############################################
Sub CheckCasterState
	/if (!${Defined[SitAt]}) /declare SitAt int outer 80
	
	|- Sit if no NPC's close
	/if (${Me.State.Equal[Stand]} && (${Me.PctMana} < ${SitAt}) && !${Me.Moving} && ${Stick.Status.Equal[OFF]} && ${Me.Casting.ID} <= 0 && !${Attacking}) {
		/call NPCInCamp 20
		/if (${CampClear}) {
			/sit 
		}
	|- Stand up if NPC in camp
	} else /if (${Me.State.Equal[Sit]} && ${Stick.Status.Equal[ON]}) {
		/stand
	}
	
	|- CLERIC
	/if (${Me.Class.Name.Equal[Cleric]}) {
		|- Yaulp 4
		| /if (!${Defined[SpellYaulp]}) /declare SpellYaulp outer "Yaulp VI"
		| /if (!${Defined[YaulpMana]}) /declare YaulpMana int outer 75
		| /if (!${Me.Buff["Yaulp VI"].ID} && !${Me.Combat} && !${Me.Moving} && ${Me.PctMana} <= ${YaulpMana}&& (${Me.SpellReady[${SpellYaulp}]} || !${Me.Gem[${SpellYaulp}]})) {
		|	/call cast ${SpellYaulp} gem9 2s
		| }
	|- SHAMAN
	} else /if (${Me.Class.Name.Equal[Shaman]}) {
		|- Canni 4
		/if (!${Defined[CanniMana]}) /declare CanniMana int outer 80
		/if (!${Defined[SpellCanni]}) /declare SpellCanni outer "Cannibalize IV"
		/if (!${Me.Combat} && !${Me.Moving} && ${Me.PctMana} <= ${CanniMana} && (${Me.SpellReady[${SpellCanni}]} || !${Me.Gem[${SpellCanni}]}) && ${Me.PctHPs} > 65 && ${Stick.Status.NotEqual[ON]} && !${Attacking}) {
			/call cast ${SpellCanni} gem8 1s
		}
		
		/if (!${Me.Moving} && ${Me.PctMana} <= 60 && (!${Defined[timerCannibalization]} || ${timerCannibalization} == 0) && ${Me.CurrentHPs} > 2000) {
			/call Cast "Cannibalization" alt
			/if (!${Defined[timerCannibalization]}) {
				/declare timerCannibalization    timer    outer
			}

			/varset timerCannibalization    1800
		}
	|- WIZARD
	} else /if (${Me.Class.Name.Equal[Wizard]}) {
		|- Druzzil Harvest
		/if (${Me.PctMana} < 80 && !${Me.Combat} && !${Me.Moving} && (!${Defined[DruzzilTimer]} || ${DruzzilTimer} == 0)) {
			/if (!${Defined[DruzzilTimer]}) /declare DruzzilTimer timer outer 0
			
			/call Cast "Harvest of Druzzil" alt
			/delay 9s
			/varset DruzzilTimer 4810
		}
		
		|- Mana Robe kept getting left on. This just equips my primary robe if it isn't equipped.
		/if ((${InvSlot[17].Item.Name.Equal[Mana Robe]} || ${InvSlot[17].Item.Name.Equal[Koadic's Robe of Heightened Focus]}) && ${FindItemCount[Robe of Secrets]} > 0) {
			/exchange "Robe of Secrets" 17
		}
		
		/if (!${Me.Buff[Improved Familiar].ID} && ${Me.AltAbilityReady[Improved Familiar]}) {
			/pet get lost
			/call cast 167 alt
			/pet get lost
		}
		|- Mana Robe
		| /if (${Me.State.Equal[Stand]} && (${Me.PctMana} < 50) && (${Me.PctHPs} > 70) && !${Me.Moving} && ${Stick.Status.Equal[OFF]} && ${Me.Casting.ID} <= 0 && !${Attacking}) {
		|	/call Cast "Mana Robe" item
		|}
	}
/return

| ###########################################
| # Need a function to find and attack
| # npc's that are in camp. Check for LOS 
| # and all that happy shit. This is a hack
| # and is not nearly a working copy.
| # 
| # @PARAM - int Radius - (Optional) The radius to check for NPC's. Default is 100.
| ###########################################
Sub GetClosestNPC(int Radius)
	/if (!${Defined[Radius]}) /declare Radius int local 100
	
	/if (${SpawnCount[NPC radius ${Radius} los]} >= 1) {
		/target ${NearestSpawn[NPC radius ${Radius} los]}
		/delay 2s ${Target.Type.Equal[NPC]}		
		
		/call DoStick hold 12
		/attack on
		
		/bca Combat ${Target.Name} 12
		/varset Assisting TRUE
	} 
/return

| ##
| #
| ##
Sub Event_FindItem(string Line, string ToFind)
	/declare Found bool local TRUE
	/if (${FindItemCount[${ToFind}]} > 0) {
		/say I have ${FindItemCount[${ToFind}]} of ${FindItem[${ToFind}]} on me.
	} else {
		/varset Found FALSE
	} 
	
	/if (${FindItemBankCount[${ToFind}]} > 0) {
		/say I have ${FindItemBankCount[${ToFind}]} of ${FindItemBank[${ToFind}]} in my bank.
		/varset Found FALSE
	}
	
	/if (!${Found}) {
		/say No item found!
	}
/return

| ##
| #
| ##
Sub Event_Rez(string Line, string Sender)
	/if (${FindItemCount[Water Sprinkler of Nem Ankh]} < 1) /return

	/target "${Sender}'s corpse"
	/delay 3s ${Target.CleanName.Equal[${Sender}'s corpse]}
	
	/if (${Target.CleanName.NotEqual[${Sender}'s corpse]}) /return
	
	/call cast "Water Sprinkler of Nem Ankh" item
	
/return

| ##
| # Populates a map from an ini file.
| #
| # @PARAM - string FileName - The Ini file to be read
| # @PARAM - string SectioName - The section of the Ini to be used
| # @PARAM - string NameOfMap - The name of the map we wish to populate.
| ##
Sub IniToHashMap(FileName,SectionName,NameOfMap)
	|-Does the key exist?
	/if (${Ini[${FileName},${SectionName},-1,NO].Equal[NO]}) {
		| /echo KeyName not valid
		/return
	}
	
	/data ${HashMap[${NameOfMap}].Erase}
	|- Sets KeySet. This will be a single string of all the KeyNames in the ini.
	|- Ex) KeySet = KeyName1|KeyName2||
	/declare KeySet string local ${Ini[${FileName},${SectionName}]}

	/if (!${Defined[bValues]}) /declare bValues int local 1

	:IniHashMapWhileLoop
	/if (${KeySet.Arg[${bValues},|].Length}) {
		/data ${HashMap[${NameOfMap}].Insert[${KeySet.Arg[${bValues},|]},${Ini[${FileName},${SectionName},${KeySet.Arg[${bValues},|]},NULL]}]}
		| /call HashAdd ${NameOfMap} ${KeySet.Arg[${bValues},|]} "${Ini[${FileName},${SectionName},${KeySet.Arg[${bValues},|]},NULL]}"
		/varcalc bValues ${bValues} + 1
		/goto :IniHashMapWhileLoop
	}
	
/return 

| ##
| # Populates a Dynamic array from an ini file.
| #
| # @PARAM - string FileName - The Ini file to be read
| # @PARAM - string SectioName - The section of the Ini to be used
| # @PARAM - string NameOfArray - The name of the map we wish to populate.
| ##
Sub IniToDynArray(FileName,SectionName,NameOfArray)
	|-Does the key exist?
	/if (${Ini[${FileName},${SectionName},-1,NO].Equal[NO]}) {
		| /echo KeyName not valid
		/return
	}
	
	/data ${Vector[${NameOfArray}].Erase}
	
	|- Sets KeySet. This will be a single string of all the KeyNames in the ini.
	|- Ex) KeySet = KeyName1|KeyName2||
	/declare KeySet string local ${Ini[${FileName},${SectionName}]}

	/if (!${Defined[bValues]}) /declare bValues int local 1

	:IniDynArrayWhileLoop
	/if (${KeySet.Arg[${bValues},|].Length}) {
		/data ${Vector[${NameOfArray}].Insert[${Ini[${FileName},${SectionName},${KeySet.Arg[${bValues},|]},NULL]}]}]}
		| /call DynArrayAdd ${NameOfArray} "${Ini[${FileName},${SectionName},${KeySet.Arg[${bValues},|]},NULL]}"
		| /gsay VALUE [${bValues}] [${Ini[${FileName},${SectionName},${KeySet.Arg[${bValues},|]},NULL]}]
		| /gsay ${Vector[${NameOfArray}].At[${Math.Calc[${bValues} - 1]}]} AT ${bValues} - 1
		/varcalc bValues ${bValues} + 1
		/goto :IniDynArrayWhileLoop
	}
	
/return 

| ##
| #
| ##
Sub DoClickyWithTimer(string ToClick, int RecastTime)
	/if (!${Me.Moving} && ${FindItemCount[${ToClick}]} > 0 && ${Target.Distance} <= ${FindItem[${ToClick}].Spell.Range} && (!${Defined[t${FindItem[${ToClick}].Spell.ID}]} || ${t${FindItem[${ToClick}].Spell.ID}} == 0)) {
		/call Cast "${ToClick}" item
		/gsay Using item ${ToClick} !@!
		/if ( !${Defined[t${FindItem[${ToClick}].Spell.ID}]} ) {
			/declare t${FindItem[${ToClick}].Spell.ID}    timer    outer
		}

		| The re-use timer on the item
		/varset t${FindItem[${ToClick}].Spell.ID} ${RecastTime}
		
		/return TRUE
	}
	
/return FALSE

| ##
| #
| ##
Sub Event_RaidInvited
	/raidaccept
/return

| ##
| #
| ##
Sub Event_RaidInvite(string Line, string Sender)
	/if (${SpawnCount[pc ${Sender}]} < 1) /return
	
	/if (${Raid.Leader.Name.Equal[${Me.CleanName}]} || ${Me.Name.Equal[Reno]}) {
		/target pc ${Sender}
		/delay 3s ${Target.CleanName.Equal[${Sender}]}
		/raidinvite
	}
/return
