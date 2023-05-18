#include "playlist.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int count_playlist_songs(Node* head) {
	Node* current_node = head;
	int counter = 0;
	while (current_node->next != NULL) {
		counter += 1;
		current_node = current_node->next;
	}
	// Last node
	counter += 1;

	return counter;
}

void print_playlist(Playlist* playlist_pointer, FILE* output_file) {
	fprintf(output_file, "ESTADO BATIPLAYLIST %d\n", playlist_pointer->id);
	fprintf(output_file, "\t%d\n", playlist_pointer->length);
	fprintf(output_file, "\tCANCIONES\n");
	// Non-empty playlist
	if (playlist_pointer->head != NULL) {
		Node* current_node = playlist_pointer->head;
		if (current_node->next != NULL) {
			while (current_node->next != NULL) {
				fprintf(output_file, "\t\t%d %d\n", current_node->song.id, current_node->song.disc_id);
				current_node = current_node->next;
			}
		}
		fprintf(output_file, "\t\t%d %d\n", current_node->song.id, current_node->song.disc_id);
	}
	fprintf(output_file, "FIN ESTADO\n");
}

void push_song(Node** head, Song song) {
	Node* new_node = calloc(1, sizeof(Node));
	Node* current_node = *head;
	new_node->song = song;
	new_node->next = NULL;

	// Empty playlist case
	if (*head == NULL) {
		*head = new_node;
	}
	// Non-empty playlist
	else {
		while (current_node->next != NULL) {
			current_node = current_node->next;
		}
		current_node->next = new_node;
	}
}

void delete_song(Node* head, int disc_id, int song_id) {
	Node* current_node = head;
	Node* next_node;
	while (current_node->next != NULL) {
		next_node = current_node->next;
		if (next_node->song.disc_id == disc_id && next_node->song.id == song_id) {
			current_node->next = next_node->next;
			free(next_node);
			break;
		}
		current_node = current_node->next;
	}
}

bool verify_song_in_disc(Disc* disc_pointer, int disc_id, int song_id) {
	for (int i = 0; i < disc_pointer->capacity; i++) {
		Song current_song = disc_pointer->songs[i];
		if (current_song.id == song_id && current_song.disc_id == disc_id) {
			return true;
		};
	}
	return false;
}

bool verify_song_in_playlist(Node* head, int disc_id, int song_id) {
	// Empyt playlist case
	if (head == NULL) {
		return false;
	}
	else {
		Node* current_node = head;
		while (current_node->next != NULL) {
			if (current_node->song.disc_id == disc_id && current_node->song.id == song_id) {
				return true;
			}
			current_node = current_node->next;
		}
		if (current_node->song.disc_id == disc_id && current_node->song.id == song_id) {
			return true;
		}
	}
	return false;
}

/*Adds each song in disc to playlist an returns the amount of songs added*/
int add_disc_to_playlist(Node** head, Disc* disc_pointer) {
	int added_songs_count = 0;
	for (int song_index = 0; song_index < disc_pointer->capacity; song_index++) {
		int disc_id = disc_pointer->id;
		Song song = disc_pointer->songs[song_index];
		if (verify_song_in_playlist(*head, disc_id, song.id) == false) {
			push_song(head, song);
			added_songs_count += 1;
		}
	}
	return added_songs_count;
}

float compute_playlist_rating(Node* head) {
	if (head == NULL) {
		return 0;
	}
	else {
		Node* current_node = head;
		int song_counter = 0;
		int scores_sum = 0;
		while (current_node->next != NULL) {
			scores_sum += current_node->song.rating;
			song_counter += 1;
			current_node = current_node->next;
		}
		scores_sum += current_node->song.rating;
		song_counter += 1;
		return (float)scores_sum / song_counter;
	}
}

int empty_playlist(Node** head) {
	if (head != NULL) {
		int deletions_counter = 0;
		// Free each song in playlist
		Node* current_node = *head;
		Node* next;

		while (current_node != NULL) {
			next = current_node->next;
			free(current_node);
			current_node = next;
			deletions_counter += 1;
		}
		*head = NULL;
		deletions_counter += 1;

		return deletions_counter;
	}
	else {
		return 0;
	}
}

void join_playlists(Node** head1, Node** head2) {
	Node* current_node1 = *head1;
	Node* current_node2 = *head2;
	while (current_node1->next != NULL) {
		current_node1 = current_node1->next;
	}
	// Add each song on list 2 to list 1
	while (current_node2->next != NULL) {
		push_song(&current_node1, current_node2->song);
		current_node2 = current_node2->next;
	}
	push_song(&current_node1, current_node2->song);
	// Delete list 2
	empty_playlist(head2);
}

Node* split_playlist(Node** head, int split_position) {
	Node* current_node = *head;
	Node* previous_node = *head;
	int current_position = 0;
	// Split not in the begining
	if (split_position > 0) {
		// Identify split node
		Node* new_list_head;
		while (current_position < split_position) {
			previous_node = current_node;
			current_node = current_node->next;
			current_position += 1;
		}
		new_list_head = current_node;
		previous_node->next = NULL;
		return new_list_head;
	}
	// Split in the begining
	else {
		// Copy list in new playlist and empty previous playlist
		Node* new_list_head = calloc(1, sizeof(Node));
		while (current_node->next != NULL) {
			push_song(&new_list_head, current_node->song);
			current_node = current_node->next;
		}
		push_song(&new_list_head, current_node->song);
		empty_playlist(head);
		return new_list_head;
	}
}

int purge_playlist(Node** head, float rating) {
	Node* previous_node = *head;
	Node* current_node;
	int purged_songs = 0;
	// Assume that first node passes filter
	while (previous_node->next != NULL) {
		current_node = previous_node->next;
		if (current_node->song.rating < rating) {
			// Remove filtered song
			previous_node->next = current_node->next;
			Node* deleted_node = current_node;
			free(deleted_node);
			purged_songs += 1;
		}
		else {
			previous_node = previous_node->next;
		}
	}
	// Verify if first node passes filter
	Node* first_node = *head;
	if (first_node->song.rating < rating) {
		// Change first node if filter is not passed
		Node* deleted_node = first_node;
		first_node = first_node->next;
		free(deleted_node);
		// Update head
		*head = first_node;
		purged_songs += 1;
	}
	return purged_songs;
}