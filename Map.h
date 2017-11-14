#ifndef HEADER_MAP_
#define HEADER_MAP_

/* Character data */
typedef struct {

	/* (lock = 1) == locked */
	int lock;

	/* Starting position */ 
	int x_pos;
	int y_pos;

	/* Abstract position */
	int c_x;
	int c_y;

	/* char texture */
	char char_texture;

	/* texture the ground the char is standing on */
	char stand_texture;

	/* Pending movements */
	int x_pending;
	int y_pending;

} character;

/* Map.c */
char **free_Map(char **map);
char **alloc_Map(void);
char **free_Screen(char **Screen);
char **alloc_Screen (void);
char **update_Map(char **map, character *man);
void shift_mapUp(char **map, character *man);
void shift_mapLeft(char **map, character *man);
void realloc_x(char **old_map);
char **realloc_y(char **old_map);
void write_Screen(char **map, char **Screen, character *man);
void write_Map(char **map ,int x_start,
		       int y_start, int x_end, int y_end);

/* Movement.c */
pthread_t *mov_Initialize(character *);
void *thr_Mov(void *);
void movement(character *);
void *mov_exit(pthread_t *);
void character_pos(char **map, character *man);

/* Keyboard.c */
void new_Inputmode(void);
void old_Inputmode(void);
void read_Scancode(char *);

#endif
