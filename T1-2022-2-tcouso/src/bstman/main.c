#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bst/bst.h"

/* Retorna true si ambos strings son iguales */
bool string_equals(char* string1, char* string2) {
	return !strcmp(string1, string2);
}

/* Revisa que los parametros del programa sean válidos */
bool check_arguments(int argc, char** argv) {
	if (argc != 3) {
		printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
		printf("Donde:\n");
		printf("\tINPUT es la ruta del archivo de input\n");
		printf("\tOUTPUT es la ruta del archivo de output\n");
		exit(1);
	}

	return true;
}

int main(int argc, char** argv) {
	check_arguments(argc, argv);

	FILE* input_file = fopen(argv[1], "r");
	FILE* output_file = fopen(argv[2], "w");

	int node_count;
	int query_count;

	fscanf(input_file, "%d", &node_count);

	// Init root
	BstNode* root = NULL;

	/* leemos Cada nodo */
	int k;
	for (int i = 0; i < node_count; i++) {
		fscanf(input_file, "%d", &k);
		if (!root) {
			root = createBstNode(k);
		}
		else {
			BstNode* new_node = createBstNode(k);
			appendBstNode(root, new_node);
		}
	}

	fscanf(input_file, "%d", &query_count);
	/* leemos las consultas */
	char command[32];
	int value;
	for (int i = 0; i < query_count; i++) {
		fscanf(input_file, "%s %d", command, &value);
		/* completar la revision de comando y ejecucion de los mismos */

		if (!strcmp(command, "PATH")) {
			path_value(root, output_file, value);
			fprintf(output_file, "\n");
		}
		else if (!strcmp(command, "DEEP")) {
			deep(root, output_file, value);
			fprintf(output_file, "\n");
		}
		else if (!strcmp(command, "ORDER")) {
			order(root, output_file);
			fprintf(output_file, "\n");
		}
		else if (!strcmp(command, "SUBTREE")) {
			// Create subtree
			BstNode* subtree_root = NULL;

			for (int j = 0; j < value; j++) {
				fscanf(input_file, "%d", &k);
				if (!subtree_root) {
					subtree_root = createBstNode(k);
				}
				else {
					BstNode* new_subtree_node = createBstNode(k);
					appendBstNode(subtree_root, new_subtree_node);
				}
			}
			// Verify subtree-ness of subtree
			fprintf(output_file, "%d\n", is_subtree(root, subtree_root));

			// Destroy subtree
			destroyBstNode(subtree_root);
		}
	}

	destroyBstNode(root);
	fclose(input_file);
	fclose(output_file);
	return 0;
}
