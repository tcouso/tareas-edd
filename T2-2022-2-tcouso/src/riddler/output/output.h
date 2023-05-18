#pragma once

/*Code obtained from https://www.geeksforgeeks.org/given-a-linked-list-which-is-sorted-how-will-you-insert-in-sorted-way/*/
#include <stdio.h>
#include <stdlib.h>

// Structs

/* Link list node */
typedef struct sortedlistnode {
	int data;
	struct sortedlistnode* next;
} SortedListNode;

// Functions

/* Function to insert a new_node in a list in a sorted way*/
void sortedInsert(struct sortedlistnode** head_ref, struct sortedlistnode* new_node);

/* A utility function to create a new node */
struct sortedlistnode* newSortedListNode(int new_data);

/* Function to print linked list in output file*/
void printSortedList(struct sortedlistnode* head, FILE* output_file);

/*Free resources of sorted list*/
void deleteSortedList(struct sortedlistnode* head);
