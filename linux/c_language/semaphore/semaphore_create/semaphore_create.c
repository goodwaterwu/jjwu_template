#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

int main(int argc, char *argv[])
{
	char buf[32] = {0};
	int id = 0;
	int flags = IPC_CREAT;
	int perms = S_IRUSR | S_IWUSR;
	key_t key = 0;
	char *tmp = "/tmp/sem-XXXXXX";

	memcpy(buf, tmp, strlen(tmp));

	if (mkstemp(buf) == -1)
		error_en("Create sem file failed");

	printf("sem file: %s\n", buf);

	key = ftok(buf, 1);
	if (key == -1)
		error_en("Create key failed");

	id = semget(key, 1, flags | perms);
	if (id == -1)
		error_en("Create semaphore failed");

	printf("Semaphore id: %d\n", id);

	if (semctl(id, 0, IPC_RMID, NULL) == -1)
		error_en("Delete semaphore id failed");

	return 0;
}
