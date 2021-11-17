| ################################################################################
| # Find and loot nearest corpse
| ################################################################################
SUB LootNearestCorpse
  /if (${Debug} || ${Debug_Loot}) /echo |- LootNearestCorpse ==>
  /declare startX         int     ${Me.X}
  /declare startY         int     ${Me.Y}
  /declare isTwisting    bool    ${Twist.Twisting}
  /squelch /hidecorpse looted
  /if (${isTwisting}) {
    /squelch /twist stop
    /delay 20 !${Me.Casting.ID}
  }

  /delay 10s !${Me.Casting.ID}
  /if (${Navigation.Active}) /squelch /nav stop
  /if (${NetAdvPath.Following}) /squelch /netfollow off
  /if (${Stick.Active}) /squelch /stick off

  /declare seek_Radius        int local 50
  /declare closestCorpseID    int local

  /varset closestCorpseID ${NearestSpawn[1,npc corpse zradius 50 radius ${seek_Radius} "_"].ID}

  /if (!${Bool[${Spawn[${closestCorpseID}]}]}) /goto :skipCorpse

  /call EnsureTarget ${closestCorpseID}
  /if (!${Target.ID}) {
    /if (${Debug} || ${Debug_Loot}) /echo Cant Target ${closestCorpseID}
    /goto :skipCorpse
  }

  /if (${Target.Distance}>16 && ${Target.DistanceZ} < 80) {
    /call MoveToLoc ${Target.Y} ${Target.X} 20 12
  }

  /if (${Target.Distance}<=20 &&  ${Target.DistanceZ} < 40) {
    /call LootCorpse
  } else {
    /echo |- Corpse ${closestCorpseID} is > ${Target.Distance}|${Target.DistanceZ} distance, skipping
  }

  :skipCorpse
  /call MoveToLoc ${startY} ${startX} 15 20
  /if (${isTwisting}) /twist

  /if (${Debug} || ${Debug_Loot}) /echo <== LootNearestCorpse -|
/return

| ################################################################################
| # Loot current corpse
| ################################################################################
SUB LootCorpse
  /if (${Debug} || ${Debug_Loot}) /echo |- LootCorpse ==>

  /if (!${Defined[canLootCorpse]}) /declare canLootCorpse bool outer TRUE

  /varset canLootCorpse TRUE
  /call ClearCursor
  /loot
  /delay 5 ${Window[LootWnd].Open}
  /doevents CheckCanLootCorpse
  /delay 1s ${Corpse.Items} || !${canLootCorpse}

  /if (!${canLootCorpse}) {
    /if (${Debug} || ${Debug_Loot}) /echo |- Could not loot corpse
    /goto :skipLoot
  }

  /declare numItems int local ${Corpse.Items}
  /if (!${Bool[${numItems}]}) {
    /if (${Debug} || ${Debug_Loot}) /echo |- Ending loot on ${Target.ID}, no items left: ${Corpse.Items}
    /goto :skipLoot
  }

  /if (${Debug} || ${Debug_Loot}) /echo |- Looting ${Target.ID}-Items:${Int[${numItems}]} - ${Time}

  /declare i int local
  /declare j int local
  /declare itemName string local
  /declare itemSlot int local
  /declare canDestroy bool local

  /for i 1 to ${numItems}
    /if (${Debug} || ${Debug_Loot}) /echo |- starting loot slot: ${i} of ${numItems}

    /delay 1s ${Corpse.Item[${i}].ID}

    /varset itemName ${Corpse.Item[${i}].Name}
    /if (${Debug} || ${Debug_Loot}) /echo |- Looting ${itemName}

    | Lore check
    /if (${Corpse.Item[${i}].Lore}) {
      /if (${FindItemCount[=${Corpse.Item[${i}].Name}]} || ${FindItemBankCount[=${Corpse.Item[${i}].Name}]}) {
        /bca [Loot]: [${Corpse.Item[${i}].Name}] is lore, and I already have one${If[${FindItemBankCount[=${Corpse.Item[${i}].Name}]}, in the bank,]}.
        /beep
        /goto :skipItem
      }
    }

    | Destroy the item
    /call CanDestroyItem "${itemName}" ${Corpse.Item[${i}].ID}
    /varset canDestroy ${Macro.Return}

    /if (${canDestroy}) {
      /bca [Loot]: Destroying [${Corpse.Item[${i}].Name}]
      /call LootItem ${i} destroy
      /goto :skipItem
    }

    | Check stackables
    /if (${Corpse.Item[${i}].Stackable}) {
      /if (!${Me.FreeInventory} && !${Bool[${FindItem[=${itemName}].ItemSlot}]}) /goto :skipItem

      /varset itemSlot ${FindItem[=${itemName}].ItemSlot}
      /if (${Debug} || ${Debug_Loot}) /bca I've got a stack of ${itemName} in itemSlot: ${itemSlot}

      /for j 1 to 10
        /if (${Me.Inventory[${itemSlot}].Item[${j}].Name.Equal[${itemName}]}) {
          /if (!${Me.Inventory[${itemSlot}].Item[${j}].FreeStack} ) {
            /if (${Debug} || ${Debug_Loot}) /echo I've got a stack, but its full, skipping
            /beep
            /goto :skipItem
          }
        }
      /next j
    }

    | No free large enough spot for item
    /if (${Me.LargestFreeInventory} < ${Corpse.Item[${i}].Size}) {
      /bca [Loot]: I don't have a free inventory space large enough to hold [${itemName}].
      /beep
      /goto :skipItem
    }

    /if (!${Me.FreeInventory}) {
      /beep
      /bca My inventory is full!
      /if (${Debug} || ${Debug_Loot}) /echo |- No free inventory slots
      /goto :couldNotLoot
    }

    /call LootItem ${i} keep
    /if (${Debug} || ${Debug_Loot}) /echo Done Looting i: ${i}
    /delay 1

    :skipItem
    /if (${Debug} || ${Debug_Loot}) /echo Done with slot i: ${i}
  /next i

  :couldNotLoot
  /delay 1
  |looting done, if items leftover link them
  /if (${Corpse.Items}) {
    /keypress /
    /delay 1
    /call TypeChrs "${linkChannel} ${Target.ID}-"
    /delay 1
    /notify LootWnd BroadcastButton leftmouseup
    /delay 1
    /keypress enter chat
  }

  :skipLoot
  /if (${Corpse.Open}) /notify LootWnd DoneButton leftmouseup
  /delay 1s !${Window[LootWnd].Open}
  /if (${Debug} || ${Debug_Loot}) /echo Done Looting ${Time}

  /if (${Debug} || ${Debug_Loot}) /echo <== LootCorpse -|
/return


Sub LootItem(int slotNum, action)
  /if (${Debug} || ${Debug_Loot}) /echo |- LootItem ==> slot ${slotNum} ${action}

  /declare itemName   string  local
	/declare lootTimer  timer   local 3s
	:retry_Loot
	| Try to loot the specified item from the corpse.
	/itemnotify loot${slotNum} leftmouseup
	/delay 1s ${Cursor.ID} || ${Window[ConfirmationDialogBox].Open} || ${Window[QuantityWnd].Open}
	| If the item has been looted, decide what to do with it.
	/if (${Cursor.ID}) {
    /varset itemName ${Cursor.Name}
		| Destroy the item.
		/if (${action.Equal[destroy]}) {
			:destroy_Item
			/destroy
			| If the destroy attempt was unsuccessful, try again.
			/if (${Cursor.ID} && ${lootTimer}) /goto :destroy_Item
      /if (!${Cursor.ID}) /bca Destroying item ${itemName}
      /if ((${Debug} || ${Debug_Loot}) && !${Cursor.ID}) /echo |- Succesfully destroyed ${itemName} from slot# ${slotNum}
		| Else, keep the item.
		} else {
			/call ClearCursor
			/if (!${Cursor.ID}) /bca Succesfully looted ${itemName}
      /if ((${Debug} || ${Debug_Loot}) && !${Cursor.ID}) /echo |- Succesfully looted ${itemName} from slot# ${slotNum}
		}
	| Else, if loot attempt was unsuccessful, try again.
	} else {
		| Click yes on confirmation box.
		/if (${Window[ConfirmationDialogBox].Open}) {
			/notify ConfirmationDialogBox Yes_Button leftmouseup
		} else /if (${Window[QuantityWnd].Open}) {
			/notify QuantityWnd QTYW_Accept_Button leftmouseup
		}
		/if (${lootTimer}) /goto :retry_Loot
	}

  /if (${Debug} || ${Debug_Loot}) /echo <== LootItem -|
/return


| ################################################################################
| # Find closest merchant and open dialog with him/her
| ################################################################################
Sub OpenMerchant(int spawnID)
  /if (${Debug} || ${Debug_Sell}) /echo |- OpenMerchant ==>

	/declare openMerchantTimeout timer local
	/declare merchantTotal int local -1
	/declare merchantID int local ${If[${spawnID},${spawnID},${Spawn[Merchant].ID}]}

	/if (!${Spawn[Merchant].ID} || !${Spawn[Merchant].LineOfSight}) {
		/echo There are no merchants nearby!
    /goto :endOpenMerchant
	}

  | Move to the merchant.
  /call EnsureTarget ${merchantID}
  /call MoveToLoc ${Target.Y} ${Target.X} 80 15

  | Open Trade with merchant.
  /if (${Target.Distance} >= 20) {
		/echo Merchant out of range!
    /goto :endOpenMerchant
	}

  | Set timer
  /varset openMerchantTimeout 100

  | Right click merchant, and wait for window to open.
  :openMerchant
      /if (${Debug} || ${Debug_Sell}) /echo |- OpenMerchant -| :openMerchant

      /if (!${Window[MerchantWnd].Open}) /click right target
      /delay 5 ${Window[MerchantWnd].Open}
      /if (!${Window[MerchantWnd].Open}) {
        /if (${openMerchantTimeout}) {
          /goto :openMerchant
        } else {
          /echo Failed to open trade with [${Target.CleanName}].
          /goto :endOpenMerchant
        }
      }

  | Wait for merchant's item list to populate.
  :populateMerchant
    /if (${Debug} || ${Debug_Sell}) /echo |- OpenMerchant -| :populateMerchant
    /if (${merchantTotal}!=${Window[MerchantWnd].Child[ItemList].Items}) {
      /varset merchantTotal ${Window[MerchantWnd].Child[ItemList].Items}
      /delay 2
      /if (${openMerchantTimeout}) /goto :populateMerchant
    }

  :endOpenMerchant
  /if (${Debug} || ${Debug_Sell}) /echo <== OpenMerchant -|
/return

| ################################################################################
| # Close open dialog with merchant
| ################################################################################
Sub CloseMerchant
  /if (${Debug} || ${Debug_Sell}) /echo |- CloseMerchant ==>

	/declare closeMerhcantTimeout timer local 50

	:closeMerchant
    /if (${Debug} || ${Debug_Sell}) /echo |- CloseMerchant -| :closeMerchant
    /notify MerchantWnd MW_Done_Button leftmouseup
    /delay 5 ${Window[MerchantWnd].Open}
    /if (${Window[MerchantWnd].Open} && ${closeMerhcantTimeout}) /goto :closeMerchant


  /if (${Debug} || ${Debug_Sell}) /echo <== CloseMerchant -|
/return

| ################################################################################
| # Check if item is listed as sell in ini
| ################################################################################
Sub CanSellItem(string itemName, int itemID)
  /if (${Debug} || ${Debug_Sell}) /echo |- CanSellItem ==> ${itemName} ${itemID}

  /declare canSellItem bool   local false
  /declare lootSetting string local
  /varset lootSetting ${Ini[${lootSettingsIniFileName},"Items","${itemName} (${itemID})"]}

    /if (${lootSetting.Length}) {
      /varset canSellItem ${lootSetting.Find[Sell]}
    }

  /if (${Debug} || ${Debug_Sell}) /echo <== CanSellItem -|
/return ${canSellItem}

| ################################################################################
| # Check if item is listed as destroy in ini
| ################################################################################
Sub CanDestroyItem(string itemName, int itemID)
  /if (${Debug} || ${Debug_Destroy}) /echo |- CanDestroyItem ==>

  /declare canDestroyItem bool    local false
  /declare lootSetting    string  local
  /varset lootSetting ${Ini[${lootSettingsIniFileName},"Items","${itemName} (${itemID})"]}

  /if (${lootSetting.Length}) {
    /varset canDestroyItem ${lootSetting.Find[Destroy]}
  }

  /if (${Debug} || ${Debug_Sell}) /echo <== CanDestroyItem -|
/return ${canDestroyItem}

| ################################################################################
| # Sell items in inventory
| ################################################################################
Sub SellItems(int numInventorySlots)
  /if (${Debug} || ${Debug_Sell}) /echo |- SellItems ==> ${numInventorySlots}
	/declare retryTimer     timer local
	/declare i  int local
	/declare p  int local

	| Scan inventory for items to sell
	/for i 1 to ${numInventorySlots}
    /if (${Debug} || ${Debug_Sell}) /echo |- Slot# ${i}

    /if (${Bool[${Me.Inventory[pack${i}]}]}) {
      /if (${Debug} || ${Debug_Sell}) /echo |- Slot# ${i} has ${Me.Inventory[pack${i}]}

      | If the item in pack slot 'i' IS a container
      /if (${Me.Inventory[pack${i}].Container}) {
        /for p 1 to ${Me.Inventory[pack${i}].Container}
          /if (${Bool[${Me.Inventory[pack${i}].Item[${p}]}]}) {
            /call SellItem ${i} ${p}
          }
        /next p
      } else /if (${Bool[${Me.Inventory[pack${i}]}]}) {
        /call SellItem ${i}
      } else {
        /if (${Debug} || ${Debug_Sell}) /echo |- Unable to sell Slot# ${i} has ${Me.Inventory[pack${i}]}
      }
    }
  /next i

  /if (${Debug} || ${Debug_Sell}) /echo <== SellItems -|
/return

| ################################################################################
| # Sell a given item from given inventory slot / pack slot
| ################################################################################
Sub SellItem(int inventorySlot, int packSlot)
  /if (${Debug} || ${Debug_Sell}) /echo |- SellItem ==> ${inventorySlot} ${packSlot}

  /if (${Defined[packSlot]}) {
    /if (${Debug} || ${Debug_Sell}) /echo |- Inside bag
    /declare itemToSell   local ${Me.Inventory[pack${inventorySlot}].Item[${packSlot}]}
    /declare itemToSellID local ${Me.Inventory[pack${inventorySlot}].Item[${packSlot}].ID}
    /declare itemToSellValue local ${Me.Inventory[pack${inventorySlot}].Item[${packSlot}].Value}
  } else {
    /declare itemToSell   local ${Me.Inventory[pack${inventorySlot}]}
    /declare itemToSellID local ${Me.Inventory[pack${inventorySlot}].ID}
    /declare itemToSellValue local ${Me.Inventory[pack${inventorySlot}].Value}
  }

  /call CanSellItem "${itemToSell}" ${itemToSellID}
  /declare canSell bool local ${Macro.Return}

  /if (!${canSell}) {
    /if (${Debug} || ${Debug_Sell}) /echo |- SellItem > Item is not listed for selling: ${itemToSell}
    /goto :endSellItem
  }

  /if (!${itemToSellValue}) {
    /if (${Debug} || ${Debug_Sell}) /echo |- SellItem > Item has no value so skipping it: ${itemToSell}
    /goto :endSellItem
  }

  /declare retryTimer timer local 30

  :retrySelectItem
  /if (${Debug} || ${Debug_Sell}) /echo |- SellItem > :retrySelectItem
  /if (${Defined[packSlot]}) {
    /itemnotify in pack${inventorySlot} ${packSlot} leftmouseup
  } else {
    /itemnotify pack${inventorySlot} leftmouseup
  }

  /delay ${retryTimer} ${Window[MerchantWnd].Child[MW_SelectedItemLabel].Text.Equal[${itemToSell}]}

  | If the item was not selected
  /if (!${Window[MerchantWnd].Child[MW_SelectedItemLabel].Text.Equal[${itemToSell}]}) {
    | If I have time, retry to select the item
    /if (${retryTimer}) {
      /goto :retrySelectItem
    } else {
      /echo ERROR: Failed to select [${itemToSell}], skipping.
    }
  } else {
    | Attempt to sell the item
    /varset retryTimer 30
    /echo Selling [${itemToSell}].

    :SellItem
    /if (${Debug} || ${Debug_Sell}) /echo |- SellItem -| :SellItem -> ${retryTimer}
    /notify MerchantWnd MW_Sell_Button leftmouseup
    /delay ${retryTimer} ${Window[QuantityWnd].Open} || ((${Defined[packSlot]} && !${Bool[${Me.Inventory[pack${inventorySlot}].Item[${packSlot}]}]}) || (!${Defined[packSlot]} && !${Bool[${Me.Inventory[pack${inventorySlot}].Item}]}))

    | Close the quantity window
    /if (${Window[QuantityWnd].Open}) {
      /notify QuantityWnd QTYW_Accept_Button leftmouseup
      /delay ${retryTimer} !${Window[QuantityWnd].Open}
      /delay ${retryTimer} ((${Defined[packSlot]} && !${Bool[${Me.Inventory[pack${inventorySlot}].Item[${packSlot}]}]}) || (!${Defined[packSlot]} && !${Bool[${Me.Inventory[pack${inventorySlot}].Item}]}))
    }

    | If the item is still in my inventory
    /if (${Defined[packSlot]} && ${Bool[${Me.Inventory[pack${inventorySlot}].Item[${packSlot}]}]}) {
      /if (${retryTimer}) {
        /goto :SellItem
      } else {
        /echo ERROR: Failed to sell [${itemToSell}], skipping.
      }
    } else /if (!${Defined[packSlot]} && ${Bool[${Me.Inventory[pack${inventorySlot}].Item}]}) {
      /if (${retryTimer}) {
        /goto :SellItem
      } else {
        /echo ERROR: Failed to sell [${itemToSell}], skipping.
      }
    }
  }

  :endSellItem
  /if (${Debug} || ${Debug_Sell}) /echo <== SellItem -|
/return

| ################################################################################
| # Set item on cursor for selling
| ################################################################################
#Event SetCursorItemForSelling "<#*#> mark item for selling#*#"
#Event SetCursorItemForSelling "#*#[MQ2] mark item for selling#*#"
Sub Event_SetCursorItemForSelling
  /if (${Debug} || ${Debug_Sell}) /echo |- Event_SetCursorItemForSelling ==>

  /if (${Cursor.ID}) {
    /call CanSellItem "${Cursor}" ${Cursor.ID}
    /declare canSell bool local ${Macro.Return}

    /if (!${canSell}) {
      /declare lootSetting string  local ${Ini[${lootSettingsIniFileName},"Items","${Cursor} (${Cursor.ID})"]}
      /if (${lootSetting.NotEqual[NULL]}) {
        /varset  lootSetting "Sell,${lootSetting}"
      } else {
        /varset  lootSetting "Sell"
      }

      /if (${Debug}) /echo |- "${Cursor} (${Cursor.ID})" --> "${lootSetting}"

      /ini ${lootSettingsIniFileName} "Items" "${Cursor} (${Cursor.ID})" "${lootSetting}"
      /bc Marked <${Cursor} (${Cursor.ID})> for selling
    }
  }

  /if (${Debug} || ${Debug_Sell}) /echo <== Event_SetCursorItemForSelling -|
/return

| ################################################################################
| # Set item on cursor for destroying
| ################################################################################
#Event SetCursorItemForDestroy "<#*#> mark item for destroy#*#"
#Event SetCursorItemForDestroy "#*#[MQ2] mark item for destroy#*#"
Sub Event_SetCursorItemForDestroy
  /if (${Debug} || ${Debug_Loot}) /echo |- Event_SetCursorItemForDestroy ==>

  /if (${Cursor.ID}) {
    /call CanDestroyItem "${Cursor}" ${Cursor.ID}
    /declare canDestroy bool local ${Macro.Return}

    /if (!${canDestroy}) {
      /declare lootSetting string  local ${Ini[${lootSettingsIniFileName},"Items","${Cursor} (${Cursor.ID})"]}
      /if (${lootSetting.NotEqual[NULL]}) {
        /varset  lootSetting "Destroy,${lootSetting}"
      } else {
        /varset  lootSetting "Destroy"
      }

      /if (${Debug}) /echo |- "${Cursor} (${Cursor.ID})" --> "${lootSetting}"

      /ini ${lootSettingsIniFileName} "Items" "${Cursor} (${Cursor.ID})" "${lootSetting}"
      /bc Marked <${Cursor} (${Cursor.ID})> for destroying
    } else {
      /bc Could NOT mark <${Cursor} (${Cursor.ID})> for destroying
      /beep
    }
  }

  /if (${Debug} || ${Debug_Loot}) /echo <== Event_SetCursorItemForDestroy -|
/return


| ################################################################################
| # Set item on cursor for selling
| ################################################################################
#Event SellItems "<#*#> sell items to vendor#*#"
#Event SellItems "#*#[MQ2] sell items to vendor#*#"
Sub Event_SellItems
  /if (${Debug} || ${Debug_Sell}) /echo |- Event_SellItems ==>

    | Record starting location.
	/declare startingLoc string local ${Me.Loc.Replace[ ,]}

	| Open trade with the nearest merchant.
	/call OpenMerchant

	/if (${Window[MerchantWnd].Open}) {
		/keypress OPEN_INV_BAGS
		/call SellItems ${numInventorySlots}

		| Close merchant.
		/if (${Window[MerchantWnd].Open}) /call CloseMerchant

		| Close bags.
		/keypress CLOSE_INV_BAGS

		| Move back to starting location.
		/squelch /moveto loc ${startingLoc.Arg[1,,]} ${startingLoc.Arg[2,,]}

		/echo I have finished selling.
	}

  /if (${Debug} || ${Debug_Sell}) /echo <== Event_SellItems -|
/return


| ################################################################################
| # Set item on cursor for selling
| ################################################################################
#Event LootNearest "<#*#> loot nearest corpse#*#"
#Event LootNearest "#*#[MQ2] loot nearest corpse#*#"
Sub Event_LootNearest
  /if (${Debug} || ${Debug_Loot}) /echo |- Event_LootNearest ==>

  /call LootNearestCorpse

  /if (${Debug} || ${Debug_Loot}) /echo <== Event_LootNearest -|
/return

| ################################################################################
| # Adds corpses that are not yours to the looted corpse list.
| ################################################################################
#Event CheckCanLootCorpse "Someone is already looting that corpse."
#Event CheckCanLootCorpse "You may not loot this corpse at this time."
#Event CheckCanLootCorpse "You are too far away to loot that corpse."
Sub Event_CheckCanLootCorpse
  /varset canLootCorpse FALSE
/RETURN

Sub InitLoot(string iniFileName)
  /declare lootSettingsIniFileName string outer "ini/${MacroQuest.Server}/Loot Settings.ini"

  /declare Debug_Loot     bool    outer FALSE
  /declare Debug_Sell     bool    outer FALSE
  /declare linkChannel    string  outer "/bca"

  /if (${Ini[${iniFileName},Debug,Debug_Loot].Length}) {
    /varset Debug_Loot ${Bool[${Ini[${iniFileName},Debug,Debug_Loot]}]}
  }

  /if (${Ini[${iniFileName},Debug,Debug_Sell].Length}) {
    /varset Debug_Sell ${Bool[${Ini[${iniFileName},Debug,Debug_Sell]}]}
  }

  /squelch /alias /setsellitem /echo mark item for selling
  /squelch /alias /setdestroyitem /echo mark item for destroy
  /squelch /alias /sellitems /echo sell items to vendor
  /squelch /alias /doloot /echo loot nearest corpse
/return

Sub DoLootEvents
  /doevents LootNearest
  /doevents SellItems
  /doevents SetCursorItemForSelling
  /doevents SetCursorItemForDestroy
/return