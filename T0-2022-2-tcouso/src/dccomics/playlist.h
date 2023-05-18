#include <stdbool.h>

#include "disc.h"

// Define structs

typedef struct playlist {
	int id;
	int length;
	struct node* head;
} Playlist;

typedef struct node {
	Song song;
	struct node* next;
} Node;

// Declare functions
int count_playlist_songs(Node* head);

void print_playlist(Playlist* playlist_pointer, FILE* output_file);

void push_song(Node** head, Song song);

void delete_song(Node* head, int disc_id, int song_id);

// Playlist* get_playlist(Playlist** playlists, int n_playlists, int playlist_id);

bool verify_song_in_disc(Disc* disc_pointer, int disc_id, int song_id);

bool verify_song_in_playlist(Node* head, int disc_id, int song_id);

int add_disc_to_playlist(Node** head, Disc* disc_pointer);

float compute_playlist_rating(Node* head);

int empty_playlist(Node** head);

void join_playlists(Node** head1, Node** head2);

Node* split_playlist(Node** head, int split_position);

int purge_playlist(Node** head, float rating);
