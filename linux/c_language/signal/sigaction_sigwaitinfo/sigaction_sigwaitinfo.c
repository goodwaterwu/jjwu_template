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

	printf("SIGINT received: %zu\n", ++count);
}

int main(int argc, char *argv[])
{
	sigset_t mask;
	siginfo_t siginfo;
	struct sigaction act;
	struct sigaction old_act;

	sigfillset(&mask);
	if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
		error_en("Set Mask failed");

	if (sigaction(SIGINT, NULL, &old_act))
		error_en("Get old act failed");

	act = old_act;
	act.sa_handler = signal_handler;

	if (sigaction(SIGINT, &act, NULL))
		error_en("Set new act failed");

	for (int i = 0; i != 3; i++) {
		char *code = NULL;

		printf("Wait SIGINT...\n");
		sigwaitinfo(&mask, &siginfo);
		printf("\rsi_signo: %d\n", siginfo.si_signo);

		if (siginfo.si_code == SI_USER)
			code = "SI_USER";
		else if (siginfo.si_code == SI_QUEUE)
			code = "SI_QUEUE";
		else
			code = "others";

		printf("si_code: %s\n", code);
		printf("sival_int: %d\n", siginfo.si_value.sival_int);
		printf("si_errno: %d\n", siginfo.si_errno);
		printf("si_pid: %d\n", siginfo.si_pid);
		printf("si_uid: %d\n", siginfo.si_uid);
	}

	return 0;
}
