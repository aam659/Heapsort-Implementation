/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 1 */
/* Date: 15 February, 2018 */

/*This is the implementation file for the header file "bst.h",
which is used to create a binary search tree 'class' for
generic data types.*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include "bst.h"
#include "queue.h"

// NODE Definition

struct bstnode {
	BSTNODE *left, *right, *parent;				// node pointers
	void *value;						// node value
};

struct bst {
	void (*disp)(void *,FILE *);				// display field
	int (*compare)(void *, void *);				// comparator method for values
	void (*swap)(BSTNODE *,BSTNODE *);			// swapper method for values
	void (*free)(void *);					// free method
	int size;						// tree size
	BSTNODE *root;						// tree root
};

/*************** private interface ***************/
static void preOrder(FILE *, BST *, BSTNODE *);
static void postOrderFree(BST *, BSTNODE *);
static void levelOrder(FILE *, BST *);
static bool hasLeft(BSTNODE *);
static bool hasRight(BSTNODE *);
static bool isLeaf(BSTNODE *);
static bool isLeftChild(BSTNODE *);
static bool isRightChild(BSTNODE *);
static void swapMethod(BSTNODE *,BSTNODE *);
static void levelAdd(BSTNODE *,QUEUE *);
static BSTNODE *findMin(BSTNODE *);
static BSTNODE *findMax(BSTNODE *);
static BSTNODE *findSuccessor(BSTNODE *);
static BSTNODE *findPredecessor(BSTNODE *);
static int findMinDepth(BSTNODE *);
static int findMaxDepth(BSTNODE *);

/*************** BSTNODE public interface ***************/
/* Node constructor */
/* This is the constructor for the node objects. */

BSTNODE *newBSTNODE(void *v) {
	BSTNODE *leaf = malloc(sizeof(BSTNODE));		// Allocate BSTNODE space

	assert(leaf != 0);
	leaf->value = v;
	leaf->left = leaf->right = leaf->parent = NULL;

	return leaf;
}

/* Node Value Accessor */
/* Returns node value. */ 

void *getBSTNODEvalue(BSTNODE *n) {
	return n->value;
}

/* Node Value Mutator */
/* Sets node value. */

void setBSTNODEvalue(BSTNODE *n, void *v) {
//	void *old = n->value;
	n->value = v;
//	free(old);						// Free old node value
	return;
}

/* Node Left Accessor */
/* Returns node left pointer. */

BSTNODE *getBSTNODEleft(BSTNODE *n) {
	if (n->left == NULL) {					// No left subtree
		return NULL;
	}

	return n->left;
}

/* Node Left Mutator */
/* Sets node left pointer. */

void setBSTNODEleft(BSTNODE *n,BSTNODE *r) {
	n->left = r;	
	return;
}

/* Node Right Accessor */
/* Returns node right pointer. */

BSTNODE *getBSTNODEright(BSTNODE *n) {
	if (n->right == NULL) {					// No right subtree
		return NULL;
	}

	return n->right;
}

/* Node Right Mutator */
/* Sets node right pointer. */

void setBSTNODEright(BSTNODE *n, BSTNODE *r) {
	n->right = r;
	return;
}


/* Node Right Accessor */
/* Returns node right pointer. */

BSTNODE *getBSTNODEparent(BSTNODE *n) {
	if (n->parent == NULL) {				// Node is root
		return NULL;
	}

	return n->parent;
}

/* Node Right Mutator */
/* Sets node right pointer. */

void setBSTNODEparent(BSTNODE *n, BSTNODE *r) {
	n->parent = r;
	return;
}

/* Node Free Method */
/* Frees node */
void freeBSTNODE(BSTNODE *n, void (*f)(void *)) {
	void *old = n->value;
	BSTNODE *oldNode = n;

	if (f != NULL) {
		f(old);						// Free node value

		free(oldNode);					// Free node
	}

	return;
}

/*************** BST public interface ***************/

/* constructor */
/* This is the constructor for the bst objects. */

BST *newBST(
	void (*display)(void *,FILE *),
	int (*comparator)(void *,void *),
	void (*swapper)(BSTNODE *,BSTNODE *),
	void (*freer)(void *)) {
	BST *tree = malloc(sizeof(BST));			// Allocate space for BST object

	assert(tree != 0);

	tree->size = 0;						// Set BST size to 0
	tree->disp = display;					// disp field = display method
	tree->compare = comparator;				// compare field = comparator method
	tree->free = freer;					// free field = freer method

	if (swapper == NULL) {
		tree->swap = swapMethod;
	}

	else {
		tree->swap = swapper;
	}

	tree->root = NULL;					// Set root to NULL

	return tree;
}

/* BST Root Accessor */
/* Returns the BST root. */
BSTNODE *getBSTroot(BST *tree) {
	if (tree->root == NULL) {				// Empty tree
		return NULL;
	}

	return tree->root;
}

/* BST Root Mutator */
/* Sets the BST root. */
void setBSTroot(BST *tree, BSTNODE *r) {
	tree->root = r;
	
	return;
}

/* BST Set Size Method */
/* Sets the BST size. */
void setBSTsize(BST *tree, int s) {
	tree->size = s;
	return;
}

/* insert method */

BSTNODE *insertBST(BST *tree,void *v) {
	BSTNODE *leaf = newBSTNODE(v);
	BSTNODE *r = getBSTroot(tree);

	if (r == NULL) {					// Insert at root
		setBSTroot(tree, leaf);
		setBSTsize(tree, sizeBST(tree) + 1);
		//tree->root = leaf;
		//tree->size += 1;
	}

	else {
		//NODE *cur = tree->root;
		BSTNODE *cur = getBSTroot(tree);

		while (cur != NULL) {
			void *val = getBSTNODEvalue(cur);
			BSTNODE *leftNode = getBSTNODEleft(cur);
			BSTNODE *rightNode = getBSTNODEright(cur);

			if (tree->compare(v, val) < 0) {
				if (leftNode == NULL) {		// Left child insert
					//cur->left = leaf;
					setBSTNODEleft(cur, leaf);
					//leaf->par = cur;
					setBSTNODEparent(leaf, cur);
					//cur = NULL;
					//tree->size += 1;
					setBSTsize(tree, sizeBST(tree) + 1);
				}

				else {
					//cur = cur->left;	// Traverse left
					cur = getBSTNODEleft(cur);
				}
			}
		
			else if (tree->compare(v, val) > 0) {
				if (rightNode == NULL) {	// Right child insert
					//cur->right = leaf;
					setBSTNODEright(cur, leaf);
					//leaf->par = cur;
					setBSTNODEparent(leaf, cur);
					//cur = NULL;
					//tree->size += 1;
					setBSTsize(tree, sizeBST(tree) + 1);
				}

				else {
					//cur = cur->right;	// Traverse right
					cur = getBSTNODEright(cur);
				}
			}

			else {
				return leaf;				// Prevents duplicate nodes	
			}
		}
	}

	return leaf;
}

/* search method */
/* This method searches for a node value, based on its key value. */

BSTNODE *findBST(BST *tree,void *v) {
	if (getBSTroot(tree) == NULL) {
		return NULL;					// EMPTY List
	}	

	else {
		BSTNODE *cur = getBSTroot(tree);

		while (cur != NULL) {				// While not NULL, traverse tree
			void *val = getBSTNODEvalue(cur);
			BSTNODE *leftNode = getBSTNODEleft(cur);
			BSTNODE *rightNode = getBSTNODEright(cur);

			if (tree->compare(v, val) == 0) {
				return cur;			// Match returns node
			}

			else if (tree->compare(v, val) < 0) {
				cur = leftNode;			// Traverse left
			}

			else {
				cur = rightNode;		// Traverse right
			}
		}
	}

	return NULL;						// Return NULL for void * return type
}

/* Delete Method */
/* Deletes a given node via swap-to-leaf and
prune-leaf methods. */

BSTNODE *deleteBST(BST *t, void *value) {
	if (sizeBST(t) <= 0) {					// Empty tree check 
		return NULL;
	}

	BSTNODE *node = findBST(t, value); 
	BSTNODE *leaf = swapToLeafBST(t, node);

	pruneLeafBST(t, leaf);
	
	return leaf;
}

/* Swap-To-Leaf Method */
/* Swaps a node value recursively with
successor or predecessor's value until a leaf
holds the former value. */

BSTNODE *swapToLeafBST(BST *t, BSTNODE *node) {
	BSTNODE *successor = findSuccessor(node);
	BSTNODE *predecessor = findPredecessor(node);
	BSTNODE *newNode;

	if ((node == NULL) || (isLeaf(node))) {			// NULL or leaf node	
		return node;
	}

	if (successor != NULL) {				// Found successor
		t->swap(node, successor);
		newNode = successor;
	}

	else {
		t->swap(node, predecessor);			// No successor
		newNode = predecessor;
	}

	node = swapToLeafBST(t,newNode); 
	
	return node;
}

void pruneLeafBST(BST *t, BSTNODE *leaf) {
	BSTNODE *parent = NULL;
	
	if (leaf != NULL) {	
 		parent = getBSTNODEparent(leaf);
	}
	
	if (sizeBST(t) == 1) {
		if (leaf != NULL) {
			if (getBSTroot(t) == leaf) {
				setBSTroot(t, NULL);		// Prune root
			}
		}
		
		setBSTsize(t, sizeBST(t) - 1);

		return;
	}

	if (parent != NULL) {
		if (isLeftChild(leaf)) {
			setBSTNODEleft(parent, NULL);
			setBSTNODEparent(leaf, NULL);
		}

		else if (isRightChild(leaf)) {
			setBSTNODEright(parent, NULL);
			setBSTNODEparent(leaf, NULL);
		}
	}

	setBSTsize(t, sizeBST(t) - 1);				// Decrease tree size

	return;
}

/* size method */
/* Returns the tree size. */

int sizeBST(BST *tree) {
	return tree->size;					// Return tree size
}

/* Statistics Method */
/* Displays number of nodes, min, and max heights. */

void statisticsBST(BST *t, FILE *fp) {
	int size = sizeBST(t);
	BSTNODE *r = getBSTroot(t);

	if (r == NULL) {
		fprintf(fp, "Nodes: 0\n");
		fprintf(fp, "Minimum depth: -1\n");
		fprintf(fp, "Maximum depth: -1\n");
		
		return;
	}

	int minDepth = findMinDepth(r);
	int maxDepth = findMaxDepth(r);
	
	fprintf(fp, "Nodes: %d\n", size);
	fprintf(fp, "Minimum depth: %d\n", minDepth);
	fprintf(fp, "Maximum depth: %d\n", maxDepth);
	
	return;
} 

/* display method */
/* Displays the values in the BST, using pre-order traversal.
For any given node, shows left and right subtrees, 
each surrounded by brackets, if these exist.
A space is put between an existing subtree
and the node value. An empty tree is []. */

void displayBST(BST *tree, FILE *fp) {
	BSTNODE *r = getBSTroot(tree);				// Get root

	if (r == NULL) {
		fprintf(fp, "[]");				// Empty tree
		return;
	}

	preOrder(fp, tree, r);

	return;	
}

void displayBSTdebug(BST *tree, FILE *fp) {
	int size = sizeBST(tree);

	if (size != 0) {
		levelOrder(fp,tree);
	}

	return;
}

void freeBST(BST *t) {
	BSTNODE *r = getBSTroot(t);

	if (sizeBST(t) > 0) {
		postOrderFree(t, r);
	}

	free(t);

	return;
}

/*************** private method definitions ***************/

/* Pre-order Traversal */
/* Performs Pre-order traversal. */
static void preOrder(FILE *fp, BST *t, BSTNODE *n) {
	void *val = 0;
	BSTNODE *nodeLeft = 0;
	BSTNODE *nodeRight = 0;

	if (n != NULL) {
		fprintf(fp, "[");
		val = getBSTNODEvalue(n);
		t->disp(val, fp);				// Display node->value
		if (hasLeft(n)) {
			fprintf(fp, " ");			// Space if left-child
		}

		nodeLeft = getBSTNODEleft(n);			// Get node->left
		preOrder(fp, t, nodeLeft);			// Traverse left

		if (hasRight(n)) {
			fprintf(fp, " ");			// Space if right-child
		}
		
		nodeRight = getBSTNODEright(n);			// Get node->right
		preOrder(fp, t, nodeRight);			// Traverse right
		fprintf(fp, "]");	
	}	

	return;
}

/* Level-order Traversal */
/* Performs Level-order traversal. */

static void levelOrder(FILE *fp, BST *t) {
	int size = sizeBST(t);

	if (size == 0) return;

	QUEUE *parentQueue = newQUEUE(t->disp, t->free);	// Create parent queue
	QUEUE *childQueue = newQUEUE(t->disp, t->free);		// Create child queue
	BSTNODE *cur = getBSTroot(t);				// Get BST root
	void *val = 0;

	enqueue(parentQueue, cur);				// Initialize queue

	while (sizeQUEUE(parentQueue) != 0) {
		cur = (BSTNODE *) peekQUEUE(parentQueue);	// Check top of queue
		val = getBSTNODEvalue(cur);
		t->disp(val, fp);				// Display value

		levelAdd(cur, childQueue);			// Add left/right values
		dequeue(parentQueue);				// Dequeue from parent

		if (sizeQUEUE(parentQueue) == 0) {
								// Add children to parent
			while (sizeQUEUE(childQueue) > 0) {
				enqueue(parentQueue, (BSTNODE *) dequeue(childQueue));
			}

			fprintf(fp, "\n");
		}

		else {
			fprintf(fp, " ");
		}
		
	}

	freeQUEUE(parentQueue);
	freeQUEUE(childQueue);

	return;
}

static void levelAdd(BSTNODE *currentNode, QUEUE *lQueue) {
//	void *lval = 0;
//	void *rval = 0;

	if (currentNode != NULL) {
		if (hasLeft(currentNode)) {			// Add left node value
			BSTNODE *leftNode = getBSTNODEleft(currentNode);
			//lval = getBSTNODEvalue(leftNode);
			enqueue(lQueue, leftNode);
		}

		if (hasRight(currentNode)) {			// Add right node value
			BSTNODE *rightNode = getBSTNODEright(currentNode);
			//rval = getBSTNODEvalue(rightNode);
			enqueue(lQueue, rightNode);
		}
	}

	return;
}

static void postOrderFree(BST *tree, BSTNODE *node) {
        if (node != NULL) {
		BSTNODE *leftNode = getBSTNODEleft(node);
		BSTNODE *rightNode = getBSTNODEright(node);

                postOrderFree(tree, leftNode);                // Traverse left
                postOrderFree(tree, rightNode);               // Traverse right
		freeBSTNODE(node, tree->free);
		setBSTsize(tree, sizeBST(tree) - 1);
        }

        return;
}

static bool hasLeft(BSTNODE *node) {
        bool leftChild = false;
	BSTNODE *nodeLeft = getBSTNODEleft(node);		// Get node->left

        if (nodeLeft != NULL) {
                leftChild = true;
        }

        return leftChild;
}

static bool hasRight(BSTNODE *node) {
        bool rightChild = false;
	BSTNODE *nodeRight = getBSTNODEright(node);		// Get node->right

        if (nodeRight != NULL) {
                rightChild = true;
        }

        return rightChild;
}

static bool isLeaf(BSTNODE *node) {
	bool leaf = false;
	BSTNODE *nodeLeft = getBSTNODEleft(node);
	BSTNODE *nodeRight = getBSTNODEright(node);

	if ((nodeLeft == NULL) && (nodeRight == NULL)) {
		leaf = true;
	}

	return leaf;
}

static bool isLeftChild(BSTNODE *node) {
	bool leftChild = false;
	BSTNODE *nodeParent = getBSTNODEparent(node);
	BSTNODE *parentLeft;
	
	if (nodeParent != NULL) {
		parentLeft = getBSTNODEleft(nodeParent);
	}

	if (parentLeft == node) {
		leftChild = true;
	}

	return leftChild;
}

static bool isRightChild(BSTNODE *node) {
	bool rightChild = false;
	BSTNODE *nodeParent = getBSTNODEparent(node);
	BSTNODE *parentRight;

	if (nodeParent != NULL) {
		parentRight = getBSTNODEright(nodeParent);
	}

	if (parentRight == node) {
		rightChild = true;
	}

	return rightChild;
}

static void swapMethod(BSTNODE *node1, BSTNODE *node2) {
	if ((node1 == NULL) || (node2 == NULL)) {
		return;
	}

	void *value1 = getBSTNODEvalue(node1);
	void *value2 = getBSTNODEvalue(node2);

	setBSTNODEvalue(node2, value1);
	setBSTNODEvalue(node1, value2);

	return;
}

static BSTNODE *findMin(BSTNODE *n) {
	BSTNODE *min = n;
	BSTNODE *leftNode;

	if (min == NULL) {
		return NULL;
	}
	
/*	if (min != NULL) {
		leftNode = getBSTNODEleft(min);
	}*/

	while (getBSTNODEleft(min) != NULL) {
		leftNode = getBSTNODEleft(min);
		min = leftNode;

	//	leftNode = getBSTNODEleft(min);
	}

	return min;
}

static BSTNODE *findMax(BSTNODE *n) {
	BSTNODE *max = n;
	BSTNODE *rightNode;

	if (max == NULL) {
		return NULL;
	}
	
/*	if (max != NULL) {
		rightNode = getBSTNODEright(max);
	}*/

	while (getBSTNODEright(max) != NULL) {
		rightNode = getBSTNODEright(max);
		max = rightNode;
	
/*		if (max == NULL) {
			printf("Made it here!\n");
			return NULL;
		}*/
		
//		rightNode = getBSTNODEright(max);
	}
	
	return max;
}

static BSTNODE *findSuccessor(BSTNODE *n) {
	if (getBSTNODEright(n) != NULL) {
		BSTNODE *rightNode = getBSTNODEright(n);
		return findMin(rightNode);
	}

/*	BSTNODE *par = getBSTNODEparent(n);

	while ((par != NULL) && (x == getBSTNODEright(par))) {
		x = par;
		par = getBSTNODEparent(par);
	}

	return par;*/

	return NULL;
}

static BSTNODE *findPredecessor(BSTNODE *n) {
	if (getBSTNODEleft(n) != NULL) {
		BSTNODE *leftNode = getBSTNODEleft(n);
		return findMax(leftNode);
	}

/*	BSTNODE *par = getBSTNODEparent(n);

	while ((par != NULL) && (x == getBSTNODEleft(par))) {
		x = par;
		par = getBSTNODEparent(par);
	}

	return par;*/

	return NULL;
}

static int findMinDepth(BSTNODE *n) {
	int min;

	if (n == NULL) {					// Empty tree 
		return -1;
	}

	else {	
		BSTNODE *leftNode = getBSTNODEleft(n);	
		BSTNODE *rightNode = getBSTNODEright(n);

		int minl = findMinDepth(leftNode);
		int minr = findMinDepth(rightNode);

		if (minl < minr) {
			min = minl + 1;
			return min;
		}

		else { 
			min = minr + 1;
			return min;
		}
	}

	return min;
}

static int findMaxDepth(BSTNODE *n) {
	int max;

	if (n == NULL) {
		return -1;
	}

	else {
		BSTNODE *leftNode = getBSTNODEleft(n);
		BSTNODE *rightNode = getBSTNODEright(n);

		int maxl = findMaxDepth(leftNode);
		int maxr = findMaxDepth(rightNode);
	

		if (maxl < maxr) {
			max = maxr + 1;
			return max;
		}

		else {	
			max = maxl + 1;
			return max;
		}
	}

	return max;
}
