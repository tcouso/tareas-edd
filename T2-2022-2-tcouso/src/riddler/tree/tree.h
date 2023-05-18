#pragma once

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct treenode {
	int id;
	int value;
	int l_index;
	int r_index;
} TreeNode;

typedef struct tree {
	int h;   // Height of tree
	int node_count;
	struct treenode* root;     // Root node
	struct treenode** nodes;   // Array of nodes
} Tree;

TreeNode* createTreeNode(int id, int value);

Tree* createTree(int node_count, char* buffer);   // Receive an input buffer

void printTree(Tree* tree, TreeNode* root, int height);

void destroyTree(Tree* tree);

TreeNode* getLeftNode(Tree* tree, TreeNode* parent);

TreeNode* getRightNode(Tree* tree, TreeNode* parent);
