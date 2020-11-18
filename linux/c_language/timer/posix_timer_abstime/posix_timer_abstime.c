#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

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
	timer_t timerid;
	struct sigaction act;
	struct timeval tval;
	struct timezone tz;
	struct itimerspec tspec;
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

	if (gettimeofday(&tval, &tz))
		error_en("Get time failed");

	tspec.it_interval.tv_sec = tval.tv_sec + 3;
	tspec.it_value = tspec.it_interval;

	if (timer_settime(timerid, TIMER_ABSTIME, &tspec, NULL))
		error_en("Set timer failed");

	pause();

	return 0;
}
