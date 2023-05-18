#include "hash.h"

int computeTableSize(int N) {
	int K = 0;

	for (int i = 1; i < N; i++) {
		K += (pow(2, i) - 1);
	}

	K = 1.3 * K;
	return (int)K;
}

HashTable* createHashTable(int table_size) {
	HashTable* table = (HashTable*)calloc(1, sizeof(HashTable));
	table->size = table_size;
	table->entries = (ListNode**)calloc(table_size, sizeof(ListNode*));

	return table;
}

void deleteHashTable(HashTable* table) {
	for (int i = 0; i < table->size; i++) {
		if (table->entries[i]) {
			// Iterate heights lists
			ListNode* head = table->entries[i];
			while (head) {
				if (head->head_tree) {
					// Delete height list
					destroyListNode(head->head_tree);
				}
				head = head->next;
			}
			// Delete entry list
			destroyListNode(table->entries[i]);
		}
	}

	// Delete table itself
	free(table->entries);
	free(table);
}

/*Increamental integration of hash values.*/
int h(int hash_left, int hash_right, int root_value, int table_size) {
	// Implement as Xor
	return ((root_value << 1) ^ (hash_left ^ hash_right)) % table_size;
}

/*Searchs a node associated to a certain height in the linkage list of a specific hash table entry.*/
ListNode* getNodeByHeight(ListNode* head, int heigth) {
	while (head->next) {
		if (head->heigth == heigth) {
			return head;
		}
		head = head->next;
	}
	// If no match is found, a node is created for the given height
	ListNode* new_height_node = createListNode(heigth);
	head->next = new_height_node;

	return new_height_node;
}

void hashInsert(HashTable* table, int key, Tree* tree) {
	ListNode* head;
	// Empty slot case
	if (!table->entries[key]) {
		head = createListNode(tree->h);
		table->entries[key] = head;
	}
	else {
		head = table->entries[key];
	}
	// Search node with correct height
	ListNode* match_height_node = getNodeByHeight(head, tree->h);

	// Create wrapper node for tree
	ListNode* tree_wrapper = createListNode(tree->h);
	tree_wrapper->stored_tree = tree;

	// Append tree to head_tree list
	if (match_height_node->head_tree) {
		appendListNode(match_height_node->head_tree, tree_wrapper);
	}
	else {
		match_height_node->head_tree = tree_wrapper;
	}
}

int populateTable(Tree* tree, TreeNode* root, HashTable* table, int height) {
	// Base case
	if (height == 2) {
		TreeNode* left_node = getLeftNode(tree, root);
		TreeNode* right_node = getRightNode(tree, root);
		int hash_value = h(left_node->value, right_node->value, root->value, table->size);

		// Create Tree root
		Tree* tree = (Tree*)calloc(1, sizeof(Tree));
		tree->h = height;
		tree->root = root;

		// Insert value
		hashInsert(table, hash_value, tree);

		// Return hash value
		return hash_value;
	}
	// Recursion
	else {
		// Compute hash value
		TreeNode* left_node = getLeftNode(tree, root);
		TreeNode* right_node = getRightNode(tree, root);

		int left_hash_value = populateTable(tree, left_node, table, height - 1);
		int right_hash_value = populateTable(tree, right_node, table, height - 1);

		int hash_value = h(left_hash_value, right_hash_value, root->value, table->size);

		// Create Tree
		Tree* tree = (Tree*)calloc(1, sizeof(Tree));
		tree->h = height;
		tree->root = root;

		// Insert tree in hash table
		hashInsert(table, hash_value, tree);

		// Return hash value
		return hash_value;
	}
}

int computeHashValue(Tree* tree, TreeNode* root, HashTable* table, int height) {
	// Base case
	if (height == 2) {
		TreeNode* left_node = getLeftNode(tree, root);
		TreeNode* right_node = getRightNode(tree, root);
		int hash_value = h(left_node->value, right_node->value, root->value, table->size);

		// Return hash value
		return hash_value;
	}
	// Recursion
	else {
		// Compute hash value
		TreeNode* left_node = getLeftNode(tree, root);
		TreeNode* right_node = getRightNode(tree, root);

		int left_hash_value = computeHashValue(tree, left_node, table, height - 1);
		int right_hash_value = computeHashValue(tree, right_node, table, height - 1);

		int hash_value = h(left_hash_value, right_hash_value, root->value, table->size);

		// Return hash value
		return hash_value;
	}
}

/*
Returns true if every node in root1 has the same value in the correspondig node of root2.
Otherwise, returns false.
*/
bool equalTrees(Tree* tree1, TreeNode* root1, Tree* tree2, TreeNode* root2, int height) {
	// Base case
	if (height == 1) {
		if (root1->value == root2->value) {
			return true;
		}
		return false;
	}
	// Recursion
	else {
		TreeNode* left_node1 = getLeftNode(tree1, root1);
		TreeNode* right_node1 = getRightNode(tree1, root1);

		TreeNode* left_node2 = getLeftNode(tree2, root2);
		TreeNode* right_node2 = getRightNode(tree2, root2);

		bool left_match = equalTrees(tree1, left_node1, tree2, left_node2, height - 1);
		bool right_match = equalTrees(tree1, right_node1, tree2, right_node2, height - 1);
		if (left_match && right_match && root1->value == root2->value) {
			return true;
		}
		return false;
	}
}

void hashSearch(HashTable* table, int key, Tree* tree, Tree* subtree, FILE* output_file) {
	ListNode* head = table->entries[key];
	int height = subtree->h;
	SortedListNode* sorted_list_head = NULL;

	// Search height list
	if (head) {
		while (head) {
			// Search for height match
			if (head->heigth == height) {
				ListNode* head_tree = head->head_tree;
				while (head_tree) {
					if (equalTrees(tree, head_tree->stored_tree->root, subtree, subtree->root, height)) {
						SortedListNode* match = newSortedListNode(head_tree->stored_tree->root->id);
						sortedInsert(&sorted_list_head, match);
					}
					head_tree = head_tree->next;
				}
			}
			head = head->next;
		}

		if (sorted_list_head) {
			printSortedList(sorted_list_head, output_file);
			deleteSortedList(sorted_list_head);
		}
		else {
			fprintf(output_file, "%d\n", -1);
		}
	}
	else {
		fprintf(output_file, "%d\n", -1);
	}
}