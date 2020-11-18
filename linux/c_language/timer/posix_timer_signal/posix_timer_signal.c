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
	printf("SIGALRM received\n");
}

int main(int argc, char *argv[])
{
	static int count = 10;
	timer_t timerid;
	struct sigaction act;
	struct itimerspec tspec = {
		.it_interval = {
			.tv_sec = 1,
			.tv_nsec = 0
		},
		.it_value = {
			.tv_sec = 0,
			.tv_nsec = 1
		}
	};
	struct sigevent evt = {
		.sigev_notify = SIGEV_SIGNAL,
		.sigev_signo = SIGALRM
	};

	if (sigaction(SIGALRM, NULL, &act))
		error_en("Get old act failed");

	act.sa_handler = signal_handler;

	if (sigaction(SIGALRM, &act, NULL))
		error_en("Set new act failed");

	if (timer_create(CLOCK_REALTIME, &evt, &timerid))
		error_en("Create timer failed");

	if (timer_settime(timerid, 0, &tspec, NULL))
		error_en("Set timer failed");

	while (count-- > 0) {
		sleep(1);
		if (count == 5 && timer_delete(timerid))
			error_en("Delete timer failed");
	}

	return 0;
}
