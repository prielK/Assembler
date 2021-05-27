/* compile.c creates the output files using the functions in the other files */

#ifndef BASICS_H
	#define BASICS_H
	#include "basics.h"
#endif

#ifndef PROCESSER_H
	#define PROCESSER_H
	#include "processor.h"
#endif

#ifndef EXTRA_H
	#define EXTRA_H
	#include "extra.h"
#endif

int lineNum; /* Line counter*/
bool errorFound; /* Error tracker */

/* Compiles one file, if successful creates output files, otherwise prints errors */
void compile(char file_name[]);
