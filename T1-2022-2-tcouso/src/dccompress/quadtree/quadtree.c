#include "quadtree.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../imagelib/imagelib.h"

// Pending:
// Implement compression
// Test everything :O

double compute_mean(Image* image, char* channel) {
	double mean = 0;
	int counter = 0;
	for (int row = 0; row < image->height; row++) {
		for (int col = 0; col < image->width; col++) {
			double channel_value;
			if (!strcmp(channel, "L")) {
				channel_value = image->pixels[row][col].L;
			}
			else if (!strcmp(channel, "a")) {
				channel_value = image->pixels[row][col].a;
			}
			else {
				channel_value = image->pixels[row][col].b;
			}

			counter += 1;
			mean += (channel_value - mean) / counter;
		}
	}

	return mean;
}

double compute_sd(Image* image, char* channel) {
	double Sn = 0;
	double mean = 0;
	double prev_mean = 0;
	int counter = 0;

	for (int row = 0; row < image->height; row++) {
		for (int col = 0; col < image->width; col++) {
			double channel_value;
			if (!strcmp(channel, "L")) {
				channel_value = image->pixels[row][col].L;
			}
			else if (!strcmp(channel, "a")) {
				channel_value = image->pixels[row][col].a;
			}
			else {
				channel_value = image->pixels[row][col].b;
			}

			counter += 1;
			prev_mean = mean;
			mean += (channel_value - mean) / counter;
			Sn += (channel_value - prev_mean) * (channel_value - mean);
		}
	}

	return sqrt(Sn / counter);
}

/*Receives an image and returns an array of four sub-images, correspondig to each quadrant of the original image.*/
Image* get_subimage(Image* image, int height_offset, int width_offset) {
	int new_height = image->height / 2;
	int new_width = image->width / 2;

	Color** new_pixels = calloc(new_height, sizeof(Color*));
	for (int row = height_offset; row < new_height + height_offset; row++) {
		Color* cols = calloc(new_width, sizeof(Color));
		for (int col = width_offset; col < new_width + width_offset; col++) {
			cols[col - width_offset] = (Color){
			    .L = image->pixels[row][col].L,
			    .a = image->pixels[row][col].a,
			    .b = image->pixels[row][col].b};
		}
		new_pixels[row - height_offset] = cols;
	}
	Image* new_image = (Image*)calloc(1, sizeof(Image));
	new_image->height = new_height;
	new_image->width = new_width;
	new_image->pixels = new_pixels;

	return new_image;
}

Image* join_subimages(Image* sub_ll, Image* sub_lc, Image* sub_rc, Image* sub_rr) {
	int previous_width = sub_ll->width;
	int previous_height = sub_ll->height;
	int new_width = 2 * previous_width;
	int new_height = 2 * previous_height;

	Color** new_pixels = (Color**)calloc(new_height, sizeof(Color*));
	for (int row = 0; row < new_height; row++) {
		Color* cols = (Color*)calloc(new_width, sizeof(Color));
		for (int col = 0; col < new_width; col++) {
			// ll case
			if (col < previous_width && row < previous_height) {
				cols[col].L = sub_ll->pixels[row][col].L;
				cols[col].a = sub_ll->pixels[row][col].a;
				cols[col].b = sub_ll->pixels[row][col].b;
			}
			// lc case
			else if (col >= previous_width && row < previous_height) {
				cols[col].L = sub_lc->pixels[row][col - previous_width].L;
				cols[col].a = sub_lc->pixels[row][col - previous_width].a;
				cols[col].b = sub_lc->pixels[row][col - previous_width].b;
			}
			// rc case
			else if (col < previous_width && row >= previous_height) {
				cols[col].L = sub_rc->pixels[row - previous_height][col].L;
				cols[col].a = sub_rc->pixels[row - previous_height][col].a;
				cols[col].b = sub_rc->pixels[row - previous_height][col].b;
			}
			// rr case
			else {
				cols[col].L = sub_rr->pixels[row - previous_height][col - previous_width].L;
				cols[col].a = sub_rr->pixels[row - previous_height][col - previous_width].a;
				cols[col].b = sub_rr->pixels[row - previous_height][col - previous_width].b;
			}
			new_pixels[row] = cols;
		}
	}
	Image* new_image = (Image*)calloc(1, sizeof(Image));
	new_image->height = new_height;
	new_image->width = new_width;
	new_image->pixels = new_pixels;

	return new_image;
}

/*Receives an image and creates a QuadTree that hierarchically stores each pixel*/
Node* create_tree(Image* image) {
	// Base case: create pixel node
	if (image->width == 1 && image->height == 1) {
		Node* new_node = (Node*)calloc(1, sizeof(Node));

		new_node->L = (Channel*)calloc(1, sizeof(Channel));
		new_node->L->mean = image->pixels[0][0].L;
		new_node->L->sd = 0;

		new_node->a = (Channel*)calloc(1, sizeof(Channel));
		new_node->a->mean = image->pixels[0][0].a;
		new_node->a->sd = 0;

		new_node->b = (Channel*)calloc(1, sizeof(Channel));
		new_node->b->mean = image->pixels[0][0].b;
		new_node->b->sd = 0;

		new_node->gamma = 0;

		new_node->height = 1;
		new_node->width = 1;

		return new_node;
	}
	// Recursive case: divide image and create subtrees
	else {
		Node* tree = (Node*)calloc(1, sizeof(Node));

		// Divide image

		Image* img_tl = get_subimage(image, 0, 0);
		Image* img_tr = get_subimage(image, 0, image->width / 2);
		Image* img_bl = get_subimage(image, image->height / 2, 0);
		Image* img_br = get_subimage(image, image->height / 2, image->width / 2);

		// Tree childs

		Node* subtree_ll = create_tree(img_tl);
		Node* subtree_lc = create_tree(img_tr);
		Node* subtree_rc = create_tree(img_bl);
		Node* subtree_rr = create_tree(img_br);

		// Free images

		img_png_destroy(img_tl);
		img_png_destroy(img_tr);
		img_png_destroy(img_bl);
		img_png_destroy(img_br);

		tree->ll = subtree_ll;
		tree->lc = subtree_lc;
		tree->rc = subtree_rc;
		tree->rr = subtree_rr;

		// Tree channels

		tree->L = (Channel*)calloc(1, sizeof(Channel));
		tree->a = (Channel*)calloc(1, sizeof(Channel));
		tree->b = (Channel*)calloc(1, sizeof(Channel));

		tree->L->mean = compute_mean(image, "L");
		tree->L->sd = compute_sd(image, "L");

		tree->a->mean = compute_mean(image, "a");
		tree->a->sd = compute_sd(image, "a");

		tree->b->mean = compute_mean(image, "b");
		tree->b->sd = compute_sd(image, "b");

		// Tree gamma

		tree->gamma = (tree->L->sd + tree->a->sd + tree->b->sd) / 3;

		// Image dimensions

		tree->height = 2 * tree->ll->width;
		tree->width = 2 * tree->ll->height;

		return tree;
	}
}

void delete_tree(Node* node) {
	// Null node case
	if (!node) {
		return;
	}
	// Base
	else if (!node->ll && !node->lc && !node->rc && !node->rr) {
		// Free channels
		free(node->L);
		free(node->a);
		free(node->b);

		// Free node itself
		free(node);

		return;
	}
	// Recursion
	else {
		// Free child trees
		delete_tree(node->ll);
		delete_tree(node->lc);
		delete_tree(node->rc);
		delete_tree(node->rr);

		// Free channels
		free(node->L);
		free(node->a);
		free(node->b);

		// Free node itself
		free(node);

		return;
	}
}

Image* tree_to_image(Node* node) {
	// One pixel image
	if (!node->ll && !node->lc && !node->rc && !node->rr) {
		// Create one pixel image
		int size = node->height;

		Image* image = create_empty_image(size, size);

		image->height = size;
		image->width = size;

		// Create empty image
		Color color;
		color.L = node->L->mean;
		color.a = node->a->mean;
		color.b = node->b->mean;

		// Paint empty image
		img_square_paint(image, 0, 0, size, color);

		return image;
	}

	// Recursion
	else {
		// Create sub images

		Image* sub_ll = tree_to_image(node->ll);
		Image* sub_lc = tree_to_image(node->lc);
		Image* sub_rc = tree_to_image(node->rc);
		Image* sub_rr = tree_to_image(node->rr);

		// Join sub images

		Image* new_image = join_subimages(sub_ll, sub_lc, sub_rc, sub_rr);

		// Free previous images
		img_png_destroy(sub_ll);
		img_png_destroy(sub_lc);
		img_png_destroy(sub_rc);
		img_png_destroy(sub_rr);

		return new_image;
	}
}

void alpha_filter(Node** node, double alpha) {
	Node* node_prt = *node;
	if (!node_prt) {
		return;
	}
	// Base
	if (node_prt->gamma <= alpha) {
		// Free child trees
		delete_tree(node_prt->ll);
		delete_tree(node_prt->lc);
		delete_tree(node_prt->rc);
		delete_tree(node_prt->rr);

		// Set subtrees as null
		node_prt->ll = NULL;
		node_prt->lc = NULL;
		node_prt->rc = NULL;
		node_prt->rr = NULL;

		return;
	}
	// Recursion
	else {
		alpha_filter(&node_prt->ll, alpha);
		alpha_filter(&node_prt->lc, alpha);
		alpha_filter(&node_prt->rc, alpha);
		alpha_filter(&node_prt->rr, alpha);
	}
}

int count_leaves(Node* node) {
	// Base
	if (!node->ll && !node->lc && !node->rc && !node->rr) {
		return 1;
	}
	// Recursion
	else {
		return count_leaves(node->ll) + count_leaves(node->lc) + count_leaves(node->rc) + count_leaves(node->rr);
	}
}

void compress(Node** node, int h) {
	for (double alpha = 0; alpha < 129; alpha++) {
		alpha_filter(node, alpha);
		int amount_of_leaves = count_leaves(*node);
		if (amount_of_leaves < h){
			break;
		}
	}
}