/* processer.c contains the processing functions */

#include <ctype.h>

#ifndef BASICS_H
	#define BASICS_H
	#include "basics.h"
#endif

#ifndef DATA_H
	#define DATA_H
	#include "data.h"
#endif

#ifndef SYMBOL_H
	#define SYMBOL_H
	#include "symbol.h"
#endif

#ifndef BREAKER_H
	#define BREAKER_H
	#include "breaker.h"
#endif

#ifndef EXTRA_H
	#define EXTRA_H
	#include "extra.h"
#endif

extern char *globalBufferENT;
int lineNum;	/* Line counter*/
bool errorFound;	/* Error tracker */

typedef struct 
{
	unsigned int data : 24;
} wordBuff;

/* Processes each line to code, data and symbol lists */
void first_transition(char *buffer);

/* Adds the symbol references to the processed code */
void second_transition();

/* Saves object content into buffer, returns a pointer to the buffer */
char *ob_content();

/* Saves entry content into buffer, returns a pointer to the buffer */
char *ent_content();

/* Saves extern content into buffer, returns a pointer to the buffer */
char *ext_content();

/* Clears all instructions, data and symbols */
void clear_all();
