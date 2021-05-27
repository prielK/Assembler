/* breaker.c sections the input file's content to small parts which are directed to the corresponding processing functions */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "breaker.h"

/* Returns the decimal number with a bit representation of all bits off except for the index of the register received */
int get_register(char *name)
{
	int regVal, regInd, i;

	/* Input validation */
	if (name[0] == 'r')
	{
		regVal = atoi(name + 1); /* register's index*/
		/* Checks register index */
		if (!(regVal == 0 && name[1] != '0' && name[2] != NULL_TERMINATOR))
		{
			/* Checks register index */
			if (regVal >= 0 && regVal < REGISTER_NUMBER)
			{
				/* Getting the number with the bit representation of all bits off except for the index */
				if (regVal != 0)	
				{
					regInd=regVal;
					regVal=1;
					for (i=0 ; i<regInd ; i++)
						regVal = regVal*2;
				}
				/* Returns the decimal value of the bit representation of the register's index */
				return regVal;
			}
		}
	} 
	
	/* If invalid register */
	printf("(In line: %d) Unknown register: %s.\n",lineNum,name);
	errorFound = TRUE;
	return -1;
} /* end of get_register */

/* Adds the processed instruction to the list */
void add_instruction(nodePtr fullInstruction)
{
	nodePtr iter = fullInstruction; /* list head */

	/* Adds all nodes from the list received to the main list */
	while (iter != NULL)
	{
		add_node(&instructions_head, iter->data, sizeof(instruction)); 
		iter = iter->next;
		ic++;
	}

	/* Clears the temporary list */
	clear_list(&fullInstruction);
} /* end of add_instruction */

/* Processes a parameter and adds it to the list */
void process_parameter(nodePtr head, char *param, addressingMethod methods, instruction *inst)
{
	instructionEnc instData = {0}; /* initializes */
	int value; /* for immediate addressing method */
	
	/* checks if parameter was received */
	if (param[0] == NULL_TERMINATOR || param[0] == '\0' || param[0] == NEW_LINE)
	{
		printf("(In line: %d) Missing parameter.",lineNum);
		errorFound = TRUE;
		return;
	}
	
	/* If the parameter is of the immediate addressing method */
	if (methods == IMMEDIATE)
	{
		value = atoi(param + 1); /* gets the parameter's value (skips '#') */
		
		/* checks if value not outside limits of 12 bits */
		if (value > MAX_NUM || value < MIN_NUM)
		{
			printf("(In line: %d) Invalid operand - value exceeds limits.",lineNum);
			errorFound = TRUE;
		}
		/* adds to the list */
		else
		{
			instData.data = value;
			instData.encoding = ABSOLUTE;
			add_node(&head, &instData, sizeof(instructionEnc));
		}
		return;
	}
	/* If the parameter is of the direct addressing method */
	if (methods == DIRECT)
	{
		/* parameter validation */
		if (!valid_symbol_name(param))
		{
			return;
		}
		/* adds the symbol to be handled in the second transition */
		add_symbol(&symLineHead, ic + list_len(head) + 1, param, DATA , FALSE);		
		instData.data = 0;
		instData.encoding = RELOCATABLE;
		/* if symbol is of extern type*/
		if(get_symbol(head, param) != NULL && (get_symbol(head, param)->type == EXTERNAL))
			instData.encoding = EXTERN;
		add_node(&head, &instData, sizeof(instructionEnc)); /* adds to the list */
		return;
	}
	/* If the parameter is of the relative addressing method */
	if (methods == RELATIVE)
	{
		/* parameter validation */
		if (!valid_symbol_name(param + 1) || (get_symbol(head, param) != NULL && get_symbol(head, param + 1)->type == EXTERNAL))
			return;
		/* adds the symbol to be handled in the second transition */
		add_symbol(&symLineHead, ic + list_len(head) + 1, param, DATA, FALSE);
		instData.data = ic + list_len(head);
		instData.encoding = ABSOLUTE;
		add_node(&head, &instData, sizeof(instructionEnc)); /* adds to the list */
		return;
	}
	/* If the parameter is a register*/
	if (methods == REGISTER)
	{
		instData.data = get_register(get_word(param)); /* gets register ID in bit form*/
		instData.encoding = ABSOLUTE;
		add_node(&head, &instData, sizeof(dataWord)); /* adds to the list */
		return;	
	}
	
} /* end of process_parameter */

/* Processes an instruction and adds it to the list */
void process_instruction(char *inst, int length) 
{
	int opcode,funct;
	char *firstParam,*secondParam;
	nodePtr fullInstruction = NULL;
	instruction tempInst = {0};
	cmdAddressMethod method;
	
	/* Validates opcode and funct */
	if ((opcode = get_opcode(inst)) == -1 || (funct = get_funct(inst)) == -1)
	{
		printf("(In line: %d) The following command doesn't exist: %s.\n",lineNum,inst);
		errorFound = TRUE;
		return;
	}

	/* Checks if addressing method matches the command */
	method = get_command_method(inst);
	if (valid_method(method, opcode, funct) == FALSE)
	{
		return;
	}

	/* Sets values */
	tempInst.opcode = opcode;
	tempInst.funct = funct;
	tempInst.srcAddMeth = method.srcMethods;
	tempInst.dstAddMeth = method.dstMethods;
	tempInst.encoding = ABSOLUTE;

	add_node(&fullInstruction, &tempInst, sizeof(instruction)); /* adds to the list */
	
	/* If there are no parameters */
	if (method.dstMethods == NONE && method.srcMethods == NONE)
	{
		add_instruction(fullInstruction); /* adds to the instruction list*/
		return;
	}
	
	length -= strlen(inst);
	
	/* Sets parameters values */
	firstParam = get_word(strtok(inst + strlen(inst) + 1, SPACE_STR));
	secondParam = strtok(null_skip(firstParam + strlen(firstParam)), DATA_DELIMITER);

	/* Processes first parameter */
	if (method.srcMethods != NONE)
	{
		length -= strlen(firstParam);
		process_parameter(fullInstruction, firstParam, method.srcMethods, &tempInst);
	}
	
	/* Processes second parameter */
	if (method.dstMethods != NONE)
	{
		/* incase there is only one parameter, use the first parameter */
		if (method.srcMethods == NONE)
		{
			secondParam = firstParam;
		}
		else
			length--;
		length -= strlen(secondParam);
		process_parameter(fullInstruction, secondParam, method.dstMethods, &tempInst);
	}
	
	/* Checks for extra text after parameters */
	if (length!=0)
	{
		printf("(In line: %d) Too many parameters.",lineNum);
		errorFound = TRUE;
		return;
	}
	/* adds the processed instruction to the list */
	add_instruction(fullInstruction);
} /* end of process_instruction */

/* Returns a pointer to the instruction which correlates to the index received */
instructionEnc *get_instruction_word(int ic)
{
	nodePtr iter; /* node iterator */
	
	iter = instructions_head;
	while (iter != NULL && ic > 0)
	{
		ic--;
		/* When correct node reached */
		if (ic == 0)
			return (instructionEnc *)iter->data;
		iter = iter->next;
	}

	return NULL;
} /* end of get_instruction_word */

/* Clears the instruction list and resets the instrucion counter */
void clear_instruction_list()
{
	clear_list(&instructions_head); /* clears all nodes from instruction list*/
	ic = 0; /* resets instruction counter */
} /* end of clear_instruction_list */

