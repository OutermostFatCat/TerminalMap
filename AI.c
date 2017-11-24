#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Map.h"

monster *gen_Monster(char **map, character *c) {

	monster *m;
	if ((m  = malloc(sizeof(monster))) == NULL) {
		printf("ERROR: malloc");
		exit(0);
	}

	m->m_texture = 'M';

	/* Generate monster between specified radius from player */
	srand((unsigned) time(NULL));
	int r;

	/* randomize x_pos */
	r = rand() % 10;
	if (rand() % 2) {
		m->x_pos = c->x_pos + 35 + r;
	} else {
		m->x_pos = c->x_pos - 45 + r;
	}

	/* randomize y_pos */
	r = rand() % 10;
	if (rand() % 2) {
		m->y_pos = c->y_pos + 10 + r;
	} else {
		m->y_pos = c->y_pos - 20 + r;
	}

	/* if spawn taken, change position slightly 25% chance to increase each side */
	while (map[m->y_pos][m->x_pos] == 'M') {
		if ((rand() % 4) == 1) {
			++m->y_pos;
		} else if ((rand() % 4) == 1) {
			--m->y_pos;
		} else if ((rand() % 4) == 1) {
			++m->x_pos;	
		} else {
			--m->x_pos;
		}
	}

	m->stand_texture = map[m->y_pos][m->x_pos];
	map[m->y_pos][m->x_pos] = m->m_texture;

	m->health = MONSTER_HP;
	m->x_pending = 0;
	m->y_pending = 0;

	/* pythagoras range from player */
	m->a_radius = 10;

	return m;
}

/* Free monster when 0 health */
monster *kill_Monster (char **map, monster *m) {

	map[m->y_pos][m->x_pos] = m->stand_texture;
	free(m);
	return NULL;
}

void mon_Position(char **map, monster *m) {

	int x_old = m->x_pos;
	int y_old = m->y_pos;
	if (m->x_pending != 0) {
		if (m->x_pending == 1) {
			++m->x_pos;
			--m->x_pending;
		} else {
			--m->x_pos;
			++m->x_pending;
		}
	}
	if (m->y_pending != 0) {
		if (m->y_pending == 1) {
			++m->y_pos;
			--m->y_pending;
		} else {
			--m->y_pos;
			++m->y_pending;
		}
	}

	map[y_old][x_old] = m->stand_texture;
	m->stand_texture = map[m->y_pos][m->x_pos];
	map[m->y_pos][m->x_pos] = m->m_texture;
}

int check_Radius(monster *m, character *c) {

	if (sqrt((m->x_pos - c->x_pos) * (m->x_pos - c->x_pos) +
	    (m->y_pos - c->y_pos) * (m->y_pos - c->y_pos)) <= m->a_radius) {
		return ATTACK;
	} else {
		return PASSIVE; 
	}
}

void mon_Follow(char **map, monster *m, character *c) {

	int move = 0;

	/* monster to the left of character */

	if (c->x_pos - m->x_pos > 1) {

		/* Check if monster (or obstacle) infront of current monster */
		if (map[m->y_pos][m->x_pos + 1] != 'M') {
			++m->x_pending;
			move = 1;

			/* monster next to monster next to character */
		} else if (map[m->y_pos][m->x_pos + 2] == 'C') {

			/* go up then right */
			if (map[m->y_pos - 1][m->x_pos + 1] != 'M' && map[m->y_pos - 1][m->x_pos] != 'M') {
				--m->y_pending;
				++m->x_pending;
				move = 1;

			/* else go down then right */
			} else if (map[m->y_pos + 1][m->x_pos + 1] != 'M' && map[m->y_pos + 1][m->x_pos] != 'M') {
				++m->y_pending;
				++m->x_pending;
				move = 1;
			}

		/* if monster to right but not right up */
		} else if (map[m->y_pos - 1][m->x_pos + 1] != 'M' && map[m->y_pos - 1][m->x_pos] != 'M') {
			--m->y_pending;
			move = 1;

		/* else if not right down */
		} else if (map[m->y_pos + 1][m->x_pos + 1] != 'M' && map[m->y_pos + 1][m->x_pos] != 'M') {
			++m->y_pending;
			move = 1;
		}

	/* monster to the right of character */	
	} else if (c->x_pos - m->x_pos < -1) {
		
		/* check if monster (or obstacle) infront of current monster */
		if (map[m->y_pos][m->x_pos - 1] != 'M') {
			--m->x_pending;
			move = 1;
		
		/* monster next to monster next to character */
		} else if (map[m->y_pos][m->x_pos - 2] == 'C') {

			/* go up then left */
			if (map[m->y_pos - 1][m->x_pos - 1] != 'M' && map[m->y_pos - 1][m->x_pos] != 'M') {
				--m->y_pending;
				--m->x_pending;
				move = 1;

			/* else go down then left */
			} else if (map[m->y_pos + 1][m->x_pos - 1] != 'M' && map[m->y_pos + 1][m->x_pos] != 'M') {
				++m->y_pending;
				--m->x_pending;	
				move = 1;			
			}

		/* if monster to left but not left up */
 		} else if (map[m->y_pos - 1][m->x_pos - 1] != 'M' && map[m->y_pos - 1][m->x_pos] != 'M') {
 			--m->y_pending;
 			move = 1;
 		
		/* else if not left down */
 		} else if (map[m->y_pos + 1][m->x_pos - 1] != 'M' && map[m->y_pos + 1][m->x_pos] != 'M') {
 			++m->y_pending;
 			move = 1;
 		}
	}

	/* monster move down */
	if (c->y_pos - m->y_pos > 1 && move == 0) {

		/* If no obstacle, move down */
		if (map[m->y_pos + 1][m->x_pos] != 'M') {
			++m->y_pending;
		
		/* if character down 2 steps */
		} else if (map[m->y_pos + 2][m->x_pos] == 'C') {

			/* go right-down */
			if (map[m->y_pos + 1][m->x_pos + 1] != 'M' && map[m->y_pos][m->x_pos + 1] != 'M') {
				++m->x_pending;
				++m->y_pending;
			
			/* else go left-down */
			} else if (map[m->y_pos + 1][m->x_pos - 1] != 'M' && map[m->y_pos][m->x_pos - 1] != 'M') {
				--m->x_pending;
				++m->y_pending;
			}
	
		/* if monster down but not right */
		} else if (map[m->y_pos + 1][m->x_pos + 1] != 'M' && map[m->y_pos][m->x_pos + 1] != 'M') {
			++m->x_pending;
		
		/* else if monster down but not left */
		} else if (map[m->y_pos + 1][m->x_pos - 1] != 'M' && map[m->y_pos][m->x_pos - 1] != 'M') {
			--m->x_pending;
		}

	/* monster move up */
	} else if (c->y_pos - m->y_pos < -1 && move == 0) {

		/* if no obstacle, move up */
		if (map[m->y_pos - 1][m->x_pos] != 'M') {
			--m->y_pending;

		/* if character up 2 steps */	
		} else if (map[m->y_pos - 2][m->x_pos] == 'C') {

			/* go right-up */
			if (map[m->y_pos - 1][m->x_pos + 1] != 'M' && map[m->y_pos][m->x_pos + 1] != 'M') {
				++m->x_pending;
				--m->y_pending;

			/* else go left-up */
			} else if (map[m->y_pos - 1][m->x_pos - 1] != 'M' && map[m->y_pos][m->x_pos - 1] != 'M') {
				--m->x_pending;
				--m->y_pending;
			}

		/* if monster up but not right */	
		} else if (map[m->y_pos - 1][m->x_pos + 1] != 'M' && map[m->y_pos][m->x_pos + 1] != 'M') {
			++m->x_pending;

		/* if monster up but not left */	
		} else if (map[m->y_pos - 1][m->x_pos - 1] != 'M' && map[m->y_pos][m->x_pos - 1] != 'M') {
			--m->x_pending;
		}
	}
} 