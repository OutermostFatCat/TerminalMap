#ifndef HEADER_MAP_
#define HEADER_MAP_

/* Character data */
typedef struct {

	/* (lock = 1) == locked */
	int lock;

	/* Starting position */ 
	int x_pos;
	int y_pos;

	/* Pending movements */
	int x_pending;
	int y_pending;

} character;

/* Movement.c */
pthread_t *mov_Initialize(character *);
void *thr_Mov(void *);
void movement(character *);
void *mov_exit(pthread_t *);

/* Keyboard.c */
void new_Inputmode(void);
void old_Inputmode(void);
void read_Scancode(char *);

#endif