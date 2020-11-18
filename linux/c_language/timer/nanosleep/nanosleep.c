#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

static void signal_handler(int signal)
{
	static int count = 10;

	printf("SIGALRM received\n");

	if (--count <= 0)
		exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	struct timespec tspec = {
		.tv_sec = 1,
		.tv_nsec = 0
	};

	if (sigaction(SIGALRM, NULL, &act))
		error_en("Get act failed");

	act.sa_handler = signal_handler;

	if (sigaction(SIGALRM, &act, NULL))
		error_en("Set new act failed");

	while (1) {
		nanosleep(&tspec, NULL);
		raise(SIGALRM);
	}

	return 0;
}
