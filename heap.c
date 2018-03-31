/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 1 */
/* Date: 15 February, 2018 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include "bst.h"
#include "queue.h"
#include "stack.h"
#include "heap.h"
#include "string.h"

// HEAP definition

struct heap {
	void (*disp)(void *,FILE *);				// display
	int (*compare)(void *,void *);				// compare
	void (*free)(void *);					// free
	int size;						// heap size
	BST *binTree;						// BST
	STACK *ps;						// Parent Stack1
	STACK *a;						// Stack of all nodes
	QUEUE *q;						// Queue
};

/*************** private interface ***************/
static bool isLeaf(BSTNODE *);
static bool minHeapViolations(HEAP *, BSTNODE *);
static BSTNODE *minHeapify(HEAP *,BSTNODE *);
static BST *getHEAPtree(HEAP *);
static QUEUE *getQUEUE(HEAP *);
static STACK *getParentSTACK(HEAP *);
static STACK *getAllSTACK(HEAP *);
static void setHEAPsize(HEAP *,int);
static void heapSwap(BSTNODE *,BSTNODE *);
static void rootSet(HEAP *, BST *, BSTNODE *);
static void childSetLeft(HEAP *, BST *, BSTNODE *, BSTNODE *);
static void childSetRight(HEAP *, BST *, BSTNODE *, BSTNODE *);
static void buildMinHEAP(HEAP *);

/*************** public interface ***************/
/* Heap constructor */
/* This is the constructor for the Heap objects. */

HEAP *newHEAP(
	void (*display)(void *,FILE *),
	int (*comparator)(void *,void*),
	void (*freer)(void *)) {	
	HEAP *h = malloc(sizeof(HEAP));				// Allocate space

	assert(h != 0);	
	h->size = 0;						// size field
	h->disp = display;					// display field
	
	h->compare = comparator; 				// compare field
	h->free = freer;					// free field
	
	h->binTree = newBST(h->disp, 0, 0, h->free);
	h->ps = newSTACK(h->disp, 0);
	h->a = newSTACK(h->disp, 0);
	h->q = newQUEUE(h->disp, 0);

	return h;
}

/* Insert Heap Method */
/* Inserts into heap (un-heapified) */

void insertHEAP(HEAP *h, void *value) {
	assert(value != 0);

	BSTNODE *leaf = newBSTNODE(value);
	BST *t = getHEAPtree(h);
	BSTNODE *r = getBSTroot(t);

	QUEUE *heapQueue = getQUEUE(h);				// Stores values
	STACK *parentStack = getParentSTACK(h);			// Stack of parents
	STACK *allStack = getAllSTACK(h);			// Second parent stack


	if (r == NULL) {					// Insert at root
		rootSet(h, t, leaf);
		enqueue(heapQueue, leaf);
		push(allStack, leaf);
		return;
	}

	else {
		while (1) {
			BSTNODE *cur = peekQUEUE(heapQueue);
			BSTNODE *leftNode = getBSTNODEleft(cur);
			BSTNODE *rightNode = getBSTNODEright(cur);

			if (leftNode == NULL) {			// Left insert
				push(parentStack, cur);
				childSetLeft(h, t, cur, leaf);
				enqueue(heapQueue, leaf);
				push(allStack, leaf); 
				return;
			}

			else if (rightNode == NULL) {		// Right insert
				childSetRight(h, t, cur, leaf);
				enqueue(heapQueue, leaf);
				push(allStack, leaf);
				return;
			}

			else {					// Dequeue
								// Removes stored value
				dequeue(heapQueue);	
			}
		}
	}
	
	return;
}

/* Build Heap Method */
/* Builds heap via max-heapify method */

void buildHEAP(HEAP *h) {
	QUEUE *leafQueue = getQUEUE(h);
	int size = sizeHEAP(h);

	if (size == 0) return;

	if (!(isLeaf(peekQUEUE(leafQueue)))) {
		dequeue(leafQueue);				// Remove last parent
	}

	buildMinHEAP(h);

	return;
}

/* Peek Heap Method */
/* Returns value at root of heap. */
void *peekHEAP(HEAP *h) {
	BST *t = getHEAPtree(h);
	void *rootValue = getBSTNODEvalue(getBSTroot(t));
	
	return rootValue;
}

/* Extract Heap Method */
/* Returns value at root of heap. */
void *extractHEAP(HEAP *h) {
	int size = sizeHEAP(h);
	BST *t = getHEAPtree(h);
	BSTNODE *r = getBSTroot(t);
	BSTNODE *old = 0;
	STACK *allStack = getAllSTACK(h);
	BSTNODE *min = 0;

	assert(size > 0);					// Size check	

	min = getBSTNODEvalue(r);

	heapSwap(r, peekSTACK(allStack));			// Swap with last leaf

	pruneLeafBST(t, peekSTACK(allStack));

	BSTNODE *popped = pop(allStack);			// Pop value

	setHEAPsize(h, sizeHEAP(h) - 1);

	old = minHeapify(h, r);

	while ((!(isLeaf(old))) && minHeapViolations(h, old)) {
		old = minHeapify(h, old);
	}
	
	free(popped);

	return min;
}

/* Size Method */
/* Returns number of values in heap. */

int sizeHEAP(HEAP *h) {
	return h->size;
}

/* Display Method */
/* Calls display method for underlying BST. */

void displayHEAP(HEAP *h,FILE *fp) {
	BST *t = getHEAPtree(h);

	displayBST(t, fp);
	
	return;	
}

/* Display Debug Method */
/* Prints size of heap, size of underlying BST,
and calls debug display method for BST. */

void displayHEAPdebug(HEAP *h, FILE *fp) {
	BST *t = getHEAPtree(h);

	fprintf(fp, "heap size: %d\n", sizeHEAP(h));
	fprintf(fp, "bst size: %d\n", sizeBST(t));
	displayBSTdebug(t, fp);

	return;
}

/* Free Method */
/* Frees the heap by freeing underlying BST. */

void freeHEAP(HEAP *h) {
	BST *t = getHEAPtree(h);
	QUEUE *q = getQUEUE(h);
	STACK *s = getParentSTACK(h);
	STACK *a = getAllSTACK(h);

	freeQUEUE(q);
	freeSTACK(s);
	freeSTACK(a);
	freeBST(t);

	free(h);

	return;
}

/*************** private method definitions ***************/

/* getHEAPtree Method */
/* Returns associated binary tree for heap. */

static BST *getHEAPtree(HEAP *h) {
	BST *tree = h->binTree;

	if (tree == NULL) return NULL;				// Empty tree

	return tree;
}

/* getSTACK Method */
/* Returns first associated Stack for heap. */

static STACK *getParentSTACK(HEAP *h) {
	STACK *ps = h->ps;

	if (ps == NULL) return NULL;
	
	return ps;
}

/* Second getSTACK Method */
/* Returns second associated Stack for heap. */

static STACK *getAllSTACK(HEAP *h) {
	STACK *a = h->a;

	if (a == NULL) return NULL;
	
	return a;
}

/* getQUEUE Method */
/* Returns associated Queue for heap. */

static QUEUE *getQUEUE(HEAP *h) {
	QUEUE *q = h->q;

	if (q == NULL) return NULL;

	return q;
}

/* setHEAPsize Method */
/* Sets the HEAP size. */

static void setHEAPsize(HEAP *h, int s) {
	h->size = s;	
	return;
}

/* childSetLeft Method */
/* Sets left child of node. */

static void childSetLeft(HEAP *h, BST *t, BSTNODE *c, BSTNODE *l) {
	setBSTNODEleft(c, l);
	setBSTNODEparent(l, c);
	setBSTsize(t, sizeBST(t) + 1);
	setHEAPsize(h, sizeBST(t));
	
	return;
}

/* childSetRight Method */
/* Sets right child of node. */

static void childSetRight(HEAP *h, BST *t, BSTNODE *c, BSTNODE *l) {
	setBSTNODEright(c, l);
	setBSTNODEparent(l, c);
	setBSTsize(t, sizeBST(t) + 1);
	setHEAPsize(h, sizeBST(t));
	
	return;
}

/* rootSet Method */
/* Sets root of Heap's BST. */

static void rootSet(HEAP *h, BST *t, BSTNODE *l) {
	setBSTroot(t, l);
	setBSTsize(t, sizeBST(t) + 1);
	setHEAPsize(h, sizeBST(t));

	return;
}

/* minHeapify Method */
/* Creates a min heap using comparisons. */

static BSTNODE *minHeapify(HEAP *h, BSTNODE *c) {
	BSTNODE *smallest = 0;
	BSTNODE *leftChild = getBSTNODEleft(c);
	BSTNODE *rightChild = getBSTNODEright(c);
	void *lvalue = 0;
	void *rvalue = 0;
	void *smallestValue = 0;
	void *cvalue = getBSTNODEvalue(c);

	if (leftChild != NULL) {				// Set left value
		lvalue = getBSTNODEvalue(leftChild);
	}

	if (rightChild != NULL) {				// Set right value
		rvalue = getBSTNODEvalue(rightChild);
	}

	if (lvalue != NULL) {
		if (h->compare(lvalue, cvalue) < 0) {		// Set smallest left
			smallest = leftChild;
		}
		
		else {						// Set smallest cur
			smallest = c;
		}
		
		smallestValue = getBSTNODEvalue(smallest);
	}

	if (rvalue != NULL) {
		if (h->compare(rvalue, smallestValue) < 0) {	// Set smallest right
			smallest = rightChild;
		}
		
		smallestValue = getBSTNODEvalue(smallest);
	}

	if ((lvalue == NULL) && (rvalue == NULL)) {
		smallest = c;

		smallestValue = getBSTNODEvalue(smallest);
	}

	if (smallest != c) {					// Recursive call
		heapSwap(c, smallest);
	}


	return smallest;
}

/* heapSwap Method */
/* Swaps two node values. */

static void heapSwap(BSTNODE *node1, BSTNODE *node2) {
	if ((node1 == NULL) || (node2 == NULL)) {
		return;
	}
	
	void *value1 = getBSTNODEvalue(node1);
	void *value2 = getBSTNODEvalue(node2);

	setBSTNODEvalue(node2, value1);
	setBSTNODEvalue(node1, value2);

	return;
}

/* isLeaf Method */
/* Checks if node is leaf. */

static bool isLeaf(BSTNODE *node) {
	bool leaf = false;
	BSTNODE *leftChild = getBSTNODEleft(node);
	BSTNODE *rightChild = getBSTNODEright(node);

	if ((leftChild == NULL) && (rightChild == NULL)) {
		leaf = true;
	}

	return leaf;
}

/* buildMinHeap Method */
/* Builds a Min Heap. */

static void buildMinHEAP(HEAP *h) {
	STACK *parentStack = getParentSTACK(h);
	BSTNODE *old = 0;

        while (sizeSTACK(parentStack) > 0) {
		BSTNODE *cur = pop(parentStack);
        	old = minHeapify(h,cur);
								// Leaf check
								// Min heap violaions?
        	if (!isLeaf(old) && minHeapViolations(h, old)) {
			push(parentStack, old);
         	}
         }

	return;
}

static bool minHeapViolations(HEAP *h, BSTNODE *c) {
	bool heapViolations = false;
	void *lvalue = 0;
	void *rvalue = 0;
	void *cvalue = 0;

	BSTNODE *cLeft = getBSTNODEleft(c);
	BSTNODE *cRight = getBSTNODEright(c);
	
	if (cLeft != NULL) {
		lvalue = getBSTNODEvalue(cLeft);
	}
		
	if (cRight != NULL) {
		rvalue = getBSTNODEvalue(cRight);
	}

	cvalue = getBSTNODEvalue(c);

	if (lvalue != NULL) {
		if ((h->compare(lvalue, cvalue) < 0)) {
			heapViolations = true;
		}
	}

	if (rvalue != NULL) {
		if ((h->compare(rvalue, cvalue) < 0)) {
			heapViolations = true;
		}
	}

	return heapViolations;
}
