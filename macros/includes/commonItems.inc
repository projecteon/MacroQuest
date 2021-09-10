|--------------------------------------------------------|
|- Searches inventory, and banks, for indicated items.	-|
|- Use, /bc Find 'Name', /echo Find 'Name'.			        -|
|- Accepts item links as 'Name'                         -|
|--------------------------------------------------------|
#EVENT FindItems "<#1#> Find Item #2#"
Sub EVENT_FindItems(line, chatSender, itemToFind)
    /if (${Debug} || ${Debug_Common}) /echo |- EVENT_FindItems ==>
    /if (${Debug} || ${Debug_Common}) /echo "${chatSender}" "${itemToFind}"

    | Find items on toon
    /if (${FindItemCount[${itemToFind}]}) {
        /call FindItemsOnToon "${chatSender}" "${itemToFind}" ${numInventorySlots}
    }
    
    | Find items in toon bank
    | FindItemBankCount crashes on some chrs, dunno why
    /if (${FindItemBankCount[${itemToFind}]}) {
        /call FindItemsInToonBank "${chatSender}" "${itemToFind}"
    }
    
    /if (${Debug} || ${Debug_Common}) /echo <== EVENT_FindItems -|
/return

Sub FindItemsOnToon(string chatSender, string itemToFind, int numberOfInventorySlots)
    /if (${Debug} || ${Debug_Common}) /echo |- FindItemsOnToon ==> InventorySlots[${numberOfInventorySlots}]

    /declare i int local
    /declare c int local
  
    /for i 1 to ${numberOfInventorySlots} {
        /if (${Debug} || ${Debug_Common}) /echo Start [InvSlot[${i}]]
        /if (${Debug} || ${Debug_Common}) /delay 5
        /if (${Me.Inventory[pack${i}].Name.Find[${itemToFind}]}) {
            /bct ${chatSender} ${Me.Inventory[pack${i}]}(${Me.Inventory[pack${i}].Stack}) -> [InvSlot[${i}]]
        }

        /if (${Me.Inventory[pack${i}].Container}) {
            /if (${Debug} || ${Debug_Common}) /echo [InvSlot[${i}]] -> Container[${Me.Inventory[pack${i}].Container}]
            /for c 1 to ${Me.Inventory[pack${i}].Container} {
                /if (${Me.Inventory[pack${i}].Item[${c}].Name.Find[${itemToFind}]}) {                    
                    /bct ${chatSender} ${Me.Inventory[pack${i}].Item[${c}]}(${Me.Inventory[pack${i}].Item[${c}].Stack}) -> [${Me.Inventory[pack${i}]}-> InvSlot[${i}]-> PackSlot[${c}]]
                }
                /if (${Debug} || ${Debug_Common}) /echo Done InvSlot[${i}] -> PackSlot[${c}]]
                /if (${Debug} || ${Debug_Common}) /delay 5
                /next c
            }

            /if (${Debug} || ${Debug_Common}) /echo Done [InvSlot[${i}]] Container
        }

        /if (${Debug} || ${Debug_Common}) /echo Done [InvSlot[${i}]]
        /if (${Debug} || ${Debug_Common}) /delay 5
        /next i
    }

    /if (${Debug} || ${Debug_Common}) /echo <== FindItemsOnToon -|
/return

Sub FindItemsInToonBank(string chatSender, string itemToFind)
    /if (${Debug} || ${Debug_Common}) /echo |- FindItemsInToonBank ==>

    /declare i int local
    /declare c int local
  
    /for i 1 to 16 {
        /if (${Debug} || ${Debug_Common}) /echo Start [BankSlot[${i}]]
        /if (${Debug} || ${Debug_Common}) /delay 5
        /if (${Me.Bank[${i}].Name.Find[${itemToFind}]}) {
            /bct ${chatSender} ${Me.Bank[${i}]}(${Me.Bank[${i}].Stack}) -> [BankSlot[${i}]]
        }

        /if (${Me.Bank[${i}].Container}) {
            /if (${Debug} || ${Debug_Common}) /echo [BankSlot[${i}]] -> Container[${Me.Bank[${i}].Container}]
            /for c 1 to ${Me.Bank[${i}].Container} {
                /if (${Me.Bank[${i}].Item[${c}].Name.Find[${itemToFind}]}) {
                    /bct ${chatSender} ${Me.Bank[${i}].Item[${c}]}(${Me.Bank[${i}].Item[${c}].Stack})-> [${Me.Bank[${i}]} -> BankSlot[${i}]-> PackSlot[${c}]]
                }
                /if (${Debug} || ${Debug_Common}) /echo Done BankSlot[${i}] -> PackSlot[${c}]]
                /if (${Debug} || ${Debug_Common}) /delay 5
                /next c
            }

            /if (${Debug} || ${Debug_Common}) /echo Done [BankSlot[${i}]] Container
        }
        /if (${Debug} || ${Debug_Common}) /echo Done [BankSlot[${i}]]
        /if (${Debug} || ${Debug_Common}) /delay 5
        /next i
    }

    /if (${Debug} || ${Debug_Common}) /echo <== FindItemsInToonBank -|
/return

Sub InitItems(string iniFileName)
    /declare Debug_Items     bool    outer FALSE

    /squelch /alias /finditem /bc Find Item
/return

Sub DoItemEvents
  /doevents FindItems
/return
