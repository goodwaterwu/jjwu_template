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

#define NUM_OF_THREAD 2

static int global_variable;
static pthread_t tid;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void *thread_t1(void *arg)
{
	int ret = 0;

	printf("Thread %zu created\n", (size_t)arg);

	ret = pthread_mutex_lock(&mutex);
	if (ret)
		error_en(ret, "Mutex lock failed");

	tid = pthread_self();

	ret = pthread_mutex_unlock(&mutex);
	if (ret)
		error_en(ret, "Mutex unlock failed");

	while (1) {
		ret = pthread_mutex_lock(&mutex);
		if (ret)
			error_en(ret, "Mutex lock failed");

		global_variable++;
		printf("thread: %zu global_variable: %d\n",
				(size_t)arg, global_variable);

		ret = pthread_mutex_unlock(&mutex);
		if (ret)
			error_en(ret, "Mutex unlock failed");

		if (global_variable >= 10) {
			ret = pthread_cond_signal(&cond);
			if (ret)
				error_en(ret, "Condition signal failed");
		}
	}

	pthread_exit(NULL);
}

static void *thread_t2(void *arg)
{
	int ret = 0;

	printf("Thread %zu created\n", (size_t)arg);

	ret = pthread_mutex_lock(&mutex);
	if (ret)
		error_en(ret, "Mutex lock failed");

	while (global_variable < 10) {
		ret = pthread_cond_wait(&cond, &mutex);
		if (ret)
			error_en(ret, "Condition wait failed");
	}

	ret = pthread_cancel(tid);
	if (ret)
		error_en(ret, "Kill thread failed");

	global_variable = 0;

	ret = pthread_mutex_unlock(&mutex);
	if (ret)
		error_en(ret, "Mutex unlock failed");

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int ret = 0;
	pthread_t t[NUM_OF_THREAD];

	ret = pthread_create(&t[0], NULL, thread_t1, (void *)0);
	if (ret)
		error_en(ret, "Thread create failed");

	ret = pthread_create(&t[1], NULL, thread_t2, (void *)1);
	if (ret)
		error_en(ret, "Thread create failed");

	for (size_t i = 0; i != NUM_OF_THREAD; i++) {
		ret = pthread_join(t[i], NULL);
		if (ret)
			error_en(ret, "Thread join failed");
	}

	printf("global_variable = %d\n", global_variable);

	exit(EXIT_SUCCESS);
}
