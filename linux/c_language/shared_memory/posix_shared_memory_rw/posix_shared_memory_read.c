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
	int flags = O_RDONLY;
	int perms = S_IRUSR | S_IWUSR;
	void *addr = NULL;
	struct stat statbuf;

	fd = shm_open(SHM, flags, perms);
	if (fd == -1)
		error_en("Open shared memory failed");

	if (fstat(fd, &statbuf) == -1) {
		perror("Get shared memory state failed");
		goto exit_on_err;
	}

	addr = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED) {
		perror("Map shared memory failed");
		goto exit_on_err;
	}

	write(STDOUT_FILENO, addr, statbuf.st_size);
	printf("\n");
exit_on_err:
	if (close(fd) == -1)
		error_en("Close shared memory failed");

	if (shm_unlink(SHM) == -1)
		error_en("Unlink shared memory failed");

	return 0;
}
