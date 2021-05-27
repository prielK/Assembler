/* extra.c contains extra utility functions to be used in the other files */

#include <stdlib.h>
#include <stdio.h>

#ifndef BASICS_H
	#define BASICS_H
	#include "basics.h"
#endif

bool errorFound;	/* Error tracker */

/* Returns true if the value is in the ASCII table (up to 127) */
bool is_ascii(char c);

/* Read file content and copies it to a buffer, returns a pointer to that buffer */
char *read_file(char name[]); 

/* Same as strtok, only it also returns a pointer to the buffer if its empty */
char *strtok_no_null(char * str, char const *delim);

/*  Returns the number of words in a string */
int word_count(char *string);

/* Skips nulls in the beginning of a string and returns a pointer to the first valid char in the string */
char *null_skip(char *string);

/* Removes spaces and nulls from the beginning and end of a string */
char *get_word(char *string);

/* Saves a buffer to the file */
void save_file(char *buffer, char *path);

/* Transforms a word to its hexadecimal representation */
void hex_transform(void *word, char *result);

