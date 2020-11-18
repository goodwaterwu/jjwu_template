#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

static void signal_handler(int signal)
{
	printf("SIGALRM received\n");
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	struct sigaction act;

	if (sigaction(SIGALRM, NULL, &act))
		error_en("Get old act failed");

	act.sa_handler = signal_handler;

	if (sigaction(SIGALRM, &act, NULL))
		error_en("Set new act failed");

	alarm(5);

	while (1)
		sleep(1);

	return 0;
}
