/* breaker.c sections the input file's content to small parts which are directed to the corresponding processing functions */

#ifndef LINKEDLIST_H
	#define LINKEDLIST_H
	#include "linkedList.h"
#endif

#ifndef CMDINFO_H
	#define CMDINFO_H
	#include "cmdInfo.h"
#endif

#ifndef DATA_H
	#define DATA_H
	#include "data.h"
#endif

#ifndef SYMBOL_H
	#define SYMBOL_H
	#include "symbol.h"
#endif

#ifndef EXTRA_H
	#define EXTRA_H
	#include "extra.h"
#endif

int lineNum;	/* Line counter*/
bool errorFound;	/* Error tracker */
nodePtr instructions_head ;	/* all the instuctions and data in chronological order*/
int ic;		/* instruction counter */

/* Represents instruction word */
typedef struct 
{
	unsigned int dstAddMeth : 2;
	unsigned int srcAddMeth : 2;
	unsigned int funct : 4;
	unsigned int opcode : 4;
	unsigned int : 10;
	unsigned int encoding : 2;
	
} instruction;

/* Represents instruction data with encoding */
typedef struct 
{
	unsigned int data : 12;
	unsigned int : 10;
	unsigned int encoding : 2;
} instructionEnc;

/* Processes an instruction and adds it to the list */
void process_instruction(char *inst, int length);

/* Returns a pointer to the instruction which correlates to the index received */
instructionEnc *get_instruction_word(int ic);

/* Clears the instruction list and resets the instrucion counter */
void clear_instruction_list();
