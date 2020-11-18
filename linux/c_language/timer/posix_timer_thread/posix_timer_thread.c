#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define error_log(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

static void thread_timer(union sigval value)
{
	static int count = 10;

	printf("%s\n", (char *)value.sival_ptr);
	if (--count <= 0)
		exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	timer_t timerid;
	struct itimerspec tspec = {
		.it_interval = {
			.tv_sec = 1,
			.tv_nsec = 0
		},
		.it_value = {
			.tv_sec = 1,
			.tv_nsec = 0
		}
	};
	struct sigevent evt = {
		.sigev_notify = SIGEV_THREAD,
		.sigev_value.sival_ptr = "Timer works",
		._sigev_un._sigev_thread._function = thread_timer
	};

	if (timer_create(CLOCK_REALTIME, &evt, &timerid))
		error_log("Create timer failed");

	if (timer_settime(timerid, 0, &tspec, NULL))
		error_log("Set timer failed");

	pause();

	return 0;
}
