#include "disc.h"

#include <stdio.h>

/*Define functions*/

void print_disc(Disc* disc_pointer, FILE* output_file) {
	// Access disc
	Disc disc = *disc_pointer;
	fprintf(output_file, "ESTADO DISCO %d\n", disc.id);
	fprintf(output_file, "\t%d\n", disc.capacity);
	// Declare variables
	int max_rating = 0;
	Song max_rating_song;

	int min_rating = 100;
	Song min_rating_song;

	int total_length = 0;

	for (int song_index = 0; song_index < disc.capacity; song_index++) {
		Song song = disc.songs[song_index];

		// Compute max rating
		if (song.rating > max_rating) {
			max_rating = song.rating;
			max_rating_song = song;
		}

		// Compute min rating
		if (song.rating < min_rating) {
			min_rating = song.rating;
			min_rating_song = song;
		}

		// Compute total length
		total_length += song.length;
	}
	fprintf(output_file, "\t%d %d\n", max_rating, max_rating_song.id);
	fprintf(output_file, "\t%d %d\n", min_rating, min_rating_song.id);
	fprintf(output_file, "\t%d\n", total_length);
	fprintf(output_file, "\tCANCIONES\n");

	// Print songs
	for (int song_index = 0; song_index < disc.capacity; song_index++) {
		Song song = disc.songs[song_index];
		fprintf(output_file, "\t\t%d\n", song.id);
	}
	fprintf(output_file, "FIN ESTADO\n");
}

void print_song(Song song, FILE* output_file) {
	fprintf(output_file, "ESTADO CANCION %d\n", song.id);
	fprintf(output_file, "\t%d\n", song.length);
	fprintf(output_file, "\t%d\n", song.rating);
	fprintf(output_file, "FIN ESTADO\n");
}
