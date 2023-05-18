#include "graph.h"

Graph* createGraph(int num_of_vetices, int num_of_edges) {
	Graph* graph = calloc(1, sizeof(Graph));
	// graph->vertices_count = 0;
	// graph->edges_count = 0;
	graph->num_of_vertices = num_of_vetices;
	graph->num_of_edges = num_of_edges;
	graph->vertices = calloc(num_of_vetices, sizeof(Vertex*));
	graph->edges = calloc(num_of_edges, sizeof(Edge*));

	return graph;
}

void destroyGraph(Graph* graph) {
	for (int vertex_id = 0; vertex_id < graph->num_of_vertices; vertex_id++) {
		// free(graph->vertices[vertex_id]->neighbors);
		free(graph->vertices[vertex_id]);
	}
	free(graph->vertices);

	for (int edge_id = 0; edge_id < graph->num_of_edges; edge_id++) {
		free(graph->edges[edge_id]);
	}
	free(graph->edges);
	free(graph);
}

void destroyMST(Graph* graph) {
	free(graph->vertices);
	free(graph->edges);
	free(graph);
}

void populateGraph(Graph** graph) {
	Graph* graph_ptr = *graph;
	int edge_id = 0;

	for (int i = 0; i < graph_ptr->num_of_vertices; i++) {
		for (int j = i + 1; j < graph_ptr->num_of_vertices; j++) {
			Vertex* vertex1 = graph_ptr->vertices[i];
			Vertex* vertex2 = graph_ptr->vertices[j];
			Edge* new_edge = createEdge(edge_id, vertex1, vertex2);
			graph_ptr->edges[edge_id] = new_edge;
			// graph_ptr->edges_count += 1;
			// addEdge(&graph_ptr, new_edge);
			edge_id += 1;
		}
	}
}

void resetVerticesNeighbors(Graph** graph) {
	Graph* graph_ptr = *graph;

	for (int i = 0; i < graph_ptr->num_of_vertices; i++) {
		graph_ptr->vertices[i]->num_of_neighbors = 0;
	}
}

int getMaxNeighbors(Graph* graph) {
	int max = 0;
	for (int i = 0; i < graph->num_of_vertices; i++) {
		if (max < graph->vertices[i]->num_of_neighbors) {
			max = graph->vertices[i]->num_of_neighbors;
		}
	}
	return max;
}

int computeCost(Graph* graph) {
	int cost = 0;
	for (int i = 0; i < graph->num_of_edges; i++) {
		cost += graph->edges[i]->distance;
	}

	return cost;
}

Vertex* createVertex(int id, int x, int y) {
	Vertex* vertex = calloc(1, sizeof(Vertex));
	vertex->id = id;
	vertex->x = x;
	vertex->y = y;
	// vertex->max_neighbors = max_neighbors;
	vertex->num_of_neighbors = 0;
	// vertex->neighbors = calloc(max_neighbors, sizeof(Vertex*));

	return vertex;
}

int computeManhattanDistance(Vertex* vertex1, Vertex* vertex2) {
	return abs(vertex2->x - vertex1->x) + abs(vertex2->y - vertex1->y);
}

Edge* createEdge(int id, Vertex* vertex1, Vertex* vertex2) {
	Edge* edge = calloc(1, sizeof(Edge));
	edge->id = id;
	edge->vertex1 = vertex1;
	edge->vertex2 = vertex2;
	edge->distance = computeManhattanDistance(vertex1, vertex2);

	return edge;
}

int compareEdges(const void* a, const void* b) {
	Edge* edge_a = *(Edge**)a;
	Edge* edge_b = *(Edge**)b;

	return (edge_a->distance - edge_b->distance);
}

DisjointSet* createSet(int num_of_vertices) {
	DisjointSet* new_set = calloc(1, sizeof(DisjointSet));
	// new_set->num_of_vertices = num_of_vertices;
	new_set->parent = calloc(num_of_vertices, sizeof(int));

	return new_set;
}

void destroySet(DisjointSet* disjoint_set) {
	free(disjoint_set->parent);
	free(disjoint_set);
}

int findInSet(DisjointSet* disjoint_set, int parent_id) {
	bool found_rep = false;
	while (!found_rep) {
		if (disjoint_set->parent[parent_id] == parent_id) {
			found_rep = true;
		}
		else {
			parent_id = disjoint_set->parent[parent_id];
		}
	}

	return parent_id;
	// if (disjoint_set->parent[parent_id] == parent_id) {
	// 	return parent_id;
	// }
	// else {
	// 	return findInSet(disjoint_set, disjoint_set->parent[parent_id]);
	// }
}

void setUnion(DisjointSet** disjoint_set, int set1, int set2) {
	DisjointSet* set_ptr = *disjoint_set;
	// int set1_parent = findInSet(set_ptr, set1_rep);
	set_ptr->parent[set1] = set2;
	// printf("set %d has parent set %d\n", set1_rep, set2_rep);
}

Graph* kruskal(Graph* graph, int max_neighbors) {
	// Init MST
	Graph* mst = createGraph(graph->num_of_vertices, graph->num_of_vertices - 1);

	// Init disjoints sets and graph
	DisjointSet* disjoint_sets = createSet(graph->num_of_vertices);
	for (int i = 0; i < graph->num_of_vertices; i++) {
		disjoint_sets->parent[i] = i;
		mst->vertices[i] = graph->vertices[i];
	}

	// Take minimum edge
	// Assuming sorted edges, we can take the elements of the array in order
	Edge* min_edge;
	int vertex1_id;
	int vertex2_id;
	int subset1;
	int subset2;
	int mst_edges_counter = 0;

	for (int i = 0; i < graph->num_of_edges; i++) {
		min_edge = graph->edges[i];

		vertex1_id = min_edge->vertex1->id;
		vertex2_id = min_edge->vertex2->id;

		subset1 = findInSet(disjoint_sets, vertex1_id);
		subset2 = findInSet(disjoint_sets, vertex2_id);

		bool disjoint_cond = subset1 != subset2;
		bool vertex1_neighbors_cond;
		bool vertex2_neighbors_cond;

		if (max_neighbors == -1) {
			vertex1_neighbors_cond = true;
			vertex2_neighbors_cond = true;
		}
		else {
			vertex1_neighbors_cond = (mst->vertices[vertex1_id]->num_of_neighbors < max_neighbors);
			vertex2_neighbors_cond = (mst->vertices[vertex2_id]->num_of_neighbors < max_neighbors);
		}
		if (disjoint_cond && vertex1_neighbors_cond && vertex2_neighbors_cond) {
			mst->vertices[vertex1_id]->num_of_neighbors += 1;
			mst->vertices[vertex2_id]->num_of_neighbors += 1;

			setUnion(&disjoint_sets, subset1, subset2);
			mst->edges[mst_edges_counter] = min_edge;

			// mst->edges_count += 1;
			mst_edges_counter += 1;

			// printf("IN\n");
			// printf("Edges: %d\n", mst_edges_counter);
			// for (int i = 0; i < mst->num_of_vertices; i++) {
			// 	printf("\t%d\n", findInSet(disjoint_sets, mst->vertices[i]->id));
			// }
			// printf("--------\n");
		}
		// printf("OUT\n");

		// if (mst_edges_counter == mst->num_of_edges) {
		// 	break;
		// }
	}
	// printf("Edges: %d\n", mst_edges_counter);

	destroySet(disjoint_sets);

	return mst;
}

// Pending: We need a structure to store all of the mst's, and a way to destroy them correctly
