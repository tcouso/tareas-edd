#pragma once
#include <math.h>
#include <stdbool.h>

#include "../list/list.h"
#include "../output/output.h"
#include "../tree/tree.h"

// Structs

typedef struct hashtable {
	int size;                    // Size of table
	struct listnode** entries;   // Hash table entries
} HashTable;

// Functions

/*Computes table size from domain size K.*/
int computeTableSize(int K);

/*Returns a new hash table parametrically according to hash function domain K.*/
HashTable* createHashTable(int table_size);

/*Deletes the given hash table.*/
void deleteHashTable(HashTable* table);

/*Searchs subtree match in hash table and prints every appearance's id.*/
void hashSearch(HashTable* table, int key, Tree* tree, Tree* subtree, FILE* output_file);

/*Inserts subtree in specified key of hash table.*/
void hashInsert(HashTable* table, int key, Tree* tree);

/*Hash function of tree. Receives tree's root returns hash value*/
int h(int hash_left, int hash_right, int root_value, int table_size);

/*Populates hash table with sub-trees of up to 'height' levels*/
int populateTable(Tree* tree, TreeNode* root, HashTable* table, int height);

/*Computes hash value for a given root TreeNode*/
int computeHashValue(Tree* tree, TreeNode* root, HashTable* table, int height);

// bool equalTrees(Tree* tree1, TreeNode* root1, Tree* tree2, TreeNode* root2, int height);