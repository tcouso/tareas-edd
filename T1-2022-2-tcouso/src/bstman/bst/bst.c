#include "bst.h"

#include <stdbool.h>
#include <stdlib.h>

// Basic functions

BstNode* createBstNode(int value) {
	BstNode* new_node = (BstNode*)calloc(1, sizeof(BstNode));
	new_node->value = value;

	return new_node;
}

void appendBstNode(BstNode* root, BstNode* node) {
	BstNode* current_node = root;
	bool appended_node = false;

	while (!appended_node) {
		// Left child transition
		if (current_node->value > node->value) {
			if (current_node->left_child) {
				current_node = current_node->left_child;
			}
			else {
				current_node->left_child = node;
				appended_node = true;
			}
		}
		// Right child transition
		else {
			if (current_node->right_child) {
				current_node = current_node->right_child;
			}
			else {
				current_node->right_child = node;
				appended_node = true;
			}
		}
	}
}

void printBstNode(BstNode* node) {
	BstNode* current_node = node;

	if (current_node->left_child) {
		printBstNode(current_node->left_child);
	}
	printf("%d -> ", current_node->value);

	if (current_node->right_child) {
		printBstNode(current_node->right_child);
	}
}

void destroyBstNode(BstNode* node) {
	BstNode* current_node = node;

	if (current_node->left_child) {
		destroyBstNode(current_node->left_child);
	}
	if (current_node->right_child) {
		destroyBstNode(current_node->right_child);
	}
	free(current_node);
	return;
}

// Requested functions

void path_value(BstNode* node, FILE* output_file, int value) {
	BstNode* current_node = node;
	bool founded_node = false;

	while (!founded_node) {
		fprintf(output_file, "%d ", current_node->value);

		if (current_node->value == value) {
			founded_node = true;
		}
		else if (current_node->value > value) {
			if (current_node->left_child) {
				current_node = current_node->left_child;
			}
			else {
				fprintf(output_file, "X");
				founded_node = true;
			}
		}
		else if (current_node->value < value) {
			if (current_node->right_child) {
				current_node = current_node->right_child;
			}
			else {
				fprintf(output_file, "X");
				founded_node = true;
			}
		}
	}
}

void deep(BstNode* node, FILE* output_file, int value) {
	BstNode* current_node = node;
	bool founded_node = false;
	int depth = 0;

	while (!founded_node) {
		if (current_node->value == value) {
			founded_node = true;
			fprintf(output_file, "%d", depth);
		}
		else if (current_node->value > value) {
			if (current_node->left_child) {
				current_node = current_node->left_child;
				depth += 1;
			}
			else {
				founded_node = true;
			}
		}
		else if (current_node->value < value) {
			if (current_node->right_child) {
				current_node = current_node->right_child;
				depth += 1;
			}
			else {
				fprintf(output_file, "X");
				founded_node = true;
			}
		}
	}
}

void order(BstNode* node, FILE* output_file) {
	BstNode* current_node = node;

	if (current_node->left_child) {
		order(current_node->left_child, output_file);
	}
	fprintf(output_file, "%d ", current_node->value);

	if (current_node->right_child) {
		order(current_node->right_child, output_file);
	}
}

// Tree structure functions

bool is_leaf(BstNode* node) {
	if (!node->left_child && !node->right_child) {
		return true;
	}
	else {
		return false;
	}
}

bool both_only_left(BstNode* tree, BstNode* sub) {
	if (tree && sub) {
		if (tree->left_child && sub->left_child && !tree->right_child && !sub->right_child) {
			return true;
		}
	}
	return false;
}

bool both_only_right(BstNode* tree, BstNode* sub) {
	if (tree && sub) {
		if (tree->right_child && sub->right_child && !tree->left_child && !sub->left_child) {
			return true;
		}
	}
	return false;
}

bool both_left_and_right(BstNode* tree, BstNode* sub) {
	if (tree && sub) {
		if (tree->right_child && sub->right_child && tree->left_child && sub->left_child) {
			return true;
		}
	}
	return false;
}

// Subtree detection function

int is_subtree(BstNode* tree, BstNode* sub) {
	if (tree->value == sub->value) {
		if (is_leaf(tree) && is_leaf(sub)) {
			return 1;
		}
		else if (both_only_left(tree, sub)) {
			return is_subtree(tree->left_child, sub->left_child);
		}
		else if (both_only_right(tree, sub)) {
			return is_subtree(tree->right_child, sub->right_child);
		}
		else if (both_left_and_right(tree, sub)) {
			return is_subtree(tree->left_child, sub->left_child) &&
			       is_subtree(tree->right_child, sub->right_child);
		}
		else {
			return 0;
		}
	}
	else {
		// If tree is leaf, stop the search
		if (is_leaf(tree)) {
			return 0;
		}
		// If tree is not leaf, keep searching
		else {
			return is_subtree(tree->left_child, sub) || is_subtree(tree->right_child, sub);
		}
	}
}