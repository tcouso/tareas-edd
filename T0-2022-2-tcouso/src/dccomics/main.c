#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disc.h"
#include "playlist.h"

/* Retorna true si ambos strings son iguales */
bool string_equals(char *string1, char *string2) {
	return !strcmp(string1, string2);
}

/* Función encargada de chequear los argumentos ingresados */
bool check_arguments(int argc, char **argv) {
	if (argc != 3) {
		printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
		printf("Donde:\n");
		printf("\tINPUT es la ruta del archivo de input\n");
		printf("\tOUTPUT es la ruta del archivo de output\n");
		return false;
	}
	return true;
}

int main(int argc, char **argv) {
	/////////////////////////
	//        Input        //
	/////////////////////////

	/* Si los parámetros del programa son inválidos */
	if (!check_arguments(argc, argv)) {
		/* Salimos del programa indicando que no terminó correctamente */
		return 1;
	}

	/* Abrimos el archivo de input */
	FILE *input_file = fopen(argv[1], "r");

	/* Abrimos el archivo de output */
	FILE *output_file = fopen(argv[2], "w");

	/* String que usaremos para guardar la instrucción actual*/
	char command[32];

	// Lectura del número de batiplaylists
	int N_BATIPLAYLISTS;
	fscanf(input_file, "%d", &N_BATIPLAYLISTS);

	// Estructura para almacenar playlists
	Playlist **playlists = calloc(N_BATIPLAYLISTS, sizeof(Playlist *));

	// Lectura del número de discos
	int N_DISCS;
	fscanf(input_file, "%d", &N_DISCS);

	// Estructura para almacenar discos
	Disc **discs = calloc(N_DISCS, sizeof(Disc *));
	int disc_index = 0;
	for (int i = 0; i < N_DISCS; i++) {
		discs[i] = calloc(1, sizeof(Disc));
	}

	/* Leemos la primera instrucción */
	fscanf(input_file, "%s", command);

	/* Mientras la instrucción sea distinta a FIN */
	while (!string_equals(command, "FIN")) {
		/////////////////////////
		//       Parte A       //
		/////////////////////////

		if (string_equals(command, "CREAR-DISCO")) {
			/* Obtenemos la información del disco */
			int capacity, length, rating;
			fscanf(input_file, "%d", &capacity);

			/* COMPLETAR */
			Song *songs = calloc(capacity, sizeof(Song));

			/*Construimos el disco*/
			for (int song_index = 0; song_index < capacity; song_index++) {
				fscanf(input_file, "%d %d", &length, &rating);
				songs[song_index] = (Song){
				    .length = length,
				    .rating = rating,
				    .id = song_index,
				    .disc_id = disc_index};
				fprintf(output_file, "CANCION AGREGADA %d %d\n", songs[song_index].id, disc_index);
			}

			/*Almacenamiento del disco*/
			// Disc *new_disc_pointer = calloc(1, sizeof(Disc));
			Disc *new_disc_pointer = discs[disc_index];
			new_disc_pointer->songs = songs;
			new_disc_pointer->capacity = capacity;
			new_disc_pointer->id = disc_index;
			discs[disc_index] = new_disc_pointer;
			// Increase disc index
			disc_index += 1;
		}

		else if (string_equals(command, "ELIMINAR-CANCION")) {
			/* Obtenemos la información de la canción */
			int disc_id, song_id;
			fscanf(input_file, "%d %d", &disc_id, &song_id);

			/* COMPLETAR */
		}

		else if (string_equals(command, "IMPRIMIR-DISCO")) {
			/* Obtenemos la información correspondiente */
			int disc_id;
			fscanf(input_file, "%d", &disc_id);

			/* COMPLETAR */
			Disc *disc_pointer = discs[disc_id];
			print_disc(disc_pointer, output_file);
		}

		else if (string_equals(command, "IMPRIMIR-CANCION")) {
			/* Obtenemos la información correspondiente */
			int disc_id, song_id;
			fscanf(input_file, "%d %d", &disc_id, &song_id);

			/* COMPLETAR */
			Disc *disc_pointer = discs[disc_id];
			Disc disc = *disc_pointer;
			Song song = disc.songs[song_id];
			print_song(song, output_file);
		}

		/////////////////////////
		//       Parte B       //
		/////////////////////////

		else if (string_equals(command, "CREAR-BATIPLAYLIST")) {
			/* Obtenemos la información correspondiente */
			int playlist_id;
			fscanf(input_file, "%d", &playlist_id);

			/* COMPLETAR */
			playlists[playlist_id] = calloc(1, sizeof(Playlist));
			playlists[playlist_id]->id = playlist_id;
			playlists[playlist_id]->length = 0;
			playlists[playlist_id]->head = NULL;
			// playlists[playlist_id]->deleted = false;
			fprintf(output_file, "BATIPLAYLIST CREATED %d\n", playlist_id);
		}

		else if (string_equals(command, "AGREGAR-CANCION-BATIPLAYLIST")) {
			int playlist_id, disc_id, song_id;
			fscanf(input_file, "%d %d %d", &playlist_id, &disc_id, &song_id);

			/* COMPLETAR */
			bool song_in_disc = false;
			Disc *current_disc_pointer;
			for (int i = 0; i < N_DISCS; i++) {
				current_disc_pointer = discs[i];
				if (verify_song_in_disc(current_disc_pointer, disc_id, song_id) == true) {
					song_in_disc = true;
				}
			}
			if (song_in_disc == true) {
				Playlist *playlist_pointer = playlists[playlist_id];
				Disc *disc_pointer = discs[disc_id];
				Song song = disc_pointer->songs[song_id];
				if (playlist_pointer != NULL) {
					push_song(&playlist_pointer->head, song);
					playlist_pointer->length += 1;
					fprintf(output_file, "NEW SONG ADDED %d %d %d\n", song_id, disc_id, playlist_id);
				}
			}
			else {
				fprintf(output_file, "SONG NOT FOUND\n");
			}
		}

		else if (string_equals(command, "ELIMINAR-CANCION-BATIPLAYLIST")) {
			int playlist_id, disc_id, song_id;
			fscanf(input_file, "%d %d %d", &playlist_id, &disc_id, &song_id);

			/* COMPLETAR */
			Playlist *playlist_pointer = playlists[playlist_id];

			if (playlist_pointer != NULL) {
				if (verify_song_in_playlist(playlist_pointer->head, disc_id, song_id) == true) {
					delete_song(playlist_pointer->head, disc_id, song_id);
					playlist_pointer->length -= 1;
					fprintf(output_file, "ELIMINADO %d %d %d\n", song_id, disc_id, playlist_id);
				}
				else {
					fprintf(output_file, "SONG NOT FOUND ON PLAYLIST\n");
				}
			}
		}

		else if (string_equals(command, "AGREGAR-DISCO-BATIPLAYLIST")) {
			int playlist_id, disc_id;
			fscanf(input_file, "%d %d", &playlist_id, &disc_id);

			/* COMPLETAR */
			Playlist *playlist_pointer = playlists[playlist_id];
			Disc *disc_pointer = discs[disc_id];
			if (playlist_pointer != NULL) {
				int amount_of_added_songs = add_disc_to_playlist(&playlist_pointer->head, disc_pointer);
				playlist_pointer->length += amount_of_added_songs;
				fprintf(output_file, "AGREGADO %d %d %d\n", amount_of_added_songs, disc_id, playlist_id);
			}
		}

		else if (string_equals(command, "PLAY-BATIPLAYLIST")) {
			int playlist_id;
			fscanf(input_file, "%d", &playlist_id);

			/* COMPLETAR */
			Playlist *playlist_pointer = playlists[playlist_id];
			if (playlist_pointer != NULL) {
				print_playlist(playlist_pointer, output_file);
			}
		}

		else if (string_equals(command, "RATE-BATIPLAYLIST")) {
			int playlist_id;
			fscanf(input_file, "%d", &playlist_id);

			/* COMPLETAR */
			// Playlist *playlist_pointer = get_playlist(playlists, N_BATIPLAYLISTS, playlist_id);
			Playlist *playlist_pointer = playlists[playlist_id];
			if (playlist_pointer != NULL) {
				float rating = compute_playlist_rating(playlist_pointer->head);
				fprintf(output_file, "BATIPLAYLIST %d: %.2f\n", playlist_id, rating);
			}
		}

		/////////////////////////
		//       Parte C       //
		/////////////////////////

		else if (string_equals(command, "ELIMINAR-BATIPLAYLIST")) {
			int playlist_id;
			fscanf(input_file, "%d", &playlist_id);

			/* COMPLETAR */
			Playlist *playlist_pointer = playlists[playlist_id];
			int n_of_deleted = empty_playlist(&playlist_pointer->head);
			free(playlist_pointer);
			fprintf(output_file, "BATIPLAYLIST DELETED %d %d\n", playlist_id, n_of_deleted);
		}

		else if (string_equals(command, "UNIR-BATIPLAYLIST")) {
			int playlist_id1, playlist_id2;
			fscanf(input_file, "%d %d", &playlist_id1, &playlist_id2);

			/* COMPLETAR */
			Playlist *playlist1_pointer = playlists[playlist_id1];
			Playlist *playlist2_pointer = playlists[playlist_id2];
			if (playlist1_pointer != NULL && playlist2_pointer != NULL) {
				join_playlists(&playlist1_pointer->head, &playlist2_pointer->head);
				// free(playlist2_pointer);
				fprintf(output_file, "JOINED %d AND %d\n", playlist_id1, playlist_id2);
			}
		}

		else if (string_equals(command, "SPLIT-BATIPLAYLIST")) {
			int playlist_id, new_playlist_id, position;
			fscanf(input_file, "%d %d %d", &playlist_id, &new_playlist_id, &position);

			/* COMPLETAR */
			Playlist *playlist_pointer = playlists[playlist_id];
			// New playlist list of songs
			Node *new_playlist_head = split_playlist(&playlist_pointer->head, position);
			// Instance new playlist
			Playlist *new_playlist_pointer = calloc(1, sizeof(Playlist));
			new_playlist_pointer->id = new_playlist_id;
			new_playlist_pointer->head = new_playlist_head;
			new_playlist_pointer->length = count_playlist_songs(new_playlist_pointer->head);
			playlists[new_playlist_id] = new_playlist_pointer;
			// Update original playlist length
			playlist_pointer->length = count_playlist_songs(playlist_pointer->head);
		}

		else if (string_equals(command, "ORDENAR-BATIPLAYLIST")) {
			int playlist_id;
			fscanf(input_file, "%d", &playlist_id);

			/* COMPLETAR */
		}

		else if (string_equals(command, "PURGAR-BATIPLAYLIST")) {
			int playlist_id, rating;
			fscanf(input_file, "%d %d", &playlist_id, &rating);

			/* COMPLETAR */
			Playlist *playlist_pointer = playlists[playlist_id];
			int purged_songs = purge_playlist(&playlist_pointer->head, rating);
			fprintf(output_file, "BATIPLAYLIST PURGED %d %d\n", playlist_id, purged_songs);
		}

		/* Leemos la siguiente instrucción */
		fscanf(input_file, "%s", command);
	}

	/////////////////////////////////////
	//        Cerramos archivos        //
	////////////////////////////////////

	fclose(input_file);
	fclose(output_file);

	///////////////////////////////////
	//     Liberamos memoria         //
	///////////////////////////////////

	/* COMPLETAR */

	// Free discs
	for (int i = 0; i < N_DISCS; i++) {
		Disc *disc_pointer = discs[i];
		free(disc_pointer->songs);
		free(disc_pointer);
	}
	free(discs);

	// Free playlists

	for (int i = 0; i < N_BATIPLAYLISTS; i++) {
		Playlist *playlist_pointer = playlists[i];
		// Playlist exists
		if (playlist_pointer != NULL) {
			Node *head = playlist_pointer->head;
			while (head != NULL) {
				Node *previous_node = head;
				head = head->next;
				free(previous_node);
			}
			// Last node
			free(head);
		}
		free(playlist_pointer);
	}
	free(playlists);

	return 0;
}
