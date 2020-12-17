#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

static sem_t sem;

int main(int argc, char *argv[])
{
	if (sem_init(&sem, 0, 1) == -1)
		error_en("Semaphore init failed");

	printf("Semaphore inited\n");

	if (sem_destroy(&sem) == -1)
		error_en("Semaphore destroy failed");

	printf("Semaphore destroyed\n");

	return 0;
}
