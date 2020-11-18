#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define error_log(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

static void signal_handler(int signal)
{
	printf("SIGINT received\n");
}

int main(int argc, char *argv[])
{
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		error_log("Set signal handler failed");

	for (int i = 0; i != 10; i++) {
		printf("Use signal_handler\n");
		sleep(1);
	}

	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		error_log("Set ignore signal failed");

	for (int i = 0; i != 10; i++) {
		printf("Ignore signal\n");
		sleep(1);
	}

	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		error_log("Set default handler failed");

	for (int i = 0; i != 10; i++) {
		printf("Default handler\n");
		sleep(1);
	}

	return 0;
}
