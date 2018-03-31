/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 1 */
/* 15 February, 2018 */

/* This is the test file for the bst header file for assignment 1.
The data types that were used as tests consisted of doubles, ints, chars, etc,
where these types came from user-defined classes. */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "bst.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "queue.h"

static void showItems(BST *items) {
	printf("The items are ");
	displayBST(items, stdout);				// Calls displayBST method
	printf(".\n");
	displayBSTdebug(items, stdout);
	statisticsBST(items, stdout);
	return;
}

static void sizeCall(BST *items) {
	printf("sizeBST method call: ");
	int size = sizeBST(items);				// Calls sizeBST method
	printf("%d\n", size);

	return;
}

int main(int argc, char **argv) {
	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);	// Error if argc != 1
		exit(1);
	}
								// Creates BST object
	BST *items = newBST(displayINTEGER, compareINTEGER, 0, freeINTEGER);
	showItems(items);					// Prints empty details
	sizeCall(items);
	printf("insertBST call(5, 3, and 2): ");
	insertBST(items, newINTEGER(16));			// Inserts 16
	insertBST(items, newINTEGER(15));			// Inserts 15
	insertBST(items, newINTEGER(27));			// Inserts 27
	printf("\n");						// Prints details
	sizeCall(items);
	showItems(items);
	
	printf("insertBST call (12, 25, 29, 9, 14, 23, 1, 13, 20, 24, and 0): ");
	insertBST(items, newINTEGER(12));			// Inserts 12
	INTEGER *y = newINTEGER(25);				// Insert 25
	insertBST(items, y);					// Inserts etc
	insertBST(items, newINTEGER(29));			
	insertBST(items, newINTEGER(9));
	insertBST(items, newINTEGER(14));
	INTEGER *z = newINTEGER(23);
	insertBST(items, z);
	insertBST(items, newINTEGER(1));
	insertBST(items, newINTEGER(13));
	insertBST(items, newINTEGER(20));
	INTEGER *x = newINTEGER(24);
	insertBST(items, x);
	insertBST(items, newINTEGER(0));
	printf("\n");
	sizeCall(items);
	showItems(items);
	printf("Searching for 24:\n");				// test findBST method (a=24)
	BSTNODE *node = findBST(items, x);
	INTEGER *a = getBSTNODEvalue(node);
	displayINTEGER(a, stdout);
	fprintf(stdout, " was found!\n");
	showItems(items);

	fprintf(stdout, "Deleting 24...\n");
	BSTNODE *freedValue = deleteBST(items, x);
	fprintf(stdout, "Value was ");
	displayINTEGER((INTEGER *) getBSTNODEvalue(freedValue), stdout);
	printf("\n");
	freeBSTNODE(freedValue, freeINTEGER);
	showItems(items);
	BSTNODE *node2 = findBST(items, y);
	BSTNODE *p = getBSTNODEparent(node2);
	fprintf(stdout, "Parent of 25 is ");
	displayINTEGER((INTEGER *) getBSTNODEvalue(p), stdout);
	printf("\n");
	BSTNODE *node3 = findBST(items, z);
	BSTNODE *rc = getBSTNODEright(node3);
	fprintf(stdout, "Right child of 23 is now ");
	
	if (rc == NULL) {
		fprintf(stdout, "empty.\n");
	}

	fprintf(stdout, "Deleting 25...\n");
	BSTNODE *freedValue2 = deleteBST(items, y);
	fprintf(stdout, "Value was ");
	if (freedValue2 == NULL) {
		printf("Well crap...\n");
	}

	displayINTEGER((INTEGER *) getBSTNODEvalue(freedValue2), stdout);
	printf("\n");
	showItems(items);
	freeBSTNODE(freedValue2, freeINTEGER);
	freeBST(items);
	 
	return 0;
}



