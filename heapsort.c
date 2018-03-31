/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 1 */
/* Date 15 February, 2018 */

/* This is the main file for implementing heapsort. */

// HEAPSORT.C

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include "stack.h"
#include "queue.h"
#include "bst.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "scanner.h"
#include "heap.h"

/* option flags */
int Author = 0;							// option -v
int Sorting = 0;						// Sort type
int Order = 1;							// Sort order (default increase)
char* Name = 0;							// File name

/*int integer = 0;						// option -i
int real = 0;							// option -r
int s = 0;							// option -s
int Increase = 0;						// option -I
int Decrease = 0;						// option -D
int Sorting = 0;*/

#define REALS 1							// -r macro
#define STRINGS 2						// strings macro
#define INTEGERS 3						// integers macro
#define INCREASING 1						// increasing macro

/*************** Private Methods ***************/
static int processOptions(int,char **);
void Fatal(char *,...);
static void (*reader)(FILE *,HEAP *);				// reader function pointer
static int (*compare)(void *,void *);				// compare function pointer
static void (*display)(void *,FILE *);				// display function pointer
static void (*release)(void *);					// free function pointer
static void (*reader)(FILE *,HEAP *);
static void readReals(FILE *,HEAP *);
static void readStringsAndTokens(FILE *,HEAP *);
static void readIntegers(FILE *,HEAP *);

/*************** Main ***************/

int main(int argc, char **argv) {
	
/*	QUEUE *final = 0;					// readIn QUEUE
	STACK *readOut = 0;					// readOut STACK
	HEAP *heap = 0;						// HEAP
	int x = 0;
	double y = 0;
	char *z;*/
	
	if (argc == 1) Fatal("%d arguments!\n", argc-1);	// Error argc=1

	if (argc < 2) {
		fprintf(stderr, "usage: %s\n", argv[0]);	// Error argc<2
		exit(1);
	}

	processOptions(argc,argv);

	if ((argc == 2) && !(Author)) {
		FILE *fp = fopen(Name, "r");

		if (fp == NULL) {
			fprintf(stderr, "Error. File %s does not exist.\n", Name);
			exit(1);
		}
		
		reader = readIntegers;
		release = freeINTEGER;
		display = displayINTEGER;
		compare = Order == INCREASING? compareINTEGER : rcompareINTEGER;

		HEAP *h = newHEAP(display, compare, release);
		reader(fp, h);
		fclose(fp);

		buildHEAP(h);

		QUEUE *q = newQUEUE(display, release);

		while (sizeHEAP(h) > 0) {
			enqueue(q, extractHEAP(h));
		}

		while (sizeQUEUE(q) > 0) {
			display(dequeue(q), stdout);

			if (sizeQUEUE(q) > 0) {
				fprintf(stdout, " ");
			}
		}
		fprintf(stdout, "\n");

		return 0;
	}

	if ((strcmp(Name, "") != 0) && (Name[0] != '-')) {
		FILE *fp = fopen(Name, "r");

		if ((fp == NULL) && (Author == 0)) {
			fprintf(stderr, "Error. File %s does not exist.\n", Name);
			exit(1);
		}
	
		if (Sorting == REALS) {				// Read reals
			reader = readReals;
			release = freeREAL;
			display = displayREAL;
			compare = Order == INCREASING? compareREAL : rcompareREAL;
		}

		else if (Sorting == STRINGS) {			// Read strings
			reader = readStringsAndTokens;
			release = freeSTRING;
			display = displaySTRING;
			compare = Order == INCREASING? compareSTRING : rcompareSTRING;
		}

		else {						// Default read
			reader = readIntegers;
			release = freeINTEGER;
			display = displayINTEGER;
			compare = Order == INCREASING? compareINTEGER : rcompareINTEGER;
		}


		HEAP *h = newHEAP(display, compare, release);
		reader(fp, h);					// Read in tokens
		fclose(fp);

		buildHEAP(h);

		QUEUE *q = newQUEUE(display, release);
		
		while (sizeHEAP(h) > 0) {
			enqueue(q, extractHEAP(h));
		}

		while (sizeQUEUE(q) > 0) {
			display(dequeue(q), stdout);
			
			if (sizeQUEUE(q) > 0) {
				fprintf(stdout, " ");
			} 
		}
		fprintf(stdout, "\n");
	}

	// Print following if appropriate flags are set

	if (Author) {
		printf("Written by Alan Manning\n");
		printf("My heapsort works in n log n time because\n");
		printf("the insertion of values into the heap takes\n");
		printf("constant time, the ordering of the heap itself\n");
		printf("runs in linear time, and the extraction of the\n");
		printf("min or max value occurs in log n time.\n");
		return 0;
	}

	return 0;
}

void Fatal(char *fmt, ...) {
	va_list ap;

	fprintf(stderr, "An error occured: ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	exit(-1);

	return;
}

/* only -oXXX  or -o XXX options */

static int processOptions(int argc, char **argv) {
        int argIndex;
        int argUsed;
        int separateArg;
        int printArg;

        argIndex = 1;

        while (argIndex < argc && *argv[argIndex] == '-') {
                /* check if stdin, represented by "-" is an argument */
                /* if so, the end of options has been reached */
                if (argv[argIndex][1] == '\0') {
                        return argIndex;
                }

                separateArg = 0;
                argUsed = 0;
                printArg = 0;

                if (argv[argIndex][2] == '\0') {
                        separateArg = 1;
                }

                switch (argv[argIndex][1]) {
                        /*
                        * when option has an argument, do this
                        *
                        *     examples are -m4096 or -m 4096
                        *
                        *     case 'm':
                        *         MemorySize = atol(arg);
                        *         argUsed = 1;
                        *         break;
                        *
                        *
                        * when option does not have an argument, do this
                        *
                        *     example is -a
                        *
                        *     case 'a':
                        *         PrintActions = 1;
                        *         break;
                        */

                        case 'i':
				Sorting = INTEGERS;				
                                argUsed = 1;
                                break;
                        case 'v':
                                Author = 1;
                                printArg = 1;
                                break;
                        case 'r':
				Sorting = REALS;
                                argUsed = 1;
                                break;
                        case 's':
				Sorting = STRINGS;
                                argUsed = 1;
                                break;
			case 'I':
				Order = INCREASING;
				argUsed = 1;
				break;
			case 'D':
				Order = !INCREASING;
				argUsed = 1;
				break;
                        default:
                                Fatal("option %s not understood\n",argv[argIndex]);
                }

                if (printArg) {
                        ++argIndex;
                }

                if (separateArg && argUsed) {
                        ++argIndex;
                }

        }

        if (argIndex == argc) {
                Name = malloc(sizeof(char) * strlen(argv[argIndex - 1]));
                strcpy(Name, argv[argIndex - 1]);
        }

        else {
                Name = malloc(sizeof(char) *strlen(argv[argIndex]));
                strcpy(Name, argv[argIndex]);
        }

        return argIndex;
}

/* readReals Method */
/* Reads reals from a given fp. */

static void readReals(FILE *fp, HEAP *h) {	
	double x = 0;

	x = readReal(fp);
	while (!feof(fp)) {
		insertHEAP(h, newREAL(x));
		x = readReal(fp);
	}

	return;
}

/* readStringsAndTokens Method */
/* Reads strings or tokens from a given fp. */

static void readStringsAndTokens(FILE *fp, HEAP *h) {
	char *x = 0;
	
	if (stringPending(fp)) {
		x = readString(fp);
	}
	
	else {
		x = readToken(fp);
	}

	while (!feof(fp)) {
		insertHEAP(h, newSTRING(x));

		if (stringPending(fp)) {
			x = readString(fp);
		}
	
		else {
			x = readToken(fp);
		}
	}	

	return;
}

/* readIntegers Method */
/* Reads integers from a given fp. */

static void readIntegers(FILE *fp, HEAP *h) {
	int x = 0;
	
	x = readInt(fp);
	while (!feof(fp)) {
		insertHEAP(h, newINTEGER(x));
		x = readInt(fp);
	}

	return;
}
