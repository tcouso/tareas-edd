#include <stdio.h>
#include <stdlib.h>

#include "dijkstra/dijkstra.h"
#include "graph/graph.h"
#include "queue/heap.h"

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Modo de uso: %s input output\nDonde:\n", argv[0]);
		printf("\t\"input\" es la ruta al archivo de input\n");
		printf("\t\"output\" es la ruta al archivo de output\n");
		return 1;
	}

	FILE* input_stream = fopen(argv[1], "r");

	FILE* output_stream = fopen(argv[2], "w");

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

	int num_of_vertices;
	int num_of_edges;
	int start_vertex;
	int end_vertex;
	int vertex1;
	int vertex2;
	int weight;

	fscanf(input_stream, "%d", &num_of_vertices);
	fscanf(input_stream, "%d %d", &start_vertex, &end_vertex);
	fscanf(input_stream, "%d", &num_of_edges);

	struct Graph* graph = createGraph(num_of_vertices);

	for (int i = 0; i < num_of_edges; i++) {
		fscanf(input_stream, "%d %d %d", &vertex1, &vertex2, &weight);
		addEdge(graph, vertex1, vertex2, weight);
	}

	int min_distance = dijkstra(graph, start_vertex, end_vertex);

	fprintf(output_stream, "%d\n", min_distance);

	destroyGraph(graph);

	fclose(input_stream);

	fclose(output_stream);

	return 0;
}
