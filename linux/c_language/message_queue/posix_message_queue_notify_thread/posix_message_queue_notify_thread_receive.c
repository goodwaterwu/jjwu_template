#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)
#define MQ "/mq"

static void setup_notify(mqd_t *mqd);

static void notify_thread(union sigval sv)
{
	char buf[64] = {0};
	unsigned int prio = 0;
	mqd_t *mqd = sv.sival_ptr;
	struct mq_attr attr;

	if (!mqd) {
		printf("mqd is NULL\n");
		exit(EXIT_FAILURE);
	}

	if (mq_getattr(*mqd, &attr) == -1)
		error_en("Get attribute failed");

	setup_notify(mqd);

	while (1) {
		memset(buf, 0, sizeof(buf));
		while (mq_receive(*mqd, buf, (size_t)attr.mq_msgsize, &prio) >= 0)
			printf("Received message: %s\n", buf);

		break;
	}

	if (mq_close(*mqd) == -1)
		error_en("Message queue close failed");

	printf("Message queue closed\n");

	if (mq_unlink(MQ) == -1)
		error_en("Message queue unlink failed");

	printf("Message queue unlinked\n");
	pthread_exit(NULL);
}

static void setup_notify(mqd_t *mqd)
{
	struct sigevent event = {
		.sigev_notify =  SIGEV_THREAD,
		.sigev_notify_function = notify_thread,
		.sigev_notify_attributes = NULL,
		.sigev_value.sival_ptr = mqd
	};

	if (mq_notify(*mqd, &event) == -1)
		error_en("Set notify failed");
}

int main(int argc, char *argv[])
{
	int flags = O_CREAT | O_RDWR | O_NONBLOCK;
	mode_t perms = S_IRUSR | S_IWUSR;
	mqd_t mqd;

	mqd = mq_open(MQ, flags, perms, NULL);
	if (mqd == -1)
		error_en("Message queue create failed");

	printf("Message queue created\n");

	setup_notify(&mqd);
	pause();

	return 0;
}
