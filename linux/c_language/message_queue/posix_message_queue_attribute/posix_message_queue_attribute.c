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

void dump_attribute(struct mq_attr *attr)
{
	if (attr) {
		printf("--------------------\n");
		printf("mq_flags: 0x%lx\n", attr->mq_flags);
		printf("mq_maxmsg: 0x%lx\n", attr->mq_maxmsg);
		printf("mq_msgsize: 0x%lx\n", attr->mq_msgsize);
		printf("mq_curmsgs: 0x%lx\n", attr->mq_curmsgs);
		printf("--------------------\n");
	}
}

int main(int argc, char *argv[])
{
	int flags = O_CREAT | O_RDWR;
	mode_t perms = S_IRUSR | S_IWUSR;
	mqd_t mqd = 0;
	struct mq_attr attr = {
		.mq_maxmsg = 10,
		.mq_msgsize = 1024
	};
	struct mq_attr attr_r;

	mqd = mq_open(MQ, flags, perms, &attr);
	if (mqd == -1)
		error_en("Message queue create failed");

	printf("Message queue created\n");

	if (mq_getattr(mqd, &attr_r) == -1)
		error_en("Get attribute failed");

	dump_attribute(&attr_r);
	attr.mq_flags |= O_NONBLOCK;

	if (mq_setattr(mqd, &attr, NULL) == -1)
		error_en("Set attribute failed");

	if (mq_getattr(mqd, &attr_r) == -1)
		error_en("Get attribute failed");

	dump_attribute(&attr_r);

	if (mq_close(mqd) == -1)
		error_en("Message queue close failed");

	printf("Message queue closed\n");

	if (mq_unlink(MQ) == -1)
		error_en("Message queue unlink failed");

	printf("Message queue unlinked\n");

	return 0;
}
