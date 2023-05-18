#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Structs
typedef struct vertex {
	int id;
	int x;
	int y;
	// struct vertex** neighbors;
	int num_of_neighbors;
	// int max_neighbors;
} Vertex;

typedef struct edge {
	int id;
	struct vertex* vertex1;
	struct vertex* vertex2;
	int distance;
} Edge;

typedef struct graph {
	struct edge** edges;
	struct vertex** vertices;
	// int edges_count;
	// int vertices_count;
	int num_of_vertices;
	int num_of_edges;
} Graph;

typedef struct disjointset {
	// int num_of_vertices;
	int* parent;
} DisjointSet;

// Functions

// Graph
Graph* createGraph(int num_of_vertices, int num_of_edges);

void destroyGraph(Graph* graph);

void destroyMST(Graph* graph);

/*Adds edges to graph to make it complete.*/
void populateGraph(Graph** graph);

int getMaxNeighbors(Graph* graph);

void resetVerticesNeighbors(Graph** graph);

// Vertex
Vertex* createVertex(int id, int x, int y);

// void addVertex(Graph** graph, Vertex* vertex);

// Edge
Edge* createEdge(int id, Vertex* vertex1, Vertex* vertex2);

// void addEdge(Graph** graph, Edge* edge);

/*Compare function for sorting edges.*/
int compareEdges(const void* a, const void* b);

// Disjoint Set
DisjointSet* createSet(int num_of_vertices);

void destroySet(DisjointSet* disjoint_set);

/*Assigns set2 as parent of set1.*/
void setUnion(DisjointSet** disjoint_set, int set1, int set2);

int findInSet(DisjointSet* disjoint_set, int set_rep);

/*Returns MST of given graph by performing Kruskal algorithm.
If max neighbors is equal to -1, the max number of neighbors per vertex
is unrestricted.*/
Graph* kruskal(Graph* graph, int max_neighbors);

int computeCost(Graph* graph);