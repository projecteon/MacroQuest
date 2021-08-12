${FindItem[=Lockpickss]} == NULL
${FindItem[=Lockpicks].ItemSlot}

${FindItem[=Lockpicks].ItemSlot}


/itemnotify pack${FindItem[=Lockpicks].ItemSlot} rightmouseheld

/itemnotify "Lockpicks" leftmouseup
/itemnotify "Lockpicks" rightmouseheld

/echo ${NetBots[Tyrion].Attacking}


[PetOptions]
UsePet=Water

[Water]
Pet-Spell=Greater Conjuration: Water
Pet-Focus-Item=Stein of Ulissa

[Air]
Pet Spell=Greater Conjuration: Air
Pet Focus Iteem=Broom of Trilon

/bc set active pet spell PetAir


Check these
${Me.CombatState}
${Me.CombatState.Equal[COMBAT]}
${NetBots[Name].CombatState}
