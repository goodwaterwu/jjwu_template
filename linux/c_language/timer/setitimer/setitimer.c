#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

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
	struct itimerval itimer = {
		.it_interval = {
			.tv_sec = 1,
			.tv_usec = 0
		},
		.it_value = {
			.tv_sec = 1,
			.tv_usec = 0
		}
	};

	if (sigaction(SIGALRM, NULL, &act))
		error_en("Get act failed");

	act.sa_handler = signal_handler;

	if (sigaction(SIGALRM, &act, NULL))
		error_en("Set new act failed");

	if (setitimer(ITIMER_REAL, &itimer, NULL))
		error_en("Start itimer failed");

	while (1)
		sleep(1);

	return 0;
}
