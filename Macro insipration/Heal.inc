| # @file - Heal.inc
| # @author - Reno
| #
| # Call HealSetup at beginning of your macro. This will create the ini and end your macro. Forcing you to go modify it to your needs.
| # Call Check_HPs to check hitpoints. This will run everyone's hitpoints, via targeting, prioritize them and heal accordingly. It will
| # 	first do the Main Tank, then your group, then any remaining people logged into EQBC.
| # Modify your INI(youname_Heal.ini in your MQ2 folder) as needed to adjust heal spells, what percent hp to heal at, and who to heal
| # It can use group heals, but to turn off, merely delete that section from the Ini and it will not try to cast one.
| #    NOTE: Only does group heals for cleric/paladins anyways

#Event StartCast	"[#1#(msg)]#*#BEGINCASTING#*#"

| ######################################
| # Setup everything before we use it.
| # This should be called by any macro 
| # that included this file before the
| # main loop.
| ######################################
Sub HealSetup
	/echo Loading heal system.....
	
	|- Declare Some Stuff
	/if (!${Defined[HealINIFile]}) /declare HealINIFile string outer "${MacroName}_${EverQuest.Server}_${Me.CleanName}.ini" 
	/if (!${Defined[MainTank]}) /declare MainTank string outer 
	/if (!${Defined[TankHeal]}) /declare TankHeal outer 
	/if (!${Defined[MeleeHeal]}) /declare MeleeHeal string outer
	/if (!${Defined[CasterHeal]}) /declare CasterHeal string outer
	/if (!${Defined[PlateHeal]}) /declare PlateHeal string outer
	/if (!${Defined[TankPct]}) /declare TankPct int outer
	/if (!${Defined[PlatePct]}) /declare PlatePct int outer
	/if (!${Defined[MeleePct]}) /declare MeleePct int outer
	/if (!${Defined[CasterPct]}) /declare CasterPct int outer
	/if (!${Defined[DidWeGHeal]}) /declare DidWeGHeal outer FALSE
	/if (!${Defined[PCLeastHP]}) /declare PCLeastHP string outer NULL
	/if (!${Defined[LeastHP]}) /declare LeastHP int outer 100
	/if (!${Defined[LeastHPClass]}) /declare LeastHPClass string outer
	/if (!${Defined[NeedGHeal]}) /declare NeedGHeal int outer 0
	/if (!${Defined[GroupHeal]}) /declare GroupHeal string outer
	/if (!${Defined[HealPets]}) /declare HealPets outer FALSE
	
	|- If no INI then make one
	/if (!${Ini[${HealINIFile},HealOptions,MainAssist].Length}) /call MakeHealINI
	
	|- Load Some Variables Out of the Ini
	|-Heal Spells
	/if (${Ini[${HealINIFile},HealSpells,TankHeal].Length}) /varset TankHeal ${Ini[${HealINIFile},HealSpells,TankHeal]}
	/if (${Ini[${HealINIFile},HealSpells,PlateHeal].Length}) /varset PlateHeal ${Ini[${HealINIFile},HealSpells,PlateHeal]}
	/if (${Ini[${HealINIFile},HealSpells,MeleeHeal].Length}) /varset MeleeHeal ${Ini[${HealINIFile},HealSpells,MeleeHeal]}
	/if (${Ini[${HealINIFile},HealSpells,CasterHeal].Length}) /varset CasterHeal ${Ini[${HealINIFile},HealSpells,CasterHeal]}
	/if (${Ini[${HealINIFile},HealSpells,GroupHeal].Length}) /varset GroupHeal ${Ini[${HealINIFile},HealSpells,GroupHeal]}

	|-What percent do we heal at?
	/if (${Ini[${HealINIFile},HealAtPct,TankPct].Length}) /varset TankPct ${Ini[${HealINIFile},HealAtPct,TankPct]}
	/if (${Ini[${HealINIFile},HealAtPct,PlatePct].Length}) /varset PlatePct ${Ini[${HealINIFile},HealAtPct,PlatePct]}
	/if (${Ini[${HealINIFile},HealAtPct,MeleePct].Length}) /varset MeleePct ${Ini[${HealINIFile},HealAtPct,MeleePct]}
	/if (${Ini[${HealINIFile},HealAtPct,CasterPct].Length}) /varset CasterPct ${Ini[${HealINIFile},HealAtPct,CasterPct]}

	|- Who do we heal?
	/if (${Ini[${HealINIFile},HealOptions,MainAssist].Length}) /varset MainTank ${Ini[${HealINIFile},HealOptions,MainAssist]}
	
	|- Do we heal pets?
	/if (${Ini[${HealINIFile},HealOptions,HealGroupPets].Length}) /varset HealPets ${Ini[${HealINIFile},HealOptions,HealGroupPets]}
	
	|- Get a list of logged in clients
	/if (!${Defined[BCUsers]}) {
		/bccmd names
		/doevents ConnectedNames
	}
	
	/if (${Me.Class.Name.Equal[Cleric]}) /call BeginCHRotation
	
	/if (!${Defined[TankHealGem]}) /declare TankHealGem string outer "gem${Me.Gem[${TankHeal}]}"
	
	/if (${FindItemCount[Aged Shissar Apothic Staff]} || ${FindItemCount[Aged Hammer of the Dragonborn]} || ${FindItemCount[Aged Dragon Spine Staff]}) {
		/declare ClickHeal outer TRUE
	}
	
	/echo Heal system has been loaded.
/return

| ######################################################################################
| # Makes our Ini file if it doesn't exist, throwing in some default values
| # NOTE: This will end the macro, forcing the player to go manually update the INI
| ######################################################################################
Sub MakeHealINI
	|- Build the INI file
	/echo Building INI file.....
	
	|- MainTank section - tells us who the main tank is
	/ini "${HealINIFile}" HealOptions MainAssist NameofTank
	/ini "${HealINIFile}" HealOptions HealGroupPets FALSE
	
	|- HealSpells section - tells us what spells to use on each class
	/ini "${HealINIFile}" HealSpells TankHeal YourSpellHere
	/ini "${HealINIFile}" HealSpells PlateHeal YourSpellHere
	/ini "${HealINIFile}" HealSpells MeleeHeal YourSpellHere
	/ini "${HealINIFile}" HealSpells CasterHeal YourSpellHere
	/ini "${HealINIFile}" HealSpells GroupHeal YourSpellHere
	
	|- HealAtPct section - tells us at what percent to heal each class at
	/ini "${HealINIFile}" HealAtPct TankPct 65
	/ini "${HealINIFile}" HealAtPct PlatePct 65
	/ini "${HealINIFile}" HealAtPct MeleePct 65
	/ini "${HealINIFile}" HealAtPct CasterPct 65
	
	/if (!${Defined[INIFileWasBuilt]}) /declare INIFileWasBuilt outer TRUE
	
/return

| ##
| #
| ##
Sub UseVeeshanHealClicky
	/declare RetVal bool local FALSE
	/if (${Me.Class.Name.Equal[Cleric]} && ${FindItemCount[Aged Shissar Apothic Staff]}) {
		/call DoClickyWithTimer "Aged Shissar Apothic Staff" 2100
		/if (${Macro.Return.Equal[TRUE]}) /varset RetVal TRUE
	} else /if (${Me.Class.Name.Equal[Shaman]} && ${FindItemCount[Aged Hammer of the Dragonborn]}) {
		/call DoClickyWithTimer "Aged Hammer of the Dragonborn" 2100
		/if (${Macro.Return.Equal[TRUE]}) /varset RetVal TRUE
	} else /if (${Me.Class.Name.Equal[Druid]} && ${FindItemCount[Aged Dragon Spine Staff]}) {
		/call DoClickyWithTimer "Aged Dragon Spine Staff" 2100
		/if (${Macro.Return.Equal[TRUE]}) /varset RetVal TRUE
	}
	
/return ${RetVal}

| ##########################################
| # Used to check the MainTank's hitpoints.
| ##########################################
Sub CheckTank
	|- Check to see if target is in zone
	/if (${SpawnCount[pc ${MainTank}]} > 0) {
		|- Make sure they are not in our group.
		/call InMyGroup ${MainTank}	
		/if (${Macro.Return.Equal[FALSE]} && !${NetBots[${MainTank}]}) {
			|- Make sure we have the proper target
			/target pc ${MainTank}
			/delay 2s ${Target.Name.Equal[${MainTank}]}

			|- Check their hitpoints and cast if required
			/if (${Target.Name.Equal[${MainTank}]}) {
				/if (${Target.PctHPs} <= ${TankPct}) { 
					/call CastHeal "${TankHeal}" "${MainTank}"
				}
			}
		}
	}
	
/return

| ##############################################################
| # NOTE: This is a work in progress
| # Builds a dynamic array of all group members that need heals.
| # It will eventually prioritize those heals based on class. 
| # For the moment though it just goes first come, first serve.
| # The array name it generates is GroupMembers and is in the
| # the "outer" scope.
| ##############################################################
Sub CheckGroupHeals		
	/if (${Heal.WorstGroup.Type.NotEqual[PC]}) /return
	|- Casters(wis & int) then Plate, then Melee(bard here)
	|- Add them to the array if they are in need of a heal
	/if (${Heal.WorstGroup.Class.Name.Equal[Shaman]} || ${Heal.WorstGroup.Class.Name.Equal[Cleric]} || ${Heal.WorstGroup.Class.Name.Equal[Wizard]} || ${Heal.WorstGroup.Class.Name.Equal[Magician]} || ${Heal.WorstGroup.Class.Name.Equal[Enchanter]} || ${Heal.WorstGroup.Class.Name.Equal[Necromancer]} || ${Heal.WorstGroup.Class.Name.Equal[Druid]}) {
		/if (${Heal.WorstGroup.PctHPs} <= ${CasterPct}) {
			/call CastHeal "${CasterHeal}" "${Heal.WorstGroup.Name}"
		}
	} else /if (${Heal.WorstGroup.Class.Name.Equal[Warrior]} || ${Heal.WorstGroup.Class.Name.Equal[Shadow Knight]} || ${Heal.WorstGroup.Class.Name.Equal[Paladin]}) {
		/if (${Heal.WorstGroup.PctHPs} <= ${PlatePct}) {
			/call CastHeal "${PlateHeal}" "${Heal.WorstGroup.Name}"
		}
	} else /if (${Heal.WorstGroup.Class.Name.Equal[Ranger]} || ${Heal.WorstGroup.Class.Name.Equal[Rogue]} || ${Heal.WorstGroup.Class.Name.Equal[Bard]} || ${Heal.WorstGroup.Class.Name.Equal[Berserker]} || ${Heal.WorstGroup.Class.Name.Equal[Beastlord]} || ${Heal.WorstGroup.Class.Name.Equal[Monk]}) {		
		/if (${Heal.WorstGroup.PctHPs} <= ${MeleePct}) {
			/call CastHeal "${MeleeHeal}" "${Heal.WorstGroup.Name}"
		}
	}	
/return

| ###############################################################################
| # Checks the tank and specified PC's for hitpoint levels and heals accordingly.
| # This gets called in the main loop for every healer. It is the primary point 
| # for all healing.
| ###############################################################################
Sub Check_HPs
	|- Start witha  Divine Arb check...
	/if (${Me.AltAbilityReady[Divine Arbitration]}) /call DivineArbitration
	
	|- Check the tank's hitpoints before we start to make sure we have time to check everyone else
   	/call CheckTank

	|- Do we use a group heal?
	/if ((${Me.Class.Name.Equal[Cleric]} || ${Me.Class.Name.Equal[Paladin]}) && ${Heal.InjuredCount[65]} > 2) /call GroupHeal
	
	|- Check our group for heals
	/if (${Heal.InjuredCount} > 0) /call CheckGroupHeals
	
	|- Check for NetBots heals
	/if (${NetBots[${Heal.WorstNetBots.Name}].PctHPs} < 100) /call CheckNetBotsHeals
	
/return

| ##
| #
| ##	
Sub CheckNetBotsHeals
	/if (${Heal.WorstNetBots.Type.NotEqual[PC]}) /return
	|- Casters(wis & int) then Plate, then Melee(bard here)
	|- Add them to the array if they are in need of a heal
	/if (${Heal.WorstNetBots.Class.Name.Equal[Shaman]} || ${Heal.WorstNetBots.Class.Name.Equal[Cleric]} || ${Heal.WorstNetBots.Class.Name.Equal[Wizard]} || ${Heal.WorstNetBots.Class.Name.Equal[Magician]} || ${Heal.WorstNetBots.Class.Name.Equal[Enchanter]} || ${Heal.WorstNetBots.Class.Name.Equal[Necromancer]} || ${Heal.WorstNetBots.Class.Name.Equal[Druid]}) {
		/if (${NetBots[${Heal.WorstNetBots.Name}].PctHPs} <= ${CasterPct}) {
			/call CastHeal "${CasterHeal}" "${Heal.WorstNetBots.Name}"
		}
	} else /if (${Heal.WorstNetBots.Class.Name.Equal[Warrior]} || ${Heal.WorstNetBots.Class.Name.Equal[Shadow Knight]} || ${Heal.WorstNetBots.Class.Name.Equal[Paladin]}) {
		/if (${NetBots[${Heal.WorstNetBots.Name}].PctHPs} <= ${PlatePct}) {
			/call CastHeal "${PlateHeal}" "${Heal.WorstNetBots.Name}"
		}
	} else /if (${Heal.WorstNetBots.Class.Name.Equal[Ranger]} || ${Heal.WorstNetBots.Class.Name.Equal[Rogue]} || ${Heal.WorstNetBots.Class.Name.Equal[Bard]} || ${Heal.WorstNetBots.Class.Name.Equal[Berserker]} || ${Heal.WorstNetBots.Class.Name.Equal[Beastlord]} || ${Heal.WorstNetBots.Class.Name.Equal[Monk]}) {		
		/if (${NetBots[${Heal.WorstNetBots.Name}].PctHPs} <= ${MeleePct}) {
			/call CastHeal "${MeleeHeal}" "${Heal.WorstNetBots.Name}"
		}
	}	
/return

| ######################################################################
| # Casts the group heal if 3 or more group players are in need.
| ######################################################################
Sub GroupHeal
	/if (${Me.CurrentMana} > ${Spell[${GroupHeal}].Mana}) {
		/gsay <<< ${GroupHeal} >>>			
		/call cast "${GroupHeal}"
		|- Delay to give everyones hp time to update
		/delay 50
	}
/return

| #################################################
| # Called during the cast of any spell and will
| # interuppt it if any condition pops true.
| #################################################
Sub CancelHeal
	/if (${Target.PctHPs} >= 90) /call Interrupt
	/if (${Target.Distance} > 200) /call Interrupt
	/if (${Me.AltAbilityReady[Divine Arbitration]}) /call DivineArbitration
	/doevents StartCast
/return

| ######################################################
| # Casts the specified heal spell on
| # the specified target(PC only)
| #
| # @PARAM - string HealSpell - The spell to be casted.
| # @PARAM - string HealTarget - The target to be healed.
| #######################################################
Sub CastHeal(string HealSpell, string HealTarget, bool DontStop)
	/if (!${Defined[DontStop]}) /declare DontStop bool local FALSE
	|- Is spell ready?
	/if (${Me.SpellReady[${HealSpell}]}) {
		|- Do we have mana
		/if (${Me.CurrentMana} < ${Spell[${HealSpell}].Mana}) {		
			/delay 1s
			/bca NO MANA TO HEAL ${HealTarget} !!!
			/return
		} else {			
			/target ${HealTarget}
			/delay 2s ${Target.Name.Equal[${HealTarget}]}
			/if (${Target.Name.NotEqual[${HealTarget}]}) /return
			
			|- Do we have a heal clicky item that we should use first?
			/if (${ClickHeal}) {
				/call UseVeeshanHealClicky
				/if (${Macro.Return.Equal[TRUE]}) /return
			}
			
			|- If right target and in range, cast the spell
			/if (${Target.Name.Equal[${HealTarget}]} && (${Target.Distance} <= ${Spell[${HealSpell}].Range}) && ${Target.Type.NotEqual[Corpse]}) {	
				/gsay <<< ${HealSpell} -> ${HealTarget} >>>			
				/bca <<< ${HealSpell} -> ${HealTarget} >>>
				/call cast "${HealSpell}" gem${Me.Gem[${HealSpell}]} 0 CancelHeal
				/bca MANA: ${Me.PctMana}
			}
		}
	}
	
/return

| #############################################
| # Check's all group member's pets for heals
| #############################################
Sub CheckPets
	/declare i int local 0
	
	/for i 1 to ${Group.Members}		
		/if (${Group.Member[${i}].Pet.PctHPs} < 60 && ${Group.Member[${i}].Pet.ID}) {
			/target ${Group.Member[${i}].Pet}
		  	/delay 2s ${Target.Name.Equal[${Group.Member[${i}].Pet}]}
			
		 	/call CastHeal "${MeleeHeal}" "${Group.Member[${i}].Pet.Name}"
		}		
	/next i	
	
/return

| ##
| # Used for the guild CH chain system
| #
| ##
Sub Event_StartCast(string line, string tank) 
	/if (!${Defined[RotSpell]}) /declare RotSpell string outer "Complete Heal"
	
	/if (${SpawnCount[pc ${tank}]} < 1) {
		/g >>>> ${tank} is not in my zone!! <<<<
	}
	
	/target pc ${tank}	
	/delay 3s ${Target.Name.Equal[${tank}]}
	/if (!${Target.Name.Equal[${tank}]}) /return
	
	/if (!${Me.Book[${RotSpell}]}) {
		/g >>>> I do not have the spell ${RotSpell}! <<<<
	} else /if (${Target.Distance} > ${Spell[${RotSpell}].Range}) {
		/g >>>> ${tank} IS OUT OF RANGE!! <<<<
	} else /if (${Me.CurrentMana} < ${Spell[${RotSpell}].Mana}) {
		/g >>>> NOT ENOUGH MANA TO CAST ${RotSpell} <<<<
	} 
	
	/g Casting ${RotSpell} ON ${tank}
	/call cast ${RotSpell} gem1 3s
	
/return

| ##
| #
| #
| ##
Sub BeginCHRotation
	/if (!${Defined[BHServer]}) /declare BHServer string outer "192.168.1.136"
	/if (!${Defined[BHPort]}) /declare BHPort int outer 21
	
	/bhcmd connect ${BHServer} ${BHPort}
	
/return

| ##
| #
| ##
Sub DivineArbitration
	/if (${Group.Members} < 3) /return
	
	/if ((${Math.Calc[${Heal.UninjuredCount[80]} - 1]} > ${Heal.InjuredCount[35]}) && ${Heal.InjuredCount[35]} > 0) {
		/call Interrupt
		/gsay <<< DIVINE ARBITRATION >>>
		/call cast "Divine Arbitration" alt
	}
	
/return
