#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define SHM "/shm"
#define SHM_SIZE 1024

int main(int argc, char *argv[])
{
	int fd = 0;
	int flags = O_CREAT | O_RDWR;
	int perms = S_IRUSR | S_IWUSR;
	void *addr = NULL;

	fd = shm_open(SHM, flags, perms);
	if (fd == -1)
		error_en("Open shared memory failed");

	if (ftruncate(fd, SHM_SIZE) == -1)
		perror("Truncate shared_memory failed");

	addr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
		perror("Map shared memory failed");

	if (close(fd) == -1)
		error_en("Close shared memory failed");

	if (shm_unlink(SHM) == -1)
		error_en("Unlink shared memory failed");

	return 0;
}
