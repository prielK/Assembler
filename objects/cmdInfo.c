/* cmdInfo.c contains all the functions to get info about a command */

#include <string.h>
#include "cmdInfo.h"

/* Returns the parameter addressing method */
addressingMethod get_param_method(char *param)
{
	if (param == NULL)
		return NONE;
	if (param[0] == '#')
		return IMMEDIATE;
	if (param[0] == 'r')
		return REGISTER;
	if (param[0] == '%')
		return RELATIVE;
	/* If none of the others */
	return DIRECT;
} /* end of get_param_method */

/* Returns the addressing methods in a command  */
cmdAddressMethod get_command_method(char *command)
{
	cmdAddressMethod cmdMethod; /* output */
	char *buffer;
	int index;

	buffer = strtok(NULL, ",");

	/* If there are no parameters */
	if (buffer == NULL)
	{
		cmdMethod.srcMethods = NONE;
		cmdMethod.dstMethods = NONE;
		return cmdMethod;
	}
	/* If there is one parameter */
	if (strtok(NULL, ",") == NULL)
	{
		cmdMethod.srcMethods = NONE;
		cmdMethod.dstMethods = get_param_method(get_word(buffer));
		return cmdMethod;
	}
	/* If there are two parameters */
	index = strlen(buffer) + 1;
	cmdMethod.srcMethods = get_param_method(get_word(buffer));
	cmdMethod.dstMethods = get_param_method(get_word(buffer + index));
	return cmdMethod;
} /* end of get_command_method */

/* Checks if the command's addressing method matches the given opcode and funct */
bool valid_method(cmdAddressMethod method, int opcode, int funct)
{
	bool found = FALSE; /* for compatability checks*/
	int size, i, j;
	
	/* Checks if opcode and funct match */
	for (i = 0; i < NUM_OF_COMMANDS; i++)
	{
		if (validCommands[i].opcode == opcode && validCommands[i].funct == funct)
		{
			found = TRUE;
			break;
		}
	}
	
	/* If opcode and funct match*/
	if (found == TRUE)
	{
		found = FALSE; /* reset */
		size = sizeof(validCommands[i].srcMethods) / sizeof(validCommands[i].srcMethods[0]);

		/* Checks source addressing method */
		for (j = 0; j < size; j++)
		{
			if (validCommands[i].srcMethods[j] == method.srcMethods)
			{
				found = TRUE;
				break;
			}
		}
		if (found == FALSE)
		{
			if (method.srcMethods == NONE)
				printf("(In line: %d) Missing operand.\n",lineNum);
			else
				printf("(In line: %d) Incompatible addressing method.\n",lineNum);
			errorFound = TRUE;
		}
		
		/* Checks destination addressing method */
		else 
		{
			size = sizeof(validCommands[i].dstMethods) / sizeof(validCommands[i].dstMethods[0]);
			for (j = 0; j < size; j++)
			{
				if (validCommands[i].dstMethods[j] == method.dstMethods)
					return TRUE;
			}
			if (method.dstMethods == NONE)
				printf("(In line: %d) Missing operand.\n",lineNum);
			else
				printf("(In line: %d) Incompatible addressing method.\n",lineNum);
			errorFound = TRUE;
		}
	}
	/* If opcode and funct don't match*/
	else
	{
		printf("(In line: %d) Opcode and funct don't match.\n",lineNum);
		errorFound = TRUE;
	}
	return FALSE;
} /* end of valid_method */

/* Returns the command's opcode, if the command doesn't exist return -1 */
int get_opcode(char *nameWord)
{
	int i;
	/* Searches for the command name */
	for (i = 0; i < NUM_OF_COMMANDS; i++)
	{
		if (strcmp(nameWord, validCommands[i].name) == 0)
			return validCommands[i].opcode;
	}
	/* If command wasn't found */
	return -1;
} /* end of get_opcode */

/* Returns the command's funct, if the command doesn't exist return -1 */
int get_funct(char *nameWord)
{
	int i;
	/* Searches for the command name */
	for (i = 0; i < NUM_OF_COMMANDS; i++)
	{
		if (strcmp(nameWord, validCommands[i].name) == 0)
			return validCommands[i].funct;
	}
	/* If command wasn't found */
	return -1;
} /* end of get_funct */
