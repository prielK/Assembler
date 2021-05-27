/* 

This program represents an assembler - it receives a file with the extension .as and after processing it 3 files are created with the extensions .ob (the decoded file in "machine" language, .ent (all .entry labels listed with their line number) and .ext (all .extern labels listed with their line number).
main.c starts the assembler's process by sending each arg received to the compiler.

*/

#include <stdio.h>
#include "compile.h"

/* Start of running */
int main(int argc, char *argv[]) 
{
	int fileNum; /* arg index */
	
	/* If no filename received */
	if (argc < 2)
	{
		printf("No input file received.");
		exit(1);
	}
	
	/* Sends all files to compile */
	for (fileNum = 1; fileNum < argc; fileNum++)
	{
		lineNum = 0;
		errorFound = FALSE;
		compile(argv[fileNum]);
	}
	
	exit(0);
} /* end of main */

