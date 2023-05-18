#include "output.h"

void sortedInsert(struct sortedlistnode** head_ref,
                  struct sortedlistnode* new_node) {
	struct sortedlistnode* current;
	/* Special case for the head end */
	if (*head_ref == NULL || (*head_ref)->data >= new_node->data) {
		new_node->next = *head_ref;
		*head_ref = new_node;
	}
	else {
		/* Locate the node before
the point of insertion */
		current = *head_ref;
		while (current->next != NULL && current->next->data < new_node->data) {
			current = current->next;
		}
		new_node->next = current->next;
		current->next = new_node;
	}
}

struct sortedlistnode* newSortedListNode(int new_data) {
	/* allocate node */
	struct sortedlistnode* new_node = (struct sortedlistnode*)malloc(
	    sizeof(struct sortedlistnode));

	/* put in the data */
	new_node->data = new_data;
	new_node->next = NULL;

	return new_node;
}

void printSortedList(struct sortedlistnode* head, FILE* output_file) {
	// struct sortedlistnode* temp = head;
	while (head != NULL) {
		fprintf(output_file, "%d ", head->data);
		head = head->next;
	}
	fprintf(output_file, "\n");
}

void deleteSortedList(struct sortedlistnode* head) {
	if (head->next) {
		deleteSortedList(head->next);
	}
	free(head);
}

// /* Driver program to test count function*/
// int main() {
// 	/* Start with the empty list */
// 	struct Node* head = NULL;
// 	struct Node* new_node = newSortedListNode(5);
// 	sortedInsert(&head, new_node);
// 	new_node = newSortedListNode(10);
// 	sortedInsert(&head, new_node);
// 	new_node = newSortedListNode(7);
// 	sortedInsert(&head, new_node);
// 	new_node = newSortedListNode(3);
// 	sortedInsert(&head, new_node);
// 	new_node = newSortedListNode(1);
// 	sortedInsert(&head, new_node);
// 	new_node = newSortedListNode(9);
// 	sortedInsert(&head, new_node);
// 	printf("\n Created Linked List\n");
// 	printList(head);

// 	return 0;
// }
