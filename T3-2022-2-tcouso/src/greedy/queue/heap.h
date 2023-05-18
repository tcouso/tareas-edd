#pragma once
#include <stdbool.h>
#include <stdlib.h>

// Code from https://github.com/IIC2133-PUC/Talleres/tree/master/heaps

// Structs

typedef struct node_t {
	int key;
	int priority;
} Node;

typedef struct heap_t {
	Node **nodes;
	int size;
	int capacity;
} Heap;

// Functions

Heap *heap_create(int capacity);

void heap_free(Heap *heap);

void heap_insert(Heap *heap, int key, int priority);

Node *heap_extract_min(Heap *heap);

void percolate_down(Heap *heap, int index);

void percolate_up(Heap *heap, int index);

void heapify(Heap *heap);

bool heap_is_empty(Heap *heap);

void decrease_key(Heap *heap, int vertex_index, int priority);
