#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct bstnode {
	int value;
	struct bstnode* left_child;
	struct bstnode* right_child;
} BstNode;

BstNode* createBstNode(int value);

void appendBstNode(BstNode* root, BstNode* node);

void printBstNode(BstNode* node);

void destroyBstNode(BstNode* node);

void path_value(BstNode* node, FILE* output_file, int value);

void deep(BstNode* node, FILE* output_file, int value);

void order(BstNode* node, FILE* output_file);

int is_subtree(BstNode* tree, BstNode* sub);