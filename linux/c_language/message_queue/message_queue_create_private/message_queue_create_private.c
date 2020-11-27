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

int main(int argc, char *argv[])
{
	int id = 0;
	int flags = IPC_CREAT;
	int perms = S_IRUSR | S_IWUSR | S_IWGRP;

	id = msgget(IPC_PRIVATE, flags | perms);
	if (id == -1)
		error_en("Create message queue failed");

	printf("Message queue id: %d\n", id);

	if (msgctl(id, IPC_RMID, NULL) == -1)
		error_en("Delete message queue id failed");

	return 0;
}
