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

static int global_variable;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *new_thread(void *arg)
{
	int ret = 0;
	int automatic_variable = 0;

	printf("Thread %zu created\n", (size_t)arg);

	for (int i = 0; i != 10; i++) {
		ret = pthread_mutex_lock(&mutex);
		if (ret)
			error_en(ret, "Mutex lock failed");

		automatic_variable = global_variable;
		sleep(1); /* Expire time slice. */
		automatic_variable++;
		global_variable = automatic_variable;

		ret = pthread_mutex_unlock(&mutex);
		if (ret)
			error_en(ret, "Mutex unlock failed");

		printf("thread: %zu global_variable: %d\n",
				(size_t)arg, global_variable);
	}

	return NULL;
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

	}

	printf("global_variable = %d\n", global_variable);

	exit(EXIT_SUCCESS);
}
