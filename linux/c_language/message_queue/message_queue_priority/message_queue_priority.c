#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

struct message {
	long mtype;
	char mtext[128];
};

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

int main(int argc, char *argv[])
{
	char buf[32] = {0};
	int id = 0;
	int flags = IPC_CREAT;
	int perms = S_IRUSR | S_IWUSR;
	size_t size = sizeof(msg) / sizeof(msg[0]);
	long priority = size;
	key_t key = 0;
	char *tmp = "/tmp/mq-XXXXXX";
	struct message smsg;
	struct message rmsg;

	memcpy(buf, tmp, strlen(tmp));

	if (mkstemp(buf) == -1)
		error_en("Create mq file failed");

	printf("mq file: %s\n", buf);

	key = ftok(buf, 1);
	if (key == -1)
		error_en("Create key failed");

	id = msgget(key, flags | perms);
	if (id == -1)
		error_en("Create message queue failed");

	printf("Message queue id: %d\n", id);

	for (size_t i = 0; i != size; i++) {
		smsg.mtype = priority--;
		memcpy(smsg.mtext, msg[i], strlen(msg[i]));
		if (msgsnd(id, &smsg, strlen(smsg.mtext) + 1, 0) == -1)
			perror("Send message failed");
	}

	for (size_t i = 0; i != 5; i++) {
		if (msgrcv(id, &rmsg, sizeof(rmsg.mtext) - 1, -5, 0) == -1)
			perror("Receive message failed");
		else
			printf("Received: %s\n", rmsg.mtext);
	}

	if (msgctl(id, IPC_RMID, NULL) == -1)
		error_en("Delete message queue id failed");

	return 0;
}
