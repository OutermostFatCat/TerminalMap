
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <pthread.h>
#include "Map.h"

/* Canonical attributes */
static struct termios old_ttr;

void new_Inputmode() {

	/* Check if STDIN_FILENO is a file descriptor
	   referring to a terminal
    */
	if (!isatty(STDIN_FILENO)) {
	    printf("ERROR: STDIN_FILENO not ref to terminal");
	}
	/* Save old ttr values to old_ttr */
	if (tcgetattr(STDIN_FILENO, &old_ttr) != 0) {
		printf("ERROR: Didn't save terminal attributes to old_ttr");
	}

	struct termios new_ttr;

	/* Turn off char printing when key is pressed, and
	   turn on raw keyboard mode;
    */
    if (tcgetattr(STDIN_FILENO, &new_ttr) != 0) {
		printf("ERROR: Didn't save terminal attributes to old_ttr");
	}
	new_ttr.c_lflag &= ~(ICANON | ECHO);

	/* Initialize new attributes to terminal */
	tcsetattr(STDIN_FILENO, TCSANOW, &new_ttr);
}

/* Initialize terminal to old attributes */
void old_Inputmode() {

	tcsetattr(STDIN_FILENO, TCSANOW, &old_ttr);
}

/* read assigns scancode to buffer */
void read_Scancode(char buffer[]) {

	read(STDIN_FILENO, &buffer[0], 1);
}