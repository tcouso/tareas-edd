#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash/hash.h"
#include "tree/tree.h"

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Modo de uso: %s input output\nDonde:\n", argv[0]);
		printf("\t\"input\" es la ruta al archivo de input\n");
		printf("\t\"output\" es la ruta al archivo de output\n");
		return 1;
	}

	// Open input file
	FILE* input_stream = fopen(argv[1], "r");

	// Open output file
	FILE* output_stream = fopen(argv[2], "w");

	// Si alguno de los dos archivos no se pudo abrir
	if (!input_stream) {
		printf("El archivo %s no existe o no se puede leer\n", argv[1]);
		return 2;
	}
	if (!output_stream) {
		printf("El archivo %s no se pudo crear o no se puede escribir\n", argv[2]);
		printf("Recuerda que \"fopen\" no puede crear carpetas\n");
		fclose(input_stream);
		return 3;
	}

	// Create main tree

	int node_count;
	fscanf(input_stream, "%d", &node_count);

	// Create and fill main tree buffer

	char buffer[node_count];

	for (int i = 0; i < node_count; i++) {
		fscanf(input_stream, "%hhd", &buffer[i]);
	}

	Tree* main_tree = createTree(node_count, buffer);

	// Store queries and lengths

	int query_count;
	int subtree_node_count;
	fscanf(input_stream, "%d", &query_count);

	int node_value;
	char** queries = calloc(query_count, sizeof(char*));
	int* queries_lengths = calloc(query_count, sizeof(int));

	for (int i = 0; i < query_count; i++) {
		fscanf(input_stream, "%d", &subtree_node_count);
		queries_lengths[i] = subtree_node_count;
		queries[i] = calloc(subtree_node_count, sizeof(char));

		for (int j = 0; j < subtree_node_count; j++) {
			fscanf(input_stream, "%d", &node_value);
			queries[i][j] = node_value;
		}
	}

	// Get largest query size

	int largest_query_lenght = 0;
	for (int i = 0; i < query_count; i++) {
		if (queries_lengths[i] > largest_query_lenght) {
			largest_query_lenght = queries_lengths[i];
		}
	}

	// Determine table size parametrically
	int largest_query_height = log(largest_query_lenght + 1) / log(2);

	int table_size = computeTableSize(largest_query_height);

	// Create hash table
	HashTable* table = createHashTable(table_size);

	// Populate hash table
	for (int i = main_tree->h; i > 1; i--) {
		populateTable(main_tree, main_tree->root, table, i);
	}

	// Query hash table

	for (int i = 0; i < query_count; i++) {
		// Create query subtree
		Tree* subtree = createTree(queries_lengths[i], queries[i]);

		// Compute hash value of query subtree
		int subtree_hash_value = computeHashValue(subtree, subtree->root, table, subtree->h);

		// Search subtree in hash table and print ids when there is a match
		hashSearch(table, subtree_hash_value, main_tree, subtree, output_stream);

		// Delete subtree
		destroyTree(subtree);
	}

	// Delete hast table
	deleteHashTable(table);

	// Delete main tree
	destroyTree(main_tree);

	// Delete queries
	for (int i = 0; i < query_count; i++) {
		free(queries[i]);
	}
	free(queries);
	free(queries_lengths);

	// Close input file
	fclose(input_stream);

	// Close output file
	fclose(output_stream);

	return 0;
}
