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

#define MSG "Hello message queue"

struct message {
	long mtype;
	char mtext[128];
};

int main(int argc, char *argv[])
{
	char buf[32] = {0};
	int id = 0;
	int flags = IPC_CREAT;
	int perms = S_IRUSR | S_IWUSR;
	key_t key = 0;
	char *tmp = "/tmp/mq-XXXXXX";
	struct message smsg;
	struct message rmsg;

	smsg.mtype = 1;
	memcpy(smsg.mtext, MSG, strlen(MSG));

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

	if (msgsnd(id, &smsg, strlen(smsg.mtext) + 1, 0) == -1) {
		perror("Send message failed");
	} else {
		if (msgrcv(id, &rmsg, sizeof(rmsg.mtext) - 1, 0, 0) == -1)
			perror("Receive message failed");
		else
			printf("Received: %s\n", rmsg.mtext);
	}

	if (msgctl(id, IPC_RMID, NULL) == -1)
		error_en("Delete message queue id failed");

	return 0;
}
