#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define error_en(en, msg) \
	do { \
		errno = en; \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define error_log(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define NUM_OF_THREAD 2
#define SIG SIGUSR1

static pthread_t tid;

static void signal_handler(int sig, siginfo_t *si, void *ucontext)
{
	char *code = NULL;

	printf("\r[%s] si_signo: %d\n", __func__, si->si_signo);

	if (si->si_code == SI_USER)
		code = "SI_USER";
	else if (si->si_code == SI_QUEUE)
		code = "SI_QUEUE";
	else
		code = "others";

	printf("[%s] si_code: %s\n", __func__, code);
	printf("[%s] sival_int: %d\n", __func__, si->si_value.sival_int);
	printf("[%s] si_errno: %d\n", __func__, si->si_errno);
	printf("[%s] si_pid: %d\n", __func__, si->si_pid);
	printf("[%s] si_uid: %d\n", __func__, si->si_uid);
}

static void *thread_t1(void *arg)
{
	sigset_t mask;
	siginfo_t siginfo;
	struct sigaction act;
	struct sigaction old_act;

	tid = pthread_self();

	sigemptyset(&mask);
	sigaddset(&mask, SIG);
	if (pthread_sigmask(SIG_UNBLOCK, &mask, NULL) == -1)
		error_log("Set Mask failed");

	if (sigaction(SIG, NULL, &old_act))
		error_log("Get old act failed");

	act = old_act;
	act.sa_flags |= SA_SIGINFO;
	act.sa_sigaction = signal_handler;

	if (sigaction(SIG, &act, NULL))
		error_log("Set new act failed");

	for (int i = 0; i != 3; i++) {
		char *code = NULL;

		printf("Wait signal...\n");
		if (sigwaitinfo(&mask, &siginfo) == -1)
			error_log("Wait signal info failed");

		printf("\r[%s] si_signo: %d\n", __func__, siginfo.si_signo);

		if (siginfo.si_code == SI_USER)
			code = "SI_USER";
		else if (siginfo.si_code == SI_QUEUE)
			code = "SI_QUEUE";
		else
			code = "others";

		printf("[%s] si_code: %s\n", __func__, code);
		printf("[%s] sival_int: %d\n", __func__, siginfo.si_value.sival_int);
		printf("[%s] si_errno: %d\n", __func__, siginfo.si_errno);
		printf("[%s] si_pid: %d\n", __func__, siginfo.si_pid);
		printf("[%s] si_uid: %d\n", __func__, siginfo.si_uid);
	}

	pthread_exit(NULL);
}

static void *thread_t2(void *arg)
{
	union sigval sv = { .sival_int = 100 };

	for (int i = 0; i != 3; i++) {
		if (pthread_sigqueue(tid, SIG, sv) == -1)
			error_log("Send realtime signal failed");

		sleep(1);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int ret = 0;
	pthread_t t[NUM_OF_THREAD];
	sigset_t mask;

	sigfillset(&mask);
	if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
		error_log("Set Mask failed");

	ret = pthread_create(&t[0], NULL, thread_t1, (void *)0);
	if (ret)
		error_en(ret, "Thread create failed");

	sleep(1);

	ret = pthread_create(&t[1], NULL, thread_t2, (void *)1);
	if (ret)
		error_en(ret, "Thread create failed");

	for (size_t i = 0; i != NUM_OF_THREAD; i++) {
		ret = pthread_join(t[i], NULL);
		if (ret)
			error_en(ret, "Thread join failed");
	}

	exit(EXIT_SUCCESS);
}
