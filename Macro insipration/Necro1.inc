|--------------------------------------------------------------------------------
|SUB: GM in zone Check
|--------------------------------------------------------------------------------

Sub Gmcheck

   /if (${Spawn[gm].ID}) {

     
      /echo GM has entered the zone!
      /popup A GM has entered the zone, shutting down macros
      /endmacro
   }
   
/return 
|=================================================================
Sub Whatever

/declare r int local
/declare RaidY float local
/declare RaidX float local
/declare RaidCount int local

/for r ${Raid.Members} downto 0
/if (${Raid.Member[${r}].Spawn.ID} != NULL && ${Raid.Member[${r}].Spawn.Type.NotEqual[Corpse]}) {
   /varset RaidY ${Math.Calc[${RaidY}+${Raid.Member[${r}].Y}]}
   /varset RaidX ${Math.Calc[${RaidX}+${Raid.Member[${r}].X}]}
   /varset RaidCount ${Math.Calc[${RaidCount}+1]}
}
/next r

/varset RaidY ${Math.Calc[${RaidY}/${RaidCount}]}
/varset RaidX ${Math.Calc[${RaidX}/${RaidCount}]}

/echo ${RaidY}, ${RaidX}

   :moveloop
    /face nolook loc ${RaidY}, ${RaidX}
    /if (${Math.Distance[${RaidY}, ${RaidX}]}>12) {
       /keypress forward hold
    } else {
       /keypress forward
       /return
    }
    /goto :moveloop
/return
|=================================================================

Sub ClickIt(SlotList)
  /if (!${SlotList.Length}) {
    /echo ClickIt- Required Parameter Missing
    /return
  }
  /declare cSlotName string local
  /declare nCount int local 1
  :ClickLoop
    /varset cSlotName ${SlotList.Arg[${nCount},;]}
    /itemnotify ${cSlotName} rightmouseup
    /delay 3
    :WaitForCast
      /if (${Me.Casting.ID}) /goto :WaitForCast
    /varcalc nCount ${nCount}+1
    /delay 5
    /if (${SlotList.Arg[${nCount},;].Length}) /goto :ClickLoop
/return

|=================================================================
Sub fs

:Loop
/if (${Me.Casting.ID}) /goto :Loop
/if (${Me.Buff["Dull Pain"].Duration}<5) {
	/keypress forward
	/delay 5
	 /cast "Dull Pain"
	 /delay 5
	 /cast "Dull Pain"
	 /delay 5
	 /cast "Dull Pain"
		/delay 40
}
/return

Sub Lich

:Loop
/if (${Me.Casting.ID}) /goto :Loop
/if (${Me.Buff["Grave Pact"].Duration}<60) {
	/cast "Grave Pact"
	/delay 5
	/cast "Grave Pact"
	/delay 5
	/cast "Grave Pact"
	/delay 2s
}


|**
/if (${Me.Buff["Grave Pact"].Duration}<60) {
	/delay 1s
	/memspell 8 "Grave Pact"
	/delay 4s
	/cast "Grave Pact"
		/delay 2
	/cast "Grave Pact"
		/delay 2
	/cast "Grave Pact"
	/delay 3s
}
**|
/return


Sub MakePet

:Loop
/if (${Me.Casting.ID}) /goto :Loop
/if (${Me.Pet.ID}) {
	/if (!${String[${Me.Inventory[Ranged]}].Find["Symbol of Ancient Summoning"]}) /exchange "Symbol of Ancient Summoning" Ranged
	/echo You already have a pet
	}
/echo Making a pet, don't move
/declare spell6 string local
/declare i int local


/varset spell6 ${Me.Gem[6]}
/sit
/delay 2s
/if (${Me.SpellReady["Lost Soul"]}) /goto :LSAlreadyMemed
/memspell 6 "Lost Soul"
/delay 35
:WaitForLS
/if (!${Me.SpellReady["Lost Soul"]}) /goto :WaitForLS


:LSAlreadyMemed
/keypress esc
/stand


/if (!${String[${Me.Inventory[Ranged]}].Find["Symbol of Ancient Summoning"]}) /exchange "Symbol of Ancient Summoning" Ranged
/delay 2




	:2
	/if (${Me.Moving}) /goto :2
	/stand
	/cast "Lost Soul"
	/delay 5
	/cast "Lost Soul"
	/delay 5
	/cast "Lost Soul"
	/delay 5

:Loop0
/if (${Me.Casting.ID}) /goto :Loop0

/keypress esc
/pet follow
/pet back off
/pet hold
/call PetHaste
:Loop1
/if (${Me.Casting.ID}) /goto :Loop1
	/for i 1 to 2
	:Loop2
	/if (${Me.Casting.ID}) /goto :Loop2
	/if (${Me.Moving}) /goto :3
	/itemnotify Ranged rightmouseup
	/delay 5
	/next i

	:Loop3
	/if (${Me.Casting.ID}) /goto :Loop3

/sit
/memspell 6 "${spell6}"
/delay 35
/stand
/echo Pet Made...hopefully
/return

|=================================================================

Sub DMFSomeone
/echo DMFing Someone
/declare PersonToDMF string local
/varset PersonToDMF ${MacroQuest.LastTell}
:ReTarget
:WaitForFinishOfCast
/target ${PersonToDMF}
/delay 2
/if (!${Me.SpellReady[8]}) /goto :WaitForFinishOfCast
/if (${Target.Type.Equal["NPC"]}) /goto :ReTarget
/if (${Target.Distance}>190) {
	/delay 2s
	/tell ${PersonToDMF} OOR
	/return
}
/delay 1s
/if (${Me.SpellReady["Dead Man Floating"]}) {

		/cast "Dead Man Floating"
		/delay 3
		/cast "Dead Man Floating"
		/delay 3
		/cast "Dead Man Floating"
		/delay 4s
	/tell ${PersonToDMF} inc DMF
		/delay 2s
} else /if (${Me.SpellReady["Dead Men Floating"]}) {
		/cast "Dead Men Floating"
		/delay 3
		/cast "Dead Men Floating"
		/delay 3
		/cast "Dead Men Floating"
		/delay 6s

} else {
		/declare spell7 string local
		/varset spell7 ${Me.Gem[7]}
	/memspell 7 "Dead Man Floating"

		/delay 10s

	:WaitForDMFToPop
		/delay 2
	/if (!${Me.SpellReady["Dead Man Floating"]}) /goto :WaitForDMFToPop

		/cast "Dead Man Floating"
		/delay 3
		/cast "Dead Man Floating"
		/delay 3
		/cast "Dead Man Floating"
		/delay 4s
	/tell ${PersonToDMF} inc DMF
	/delay 2s
	/memspell 7 "${spell7}"
	/delay 35
}
/doevents flush
/return

Sub FD
/echo You are being hit!  Trying to FD!
:Top
/if (${Me.Moving}) /goto :Top


/if (${String[${Me.State}].Equal["FEIGN"]}) {
	/echo You are FD.
	/return
} else /if (!${String[${Me.State}].Equal["FEIGN"]}) {
	:Not_FD_yet
	/cast "Death Peace"
	/delay 2
	/goto :Top
}
/goto :Top
/return

sub event_standing
:standupdammit
/stand
/if (!${Me.State.Equal[Stand]}) /goto :standupdammit
/return