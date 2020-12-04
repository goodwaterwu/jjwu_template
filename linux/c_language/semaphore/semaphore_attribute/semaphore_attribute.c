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

bool semaphore_printinfo(int id)
{
	struct semid_ds ds;
	struct seminfo info;
	union semun un_ds = { .buf = &ds };
	union semun un_info = { .__buf = &info };

	if (semctl(id, 0, IPC_STAT, un_ds) == -1) {
		semaphore_remove(id);
		perror("IPC_STAT failed");
		return false;
	}

	printf("===================================\n");
	printf("IPC_STAT\n");
	printf("===================================\n");
	printf("key: %d\n", un_ds.buf->sem_perm.__key);
	printf("uid: %d\n", un_ds.buf->sem_perm.uid);
	printf("gid: %d\n", un_ds.buf->sem_perm.gid);
	printf("cuid: %d\n", un_ds.buf->sem_perm.cuid);
	printf("cgid: %d\n", un_ds.buf->sem_perm.cgid);
	printf("mode: %u\n", un_ds.buf->sem_perm.mode);
	printf("sequence number: %u\n", un_ds.buf->sem_perm.__seq);
	printf("last semop time: %ld\n", un_ds.buf->sem_otime);
	printf("last change time: %ld\n", un_ds.buf->sem_ctime);
	printf("number of semaphores: %ld\n", un_ds.buf->sem_nsems);

	if (semctl(id, 0, IPC_INFO, un_info) == -1) {
		semaphore_remove(id);
		perror("IPC_INFO failed");
		return false;
	}

	printf("===================================\n");
	printf("IPC_INFO\n");
	printf("===================================\n");
	printf("num of entries in semaphore map: %d\n", un_info.__buf->semmap);
	printf("max of semaphore sets: %d\n", un_info.__buf->semmni);
	printf("max of semaphores in all sets: %d\n", un_info.__buf->semmns);
	printf("max of un_infodo structures: %d\n", un_info.__buf->semmnu);
	printf("max of semaphores in a set: %d\n", un_info.__buf->semmsl);
	printf("max of operations for semop: %d\n", un_info.__buf->semopm);
	printf("max of un_infodo entries per process: %d\n",
						un_info.__buf->semume);
	printf("size of struct sem_un_infodo: %d\n", un_info.__buf->semusz);
	printf("maximum semaphore value: %d\n", un_info.__buf->semvmx);
	printf("max value that can be recorded for un_infodo: %d\n",
						un_info.__buf->semaem);

	for (size_t i = 0; i != un_ds.buf->sem_nsems; i++) {
		int ret = 0;

		printf("===================================\n");
		printf("semaphore: %zu\n", i);
		printf("===================================\n");
		ret = semctl(id, i, GETNCNT);
		if (ret == -1) {
			semaphore_remove(id);
			perror("GETNCNT failed");
			return false;
		}

		printf("-----------------------------------\n");
		printf("GETNCNT\n");
		printf("-----------------------------------\n");
		printf("number of processes wait to increase semaphore: ");
		printf("%d\n", ret);

		ret = semctl(id, i, GETZCNT);
		if (ret == -1) {
			semaphore_remove(id);
			perror("GETZCNT failed");
			return false;
		}

		printf("-----------------------------------\n");
		printf("GETZCNT\n");
		printf("-----------------------------------\n");
		printf("number of processes wait semaphore become to 0: ");
		printf("%d\n", ret);

		ret = semctl(id, i, GETPID);
		if (ret == -1) {
			semaphore_remove(id);
			perror("GETPID failed");
			return false;
		}

		printf("-----------------------------------\n");
		printf("GETPID\n");
		printf("-----------------------------------\n");
		printf("pid of the last process to perform a semop: ");
		printf("%d\n", ret);
	}

	printf("-----------------------------------\n");

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

	if (!semaphore_printinfo(id))
		exit(EXIT_FAILURE);

	if (!semaphore_unlock(id))
		exit(EXIT_FAILURE);

	if (!semaphore_printinfo(id))
		exit(EXIT_FAILURE);

	printf("semaphore unlocked\n");

	semaphore_remove(id);

	printf("semaphore removed\n");

	return 0;
}
