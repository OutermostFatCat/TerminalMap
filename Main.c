#include <stdio.h>
#include <stdlib.h>

int x_size;

char **allocate_Map(char *map);
void write_Map(char *map[]);
void *free_Map(char *map[]);

int main(int argc, char *argv[])
{

	if (argc != 2) {
		printf("ERROR: Not enough arguments\n");
		return -1;
	}

	char **map = allocate_Map(argv[1]);
	write_Map(map);

	int i;
	printf("\n\n");
	for (i = 0; map[i] != NULL; ++i) {

		/* print from start of the pointed array */
		printf("%s\n", map[i] - x_size + 1);
	}
	printf("\n");

	map = free_Map(map);
	return 0;
}

void write_Map(char *map[]) {

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

	/* write last row*/
	*map[row] = ' ';
	for (x = 1; x < x_size - 2; ++x) {
		*++map[row] = '/';
	}
	*++map[row] = ' ';
	*++map[row] = '\0';

	if (map[++row] != NULL) {
		printf("ERROR: In writing map\n");
	}
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
		allocated_Map = malloc(sizeof(char *) * (size[0] + 3));
		allocated_Map[size[0] + 2] = NULL;

		for(i = 0; i < size[0] + 2; ++i) {
			allocated_Map[i] = malloc(sizeof(char) * (size[0] + 3));
		}
		return allocated_Map;
	}
}

void *free_Map(char *map[]) {
	
	int i;
	while (*map != NULL) {
		free(*map++);
		printf("%d\n", i);
	}
	free(map);

	return NULL;
}
