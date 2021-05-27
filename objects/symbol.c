/* symbol.c manages the symbol lists and contains all symbol related functions. */

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmdInfo.h"
#include "symbol.h"

/* Adds a new symbol to the list */
void add_symbol(nodeRef list_head, int node_num, char symbol_name[MAX_SYMBOL_NAME], symbolType type, bool is_def)
{
	symbolNode newSymbol;
	symbolNode *found; 

	/* If the symbol already exists */
	if (is_def)
	{
		if ((found = get_symbol(*list_head, symbol_name)) != NULL)
		{

			/* If the symbol is of code or data type*/
			if (found->type != EXTERNAL && found->type != ENTRY && (found->type != type && !(type == EXTERNAL || type == ENTRY)))
			{
				printf("(In line: %d) Label name was already used.\n",lineNum);
				errorFound = TRUE;
				return;
			}

			/* if the symbol is of entry or extern type */
			if (type == EXTERNAL || type == ENTRY)
			{
				return;
			}
		}
	}

	/* Creates the new symbol */
	newSymbol.node_number = node_num;
	newSymbol.type = type;
	strcpy(newSymbol.symbol_name, symbol_name);

	/* Adds the new symbol to the list */
	add_node(list_head, &newSymbol, sizeof(symbolNode));
} /* end of add_symbol */

/* Gets a symbol by name from the symbol list */
symbolNode *get_symbol(nodePtr list_head, char symbol_name[MAX_SYMBOL_NAME]) 
{
	symbolNode *current;
	nodePtr iter; /* iterator */

	iter = list_head;
	/* If the list is empty return null */
	if (iter == NULL)
		return NULL;
	do
	{
		current = (symbolNode *)(iter->data);
		/* If the symbol was found return a pointer to it */
		if (strcmp(current->symbol_name, symbol_name) == 0)
			return current;
	} while ((iter = iter->next) != NULL);

	return NULL;
} /* end of get_symbol */

/* Gets the top symbol from the list and removes it */
symbolNode pop_symbol(nodeRef list_head)
{
	symbolNode *poppedData;
	symbolNode poppedSymbol = { 0 };
	
	/* If the list is empty return the initialized symbol (with value of 0) */
	if (*list_head == NULL)
		return poppedSymbol;

	poppedData = (symbolNode *)(pop_node(list_head).data);
	poppedSymbol.node_number = poppedData->node_number;
	strcpy(poppedSymbol.symbol_name, poppedData->symbol_name);
	poppedSymbol.type = poppedData->type;

	return poppedSymbol;
} /* end of pop_symbol */

/* Removes a symbol from the list */
void remove_symbol(symbolNode *symbol_to_remove)
{
	nodePtr nodeToDelete = symbolsHead;
	
	/* Searches the symbol list for the symbol to remove */
	while (nodeToDelete != NULL)
	{
		if (nodeToDelete->data == symbol_to_remove)
		{
			remove_node(&symbolsHead, nodeToDelete); /* removes symbol node from the list*/
			return;
		}
		nodeToDelete = nodeToDelete->next;
	}
} /* end of remove_symbol */

/* Checks the syntax of a symbol's name, returns true if valid */
bool valid_symbol_name(char *name) {
	int size, i, index = 0;
	
	/* If symbol name wasn't received */
	if (name==NULL || name[0]=='\0' || name[0] == NEW_LINE)
	{
		printf("(In line: %d) Missing operand.",lineNum);
		errorFound = TRUE;
		return FALSE;
	}
	size = strlen(name);
	
	/* Symbol name length check */
	if (size > MAX_SYMBOL_NAME)
	{
		printf("(In line: %d) Following symbol name is too long: %s.\n",lineNum,name);
		errorFound = TRUE;
		return FALSE;
	}
	
	/* Skips spaces in the beggining of the symbol name */
	while (name[index] == SPACE || name[index] == TAB)
		index++;
		
	/* If first char is not alphabetic */
	if (!isalpha(name[index]))
	{
		printf("(In line: %d) Following symbol name must start with an alphabetic character: %s.\n",lineNum,name);
		errorFound = TRUE;
		return FALSE;
	}

	/* If invalid char */
	for (i = index; i < size; i++)
	{
		if (!(isdigit(name[i]) || isalpha(name[i])))
		{
			printf("(In line: %d) Symbol name contains invalid character.\n",lineNum);
			errorFound = TRUE;
			return FALSE;
		}
	}

	/* If symbol name is a saved word */
	if (get_opcode(name) != -1 || get_funct(name) != -1)
	{
		printf("(In line: %d) Following symbol name is a saved word: %s.\n",lineNum,name);
		errorFound = TRUE;
		return FALSE;
	}

	return TRUE;
} /* end of valid_symbol_name */

/* Clears all symbol lists */
void clear_symbol_list()
{
	clear_list(&externHead);
	clear_list(&entryHead);
	clear_list(&symLineHead);
	clear_list(&symbolsHead);
} /* end of clear_symbol_list */

