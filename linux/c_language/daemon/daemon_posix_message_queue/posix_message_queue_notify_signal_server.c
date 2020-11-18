#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <sys/stat.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define MQ "/mq"

static mqd_t mqd;

static void signal_handler(int signal)
{
	if (signal == SIGUSR1)
		printf("SIGUSR1 received\n");
	else {
		printf("SIGTERM received\n");
		if (mq_close(mqd) == -1)
			error_en("Message queue close failed");

		printf("Message queue closed\n");

		if (mq_unlink(MQ) == -1)
			error_en("Message queue unlink failed");

		printf("Message	queue unlinked\n");
	}
}

int main(int argc, char *argv[])
{
	char buf[64] = {0};
	int ret = 0;
	unsigned int prio = 0;
	int flags = O_CREAT | O_RDWR | O_NONBLOCK;
	mode_t perms = S_IRUSR | S_IWUSR;
	sigset_t new_set;
	struct sigaction act;
	struct sigaction old_act;
	struct sigevent event = {
		.sigev_notify = SIGEV_SIGNAL,
		.sigev_signo = SIGUSR1
	};
	struct mq_attr attr_r;

	if (daemon(0, 1) == -1)
		error_en("Daemonize failed");

	ret = sigemptyset(&new_set);
	if (ret)
		error_en("Empty new_set failed");

	ret = sigaddset(&new_set, SIGUSR1);
	if (ret)
		error_en("Add to new_set failed");

	ret = sigprocmask(SIG_BLOCK, &new_set, NULL);
	if (ret)
		error_en("Mask signal failed");

	if (sigaction(SIGUSR1, NULL, &old_act))
		error_en("Get old act failed");

	act = old_act;
	sigemptyset(&act.sa_mask);
	act.sa_handler = signal_handler;

	if (sigaction(SIGTERM, &act, NULL))
		error_en("Set new act failed");

	if (sigaction(SIGUSR1, &act, NULL))
		error_en("Set new act failed");

	mqd = mq_open(MQ, flags, perms, NULL);
	if (mqd == -1)
		error_en("Message queue create failed");

	printf("Message queue created\n");

	if (mq_getattr(mqd, &attr_r) == -1)
		error_en("Get attribute failed");

	if (mq_notify(mqd, &event) == -1)
		error_en("Set notify failed");

	ret = sigemptyset(&new_set);
	if (ret)
		error_en("Empty new_set failed");

	while (1) {
		sigsuspend(&new_set);

		if (mq_notify(mqd, &event) == -1)
			error_en("Set notify failed");

		memset(buf, 0, sizeof(buf));
		while (mq_receive(mqd, buf, (size_t)attr_r.mq_msgsize, &prio) >= 0) 
			printf("Received message: %s\n", buf);
	}

	if (mq_close(mqd) == -1)
		error_en("Message queue close failed");

	printf("Message queue closed\n");

	if (mq_unlink(MQ) == -1)
		error_en("Message queue unlink failed");

	printf("Message queue unlinked\n");

	return 0;
}
