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

int main(int argc, char *argv[])
{
	int flags = O_CREAT | O_RDWR;
	mode_t perms = S_IRUSR | S_IWUSR;
	mqd_t mqd = 0;

	mqd = mq_open(MQ, flags, perms, NULL);
	if (mqd == -1)
		error_en("Message queue create failed");

	printf("Message queue created\n");

	if (mq_close(mqd) == -1)
		error_en("Message queue close failed");

	printf("Message queue closed\n");

	if (mq_unlink(MQ) == -1)
		error_en("Message queue unlink failed");

	printf("Message queue unlinked\n");

	return 0;
}
