#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

int main(int argc, char *argv[])
{
	struct termios old_tm;
	struct termios tm;

	if (tcgetattr(STDIN_FILENO, &old_tm) == -1)
		error_en("Get terminal attribute failed");

	tm = old_tm;
	tm.c_cc[VINTR] = 0x2;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tm) == -1)
		error_en("Set terminal attribute failed");

	printf("Try to send new interrupt signal ^B\n");
	sleep(1);
	pause();

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_tm) == -1)
		error_en("Set terminal attribute failed");

	return 0;
}
