#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define SEM "/sem"

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

int main(int argc, char *argv[])
{
	int flags = O_CREAT;
	int perms = S_IRUSR | S_IWUSR;
	sem_t *sem = NULL;

	sem = sem_open(SEM, flags, perms, NULL);
	if (sem == SEM_FAILED)
		error_en("Semaphore create failed");

	printf("Semaphore created\n");

	if (sem_close(sem) == -1)
		error_en("Semaphore close failed");

	printf("Semaphore closed\n");

	if (sem_unlink(SEM) == -1)
		error_en("Semaphore unlink failed");

	printf("Semaphore unlinked\n");

	return 0;
}
