#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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

static int id;
static pthread_t t1_id;
static pthread_t t2_id;
static char *msg[10] = {
	"Hello message 1",
	"Hello message 2",
	"Hello message 3",
	"Hello message 4",
	"Hello message 5",
	"Hello message 6",
	"Hello message 7",
	"Hello message 8",
	"Hello message 9",
	"Hello message 10",
};

struct message {
	long mtype;
	char mtext[128];
};

static void *thread_t1(void *arg)
{
	struct message rmsg;

	printf("Thread %zu created\n", (size_t)arg);

	t1_id = pthread_self();

	while (1) {
		if (msgrcv(id, &rmsg, sizeof(rmsg.mtext) - 1, t1_id, 0) == -1)
			perror("Receive message failed");
		else
			printf("t1 received: %s\n", rmsg.mtext);

		if (!strcmp(rmsg.mtext,
				msg[sizeof(msg) / sizeof(msg[0]) - 1])) {
			pthread_cancel(t2_id);
			pthread_exit(NULL);
		}
	}

	return NULL;
}

static void *thread_t2(void *arg)
{
	struct message rmsg;

	printf("Thread %zu created\n", (size_t)arg);

	t2_id = pthread_self();

	while (1) {
		if (msgrcv(id, &rmsg, sizeof(rmsg.mtext) - 1, t2_id, 0) == -1)
			perror("Receive message failed");
		else
			printf("t2 received: %s\n", rmsg.mtext);

		if (!strcmp(rmsg.mtext,
				msg[sizeof(msg) / sizeof(msg[0]) - 1])) {
			pthread_cancel(t1_id);
			pthread_exit(NULL);
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	char buf[32] = {0};
	int ret = 0;
	int flags = IPC_CREAT;
	int perms = S_IRUSR | S_IWUSR;
	key_t key = 0;
	pthread_t t[NUM_OF_THREAD];
	void *result = NULL;
	char *tmp = "/tmp/mq-XXXXXX";
	struct message smsg;

	memcpy(buf, tmp, strlen(tmp));

	if (mkstemp(buf) == -1)
		error_log("Create mq file failed");

	printf("mq file: %s\n", buf);

	key = ftok(buf, 1);
	if (key == -1)
		error_log("Create key failed");

	id = msgget(key, flags | perms);
	if (id == -1)
		error_log("Create message queue failed");

	printf("Message queue id: %d\n", id);

	ret = pthread_create(&t[0], NULL, thread_t1, (void *)0);
	if (ret)
		error_en(ret, "Thread create failed");

	ret = pthread_create(&t[1], NULL, thread_t2, (void *)1);
	if (ret)
		error_en(ret, "Thread create failed");

	while (!t1_id || !t2_id)
		sleep(1);

	for (size_t i = 0; i != sizeof(msg) / sizeof(msg[0]); i++) {
		if (i % 2 == 0)
			smsg.mtype = t1_id;
		else
			smsg.mtype = t2_id;

		memcpy(smsg.mtext, msg[i], strlen(msg[i]));
		if (msgsnd(id, &smsg, strlen(smsg.mtext) + 1, 0) == -1)
			perror("Send message failed");
	}

	for (size_t i = 0; i != NUM_OF_THREAD; i++) {
		ret = pthread_join(t[i], &result);
		if (ret)
			error_en(ret, "Thread join failed");
	}

	if (msgctl(id, IPC_RMID, NULL) == -1)
		error_log("Delete message queue id failed");

	return 0;
}
