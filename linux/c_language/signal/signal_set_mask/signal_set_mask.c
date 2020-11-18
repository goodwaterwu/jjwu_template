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

void dump_signal_set(sigset_t *set)
{
	for (int i = 0; i != NSIG; i++) {
		if (sigismember(set, i))
			printf("%d: ON\n", i);
		else
			printf("%d: OFF\n", i);
	}
}

int main(int argc, char *argv[])
{
	int ret = 0;
	sigset_t new_set;
	sigset_t old_set;
	sigset_t pend_set;

	if (signal(SIGINT, signal_handler) == SIG_ERR)
		error_en("Set signal handler failed");
	else
		printf("Use signal_handler\n");

	ret = sigemptyset(&new_set);
	if (ret)
		error_en("Empty new_set failed");

	ret = sigaddset(&new_set, SIGINT);
	if (ret)
		error_en("Add to new_set failed");

	ret = sigprocmask(SIG_BLOCK, &new_set, &old_set);
	if (ret)
		error_en("Mask signal failed");

	for (int i = 0; i != 5; i++) {
		raise(SIGINT);
		sleep(1);
	}

	printf("++++++++++++++++++++\n");
	printf("New set\n");
	printf("++++++++++++++++++++\n");
	dump_signal_set(&new_set);
	printf("++++++++++++++++++++\n");
	printf("Old set\n");
	printf("++++++++++++++++++++\n");
	dump_signal_set(&old_set);

	ret = sigemptyset(&pend_set);
	if (ret)
		error_en("Empty pend_set failed");

	ret = sigpending(&pend_set);
	if (ret)
		error_en("Get pending set failed");

	printf("++++++++++++++++++++\n");
	printf("Pending set\n");
	printf("++++++++++++++++++++\n");
	dump_signal_set(&pend_set);

	ret = sigprocmask(SIG_UNBLOCK, &new_set, &old_set);
	if (ret)
		error_en("Unmask signal failed");

	ret = sigprocmask(SIG_SETMASK, &old_set, NULL);
	if (ret)
		error_en("Set mask failed");

	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		error_en("Set default handler failed");
	else
		printf("Default handler\n");

	return 0;
}
