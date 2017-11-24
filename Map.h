#ifndef HEADER_MAP_
#define HEADER_MAP_

#define START_SIZE 200
#define SCREEN_X 51
#define SCREEN_Y 15

#define ATTACK 1
#define PASSIVE 0


#define MONSTER_HP 3

/* monster data */
typedef struct {
	
	int x_pos;
	int y_pos;
	int x_pending;
	int y_pending;

	unsigned int health;

	int a_radius;

	char m_texture;
	char stand_texture;

} monster;

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

	/* if positive, attack target */
	int attack;

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
void character_Attack(char **map, character *c, monster **m);
int target_Monster (monster **m, int x, int y);

/* Keyboard.c */
void new_Inputmode(void);
void old_Inputmode(void);
void read_Scancode(char *);

/* AI.c */
monster *gen_Monster(char **map, character *c);
monster *kill_Monster(char **map, monster *m);
void mon_Position (char **map, monster *m);
int check_Radius(monster *m, character *c);
void mon_Follow(char **map, monster *m, character *c);

#endif
