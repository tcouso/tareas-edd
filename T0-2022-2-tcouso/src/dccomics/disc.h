#include <stdio.h>

#pragma once

/*Declare structs*/

typedef struct song {
	int length;
	int rating;
	int id;
	int disc_id;
} Song;

typedef struct disc {
	Song* songs;
	int capacity;
	int id;
} Disc;

/*Declare funcitons*/

void print_disc(Disc* disc_pointer, FILE* output_file);

void print_song(Song song, FILE* output_file);
