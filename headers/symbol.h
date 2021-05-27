/* symbol.c manages the symbol lists and contains all symbol related functions. */

#ifndef LINKEDLIST_H
	#define LINKEDLIST_H
	#include "linkedList.h"
#endif

#ifndef EXTRA_H
	#define EXTRA_H
	#include "extra.h"
#endif

int lineNum;	/* Line counter*/
bool errorFound;	/* Error tracker */
nodePtr symbolsHead;		/* Symbols list */
nodePtr entryHead;	/* All entry symbols */
nodePtr externHead;		/* All extern symbols*/
nodePtr symLineHead;		/* Where symbols have been used in code */

typedef enum 
{
	DATA,
	CODE,
	EXTERNAL,
	ENTRY
} symbolType;

typedef struct symbolNode
{
	unsigned int node_number; /* Number in linked list */
	char symbol_name[MAX_SYMBOL_NAME];
	symbolType type;
} symbolNode;

/* Adds a new symbol to the list */
void add_symbol(nodeRef list_head, int node_number, char symbol_name[MAX_SYMBOL_NAME], symbolType type, bool is_definition);

/* Gets a symbol by name from the symbol list */
symbolNode *get_symbol(nodePtr list_head, char symbol_name[MAX_SYMBOL_NAME]);

/* Gets the top symbol from the list and removes it */
symbolNode pop_symbol(nodeRef list_head);

/* Removes a symbol from the list */
void remove_symbol(symbolNode *symbol_to_remove);

/* Checks the syntax of a symbol's name, returns true if valid */
bool valid_symbol_name(char *name);

/* Clears all symbol lists */
void clear_symbol_list();
