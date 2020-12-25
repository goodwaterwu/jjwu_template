#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

static struct winsize ws;

static void signal_handler(int signal)
{
	struct winsize new_ws;

	printf("SIGWINCH received\n");

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &new_ws) == -1)
		error_en("Get window size failed");

	printf("----- new window size -----\n");
	printf("row: %u\n", ws.ws_row);
	printf("column: %u\n", ws.ws_col);
	printf("xpixel: %u\n", ws.ws_xpixel);
	printf("ypixel: %u\n", ws.ws_ypixel);
	printf("---------------------------\n");
	sleep(1);

	/* This will not really work in Linux. */
	if (ioctl(STDIN_FILENO, TIOCSWINSZ, &ws) == -1)
		error_en("Set window size failed");
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	struct sigaction old_act;

	if (sigaction(SIGWINCH, NULL, &old_act))
		error_en("Get old act failed");

	act = old_act;
	act.sa_handler = signal_handler;
	act.sa_flags |= SA_RESETHAND;

	if (sigaction(SIGWINCH, &act, NULL) == -1)
		error_en("Set new act failed");

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1)
		error_en("Get window size failed");

	printf("----- original window size -----\n");
	printf("row: %u\n", ws.ws_row);
	printf("column: %u\n", ws.ws_col);
	printf("xpixel: %u\n", ws.ws_xpixel);
	printf("ypixel: %u\n", ws.ws_ypixel);
	printf("--------------------------------\n");
	pause();
	sleep(1);

	return 0;
}
