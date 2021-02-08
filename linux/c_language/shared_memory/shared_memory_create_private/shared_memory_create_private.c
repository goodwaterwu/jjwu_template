#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define SHM_SIZE 1024

int main(int argc, char *argv[])
{
	int id = 0;
	int flags = IPC_CREAT;
	int perms = S_IRUSR | S_IWUSR | S_IWGRP;
	void * addr = NULL;

	id = shmget(IPC_PRIVATE, SHM_SIZE, flags | perms);
	if (id == -1)
		error_en("Create shared memory failed");

	printf("Shared memory id: %d\n", id);

	addr = shmat(id, NULL, 0);
	if (addr == (void *)-1)
		perror("Attach shared memory failed");

	if (shmdt(addr) == -1)
		perror("Detach shared memory failed");

	if (shmctl(id, IPC_RMID, NULL) == -1)
		error_en("Delete shared memory id failed");

	return 0;
}
