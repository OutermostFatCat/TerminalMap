#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Map.h"

int x_size;
int y_size;
struct timespec delay;


char **allocate_Map(char *map);
void write_Map(char *map[], character *man);
void *free_Map(char *map[]);
void character_pos(character *man);

int main(int argc, char *argv[])
{
	character man;
	man.x_pos = 1;
	man.y_pos = 1;
	man.x_pending = 0;
	man.y_pending = 0;

	/* FPS = 10 */
	delay.tv_sec = 0;
	delay.tv_nsec = 100000000;

	if (argc != 2) {
		printf("ERROR: Not enough arguments\n");
		return -1;
	}

	char **map = allocate_Map(argv[1]);
	pthread_t *thread =mov_Initialize(&man);
	write_Map(map, &man);

	new_Inputmode();

	int i;
	for (i = 0; 1; write_Map(map, &man)) {
		for (printf("\n\n"), i = 0; map[i] != NULL; ++i) {
	
			/* print from start of the pointed array */
			printf("%s\n", map[i] - x_size + 1);
		}
		printf("\n");
		nanosleep(&delay, NULL);
	}

	/* clean up */
	old_Inputmode();
	thread = mov_exit(thread);
	map = free_Map(map);
	return 0;
}

void write_Map(char *map[], character *man) {

	int row = 0;
	int x;

	/* write first row*/
	*map[row] = ' ';
	for (x = 1; x < x_size - 2; ++x) {
		*++map[row] = '_';
	}
	*++map[row] = ' ';
	*++map[row] = '\0';

	if (map[row + 1] == NULL) {
		exit(0);
	}

	for (++row; map[row + 1] != NULL; ++row) {
		for (x = 1, *map[row] = '|'; x < x_size - 2; ++x) {
			*++map[row] = '+';	
		}
		*++map[row] = '|';
		*++map[row] = '\0';
	}

	/* write last row */
	*map[row] = ' ';
	for (x = 1; x < x_size - 2; ++x) {
		*++map[row] = '/';
	}
	*++map[row] = ' ';
	*++map[row] = '\0';

	character_pos(man);

	/* Saves current character position on map */
	*(map[man->y_pos] + man->x_pos - x_size + 1) = 'C';

	if (map[++row] != NULL) {
		printf("ERROR: In writing map\n");
	}
}

/* Updates current position of character */
void character_pos(character *man) {

	if (man->x_pos > 1 && man->x_pending < 0) {
		while (man->x_pos != 1 && man->x_pending != 0) {
			--man->x_pos;
			++man->x_pending;
		}
	} else if (man->x_pos < x_size - 3 && man->x_pending > 0) {
		while (man->x_pos != x_size - 3 && man->x_pending != 0) {
			++man->x_pos;
			--man->x_pending;
		}
	}
	if (man->y_pos > 1 && man->y_pending < 0) {
		while (man->y_pos != 1 && man->y_pending != 0) {
			--man->y_pos;
			++man->y_pending;
		}
	} else if (man->y_pos < y_size - 3 && man->y_pending > 0) {
		while (man->y_pos != y_size - 3 && man->y_pending != 0) {
			++man->y_pos;
			--man->y_pending;
		}
	}

	man->x_pending = 0;
	man->y_pending = 0;
}

char **allocate_Map(char *map) {

	if(*map <= '0' || *map > '9') {
		printf("ERROR: Geometry doesn't add up\n");
		exit(0);
	}

	/* int size[2] isn't initialized to {0, 0} */
	int size[] = {0, 0};
	for (; *map != 'x' && *map != '\0' && *map != '\n'; *++map) {
		size[0] = size[0] * 10 + *map - '0';
	}

	x_size = size[0] + 3;
	char **allocated_Map;
	int i;

	/* if argument doesn't have BxB but instead
	   only a B, allocate memery for map of size BxB
	*/
	if (*map++ == 'x') {
		if(*map <= '0' || *map > '9') {
			printf("ERROR: Geometry doesn't add up\n");
			exit(0);
		}
		for (; *map != '\0' && *map != '\n'; *++map) {
			size[1] = size[1] * 10 + *map - '0';
		}
		y_size = size[1] + 3;

		/* Allocate memory for the row of pointers,
		   which is the height of the map + 3, the last one
		   for a null pointer (required);
		*/

		allocated_Map = malloc(sizeof(char *) * (size[1] + 3));
		allocated_Map[size[1] + 2] = NULL;

		/* Allocate memory for every pointer in the map,
		   which is the length of the map + 3, because of '\0'
		*/
		for(i = 0; i < size[1] + 2; ++i) {
			allocated_Map[i] = malloc(sizeof(char) * (size[0] + 3));
		}
		return allocated_Map;

	} else {
		y_size = x_size;
		allocated_Map = malloc(sizeof(char *) * (size[0] + 3));
		allocated_Map[size[0] + 2] = NULL;

		for(i = 0; i < size[0] + 2; ++i) {
			allocated_Map[i] = malloc(sizeof(char) * (size[0] + 3));
		}
		return allocated_Map;
	}
}

/* Frees the map and all of its pointers except the null pointer.
   Worthwhile thing to remember: To free memory from a pointer, the pointer
   has to contain the address of the start of the allocated memory? (probably)
   Also: You are allowed to free a null pointer, nothing happens? (probably)
*/
void *free_Map(char *map[]) {
	
	int i = 0;
	while (map[i] != NULL) {

		/* free from the start of the allocated memory */
		free(map[i++] - x_size + 1);
	}
	free(map);

	return NULL;
}
