/* data.c contains the data processing functions */

#ifndef BASICS_H
	#define BASICS_H
	#include "basics.h"
#endif

#ifndef LINKEDLIST_H
	#define LINKEDLIST_H
	#include "linkedList.h"
#endif

#ifndef EXTRA_H
	#define EXTRA_H
	#include "extra.h"
#endif

#define DATA_DELIMITER ","

int lineNum;	/* Line counter*/
bool errorFound;	/* Error tracker */
nodePtr dataHead;	/* Head of linked-list, contains all data */
int dc;		/* Data counter */

typedef struct 
{
	signed int data : 24; /* value of data node and encoding (only 14 bits used - 12 for data and 2 for encoding) */
} dataWord;

/* Clears the data list and resets the data counter */
void clear_data_list();

/* Processes string to dataWord and adds it to the list */
bool process_string(char string[]);

/* Processes data words and adds them to the list, returns the amount of words added */
int process_data(char *data);


