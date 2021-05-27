/* processer.c contains the processing functions */

#include <string.h>
#include <ctype.h>
#include "processor.h"

char *globalBufferENT; /* for copying the symbol addresses to .ob file */

/* Checks if word is a symbol */
bool is_symbol(char word[MAX_SYMBOL_NAME])
{
	if (word[strlen(word) - 1] != ':')
		return FALSE;
	return TRUE;
} /* end of is_symbol */

/* Checks if word is of data type */
bool is_data(char *word)
{
	return (bool)(strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0);
} /* end of is_data */

/* Gets data type from line and adds the data to the list */
void process_data_type(char *line)
{
	int len; /* how many characters to skip */
	char *word;	/* word iterator in the line */
	
	word = get_word(strtok(line, SPACE_STR));
	/* if string instruction */
	if (strcmp(word, ".string") == 0)
	{
		len = strlen(".string") + 1;
		process_string(&line[len]);
		return;
	}
	/* if data instruction */
	if (strcmp(word, ".data") == 0)
	{
		len = strlen(".data") + 1;
		process_data(&line[len]);
		return;
	}
} /* end of process_data_type */

/* Removes comments and double spaces from a line and promotes the buffer */
char *pre_process(char **bufferPtr)
{
	char *buffer, *iter, *line, current;
	int offset = 0;
	bool spaceFound = FALSE, textFound = FALSE, commentFound = FALSE;

	buffer = iter = *bufferPtr;
	line = (char *)calloc(MAX_CODE_LINE + 1, sizeof(char)); /* sets buffer */
	VERIFY_ALLOCATION(line); /* verifies memory allocation */
	
	/* If empty buffer */
	if (*buffer == NULL_TERMINATOR)
		return NULL;
	/* If comment detected */
	if (*buffer == COMMENT_PREFIX)
		commentFound = TRUE;
	/* For the whole line */
	while (*(iter = buffer++) != NEW_LINE)
	{
		current = *iter;
		
		/* If readable */
		if (!is_ascii(*iter))
			continue;

		/* If EOF reached */
		if (current == NULL_TERMINATOR)
			break;

		/* Skips double spaces and tabs */
		if (current == SPACE || current == TAB)
		{
			if (spaceFound)
			{
				*bufferPtr += 1; /* skips space in buffer */
				continue;
			}
			else
			{
				current = SPACE;
				spaceFound = TRUE;
			}
		}
		else {
			spaceFound = FALSE;
			textFound = TRUE;
		}

		offset++;
		/* Line length check */
		if (offset > MAX_CODE_LINE)
		{
			printf("(In line: %d) Line is too long.\n",lineNum);
			errorFound = TRUE;
			return (void *)-1;
		}
		line[offset - 1] = current; /* reads character */
	}

	*bufferPtr += offset + 1; /* promotes the buffer, +1 for \n */

	/* Skips empty lines and comments */
	if (textFound && !commentFound)
	{
		line[offset] = NULL_TERMINATOR;
		return line;
	}
	/* Clears buffer */
	S_FREE(line);
	
	return (void *)-1;
} /* end of pre_process */

/* Processes line */
void process_line(char *line) 
{
	bool has_symbol = FALSE;
	int index, length = 0, comma = 0;
	char symbol_name[MAX_SYMBOL_NAME];
	char *word; /* words iterator in line */
	
	/* Checks for double commas*/
   	for (index = 0 ; index < strlen(line) ; index++)
    {
    	if (isgraph(line[index])) /* if not blank char*/
    		length++;
		/* If current char is astrix */
    	if (line[index] == '"')
    	{
    		index++;
			/* Skips double comma check for text between astrixes (for .string) */
    		while ((line[index]!='"') && (index<strlen(line)))
    			index++;
    		if (line[index] != '"')
    		{
    			printf("(In line: %d) Invalid use of astrix.",lineNum);
    			errorFound = TRUE;
    		}
    		else
    			index++;
    	}
    	if (comma != 0 && line[index]!=',' && line[index]!='\t' && line[index]!=' ')
    		comma--;
		
      	if (line[index]==',')
      		comma++;
		/* If double comma */
      	if (comma > 1)
		{
			printf("(In line: %d) Illigal comma.\n",lineNum);
			errorFound = TRUE;
			return;
		}
    }
    /* If last char in line is a comma*/
    if (comma > 0)
	{
		printf("(In line: %d) Command can't end with a comma.\n",lineNum);
		errorFound = TRUE;
		return;
	}
		
	index = 0; /* reset for new use */
	word = get_word(strtok(line, SPACE_STR));
	
	/* Checks if the word is a symbol */
	if (is_symbol(word))
	{
		length-=strlen(word);
		index += strlen(word) + 1; /* updates instruction's address index */
		has_symbol = TRUE;

		/* Valid name check */
		word[index - 2] = NULL_TERMINATOR; /* remove ':' */
		if (!valid_symbol_name(word))
			return;

		strcpy(symbol_name, word);
		word = get_word(strtok(NULL, SPACE_STR)); /* gets the next word */
	}
	
	/* Checks if the word is data */
	if (is_data(word))
	{
		if (has_symbol)
			add_symbol(&symbolsHead, dc + 1, symbol_name, DATA, TRUE);

		/* Detects data type and adds the word to the list */
		process_data_type(word);
		return;
	}
	
	/* Checks if the word is .entry */
	if (strcmp(word, ".entry") == 0)
	{
		word = strtok(NULL, SPACE_STR);
		if (valid_symbol_name(word))
		{
			add_symbol(&symbolsHead, 0, word, ENTRY, TRUE);
			add_symbol(&entryHead, 0, word, ENTRY, TRUE);
		}
		return;
	}
	
	/* Checks if the word is .extern */
	if (strcmp(word, ".extern") == 0)
	{
		/* Adds to the symbol list with external type */
		word = strtok(NULL, SPACE_STR); 
		if (valid_symbol_name(word))
			add_symbol(&symbolsHead, 0, word, EXTERNAL, TRUE);
		return;
	}
	
	/* If the line contains a symbol add it to the symbol list */
	if (has_symbol)
	{
		add_symbol(&symbolsHead, ic + 1, symbol_name, CODE, TRUE);
	}
	
	process_instruction(word,length);
} /* end of process_line */

/* Processes a list of nodes and returns a buffer which contains the list's processed content */
char *generate_content(nodePtr *head, int address)
{
	defNode currNode;
	nodePtr temp = NULL;
	wordBuff holder;
	instructionEnc nodeEnc;
	int length = 0, currentBlock = 1;
	char encoding, *buffer = NULL;
	char hexForm[4];
	char line[MAX_CODE_LINE];
	
	buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
	VERIFY_ALLOCATION(buffer); /* verifies memory allocation */
	
	/* For all nodes in the list */
	while (*head != NULL)
	{
		temp = *head;
		memcpy(&holder, temp->data, sizeof(wordBuff));
		nodeEnc.data = holder.data;
		nodeEnc.encoding = holder.data>>22; /* sets encoding */
		currNode = pop_node(head);
		hex_transform(currNode.data, hexForm); /* gets hex form of the word */
		
		/* Sets encoding letter to be added to the file content */
		if (nodeEnc.encoding==EXTERN)
			encoding='E';
		else if (nodeEnc.encoding==RELOCATABLE)
			encoding='R';
		else 
			encoding='A';
			
		sprintf(line, "%04d %s %c\n", address, &hexForm, encoding); /* creates content line */

		/* Buffer size check */
		if ((length + strlen(line)) >= (BUFFER_SIZE * currentBlock))
		{
			currentBlock++;
			buffer = (char *)realloc(buffer, sizeof(char) * BUFFER_SIZE * currentBlock);
			VERIFY_ALLOCATION(buffer); /* verifies memory allocation */
		}

		/* Adds to the buffer */
		address++;
		memcpy(&buffer[length], &line, strlen(line) + 1);
		length = strlen(buffer);
	}
	/* Returns the generated code */
	return buffer;
} /* end of generate_content */

/* Processes each line to code, data and symbol lists */
void first_transition(char *buffer)
{
	char *line;	/* current line */
	char **bufferPtr = &buffer;	/* buffer iterator */

	while ((line = pre_process(bufferPtr)) != NULL)
	{
		lineNum++; /* line counter */
		
		if (line == (void *)-1)
			continue;

		/* Process each line */
		process_line(line);
		/* Clears line buffer*/
		S_FREE(line);
	}
} /* end of first_transition */

/* Adds the symbol references to the processed code */
void second_transition()
{
	symbolNode defSymbol;
	symbolNode *symbolPtr;
	instructionEnc *target;
	int address, encode, absoluteJump, symbolAddress, lineNumber;
	char *codeSymbol, *tempAddress, *entBuff = NULL, *splitBuffer = NULL;
	bool flag = FALSE;

	/* For each symbol */
	while (symLineHead != NULL)
	{
		flag = FALSE;
		address = STARTING_ADDRESS;
		defSymbol = pop_symbol(&symLineHead);
		
		if ((defSymbol.symbol_name)[0] == '%')
		{
			codeSymbol = (defSymbol.symbol_name)+1;
			symbolPtr = get_symbol(symbolsHead, codeSymbol);
		}
		else
			symbolPtr = get_symbol(symbolsHead,defSymbol.symbol_name);
		
		/* Error check */
		if (symbolPtr == NULL)
		{
			printf("Cannot find symbol: %s.\n", defSymbol.symbol_name);
			errorFound = TRUE;
			continue;
		}
		
		/* Sets symbol address by type */
		switch (symbolPtr->type)
		{
		case CODE:
			lineNumber = STARTING_ADDRESS + defSymbol.node_number - 2;
			symbolAddress = STARTING_ADDRESS + symbolPtr->node_number - 1;

			/* If relative number to jump */
			if(defSymbol.symbol_name[0] != '%')
			{
				address = symbolAddress;
				encode = RELOCATABLE;
				break;
			}

			/* If absolute number to jump */
			if(defSymbol.symbol_name[0] == '%') /* incase of code with "%" */
			{
				flag = TRUE; /* absoulte jump happend, so address needs to get absoluteJump value*/
				absoluteJump = symbolAddress - lineNumber - 1;
				encode = ABSOLUTE;
				break;
			}
			
		case DATA:
			address += ic + symbolPtr->node_number - 1;
			encode = RELOCATABLE;
			break;
			
		case EXTERNAL:
			address = 0;
			encode = EXTERN;
			add_symbol(&externHead, defSymbol.node_number, defSymbol.symbol_name, defSymbol.type, FALSE);
			break;

		case ENTRY:
			address = 0;			
			globalBufferENT = ent_content(); /* get string with all information about entry symbols*/
			if(globalBufferENT != NULL)
			{
				entBuff=(char *)calloc((strlen(globalBufferENT)+1),sizeof(char));
				VERIFY_ALLOCATION(entBuff); /* verifies memory allocation */
				strcpy(entBuff, globalBufferENT); /* copy buffer entery symbols to breaker*/
				splitBuffer = (char *)calloc(MAX_CODE_LINE,sizeof(char));
				VERIFY_ALLOCATION(splitBuffer); /* verifies memory allocation */
				splitBuffer = strtok(entBuff,SPACE_STR);
				/* For all words */
				while(splitBuffer != NULL)
				{
					if(strcmp(defSymbol.symbol_name,splitBuffer) == 0)
					{
						splitBuffer = strtok(NULL, SPACE_STR); 
						tempAddress = strtok(splitBuffer, "\n"); /* get address of entry match symbol*/
						address = atoi(tempAddress); /* conver string address to intger*/
						break;
					}
					splitBuffer = strtok(NULL, SPACE_STR); /* split according to get next line */
				}
				
			}
			encode = RELOCATABLE;
			break;
		}

		/* Set changes */
		target = get_instruction_word(defSymbol.node_number);
		/* Error check */
		if (target == NULL)
		{
			printf("Internal error.\n");
			exit(1);
		}
		
		/* For relative jump*/
		if(flag == FALSE)
			target->data = address;

		/* For absolute jump*/
		if(flag == TRUE)
			target->data = absoluteJump;
		
		/* Sets symbol encoding */
		switch (encode)
		{
			case 1:
				target->encoding=EXTERN;
				break;
			case 2:
				target->encoding=RELOCATABLE;
				break;
			case 3:
				target->encoding=ABSOLUTE;
				break;
			default:
				break;		
		}
	}
} /* end of second_transition */

/* Saves object content into buffer, returns a pointer to the buffer */
char *ob_content()
{
	char *combined, *code, *data;	
	char topLine[sizeof(int) * 2]; /* for the first row */

	/* Creates code */
	sprintf(topLine, "\t%d %d\n", ic, dc); /* the first row to print with the counters */
	code = generate_content(&instructions_head, STARTING_ADDRESS); /* intructions content buffer */
	data = generate_content(&dataHead, STARTING_ADDRESS + ic); /* data content buffer */	

	/* Combines to one buffer */
	combined = (char*)calloc(strlen(topLine) + strlen(code) + strlen(data) + 1, sizeof(char));
	VERIFY_ALLOCATION(combined); /* verifies memory allocation */

	strcat(combined, topLine); /* adds the first line to the ob file content*/
	strcat(combined, code); /* adds all code lines to the ob file content*/
	strcat(combined, data); /* adds all data lines to the ob file content*/
	
	/* Clears buffers */
	S_FREE(code);
	S_FREE(data);
	/* Returns object file content */
	return combined;
} /* end of ob_content */

/* Saves entry content into buffer, returns a pointer to the buffer */
char *ent_content()
{
	symbolNode entDec;	/* entry decleration */
	symbolNode *entUse;	/* entry use */
	int address, counter = 1, buffSize = MAX_SYMBOL_NAME + 10;
	char line[MAX_SYMBOL_NAME + 11] = {0};	/* for address and tab */
	char *buffer = NULL;	/* output */
	
	/* If there are no entry symbols */
	if (entryHead == NULL)
		return NULL;

	/* Initializes buffer */
	buffer = (char *)calloc(buffSize, sizeof(char));
	VERIFY_ALLOCATION(buffer); /* verifies memory allocation */

	/* For each entry decleration */
	while (entryHead != NULL)
	{
		entDec = pop_symbol(&entryHead); /* pops node from entry symbol list */

		/* For each use of decleration */
		while ((entUse = get_symbol(symbolsHead, entDec.symbol_name)) != NULL)
		{
			address = STARTING_ADDRESS; /* starting address value */
			
			/* Skips declerations */
			if (entUse->node_number == 0)
			{
				remove_symbol(entUse);
				continue;
			}

			/* Calculates symbol address */
			if (entUse->type == DATA)
				address += ic + entUse->node_number - 1;
			if (entUse->type == CODE)
				address += entUse->node_number - 1;

			sprintf(line, "%s %04d\n",entUse->symbol_name, address); /* creates content line */

			/* Buffer size check */
			if (strlen(buffer) + strlen(line) >= counter * buffSize)
			{
				counter++;
				buffer = (char *)realloc(buffer, counter * buffSize * sizeof(char));
				VERIFY_ALLOCATION(buffer); /* verifies memory allocation */
			}
			strcat(buffer, line); /* adds line to the file content*/
			
			/* Removes use from list */
			remove_symbol(entUse);
		}	
	}
	/* Returns ent file content */
	return buffer;
} /* end of ent_content */

/* Saves extern content into buffer, returns a pointer to the buffer */
char *ext_content()
{
	symbolNode extDec;
	int counter = 1;
	char line[MAX_SYMBOL_NAME + 101];	/* for address and tab */
	char *buffer = NULL; /* output */

	/* If there are no extern symbols */
	if (externHead == NULL)
		return NULL;

	/* initialize buffer */
	buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
	VERIFY_ALLOCATION(buffer); /* verifies memory allocation */
	
	/* Adds all extern symbols to the ext file content */
	while (externHead != NULL)
	{
		extDec = pop_symbol(&externHead); /* pops node from extern symbol list */
		sprintf(line, "%s %04d\n", extDec.symbol_name, extDec.node_number -1 + STARTING_ADDRESS); /* creates content line */

		/* Buffer size check */
		if (strlen(buffer) + strlen(line) >= BUFFER_SIZE * counter)
		{
			counter++;
			buffer = (char *)realloc(buffer, BUFFER_SIZE * counter);
			VERIFY_ALLOCATION(buffer); /* verifies memory allocation */
		}
		strcat(buffer, line); /* adds line to the file content*/
	}
	/* returns ext file content */
	return buffer;
} /* end of ext_content */

/* Clears all instructions, data and symbols */
void clear_all()
{
	clear_data_list(); 
	clear_instruction_list();
	clear_symbol_list();
} /* end of clear_all */
