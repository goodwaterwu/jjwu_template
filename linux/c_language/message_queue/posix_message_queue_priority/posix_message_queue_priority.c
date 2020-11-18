#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define MQ "/mq"

static char *msg[] = {
	"Hello message 1",
	"Hello message 2",
	"Hello message 3",
	"Hello message 4",
	"Hello message 5",
};

int main(int argc, char *argv[])
{
	char buf[64] = {0};
	unsigned int prio = 0;
	int flags = O_CREAT | O_RDWR;
	size_t size = sizeof(msg) / sizeof(msg[0]);
	mode_t perms = S_IRUSR | S_IWUSR;
	mqd_t mqd = 0;
	struct mq_attr attr_r;

	mqd = mq_open(MQ, flags, perms, NULL);
	if (mqd == -1)
		error_en("Message queue create failed");

	printf("Message queue created\n");

	if (mq_getattr(mqd, &attr_r) == -1)
		error_en("Get attribute failed");

	for (size_t i = 0; i != size; i++) {
		if (mq_send(mqd, msg[i],
				strlen(msg[i]) + 1, i) == -1) {
			error_en("Send message failed");
		}

		printf("Send message: %s priority: %lu\n", msg[i], i);

	}

	for (size_t i = 0; i != size; i++) {
		memset(buf, 0, sizeof(buf));
		if (mq_receive(mqd, buf,
				(size_t)attr_r.mq_msgsize, &prio) == -1) {
			error_en("Receive message failed");
		}

		printf("Received message: %s priority: %u\n", buf, prio);

	}

	if (mq_close(mqd) == -1)
		error_en("Message queue close failed");

	printf("Message queue closed\n");

	if (mq_unlink(MQ) == -1)
		error_en("Message queue unlink failed");

	printf("Message queue unlinked\n");

	return 0;
}
