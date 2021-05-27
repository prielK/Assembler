/* cmdInfo.c contains all the functions to get info about a command */

#include <stdio.h>

#ifndef BASICS_H
	#define BASICS_H
	#include "basics.h"
#endif

#ifndef EXTRA_H
	#define EXTRA_H
	#include "extra.h"
#endif

#define NUM_OF_COMMANDS sizeof(validCommands) / sizeof(validCommands[0])

int lineNum;	/* Line counter*/
bool errorFound;	/* Error tracker */

/* Encoding types */
typedef enum 
{
	EXTERN = 1,
	RELOCATABLE = 2,
	ABSOLUTE = 3
} encoding;

/* Addressing methods */
typedef enum 
{
	IMMEDIATE = 0,
	DIRECT = 1,
	RELATIVE = 2,
	REGISTER = 3,
	NONE = 4
} addressingMethod;

/* Command word definition */
typedef struct command 
{
	char *name;
	char opcode;
	char funct;
	addressingMethod srcMethods[4];
	addressingMethod dstMethods[4];
} command;

/* Represents command addressing method */
typedef struct cmdAddressMethod
{
	addressingMethod srcMethods;
	addressingMethod dstMethods;
} cmdAddressMethod;

/* Partitions a word to 4 bit pieces in order to apply hex conversion and limit it to 3 digits */
typedef struct hexPartition
{
	unsigned int a : 4;
	unsigned int b : 4;
	unsigned int c : 4;
} hexPartition;

/* All commands with corresponding opcodes, funct and addressing methods. */
static const command validCommands[] ={
	/* name, opcode, funct, source addressing methods, destination addressing methods */
	{ "mov", 0, 0, { IMMEDIATE, DIRECT, REGISTER},{DIRECT, REGISTER}},
	{ "cmp", 1, 0, { IMMEDIATE, DIRECT, REGISTER },{  IMMEDIATE, DIRECT, REGISTER } },
	{ "add", 2, 10, { IMMEDIATE, DIRECT, REGISTER },{ DIRECT, REGISTER } },
	{ "sub", 2, 11, { IMMEDIATE, DIRECT, REGISTER },{ DIRECT, REGISTER } },
	{ "lea", 4, 0, { DIRECT },{ DIRECT, REGISTER } },
	{ "clr", 5, 10, { NONE },{ DIRECT, REGISTER } },
	{ "not", 5, 11, { NONE }, { DIRECT, REGISTER } },
	{ "inc", 5, 12, { NONE },{ DIRECT, REGISTER } },
	{ "dec", 5, 13, { NONE },{ DIRECT, REGISTER } },
	{ "jmp", 9, 10, { NONE },{ DIRECT, RELATIVE } },
	{ "bne", 9, 11, { NONE },{ DIRECT, RELATIVE } },
	{ "jsr", 9, 12, { NONE },{ DIRECT, RELATIVE } },
	{ "red", 12, 0, { NONE },{ DIRECT, REGISTER } },
	{ "prn", 13, 0, { NONE },{ IMMEDIATE, DIRECT, REGISTER } },
	{ "rts", 14, 0, { NONE },{ NONE } },
	{ "stop", 15, 0, { NONE },{ NONE } }
};

/* Returns the addressing methods in a command  */
cmdAddressMethod get_command_method(char *command);

/* Checks if the command's addressing method matches the given opcode and funct */
bool valid_method(cmdAddressMethod method, int opcode, int funct);

/* Returns the command's opcode, if the command doesn't exist return -1 */
int get_opcode(char *nameWord);

/* Returns the command's funct, if the command doesn't exist return -1 */
int get_funct(char *nameWord);
