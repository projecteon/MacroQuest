| # @file - DataStructures.inc  
| # @author - Cantus, Reno, Zamphyr
| #
| # An include file of functions used to provide advanced data structures to MacroQuest2 macros. 
| # TODO: Upper/Lower checking on HashMaps. It currently checks by case, why, I'm not sure.

| +-----------------+
| | Queue Functions |
| +-----------------+

| ##
| #  Add value szValue to a new element appended  
| #  to the FIFO Queue named szQueueName.
| # 
| # @PARAM - string szQueueName - The name of the Queue.
| # @PARAM - string szValue - The value to be added to the Queue.
| ##
Sub QueueAdd(string szQueueName, string szValue)

    /if ( !${Defined[${szQueueName}QueueEndIndex]} ) {

        /declare ${szQueueName}QueueEndIndex        int    global    0
    }

    /if ( !${Defined[${szQueueName}QueueStartIndex]} ) {

        /declare ${szQueueName}QueueStartIndex        int    global    1
    }

    /varcalc ${szQueueName}QueueEndIndex    ${${szQueueName}QueueEndIndex} + 1

    /declare ${szQueueName}QueueElement${${szQueueName}QueueEndIndex}    string    global    ${szValue}

/return

| ##
| # Returns then deletes the first element in the FIFO Queue named szQueueName.
| #
| # @PARAM - string szQueueName - The name of the Queue.
| # @RETURN - string - The value that was removed.
| ##
Sub QueueRemove(string szQueueName)

    /if ( !${Defined[${szQueueName}QueueStartIndex]} ) {

        /return NULL
    }

    /declare szBuffer    string    global    ${${szQueueName}QueueElement${${szQueueName}QueueStartIndex}}

    /deletevar ${szQueueName}QueueElement${${szQueueName}QueueStartIndex}

    /varcalc ${szQueueName}QueueStartIndex    ${${szQueueName}QueueStartIndex} + 1

    /if ( !${Defined[${szQueueName}QueueEndIndex]} || ${${szQueueName}QueueStartIndex} > ${${szQueueName}QueueEndIndex} ) {

        /deletevar ${szQueueName}QueueStartIndex
        /deletevar ${szQueueName}QueueEndIndex
    }

/return ${szBuffer}

| ##
| # Returns the first element in the FIFO Queue
| # named szQueueName WITHOUT deleting it.
| #
| #
| # @PARAM - string szQueueName - The name of the Queue.
| # @RETURN - string - The first value of the Queue.
| ##
Sub QueuePeek(string szQueueName)

    /if ( !${Defined[${szQueueName}QueueStartIndex]} ) {

        /return NULL
    }

/return ${${szQueueName}QueueElement${${szQueueName}QueueStartIndex}}

| ##
| # Returns the number of elements currently in
| # the FIFO Queue named szQueueName.
| #
| # @PARAM - string szQueueName - The name of the Queue.
| # @RETURN - int - The number of elements in this Queue.
| ##
Sub QueueLength(string szQueueName)

    /if ( !${Defined[${szQueueName}QueueStartIndex]} || !${Defined[${szQueueName}QueueEndIndex]} ) {

        /return 0
    }

/return ${Math.Calc[${${szQueueName}QueueEndIndex} - ${${szQueueName}QueueStartIndex}]}

| +-----------------+
| | Stack Functions |
| +-----------------+

| ##
| # Add value szValue to a new element appended
| # to the LIFO Stack named szStackName.
| #
| # @PARAM - string szStackName - The name of the Stack.
| # @PARAM - string szValue - The value to be added to the stack.
| ##
Sub StackPush(string szStackName, string szValue)

    /if ( !${Defined[${szStackName}StackCounter]} ) {

        /declare ${szStackName}StackCounter        int    global    0
    }

    /varcalc ${szStackName}StackCounter    ${${szStackName}StackCounter} + 1

    /declare ${szStackName}StackElement${${szStackName}StackCounter}    string    global    ${szValue}

/return

| ##
| # Returns then deletes the last element in the LIFO Stack named szStackName.
| #
| # @PARAM - string szStackName - The name of the Stack.
| # @RETURN - string - The value that was removed
| ##
Sub StackPop(string szStackName)

    /if ( !${Defined[${szStackName}StackCounter]} ) {

        /return NULL
    }

    /declare szBuffer    string    global    ${${szStackName}StackElement${${szStackName}StackCounter}}

    /deletevar ${szStackName}StackElement${${szStackName}StackCounter}

    /varcalc ${szStackName}StackCounter    ${${szStackName}StackCounter} - 1

    /if ( ${${szStackName}StackCounter} <= 0 ) {

        /deletevar ${szStackName}StackCounter
    }

/return ${szBuffer}

| ##
| # Returns the last element in the LIFO Stack named szStackName WITHOUT deleting it.
| #
| # @PARAM - string szStackName - The name of the Stack.
| # @RETURN - string - The next value in the stack.
| ##
Sub StackTop(string szStackName)

    /if ( !${Defined[${szStackName}StackCounter]} ) {

        /return NULL
    }

/return ${${szStackName}StackElement${${szStackName}StackCounter}}

| ##
| # Returns the last element in the LIFO Stack named szStackName WITHOUT deleting it.
| #
| # @PARAM - string szStackName - The name of the Stack.
| # @RETURN - string - The value that was removed.
| ##
Sub StackPeek(string szStackName)

    /call StackTop ${szStackName}

/return ${Macro.Return}

| ##
| # Returns the number of elements currently in  
| #    the LIFO Stack named szStackName.   
| #
| # @PARAM - string szStackName - The name of the Stack.
| # @RETURN - int - The number of elements in the stack.
| ##
Sub StackLength(string szStackName)

    /if ( !${Defined[${szStackName}StackCounter]} ) {

        /return 0
    }

/return ${${szStackName}StackCounter}






| +-------------------------+
| | Dynamic Array Functions |
| +-------------------------+

| ##
| # Remove an element from the dynamic array
| # named szDynArrayName. Only removes one instance of that element.                     
| #
| # @PARAM - string szDynArrayName - The name of the array.
| # @PARAM - string szValue - The value to be removed.
| ##
Sub DynArrayRemove(string szDynArrayName, string szValue)

    /declare i int local
	
	/call DynArrayLength ${szDynArrayName}
	/declare elements int local ${Macro.Return}
	
	/for i 1 to ${elements}
		/if (${${szDynArrayName}DynArrayElement${i}.Equal[${szValue}]}) {
			/call DynArrayDelete ${szDynArrayName} ${i}
			/return
		}
			
	/next i

/return

| ##
| # Add value szValue to a new element appended to the Dynamic Array named szDynArrayName. 
| #
| # @PARAM - string szDynArrayName - The name of the array.
| # @PARAM - string szValue - The value to be added.
| # @RETURN - int - The index of the newly added value.
| ##
Sub DynArrayAdd(string szDynArrayName, string szValue)
	
    /if ( !${Defined[${szDynArrayName}DynArrayCounter]} ) {

        /declare ${szDynArrayName}DynArrayCounter        int    outer    0
    }

    /varcalc ${szDynArrayName}DynArrayCounter    ${${szDynArrayName}DynArrayCounter} + 1

    /declare ${szDynArrayName}DynArrayElement${${szDynArrayName}DynArrayCounter}    string    outer    ${szValue}

/return ${${szDynArrayName}DynArrayCounter}

| ##                                            
| # Returns the value at index iIndex from the Dynamic Array named szDynArrayName.   
| #
| # @PARAM - string szDynArrayName - The name of the array.
| # @PARAM - int iIndex - The index to be read.
| # @RETURN - string - The element that was read
| ##
Sub DynArrayRead(string szDynArrayName, int iIndex)

    /if ( !${Defined[${szDynArrayName}DynArrayCounter]} || !${Defined[${szDynArrayName}DynArrayElement${iIndex}]} ) {

        /return NULL
    }

/return ${${szDynArrayName}DynArrayElement${iIndex}}


| ##                                      
| # Sets the value at location iIndex in the   
| #    Dynamic Array named szDynArrayName.        
| # Returns NULL if iIndex does not refer to   
| # an already-existing location in the array,   
| # except if iIndex is equal to the array       
| # length + 1, in which case DynArrayModify()   
| # acts just like DynArrayAdd(), and appends   
| # the new value to the end of the array. 
| #
| # @PARAM - string szDynArrayName - The name of the array.
| # @PARAM - int iIndex - The index to modify.
| # @PARAM - string szValue - The new value for the element.
| ##
Sub DynArrayModify(string szDynArrayName, int iIndex, string szValue)

    /if ( !${Defined[${szDynArrayName}DynArrayCounter]} ) {

        /return NULL
    }

    /if ( ${iIndex} == ${${szDynArrayName}DynArrayCounter} + 1 ) {

        /call DynArrayAdd    "${szDynArrayName}"    "${szValue}"

        /return ${Macro.Return}

    } else /if ( !${Defined[${szDynArrayName}DynArrayElement${iIndex}]} ) {

        /return NULL
    }

    /varset ${szDynArrayName}DynArrayElement${iIndex}    ${szValue}

/return ${iIndex}

| ##
| # Deletes the specified element of the Dynamic Array named szDynArrayName. It will shift
| # all higher index values down so that there is not a gap.
| #
| # @PARAM - string szDynArrayName - The name of the array.
| # @PARAM - int iIndex - The index to delete.
| # @RETURN - bool - TRUE on success, FALSE on failure.
| ##
Sub DynArrayDelete(string szDynArrayName, int iIndex)

    /if ( !${Defined[${szDynArrayName}DynArrayCounter]} ) {

        | No Dynamic Array by the name szDynArrayName exists.

        /return FALSE
    }

    /declare iLoopCounter    int    local

    /if ( ${Defined[iIndex]} ) {

        | The user wants to delete a single element in the Dynamic Array, of index #iIndex.

        /if ( !${Defined[${szDynArrayName}DynArrayElement${iIndex}]} ) {

            | The array index to be deleted does not exist (either it never existed, or was deleted already).

            /return FALSE
        }

        /for iLoopCounter ${iIndex} to ${Int[${Math.Calc[${${szDynArrayName}DynArrayCounter}-1]}]}

            /varset ${szDynArrayName}DynArrayElement${iLoopCounter}    ${${szDynArrayName}DynArrayElement${Int[${Math.Calc[${iLoopCounter}+1]}]}}

        /next iLoopCounter

        /deletevar ${szDynArrayName}DynArrayElement${${szDynArrayName}DynArrayCounter}

        /varcalc ${szDynArrayName}DynArrayCounter    ${${szDynArrayName}DynArrayCounter} - 1

        /if ( ${${szDynArrayName}DynArrayCounter} == 0 ) {

            | The entire Dynamic Array has been deleted. Delete the Counter variable, essentially completely deleting the
            | array (rather than leaving it around, which represents a still defined, but zero-length array.)

            /deletevar ${szDynArrayName}DynArrayCounter
        }

    } else {

        | The user wants to delete the entire Dynamic Array.

        /for iLoopCounter 1 to ${${szDynArrayName}DynArrayCounter}

            /deletevar ${szDynArrayName}DynArrayElement${iLoopCounter}

        /next iLoopCounter

        /deletevar ${szDynArrayName}DynArrayCounter
    }

/return TRUE

| ##
| # Deletes the entire contents of an array.
| #
| # @PARAM - string ArrayName - The name of the array.
| ##
Sub DynArrayDeleteAll(string ArrayName)
	/declare i int local 0
	/call DynArrayLength "${ArrayName}"
	/declare endIndex int local ${Macro.Return}

	/for i 0 to ${endIndex}
		/call DynArrayDelete "${ArrayName}" 1
	/next i
	
/return

| ##
| # Searchs and returns the index of an element in an array.
| #
| # @PARAM - string ArrayName - The name of the array.
| # @PARAM - string Value - The value to be searched for.
| # @RETURN - int - The index of the element, 0 if it doesn't exist.
| ##
Sub DynArrayFind(string ArrayName, string Value)
	/declare i int local 0
	/call DynArrayLength "${ArrayName}"
	/declare endIndex int local ${Macro.Return}
	/declare Position int local 0
	
	/for i 1 to ${endIndex}
		/call DynArrayRead "${ArrayName}" ${i}
		/if (${Macro.Return.Equal[${Value}]}) {
			/varset Position ${i}
		}
	/next i
	
	/return ${Position}
/return

| ##
| # Swaps two elements of an array.
| #
| # @PARAM - string ArrayName - The name of the array.
| # @PARAM - int IndexA - The first element to be swapped.
| # @PARAM - int IndexB - The element to swamp IndexA with.
| ##
Sub DynArraySwap(string ArrayName, int IndexA, int IndexB)
	/if (!${Defined[TempElement]}) /declare TempElement string local
	
	/call DynArrayRead "${ArrayName}" ${IndexA}
	/varset TempElement ${Macro.Return}
	
	/call DynArrayRead "${ArrayName}" ${IndexB}
	/call DynArrayModify "${ArrayName}" ${IndexA} "${Macro.Return}"
	
	/call DynArrayModify "${ArrayName}" ${IndexB} "${TempElement}"
	
/return

| ##
| # Returns the number of elements currently in    *
| # the Dynamic Array named szDynArrayName. 
| #
| # @PARAM - string szDynArrayName - The name of the array.
| # @RETURN - int - The number of elements in szDynArrayName.
| ##
Sub DynArrayLength(string szDynArrayName)

    /if ( !${Defined[${szDynArrayName}DynArrayCounter]} ) {

        /return 0
    }

/return ${${szDynArrayName}DynArrayCounter}

| ##
| # Sorts the contents of the Dynamic Array named szDynArrayName alphabetically by the
| # contents of its elements. Orders the array in ascending order (index
| # #1 would be 'A' or '1') by default or if bAscending is set to TRUE. If bAscending is
| # FALSE, sorts in descending order. 
| #
| # @PARAM - string szDynArrayName - The name of the array.
| # @PARAM - bool bAscending - Ascending sort or Descending.
| # @PARAM - bool bCaseSensitive - Comparisons are case-insensitive by default or if bCaseSensitive is set to FALSE.
| ##
Sub DynArraySort(string szDynArrayName, bool bAscending, bool bCaseSensitive)

	/if ( !${Defined[${szDynArrayName}DynArrayCounter]} || ${${szDynArrayName}DynArrayCounter} < 2 ) {
	
		/return
	}

	/if ( !${Defined[bAscending]} ) {

		/declare bAscending bool local TRUE
	}

	/if ( !${Defined[bCaseSensitive]} ) {

		/declare bCaseSensitive bool local FALSE
	}

	/declare sBuffer string local

	/declare iGapSize int local 0

	/declare iOuterLoopCounter int local 1
	/declare iInnerLoopCounter int local 1


	:DynArraySort_GapSequenceDeterminationLoop

	/varcalc iGapSize (${iGapSize} * 3) + 1

	/if ( ${iGapSize} <= ${${szDynArrayName}DynArrayCounter} ) {

		/goto :DynArraySort_GapSequenceDeterminationLoop
	}

	| End :DynArraySort_GapSequenceDeterminationLoop


	:DynArraySort_OuterLoop

	/varcalc iGapSize ${iGapSize} \ 3

	/if ( ${Int[${Math.Calc[${iGapSize} + 1]}]} <= ${${szDynArrayName}DynArrayCounter} ) {

		/for iOuterLoopCounter ${Int[${Math.Calc[${iGapSize} + 1]}]} to ${${szDynArrayName}DynArrayCounter}

			/varset iInnerLoopCounter ${iOuterLoopCounter}

			/varset sBuffer ${${szDynArrayName}DynArrayElement${iOuterLoopCounter}}

			:DynArraySort_InnerLoop

			/if ( ${${szDynArrayName}DynArrayElement${Int[${Math.Calc[${iInnerLoopCounter}-${iGapSize}]}]}.${If[${bCaseSensitive},CompareCS,Compare]}[${sBuffer}]} == ${If[${bAscending},1,-1]} ) {

				/varset ${szDynArrayName}DynArrayElement${iInnerLoopCounter} ${${szDynArrayName}DynArrayElement${Int[${Math.Calc[${iInnerLoopCounter}-${iGapSize}]}]}}

				/varcalc iInnerLoopCounter ${iInnerLoopCounter} - ${iGapSize}

				/if ( ${Int[${Math.Calc[${iInnerLoopCounter} - ${iGapSize}]}]} >= 1 ) {

					/goto :DynArraySort_InnerLoop
				}
			}

			| End :DynArraySort_InnerLoop

			/varset ${szDynArrayName}DynArrayElement${iInnerLoopCounter} ${sBuffer}

		/next iOuterLoopCounter
	}

	/if ( ${iGapSize} != 1 ) {

		/goto :DynArraySort_OuterLoop
	}

	| End :DynArraySort_OuterLoop

/return

| +-----------------------------------------+
| | Hashtable (Associative Array) Functions |
| +-----------------------------------------+

| ##
| # Adds name/value pair szName/szValue to a
| # new element added to the Hashtable named
| # szHashName.  
| #
| # @PARAM - string szHashName - The Name of the Hash.
| # @PARAM - string szName - The name of the element NOTE: No spaces.
| # @PARAM - string szValue - The value for the element.
| ##
Sub HashAdd(string szHashName, string szName, string szValue)

    /if ( ${Defined[${szHashName}HashElement${szName}]} ) {

        /echo HashAdd(): Error, there is already an element in Hash "${szHashName}" with a name "${szName}". Use the HashReassign() function to change the value associated with a pre-existing name.

        /return
    }

    /if ( !${Defined[${szHashName}HashCounter]} ) {

        /declare ${szHashName}HashCounter        int    outer    0
    }

    /varcalc ${szHashName}HashCounter    ${${szHashName}HashCounter} + 1

    /declare ${szHashName}HashElement${szName}    string    outer    ${szValue}

/return

| ##
| # Reads the value associated with name szName   
| # in the Hashtable named szHashName. 
| #
| # @PARAM - string szHashName - The Name of the Hash.
| # @PARAM - string szName - The element to be read.
| # @RETURN - string - The value of the element.
| ##
Sub HashRead(string szHashName, string szName)

    /if ( !${Defined[${szHashName}HashCounter]} || !${Defined[${szHashName}HashElement${szName}]} ) {

        /return NULL
    }

/return ${${szHashName}HashElement${szName}}

| ##
| # If an element in Hashtable szHashName with    
| # name szName already exists, it changes the    
| # value associated with that name to szValue. 
| # If that name does not already exist, it        
| # acts just like a call to HashAdd(), creating
| # a new name/value pair for szName/szValue.
| #
| # @PARAM - string szHashName - The Name of the Hash.
| # @PARAM - string szName - The name of the element NOTE: No spaces.
| # @PARAM - string szValue - The value for the element.
| ##
Sub HashReassign(string szHashName, string szName, string szValue)

    /call HashDelete    "${szHashName}"    "${szName}"

    /call HashAdd        "${szHashName}"    "${szName}"    "${szValue}"

/return

| ##
| # Deletes the specified name/value pair from 
| # the Hashtable named szHashName. 
| #
| # @PARAM - string szHashName - The Name of the Hash.
| # @PARAM - boolean - TRUE if successful.
| ##
Sub HashDelete(string szHashName, string szName)

    /if ( !${Defined[${szHashName}HashCounter]} || !${Defined[${szHashName}HashElement${szName}]} ) {

        /return FALSE
    }

    /deletevar ${szHashName}HashElement${szName}

    /varcalc ${szHashName}HashCounter    ${${szHashName}HashCounter} - 1

/return TRUE

| ##
| # Returns the number of elements currently in the Hashtable named szHashName. 
| #
| # @PARAM - string szHashName - The Name of the Hash.
| # @RETURN - int - The number of elemnts in szHashName.
| ##
Sub HashLength(string szHashName)

    /if ( !${Defined[${szHashName,}HashCounter]} ) {

        /return 0
    }

/return ${${szHashName}HashCounter}