#include <stdio.h>
#include <stdlib.h>

#include "graph/graph.h"

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Modo de uso: %s input output\nDonde:\n", argv[0]);
		printf("\t\"input\" es la ruta al archivo de input\n");
		printf("\t\"output\" es la ruta al archivo de output\n");
		return 1;
	}

	// Abrimos el archivo de input
	FILE* input_stream = fopen(argv[1], "r");

	// Abrimos el archivo de output
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

	// Construct graph

	int num_of_vertices;
	int num_of_edges;
	int x;
	int y;

	fscanf(input_stream, "%d", &num_of_vertices);
	num_of_edges = (num_of_vertices * (num_of_vertices - 1)) / 2;

	Graph* graph = createGraph(num_of_vertices, num_of_edges);

	for (int vertex_id = 0; vertex_id < num_of_vertices; vertex_id++) {
		fscanf(input_stream, "%d %d", &x, &y);
		graph->vertices[vertex_id] = createVertex(vertex_id, x, y);
		// graph->vertices_count += 1;
	}

	populateGraph(&graph);

	// // Verify graph
	// printf("Vertex 0: %d\n", graph->vertices[0]->id);
	// printf("Edge 0: %d\n", graph->edges[0]->distance);

	// Sort edges
	qsort(graph->edges, graph->num_of_edges, sizeof(Edge*), compareEdges);

	// // Verify sorted graph
	// printf("Vertex 0: %d\n", graph->vertices[0]->id);
	// printf("Edge 0: %d\n", graph->edges[0]->distance);

	// // Test: Disjoint set
	// DisjointSet* test_set = createSet(graph->num_of_vertices);

	// for (int i = 0; i < graph->num_of_vertices; i++) {
	// 	test_set->parent[i] = i;
	// }

	// setUnion(&test_set, graph->vertices[1]->id, graph->vertices[4]->id);
	// setUnion(&test_set, graph->vertices[2]->id, graph->vertices[0]->id);
	// setUnion(&test_set, graph->vertices[3]->id, graph->vertices[4]->id);

	// printf("Parent of 2: %d\n", findInSet(test_set, graph->vertices[2]->id));
	// printf("Parent of 0: %d\n", findInSet(test_set, graph->vertices[0]->id));

	// printf("Parent of 1: %d\n", findInSet(test_set, graph->vertices[1]->id));
	// printf("Parent of 3: %d\n", findInSet(test_set, graph->vertices[3]->id));

	// setUnion(&test_set, graph->vertices[0]->id, graph->vertices[4]->id);
	// printf("\n");

	// printf("Parent of 2: %d\n", findInSet(test_set, graph->vertices[2]->id));
	// printf("Parent of 0: %d\n", findInSet(test_set, graph->vertices[0]->id));

	// printf("Parent of 1: %d\n", findInSet(test_set, graph->vertices[1]->id));
	// printf("Parent of 3: %d\n", findInSet(test_set, graph->vertices[3]->id));

	// destroySet(test_set);

	// // Test: Kruskal algorithm for finding mst's
	// Graph* test_mst = kruskal(graph, 2);
	// printf("Costo: %d\n", computeCost(test_mst));

	// destroyMST(test_mst);

	// Get min degree of mst

	// Genereate unrestricted mst and compute cost
	Graph* prev_mst = kruskal(graph, -1);
	Graph* new_mst;
	Graph* optimal_mst;

	// Get neighbors number of node with maximum neighbors
	int max_neighbors = getMaxNeighbors(prev_mst);

	// Iterate downwards from max neighbors until there's a rise in cost
	bool optimum_found = false;

	while (!optimum_found) {
		// Reset graph neighbors
		resetVerticesNeighbors(&graph);
		new_mst = kruskal(graph, max_neighbors);
		if (computeCost(prev_mst) < computeCost(new_mst)) {
			optimal_mst = prev_mst;
			optimum_found = true;
		}
		else {
			destroyMST(prev_mst);
			prev_mst = new_mst;
			max_neighbors -= 1;
		}
	}
	// printf("Optimal MST cost: %d\n", computeCost(optimal_mst));

	// Output file
	fprintf(output_stream, "%d\n", computeCost(optimal_mst));
	for (int i = 0; i < optimal_mst->num_of_edges; i++) {
		Vertex* vertex1 = optimal_mst->edges[i]->vertex1;
		Vertex* vertex2 = optimal_mst->edges[i]->vertex2;
		fprintf(
		    output_stream,
		    "%d %d %d %d\n",
		    vertex1->x,
		    vertex1->y,
		    vertex2->x,
		    vertex2->y);
	}

	destroyMST(optimal_mst);
	destroyMST(new_mst);
	destroyGraph(graph);

	fclose(input_stream);
	fclose(output_stream);

	return 0;
}
