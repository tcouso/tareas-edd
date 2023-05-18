#pragma once
#include "../graph/graph.h"
#include "../queue/heap.h"

/*Returns array of optimal costs from src to every vertex in graph*/
int dijkstra(struct Graph* graph, int src, int dest);