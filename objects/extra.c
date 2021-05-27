/* extra.c contains extra utility functions to be used in the other files */

#include <string.h>
#include <ctype.h>
#include "cmdInfo.h"
#include "extra.h"

/* Returns true if the value is in the ASCII table (up to 127) */
bool is_ascii(char c)
{
	if ((unsigned)(c) < 0x80)
		return TRUE;
	return FALSE;
} /* end of is_ascii */

/* Read file content and copies it to a buffer, returns a pointer to that buffer */
char *read_file(char name[])
{
	FILE *fp; /* file pointer */
	char *buffer;
	long fsize; /* file size */

	/* Opens the file and checks if successful */
	if ((fp = fopen(name, "r")) == NULL)
	{
		printf("Cannot access file: %s.\n",name);
		errorFound = TRUE;
		return NULL;
	}

	/* Gets file size */
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp) + 1; /* including null terminator */
	rewind(fp);

	buffer = (char *)calloc(fsize, sizeof(char));
	VERIFY_ALLOCATION(buffer); /* verifies memory allocation */
	/* Reads to memory */
	fread(buffer, sizeof(char), fsize, fp);

	/* Error check */
	if (ferror(fp) != 0) {
		printf("Error reading file: %s.",name);
		errorFound = TRUE;
		S_FREE(buffer);
	}
	
	fclose(fp);
	/* Returns pointer to buffer containing the file content */
	return buffer;
} /* end of read_file */

/* Same as strtok, only it also returns a pointer to the buffer if its empty */
char *strtok_no_null(char * str, char const *delim)
{
	static char *source = NULL;
	char *ptr, *token = 0;

	if (str != NULL)
		source = str;

	if (source == NULL)
		return NULL;

	if ((ptr = strpbrk(source, delim)) != NULL) {
		*ptr = 0;
		token = source;
		source = ++ptr;
	}
	else if (*source) 
	{
		token = source;
		source = NULL;
	}
	/* Returns token even if its empty */
	return token;
} /* end of strtok_no_null */

/* Returns the number of words in a string */
int word_count(char *string) {
	int count = 0, flag = 0;
	char *ptr = string;
	
	/* Skips spaces in the beginning of the string */
	while (isspace(*ptr)) 
		ptr++;
	/* Counts words seperated by a space */
	while (*ptr)
	{
		if (!isspace(*ptr)) 
		{
			if (flag == 0) 
			{
				flag = 1;
				count++;
			}
		}
		else 
			flag = 0;
		ptr++;
	}

	return count;
} /* end of word_count */

/* Skips nulls in the beginning of a string and returns a pointer to the first valid char in the string */
char *null_skip(char *string)
{
	while (string[0] == NULL_TERMINATOR || string[0] == SPACE)
		string++;
	return string;
} /* end of null_skip */

/* Removes spaces and nulls from the beginning and end of a string */
char *get_word(char *string)
{
	int index = 0;

	/* Removes from the beginning */
	while (!is_ascii(string[0]) || !isgraph(string[0]))
		string++;

	/* Adds a null terminator at the end */
	while (isgraph(string[index]))
		index++;
	string[index] = NULL_TERMINATOR;

	return string;
} /* end of get_word */

/* Saves a buffer to the file */
void save_file(char *buffer, char *path)
{
	FILE *fp;
	
	/* If the buffer is empty */
	if (buffer == NULL)
		return;
	/* Opens file and checks for error */
	if ((fp = fopen(path, "w")) == NULL)
	{
		printf("Cannot save output file to: %s.\n",path);
		errorFound = TRUE;
		return;
	}
	
	fprintf(fp, "%s" ,buffer); /* saves buffer to the file */
	printf("Output file created successfully: %s.\n",path); /* info for user */
	fclose(fp);
} /* end of save_file */

/* Hexadecimal characters array */
static const char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

/* Transforms a word to its hexadecimal representation */
void hex_transform(void *word, char result[4])
{
	hexPartition partition;
	memcpy(&partition, word, sizeof(hexPartition)); /* partitions the word */

	/* Makes 3 char word */
	result[0] = hex[partition.c];
	result[1] = hex[partition.b];
	result[2] = hex[partition.a];
	result[3] = NULL_TERMINATOR;
} /* end of hex_transform */
