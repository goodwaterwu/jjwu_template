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
	printf("SIGINT received\n");
}

int main(int argc, char *argv[])
{
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		error_en("Set signal handler failed");
	else
		printf("Use signal_handler\n");

	printf("Send SIGINT\n");
	raise(SIGINT);

	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		error_en("Set ignore signal failed");
	else
		printf("Ignore signal\n");

	printf("Send SIGINT\n");
	raise(SIGINT);

	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		error_en("Set default handler failed");
	else
		printf("Default handler\n");

	printf("Send SIGINT\n");
	raise(SIGINT);

	return 0;
}
