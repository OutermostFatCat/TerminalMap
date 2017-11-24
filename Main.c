#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "Map.h"

struct timespec delay;

monster *mon[10];

int main()
{	

	character man;
	man.x_pos = 100;
	man.y_pos = 100;
	man.x_pending = 0;
	man.y_pending = 0;
	man.c_x = 0;
	man.c_y = 0;
	man.attack = 0;
	man.stand_texture = 'S';
	man.char_texture = 'C';

	/* FPS = 10 */
	delay.tv_sec = 0;
	delay.tv_nsec = 100000000;

	/* allocate map and scren, start up map */
	char **map = alloc_Map();
	char **screen = alloc_Screen();
	write_Map(map, 0, 0, 199, 199);

	int i = 0;
	while (i < 10) {
		mon[i++] = gen_Monster(map, &man);
	}
	i = 0;

    character_pos(map, &man);

	/* start up the thread for movement and initilaize raw input */
	pthread_t *thread = mov_Initialize(&man);
	new_Inputmode();

	int y;
	while (man.lock) {

		write_Screen(map, screen, &man);
		for (printf("\n\n"), y = 0; screen[y] != NULL; ++y) {
	
			/* print screen */
			printf("%s\n", screen[y]);
		}
		printf("x:%d\ty:%d\n", man.c_x, man.c_y);
		nanosleep(&delay, NULL);

		character_pos(map, &man);
		if (man.attack > 0) {
			character_Attack(map, &man, mon);
		}

		while (i < 10) {
			if (mon[i] != NULL) {
				if (check_Radius(mon[i], &man) == ATTACK) {
					mon_Follow(map, mon[i], &man);
					mon_Position(map, mon[i]);
				}
			}
			++i;
		}
		i = 0;
		map = update_Map(map, &man);
	}

	/* clean up */
	old_Inputmode();
	thread = mov_exit(thread);
	map = free_Map(map);
	screen = free_Screen(screen);
	return 0;
}
