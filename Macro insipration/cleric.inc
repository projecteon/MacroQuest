| http://mqemulator.net/forum2/viewtopic.php?t=1551
| http://mqemulator.net/forum2/viewtopic.php?t=794

#include includes\spell_routines.inc
#turbo 40

Sub Main
    /declare iniFileName        string  local "${MacroName}_${EverQuest.Server}_${Me.CleanName}.ini"
    /declare options            string  outer "HealingOptions"
    /declare mainHeal           string  outer "Light Healing"
    /declare mainHealPct        int     outer 70
    /declare mainHealDist       int     outer 100

    /if (${Ini[${iniFileName},${options},MainHeal].Length}) {
       /varset mainHeal ${Ini[${iniFileName},${options},MainHeal]}       
    }

    /if (${Ini[${iniFileName},${options},MainHealPct].Length}) {
       /varset mainHealPct ${Ini[${iniFileName},${options},MainHealPct]}       
    }

    /if (${Ini[${iniFileName},${options},MainHealingDist].Length}) {
       /varset mainHealDist ${Ini[${iniFileName},${options},MainHealingDist]}       
    }

    /if (${Ini[${iniFileName},${options},MainHeal].Length}) {
        /call MakeIni iniFileName      
    }

    /if (${Spell[${mainHeal}].Range} < ${mainHealDist}) {
        /varset mainHealDist ${Spell[${mainHeal}].Range}
    }

    /call HealLoop
/return

Sub HealLoop
    /call HealGroup ${mainHealPct} ${mainHealDist}
    /goto :HealLoop
    /return

Sub HealGroup(int healPct, int healDist)
    /if (${Me.Casting.ID}) {
        /return
    }

    /declare x int local
    /for x 5 downto 0 {
        /if (${Group.Member[x].PctHPs} <= ${healPct} && ${Group.Member[x].Distance} <= ${healDist}) {
            /if (${Me.CurrentMana} > ${Spell[${mainHeal}].Mana}) {
                /call HealMember ${Group.Member[x]} ${mainHeal}
            }
            /break
        }
        /next x
    }
    /return;

Sub HealMember(member, healSpell)
    /if (${Me.Sitting}) {
        /stand
    }
    
    /gsay ${options} "${member}" with "${healSpell}".
    /target ${member}
    /cast "${healSpell}"
    /return


Sub MakeIni(string iniFileName)
    |- Build the INI file
    /echo Building INI file.....

    /ini "${iniFileName}" "${options}" "MainHeal" "${mainHeal}"
    /ini "${iniFileName}" "${options}" "MainHealPct" "${mainHealPct}"
    /ini "${iniFileName}" "${options}" "MainHealingDist" "${mainHealDist}"
/return
	