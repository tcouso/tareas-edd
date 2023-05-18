#include "graph.h"

// Code from:
// https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/

struct AdjListNode* newAdjListNode(int dest, int weight) {
	struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;

	return newNode;
}

struct Graph* createGraph(int V) {
	struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
	graph->V = V;
	graph->array = (struct AdjList*)malloc(V * sizeof(struct AdjList));
	for (int i = 0; i < V; ++i) {
		graph->array[i].head = NULL;
		graph->array[i].colour = WHITE;
	}

	return graph;
}

void destroyAdjList(struct AdjList* adj_list) {
	struct AdjListNode* curr = adj_list->head;
	struct AdjListNode* next;

	while (curr) {
		next = curr->next;
		free(curr);
		curr = next;
	}
}

void destroyGraph(struct Graph* graph) {
	for (int i = 0; i < graph->V; i++) {
		destroyAdjList(&graph->array[i]);
	}
	free(graph->array);
	free(graph);
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
	// From src to dest
	struct AdjListNode* newNode = newAdjListNode(dest, weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	// From dest to src
	newNode = newAdjListNode(src, weight);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}
