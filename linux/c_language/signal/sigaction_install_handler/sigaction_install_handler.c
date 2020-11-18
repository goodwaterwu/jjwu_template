#include <stdio.h>
#include <string.h>
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
	struct sigaction act;
	struct sigaction old_act;

	if (sigaction(SIGINT, NULL, &old_act))
		error_en("Get old act failed");

	act = old_act;
	act.sa_handler = signal_handler;
	act.sa_flags |= SA_RESETHAND;

	if (sigaction(SIGINT, &act, NULL))
		error_en("Set new act failed");

	for (int i = 10; i != 0; i--) {
		printf("Process will end in %02d\n", i);
		sleep(1);
	}

	return 0;
}
