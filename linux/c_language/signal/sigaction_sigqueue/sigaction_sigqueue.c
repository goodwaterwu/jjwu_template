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
	static size_t count = 0;

	printf("\rSIGINT received: %zu\n", ++count);
}

int main(int argc, char *argv[])
{
	union sigval sv = { .sival_int = 100 };
	struct sigaction act;
	struct sigaction old_act;

	if (sigaction(SIGINT, NULL, &old_act))
		error_en("Get old act failed");

	act = old_act;
	act.sa_handler = signal_handler;

	if (sigaction(SIGINT, &act, NULL))
		error_en("Set new act failed");

	for (int i = 0; i != 3; i++) {
		if (sigqueue(getpid(), SIGINT, sv) == -1)
			error_en("Send realtime signal failed");

		sleep(1);
	}

	return 0;
}
