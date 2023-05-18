#include "list.h"

ListNode* createListNode(int value) {
	ListNode* new_node = (ListNode*)calloc(1, sizeof(ListNode));
	new_node->heigth = value;

	return new_node;
}

void appendListNode(ListNode* head, ListNode* node) {
	ListNode* current_node = head;
	bool appended_node = false;

	while (!appended_node) {
		if (current_node->next) {
			current_node = current_node->next;
		}
		else {
			current_node->next = node;
			appended_node = true;
		}
	}
}

void destroyListNode(ListNode* node) {
	ListNode* current_node = node;

	if (current_node->next) {
		destroyListNode(current_node->next);
	}
	if (current_node->stored_tree) {
		free(current_node->stored_tree);
	}
	free(current_node);
}
