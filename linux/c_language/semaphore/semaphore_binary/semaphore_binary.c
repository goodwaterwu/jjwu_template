#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

union semun {
	int val; /* Value for SETVAL */
	struct semid_ds *buf; /* Buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /* Array for GETALL, SETALL */
	struct seminfo *__buf; /* Buffer for IPC_INFO (Linux-specific) */
};

int semaphore_init(key_t key)
{
	int id = 0;
	int flags = IPC_CREAT;
	int perms = S_IRUSR | S_IWUSR;
	union semun un;

	id = semget(key, 1, flags | perms);
	if (id == -1) {
		perror("Create semaphore failed");
		return id;
	}

	un.val = 1;
	if (semctl(id, 0, SETVAL, un) == -1)
		perror("Init semaphore failed");

	printf("Semaphore id: %d\n", id);

	return id;
}

void semaphore_remove(int id)
{
	if (semctl(id, 0, IPC_RMID, NULL) == -1)
		perror("Delete semaphore id failed");
}

bool semaphore_lock(int id)
{
	struct sembuf buf = {
		.sem_num = 0,
		.sem_op = -1,
		.sem_flg = SEM_UNDO
	};

	if (semop(id, &buf, 1) == -1) {
		semaphore_remove(id);
		perror("Semaphore lock failed");
		return false;
	}

	return true;
}

bool semaphore_unlock(int id)
{
	struct sembuf buf = {
		.sem_num = 0,
		.sem_op = 1,
		.sem_flg = SEM_UNDO
	};

	if (semop(id, &buf, 1) == -1) {
		semaphore_remove(id);
		perror("Semaphore unlock failed");
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{
	char buf[32] = {0};
	int id = 0;
	key_t key = 0;
	char *tmp = "/tmp/sem-XXXXXX";

	memcpy(buf, tmp, strlen(tmp));

	if (mkstemp(buf) == -1)
		error_en("Create sem file failed");

	printf("sem file: %s\n", buf);

	key = ftok(buf, 1);
	if (key == -1)
		error_en("Create key failed");

	id = semaphore_init(key);
	if (id < 0)
		exit(EXIT_FAILURE);

	printf("semaphore inited\n");

	if (!semaphore_lock(id))
		exit(EXIT_FAILURE);

	printf("semaphore locked\n");

	if (!semaphore_unlock(id))
		exit(EXIT_FAILURE);

	printf("semaphore unlocked\n");

	semaphore_remove(id);

	printf("semaphore removed\n");

	return 0;
}
