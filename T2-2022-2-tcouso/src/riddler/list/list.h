#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../tree/tree.h"

typedef struct listnode {
	int heigth;
	struct tree* stored_tree;
	struct listnode* next;
	struct listnode* head_tree;
} ListNode;

ListNode* createListNode(int value);

void appendListNode(ListNode* head, ListNode* node);

void destroyListNode(ListNode* node);
