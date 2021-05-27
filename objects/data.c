/* data.c contains the data processing functions */

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "data.h"

/* Adds data node to the list */
void add_data_node(dataWord word)
{
	nodeRef head_ref = &dataHead;
	add_node(head_ref, &word, sizeof(dataWord));
	dc++; /* data counter */
} /* end of add_data_node */


/* Checks for valid syntax and returns the data value */
int data_atoi(char *data)
{
	int num; /* data value */
	float check; /* to check if data is integer */

	/* If no data or delimiter */
	if (!*data || (word_count(data) != 1))
	{
		printf("(In line %d) Syntax error: no data or no delimiter found.\n",lineNum);
		errorFound = TRUE;
		return 0;
	}
	
	check = atof(data);	/* converts to float */
	num = atoi(data);	/* converts to int */

	/* Checks if data is an integer */
	if ((num == 0 && *data != '0') || (check != num))
	{
		printf("(In line %d) Syntax error: value is not an integer.\n",lineNum);
		errorFound = TRUE;
		return 0;
	}

	/* Checks if value exceeds limits */
	if (num > MAX_NUM || num < MIN_NUM)
	{
		printf("(In line: %d) The number is smaller than the minimum or larger than the maximum possible.\n",lineNum);
		errorFound = TRUE;
		return 0;
	}
	/* Value output */	
	return num;
} /* end of data_atoi */

/* Processes data words and adds them to the list, returns the amount of words added */
int process_data(char *data)
{
	int value, wordCount = 0;
	char *element; /* points to each data element at time */
	dataWord newWord; /* temp word to add */
	const char delimiter[2] = DATA_DELIMITER;

	/* Each data word */
	element = strtok_no_null(data, delimiter);
	
	/* Add all valid data words */
	while (element != NULL)
	{
		value = data_atoi(element); /* converts to num */

		/* adds the data if no errors were raised */
		if(!errorFound)
		{
			newWord.data = value;
			add_data_node(newWord);
			wordCount++;
		}

		element = strtok_no_null(NULL, delimiter);
	}

	return wordCount; /* amount of words added */
} /* end of process_data */

/* Processes string to dataWord and adds it to the list */
bool process_string(char *str)
{
	dataWord newWord;
	int index = 0, quotationStart=0, quotationEnd=0;
	char current;
	
	/* Skips blank chars in the beginning of the string */
	while(!isgraph(str[quotationStart]))
	{
		quotationStart++;
	}
	/* If the string doesn't start with a quotation mark */
	if(str[quotationStart]!='\"')
	{
		printf("(In line: %d) String must start with a quotation mark.\n",lineNum);
		errorFound = TRUE;
		return FALSE;
	}
	else
	{
		quotationEnd = quotationStart + 1;
		while (str[quotationEnd]!='\"' && str[quotationEnd]!='\0')
			quotationEnd++;
		/* If the string doesn't end with a quotation */
		if(str[quotationEnd]!='\"')
		{
			printf("(In line: %d) String must end with a quotation mark.\n",lineNum);
			errorFound = TRUE;
			return FALSE;
		}
		
		index = quotationEnd + 1;
		/* If any non-blank char after string */
		while (str[index] != '\0')
		{
			if (isgraph(str[index]))
			{
				printf("(In line: %d) Illigal string command - extra text after quotations.\n",lineNum);
				errorFound = TRUE;
				return FALSE;
			}
			index++;
		}
	}

	/* Adds characters inside quotation marks */
	while ((++quotationStart) != quotationEnd)
	{
		current = str[quotationStart];

		/* Ascii check */
		if (!is_ascii(current))
		{
			printf("String must contain only ascii characters.\n");
			errorFound = TRUE;
			return FALSE;
		}

		/* Adds character node to the data list */
		newWord.data = current;
		add_data_node(newWord);
	}

	/* Adds null terminator */
	newWord.data = 0;
	add_data_node(newWord);

	return TRUE;
} /* end of process_string */

/* Clears the data list and resets the data counter */
void clear_data_list()
{
	nodePtr iter, nodeToDelete;
	/* If there are no data words */
	if (dataHead == NULL)
		return;

	/* Clears buffers */
	iter = dataHead;
	while (iter != NULL)
	{
		nodeToDelete = iter;
		iter = iter->next;

		DELETE_NODE(nodeToDelete);
	}

	dataHead = NULL; /* clears pointer */
	dc = 0; /* resets data counter */
} /* end of clear_data_list */


