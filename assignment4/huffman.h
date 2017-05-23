/*
 * Interface provided by Darrell Long
 * Course: CMPS 12B Spring 2017
 */

# ifndef _HUFFMAN_H
# define _HUFFMAN_H
# include <stdint.h>
# include <stdbool.h>

# ifndef NIL
# define NIL (void*) 0
# endif

typedef struct DAH treenode;

struct DAH
{
	uint8_t symbol;
	uint64_t count;
	bool leaf;
	treeNode *left, *right;
};

// Create a single node with symbols, count, and leaf or not
treeNode *newNode(uint8_t s, uint64_t c, bool l);

// Delete a single node
treeNode *delNode(treeNode *t);

// Delete a tree of nodes
treeNode *delTree(treeNode *t);

// Dump a Huffman tree onto a file
void dumpTree(treeNode *t, int file);

// Build a tree from the saved tree
treeNode *loadTree(uint8_t savedTree[]);

// Step through a tree following the code
int32_t *stepTree(treeNode *root, treeNode *t, uint32_t code);

// Parse a Huffman tree to build codes
void buildCode(treeNode *t, code s, code table[256]);

static inline void delNode(treeNode *h)
{
	free(h);
	return;
}

// Compares the size of two trees
static inline bool compare(treeNode *l, treeNode *r)
{
	return l->count - r->count;
}

// Join two subtrees
treeNode *join(treeNode *l, treeNode *r);

# endif