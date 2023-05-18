#pragma once

#include <stdlib.h>

#include "../../imagelib/imagelib.h"

// Structs

typedef struct channel {
	double mean;
	double sd;
} Channel;

typedef struct node {
	struct node* ll;   // left child
	struct node* lc;   // left-center chilf
	struct node* rc;   // right-center child
	struct node* rr;   // right child
	Channel* L;
	Channel* a;
	Channel* b;
	double gamma;
	int height;
	int width;
} Node;

// Functions

Image* get_subimage(Image* image, int height_offset, int width_offset);

Image* tree_to_image(Node* node);

Node* create_tree(Image* image);

void delete_tree(Node* node);

void alpha_filter(Node** node, double alpha);

Image* join_subimages(Image* sub_ll, Image* sub_lc, Image* sub_rc, Image* sub_rr);

void compress(Node** node, int h);
