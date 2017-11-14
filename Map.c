#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Map.h"

#define START_SIZE 200
#define SCREEN_X 51
#define SCREEN_Y 15

int x_size = START_SIZE;
int y_size = START_SIZE;

char **alloc_Map(void) {

	/* Map: null pointer at the end */
	char **map = malloc(sizeof(char *) * START_SIZE);
	
	int i;
	for (i = 0; i < START_SIZE; ++i) {
		map[i] = malloc(sizeof(char) * START_SIZE);
	}

	return map;
}

/* Generate Terrain from x_start, y_start to end */
void write_Map(char **map ,int x_start,
		       int y_start, int x_end, int y_end) {
	
	int y;
	int x;
	for (y = y_start; y <= y_end; ++y) {
		for (x = x_start; x <= x_end; ++x) {
			map[y][x] = '+';
		}
	}
}

/* Increase map size in x */
void realloc_x(char **old_map) {

	char *new_y;
	int y;
	for (y = 0; y < y_size; y++) {
		if ((new_y = realloc(old_map[y], sizeof(char) * x_size * 2)) == NULL) {
			printf("ERROR: realloc");
			exit(0);
		}
		old_map[y]  = new_y;
	}
	x_size *= 2;
}

/* Increase map size in y */
char **realloc_y(char **old_map) {

	y_size *= 2;
	char **new_map;
	if ((new_map = malloc(sizeof(char *) * y_size)) == NULL) {
		printf("ERROR: realloc");
		exit(0);
	}

	int y;
	for (y = 0; y < y_size / 2; ++y) {
		new_map[y] = old_map[y];
		if ((new_map[y + y_size / 2] = malloc(sizeof(char) * x_size)) == NULL) {
			printf("ERROR: realloc");
			exit(0);
		}
	}
	free(old_map);
	return new_map;
}

/*  When new content has to be generated to the left of the map, all pointers
    must generate new space and shift its content to the right, so a new mapblock
    appears on the left side of the map instead of the right
*/
void shift_mapLeft(char **map, character *man) {

	int y;
	int x;
	for (y = 0; y < y_size; ++y) {
		for (x = 0; x < x_size / 2; ++x) {
			map[y][x + x_size / 2] = map[y][x];
		}
	}

	/* update Char position */
	man->x_pos += x_size / 2;
}

/* Same as left shift, but instead new mapblock to the north */
void shift_mapUp(char **map, character *man) {

	int y;
	char *p;
	for (y = 0; y < y_size / 2; ++y) {
		p = map[y + y_size / 2];
		map[y + y_size / 2] = map[y];
		map[y] = p;
	}

	/* update Char position */
	man->y_pos += y_size / 2;
}

char **update_Map(char **map, character *man) {
    
    /* Generate map to the right */	
    if (x_size - man->x_pos < 50) {
		realloc_x(map);   
		write_Map(map, x_size / 2, 0, x_size - 1, y_size - 1);

	/* Generate map to the left */
	} else if (x_size - man->x_pos > x_size - 50) {
		realloc_x(map);
		shift_mapLeft(map, man);
		write_Map(map, 0, 0, x_size / 2 - 1, y_size - 1);
	}

	/* Generate map downwards */
	if (y_size - man->y_pos < 50) {
		map = realloc_y(map);
		write_Map(map, 0, (y_size / 2), x_size - 1, (y_size - 1));

	/* Generate map upwards */
	} else if (y_size - man->y_pos > y_size - 50) {
		map = realloc_y(map);
		shift_mapUp(map, man);
		write_Map(map, 0, 0, x_size - 1, (y_size / 2) - 1);
	}

	/* Not returning map after reallocating y-memory will memory unaccessable */
	return map;
}

char **alloc_Screen(void) {

	char **Screen = malloc(sizeof(char *) * (SCREEN_Y + 3));

	int y;
	for (y = 0; y < SCREEN_Y + 2; ++y) {
		Screen[y] = malloc(sizeof(char) * (SCREEN_X + 3));
	}
	Screen[y] = NULL;

	return Screen;
}

void write_Screen(char **map, char **Screen, character *man) {
	
	int x;
	int y;
	int x_sc = 0;
	int y_sc = 0;

	Screen[y_sc][x_sc++] = ' ';
	for (; x_sc < 52; x_sc++) {
		Screen[y_sc][x_sc] = '_';
	}
	Screen[y_sc][x_sc++] = ' ';
	Screen[y_sc++][x_sc] = '\0';

	for (y = man->y_pos - 7, x_sc = 0; y <= man->y_pos + 7; ++y, ++y_sc, x_sc = 0) {

		Screen[y_sc][x_sc++] = '|';
		for (x = man->x_pos - 25; x <= man->x_pos + 25; ++x, ++x_sc) {
			Screen[y_sc][x_sc] = map[y][x];
		}
		Screen[y_sc][x_sc++] = '|';
		Screen[y_sc][x_sc] = '\0';
	}

	Screen[y_sc][x_sc++] = ' ';
	for (; x_sc < 52; x_sc++) {
		Screen[y_sc][x_sc] = '/';
	}
	Screen[y_sc][x_sc++] = ' ';
	Screen[y_sc++][x_sc] = '\0';

	Screen[y_sc] = NULL;
}

char **free_Screen(char **screen) {

	int i = 0;
	while (screen[i]) {
		free(screen[i++]);
	}
	free(screen);

	return NULL;
}

char **free_Map(char **map) {

	int i = 0;
	while (i < y_size) {
		free(map[i++]);
	}
	free(map);

	return NULL;
}
