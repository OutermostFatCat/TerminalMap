#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"

/* Movement lock */
pthread_mutex_t lock_Movement;

void movement(character *man) {

	/* buffer for scancode */
	char check_true = 1;
	char buffer[1];
	while (check_true) {
		
		/* Load scancode to buffer */
		read_Scancode(buffer);
		switch(buffer[0]) {

			/* a: left */
			case  0x61:
				--man->x_pending;
				break;

				/* d: right */
			case 0x64:
				++man->x_pending;
				break;

				/* s: down */
			case 0x73:
				++man->y_pending;
				break;

				/* w: up */
			case 0x77:
				--man->y_pending;
				break;

				/* done */
			default:
				man->lock = 0;
				check_true = 0;
				break;
		}			
	}
}

/* Locks the movement lock and Keep sending 
   positiondata for the character
*/
void *thr_Mov(void *arg) {

	character *man = (character *) arg;
	man->lock = 1;
	pthread_mutex_lock(&lock_Movement);
	movement(man);
	pthread_mutex_unlock(&lock_Movement);
	pthread_exit(NULL);
}

/* Initalizes and creates a thread for movement */
pthread_t *mov_Initialize(character *man) {

	int rc;
	pthread_t *thread = malloc(sizeof(pthread_t));

	/* Create thread for movement, returns 0 if successful */
	if ((rc = pthread_create(thread, NULL, thr_Mov, man))) {
     	fprintf(stderr, "ERROR: pthread_create, rc: %d\n", rc);
    	exit(0); 
    }

    return thread;
}

/* Clean up the thread */
void *mov_exit(pthread_t *thread) {
	pthread_join(*thread, NULL);
	free(thread);
	return NULL;
}