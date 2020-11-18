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

static void *thread_t1(void *arg)
{
	printf("%s\n", (char *)arg);

	return (void *)strlen(arg);
}

int main(int argc, char *argv[])
{
	int ret = 0;
	void *result = NULL;
	pthread_t t1;

	ret = pthread_create(&t1, NULL, thread_t1, "Thread t1");
	if (ret)
		error_en(ret, "Thread create failed");

	ret = pthread_join(t1, &result);
	if (ret)
		error_en(ret, "Thread join failed");

	printf("Thread return: %zu\n", (size_t)result);

	exit(EXIT_SUCCESS);
}
