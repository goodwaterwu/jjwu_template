#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

int tty_cbreak(int fd, bool echo)
{
	struct termios tm;

	if (tcgetattr(fd, &tm) == -1)
		perror("Get terminal attribute failed");

	if (echo)
		tm.c_lflag &= ~ICANON;
	else
		tm.c_lflag &= ~(ICANON | ECHO);

	tm.c_lflag |= ISIG;
	tm.c_iflag &= ~ICRNL;
	tm.c_cc[VMIN] = 1;
	tm.c_cc[VTIME] = 0;

	if (tcsetattr(fd, TCSAFLUSH, &tm) == -1)
		perror("Set terminal attribute failed");

	return 0;
}

int main(int argc, char *argv[])
{
	struct termios tm;
	int fd = STDIN_FILENO;

	if (tcgetattr(fd, &tm) == -1)
		error_en("Get terminal attribute failed");

	if (tty_cbreak(fd, false))
		return EXIT_FAILURE;

	printf("Switch to cbreak mode without echo\n");
	sleep(10);

	if (tcsetattr(fd, TCSAFLUSH, &tm) == -1)
		error_en("Set terminal attribute failed");

	if (tty_cbreak(fd, true))
		return EXIT_FAILURE;

	printf("Switch to cbreak mode with echo\n");
	sleep(10);

	if (tcsetattr(fd, TCSAFLUSH, &tm) == -1)
		error_en("Set terminal attribute failed");

	printf("Back to cooked mode\n");

	return 0;
}
