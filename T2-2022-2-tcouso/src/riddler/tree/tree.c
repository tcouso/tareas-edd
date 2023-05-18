#include "tree.h"

TreeNode* createTreeNode(int id, int value) {
	TreeNode* new_node = (TreeNode*)calloc(1, sizeof(TreeNode));
	new_node->value = value;
	new_node->id = id;

	return new_node;
}

TreeNode* getLeftNode(Tree* tree, TreeNode* parent) {
	if (parent->l_index) {
		return tree->nodes[parent->l_index - 1];
	}
	else {
		return NULL;
	}
}

TreeNode* getRightNode(Tree* tree, TreeNode* parent) {
	if (parent->r_index) {
		return tree->nodes[parent->r_index - 1];
	}
	else {
		return NULL;
	}
}

Tree* createTree(int node_count, char* buffer) {
	Tree* tree = (Tree*)calloc(1, sizeof(Tree));
	tree->nodes = (TreeNode**)calloc(node_count, sizeof(TreeNode*));
	TreeNode* root = NULL;
	int height = log(node_count + 1) / log(2);
	int node_id;
	int k;

	for (int i = 0; i < node_count; i++) {
		node_id = i + 1;
		k = buffer[i];

		if (!root) {
			root = createTreeNode(node_id, k);
			root->l_index = 2 * root->id;
			root->r_index = (2 * root->id) + 1;
			tree->nodes[root->id - 1] = root;
		}
		else {
			TreeNode* new_node = createTreeNode(node_id, k);
			// Node references for non-last level case
			if (node_id < pow(2, height - 1)) {
				new_node->l_index = 2 * new_node->id;
				new_node->r_index = (2 * new_node->id) + 1;
			}

			tree->nodes[new_node->id - 1] = new_node;
		}
	}

	tree->root = root;
	tree->h = height;
	tree->node_count = node_count;

	return tree;
}

void printTree(Tree* tree, TreeNode* root, int height) {
	if (height > 1) {
		printTree(tree, getLeftNode(tree, root), height - 1);
	}
	printf("%d -> ", root->value);

	if (height > 1) {
		printTree(tree, getRightNode(tree, root), height - 1);
	}
}

void destroyTree(Tree* tree) {
	for (int i = 0; i < tree->node_count; i++) {
		free(tree->nodes[i]);
	}
	free(tree->nodes);
	free(tree);
}
