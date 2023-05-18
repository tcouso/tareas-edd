#pragma once
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Code from:
// https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/

// Constants

#define WHITE 0
#define GRAY 1
#define BLACK 2

// Structs

struct AdjListNode {
	int dest;
	int weight;
	struct AdjListNode* next;
};

struct AdjList {
	struct AdjListNode* head;
	int colour;
};

struct Graph {
	int V;
	struct AdjList* array;
};

// Functions

struct AdjListNode* newAdjListNode(int dest, int weight);

struct Graph* createGraph(int V);

void addEdge(struct Graph* graph, int src, int dest, int weight);

void destroyGraph(struct Graph* graph);
