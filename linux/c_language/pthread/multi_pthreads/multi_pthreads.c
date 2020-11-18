#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#define error_en(en, msg) \
	do { \
		errno = en; \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define NUM_OF_THREAD 3

static void *new_thread(void *arg)
{
	printf("Thread %zu created\n", (size_t)arg);

	return arg;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	void *result = NULL;
	pthread_t t[NUM_OF_THREAD];

	for (size_t i = 0; i != NUM_OF_THREAD; i++) {
		ret = pthread_create(&t[i], NULL, new_thread, (void *)i);
		if (ret)
			error_en(ret, "Thread create failed");
	}

	for (size_t i = 0; i != NUM_OF_THREAD; i++) {
		ret = pthread_join(t[i], &result);
		if (ret)
			error_en(ret, "Thread join failed");

		printf("Thread return: %zu\n", (size_t)result);
	}

	exit(EXIT_SUCCESS);
}
