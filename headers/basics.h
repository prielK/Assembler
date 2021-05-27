/* This file contains declerations for all other files */

/* Constants */
#define BUFFER_SIZE 100 /* Max buffer to process each time */
#define NULL_TERMINATOR 0x00
#define SPACE 0x20
#define TAB 0x09
#define NEW_LINE 0x0a
#define SPACE_STR " "
#define STARTING_ADDRESS 100
#define COMMENT_PREFIX ';'
#define MAX_NAME_LEN 100 /* Max chars in input file name, extension included */
#define MAX_CODE_LINE 81 /* Max chars per line in input file */
#define MAX_SYMBOL_NAME 30 /* Max chars in symbol name*/
#define MAX_NUM 2047 /* Max integer possible to store in 12 bits of signed int*/
#define MIN_NUM -2048 /* Min integer possible to store in 12 bits of signed int*/
#define REGISTER_NUMBER 8

/* File extensions */
#define INPUT_EXT "as"
#define OBJECT_EXT "ob" 
#define EXTERN_EXT "ext"
#define ENTRIES_EXT "ent"

/* Macros */
#define S_FREE(p_buffer) if(p_buffer != NULL) free(p_buffer); p_buffer = NULL;
#define VERIFY_ALLOCATION(buffer) if (#buffer == NULL) {printf("Cannot allocate memory for this operation.\n"); exit(1); }

/* Represents a boolean */
typedef enum 
{
	FALSE,
	TRUE
} bool;

extern int lineNum;	/* line number to show in errors */
extern bool errorFound;	/* to prevent output file creation if an error was found */


