/* compile.c creates the output files */

#include <string.h>
#include "compile.h"

/* Generates and saves the output files */
bool create_files();

/* Initializes all data structures and variables */
void initialize(char *file_name);

/* File paths */
static char input_file_name[MAX_NAME_LEN];
static char ob_file_name[MAX_NAME_LEN];
static char ent_file_name[MAX_NAME_LEN];
static char ext_file_name[MAX_NAME_LEN];

/* Compiles one file, if successful creates output files, otherwise prints errors */
void compile(char file_name[])
{
	char *buffer;

	initialize(file_name);

	printf("Starting to process the file: %s\r\n", input_file_name); /* Info for user */
	
	/* If buffer wasn't created */
	if ((buffer = read_file(input_file_name)) == NULL)
	{
		return;
	}

	/* Start compilation process */
	if (errorFound == FALSE)
	{
		first_transition(buffer);
		second_transition();
	}

	/* Generate code and save the output if no errors were detected */
	if (errorFound == FALSE)
	{
		create_files();
	}

	/* Info for user */
	if (errorFound == FALSE)
		printf("* Files compiled successfully! *\n\n");
	else
		printf("X Errors found, file compilation unsuccessful. X\n\n");
} /* end of compile */

/* Generates and saves the output files */
bool create_files()
{
	char *object; /* for .ob file*/
	char *enrties; /* for .ent file */
	char *externals; /* for .ext file */

	/* Generates the content of the output files */
	object = ob_content();
	externals = ext_content();
	enrties = globalBufferENT;

	/* If object doesn't have content clear buffers */
	if (object == NULL)
	{
		S_FREE(object);
		S_FREE(externals);
		S_FREE(enrties);
		printf("Internal error - cannot generate code.\n");
	
		return FALSE;
	}

	/* Saves the content into the files */
	save_file(object, ob_file_name);
	save_file(externals, ext_file_name);
	save_file(enrties, ent_file_name);

	/* Clears buffers */
	S_FREE(object);
	S_FREE(externals);
	S_FREE(enrties);

	/* Checks if the file saved successfully */
	if (errorFound == TRUE)
		return FALSE;

	return TRUE;
} /* end of create_files */

/* Initializes all data structures and variables */
void initialize(char *file_name)
{
	int nameLength;	/* input file name length */
	int extName, entName;	/* externals and entries file name length */
	int inputName, obName; /* input file and ob file name length */

	clear_all(); /* clears all lists (instruction,data,symbol) */

	/* Sets files name length */
	nameLength = strlen(file_name) + 1; /* +1 for '.' */
	inputName = nameLength + strlen(INPUT_EXT) + 1; /* +1 for null terminator */
	obName = nameLength + strlen(OBJECT_EXT) + 1; /* +1 for null terminator */
	extName = nameLength + strlen(EXTERN_EXT) + 1; /* +1 for null terminator */
	entName = nameLength + strlen(ENTRIES_EXT) + 1; /* +1 for null terminator */

	/* Checks name limits */
	if (obName > MAX_NAME_LEN ||
		extName > MAX_NAME_LEN ||
		entName > MAX_NAME_LEN ||
		inputName > MAX_NAME_LEN)
	{
		printf("Can't compile, file name is too long.\n");
		errorFound = TRUE;
		return;
	}

	/* Sets output file names */
	sprintf(input_file_name, "%s.%s", file_name, INPUT_EXT);
	sprintf(ob_file_name, "%s.%s", file_name, OBJECT_EXT);
	sprintf(ext_file_name, "%s.%s", file_name, EXTERN_EXT);
	sprintf(ent_file_name, "%s.%s", file_name, ENTRIES_EXT);
} /* end of initialize */
