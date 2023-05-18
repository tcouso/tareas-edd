#include "dijkstra.h"

// Code based on:
// https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/

int dijkstra(struct Graph* graph, int src, int dest) {
	int num_of_vertices = graph->V;
	int dist[num_of_vertices];

	for (int i = 0; i < num_of_vertices; ++i) {
		dist[i] = INT_MAX;
		graph->array[i].colour = WHITE;
	}

	dist[src] = 0;
	graph->array[src].colour = GRAY;
	Heap* heap = heap_create(num_of_vertices + 1);
	heap_insert(heap, src, dist[src]);

	while (!heap_is_empty(heap)) {
		Node* curr_vertex = heap_extract_min(heap);
		struct AdjListNode* curr_neighbor = graph->array[curr_vertex->key].head;

		while (curr_neighbor != NULL) {
			if (graph->array[curr_neighbor->dest].colour == WHITE || graph->array[curr_neighbor->dest].colour == GRAY) {
				if (dist[curr_neighbor->dest] > dist[curr_vertex->key] + curr_neighbor->weight) {
					// Update distance
					dist[curr_neighbor->dest] = dist[curr_vertex->key] + curr_neighbor->weight;
					// Decrease key
					decrease_key(heap, curr_neighbor->dest, dist[curr_neighbor->dest]);
				}
				if (graph->array[curr_neighbor->dest].colour == WHITE) {
					graph->array[curr_neighbor->dest].colour = GRAY;
					heap_insert(heap, curr_neighbor->dest, dist[curr_neighbor->dest]);
				}
			}
			curr_neighbor = curr_neighbor->next;
		}
		graph->array[curr_vertex->key].colour = BLACK;
		free(curr_vertex);
	}

	heap_free(heap);
	return dist[dest];
}