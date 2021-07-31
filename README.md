# MacroQuest
* http://macroquest.sourceforge.net/includes/manual.php#intro
* http://mqemulator.net/forum2/viewtopic.php?t=804

## Debuffing
* http://218.90.236.254:8088/mqe/forum2/viewtopic.php-t=1415.htm | Timing tracking
 

## Pet handling
* http://mqemulator.net/forum2/viewtopic.php?t=1018

## Tips&Tricks
* https://www.redguides.com/community/threads/useful-commands-hotkeys-to-save-time-and-trouble.66988/
* https://www.redguides.com/community/threads/best-way-to-determine-aggro.12751/
* https://www.mmobugs.com/forums/index.php?threads/auto-attack.6823/
* http://mqemulator.net/forum2/viewtopic.php?t=1047 | Subs, Targets and Events in Macros
* http://mqemulator.net/forum2/viewtopic.php?t=1058 | A leason on Events
* http://mqemulator.net/forum2/viewtopic.php?t=1069 | EQBC - The Way To Skip Macros.
* https://www.mmobugs.com/forums/index.php?threads/target-targets-pet.26694/
* https://www.mmobugs.com/forums/index.php?threads/pet-target.31425/
* https://www.mmobugs.com/wiki/index.php/MQ2Bot
* http://ezserver.online/forums/index.php?topic=5340.0
* http://eqtitan.com/forums/viewtopic.php?f=53&t=2175
* https://www.redguides.com/community/threads/my-first-attempt-at-altering-a-macro-trying-to-learn-maco-need-help-lol.20774/
* https://www.redguides.com/community/threads/tank-macro.10777/
* https://www.mmobugs.com/forums/index.php?threads/pet-target.31425/

## Class specific
 
## Enchanter
* http://mqemulator.net/forum2/viewtopic.php?t=1052&postdays=0&postorder=asc&start=15&sid=211ad17f30121cd1229c354ec91e6aca
* http://mqemulator.net/forum2/viewtopic.php?t=1052
* http://eqtitan.com/forums/viewtopic.php?f=53&t=2694

### Shaman
* http://mqemulator.net/forum2/viewtopic.php?t=1050

## Bots

### MQ2Bot
* https://www.mmobugs.com/wiki/index.php/MQ2Bot

### ModBot
* https://www.macroquest2.com/wiki/index.php/ModBot
* https://github.com/kodywilson/modbot
* https://sites.google.com/site/modbot4/
* http://eqtitan.com/forums/viewtopic.php?f=53&t=650

### E3
* https://github.com/Manwe-fv/FV_project_E3
* https://docs.google.com/document/d/1mZMuB3QGwjAwjpxe-SMN13u6zM-lox7IQPkJTQDomWo/edit#heading=h.qt1z2wcvjutz
* http://lazaruseq.com/Wiki/index.php/MQ2_%2B_E3

### Bot40
* https://www.mmobugs.com/wiki/index.php?title=Bot40
* https://github.com/MMOBugs/MMOBugs-Macros

### Unknown
* https://github.com/ebzent73/macros
* https://github.com/kevrgithub/peqtgc-mq2-sod

##Plugins

### MQ2Cast
* https://www.mmobugs.com/wiki/index.php/MQ2Cast
* https://www.redguides.com/community/resources/mq2cast.290/

### MQ2CastTimer
* https://www.mmobugs.com/wiki/index.php/MQ2CastTimer

### MQ2NetBots
* https://www.macroquest2.com/wiki/index.php/MQ2NetBots#Top-Level_Object:_.24.7BNetBots.7D

### MQ2Twist
* https://www.mmobugs.com/wiki/index.php/MQ2Twist

### HUD
* https://www.macroquest2.com/wiki/index.php/MQ2HUD
* https://www.mmobugs.com/wiki/index.php/MQ2HUD
* https://www.redguides.com/community/resources/mq2hud.133/
* http://mqemulator.net/forum2/viewtopic.php?t=659
* https://github.com/bdennin/IP/blob/master/MQ2HUD.ini

```
|-- This is specifically for TS items in a bag
Sub PickUpItemFromBag(ItemName)
    /declare Slot int local ${FindItem[=${ItemName}].ItemSlot}
    /declare Slot2 int local ${FindItem[=${ItemName}].ItemSlot2}
    /if (${Slot}<23) /return    || Top level inventory item 
    /if (${Slot2}==-1) /return    || Top level bag
    /ctrl /itemnotify in pack${Math.Calc[${Slot}-22].Int} ${Math.Calc[${Slot2}+1]} leftmouseup
/return

/itemnotify #${FindItem[=9 lb. Saltwater Tuna].ID} leftmouseup
```
