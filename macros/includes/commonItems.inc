|--------------------------------------------------------|
|- Searches inventory, and banks, for indicated items.	-|
|- Use, /bc Find 'Name', /echo Find 'Name'.			        -|
|- Accepts item links as 'Name'                         -|
|--------------------------------------------------------|
#EVENT FindItems "<#1#> Find Item #2#"
Sub EVENT_FindItems(line, chatSender, itemToFind)
    /if (${Debug} || ${Debug_Common}) /echo |- EVENT_FindItems ==>

    | Find items on toon
    /if (${FindItemCount[${ItemToFind}]}) {
        /call FindItemsOnToon(${chatSender}, ${itemToFind}, ${numInventorySlots})
    }
    
    | Find items in toon bank
    /if (${FindItemBankCount[${ItemToFind}]}) {
        /call FindItemsInToonBank(${chatSender}, ${itemToFind})
    }

    /if (${Debug} || ${Debug_Common}) /echo <== EVENT_FindItems -|
/return

Sub FindItemsOnToon(string chatSender, string itemToFind, int numberOfInventorySlots)
    /if (${Debug} || ${Debug_Common}) /echo |- FindItemsOnToon ==>

    /declare inventorySlot int local
    /declare containerSlot int local
    /declare augSlot int local
  
    /for inventorySlot 1 to ${numInventorySlots}
        /if (${Me.Inventory[pack${inventorySlot}].Name.Find[${ItemToFind}]}) {
            /bct ${chatSender} ${Me.Inventory[pack${inventorySlot}]}(${Me.Inventory[pack${inventorySlot}].Stack})-> InvSlot${inventorySlot}]
        }
        /if (${Me.Inventory[pack${inventorySlot}].Container}) {
            /for containerSlot 1 to ${Me.Inventory[pack${inventorySlot}].Container}
                /if (${Me.Inventory[pack${inventorySlot}].Item[${containerSlot}].Name.Find[${ItemToFind}]}) {
                    /bct ${chatSender} ${Me.Inventory[pack${inventorySlot}].Item[${containerSlot}]}(${Me.Inventory[pack${inventorySlot}].Item[${containerSlot}].Stack})-> [${Me.Inventory[pack${inventorySlot}]}-> InvSlot${inventorySlot}-> PackSlot${containerSlot}]
                }
                /for augSlot 0 to ${Me.Inventory[pack${inventorySlot}].Item[${containerSlot}].Augs}
                    /if (${Me.Inventory[pack${inventorySlot}].Item[${containerSlot}].AugSlot[${augSlot}].Name.Find[${ItemToFind}]}) {
                    /bct ${chatSender} ${Me.Inventory[pack${inventorySlot}].Item[${containerSlot}].AugSlot[${augSlot}]}-> [${Me.Inventory[pack${inventorySlot}].Item[${containerSlot}]}]-> [${Me.Inventory[pack${inventorySlot}]}-> InvSlot${inventorySlot}-> PackSlot${containerSlot}]
                    }
                /next augSlot
            /next containerSlot
        }
    /next inventorySlot

    /if (${Debug} || ${Debug_Common}) /echo <== FindItemsOnToon -|
/return

Sub FindItemsInToonBank(string chatSender, string itemToFind)
    /if (${Debug} || ${Debug_Common}) /echo |- FindItemsInToonBank ==>

    /declare inventorySlot int local
    /declare containerSlot int local
    /declare augSlot int local
  
    /for inventorySlot 1 to 26
        /if (${Me.Bank[${inventorySlot}].Name.Find[${ItemToFind}]}) {
            /varcalc totalItems ${totalItems}+${Me.Bank[${inventorySlot}].Stack}
            /bct ${chatSender} ${Me.Bank[${inventorySlot}]}(${Me.Bank[${inventorySlot}].Stack}) ->[BankSlot${inventorySlot}]
        }
        /if (${Me.Bank[${inventorySlot}].Container}) {
            /for containerSlot 1 to ${Me.Bank[${inventorySlot}].Container}
                /if (${Me.Bank[${inventorySlot}].Item[${containerSlot}].Name.Find[${ItemToFind}]}) {
                    /varcalc totalItems ${totalItems}+${Me.Bank[${inventorySlot}].Item[${containerSlot}].Stack}
                    /bct ${chatSender} ${Me.Bank[${inventorySlot}].Item[${containerSlot}]}(${Me.Bank[${inventorySlot}].Item[${containerSlot}].Stack})-> [${Me.Bank[${inventorySlot}]}-> BankSlot${inventorySlot}-> PackSlot${containerSlot}]
                }
                /for augSlot 0 to ${Me.Bank[${inventorySlot}].Item[${containerSlot}].Augs}
                    /if (${Me.Bank[${inventorySlot}].Item[${containerSlot}].AugSlot[${augSlot}].Name.Find[${ItemToFind}]}) {
                    /varcalc totalItems ${totalItems}+1
                    /bct ${chatSender} ${Me.Bank[${inventorySlot}].Item[${containerSlot}].AugSlot[${augSlot}]}-> [${Me.Bank[${inventorySlot}].Item[${containerSlot}]}]-> ${Me.Bank[${inventorySlot}]}-> BankSlot${inventorySlot}-> PackSlot${containerSlot}
                    }
                /next augSlot
            /next containerSlot
        }
    /next inventorySlot

    /if (${Debug} || ${Debug_Common}) /echo <== FindItemsInToonBank -|
/return


Sub DoItemEvents
  /doevents FindItems
/return
