#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>

#define error_en(en, msg) \
	do { \
		errno = en; \
		perror(msg); \
	} while (0)

#define error_en_free(en, msg) \
	do { \
		errno = en; \
		perror(msg); \
		goto free_stack; \
	} while (0)

static void *thread_t1(void *arg)
{
	int ret = 0;
	size_t size = 0;
	void *stack = NULL;
	pthread_attr_t attr;

	printf("%s\n", (char *)arg);

	ret = pthread_getattr_np(pthread_self(), &attr);
	if (ret)
		error_en(ret, "Get thread attribute failed");

	ret = pthread_attr_getstack(&attr, &stack, &size);
	if (ret)
		error_en(ret, "Get stack failed");

	printf("stack: %p size: %zu\n", stack, size);

	ret = pthread_attr_destroy(&attr);
	if (ret)
		error_en(ret, "Thread attribute destroy failed");

	return (void *)strlen(arg);
}

int main(int argc, char *argv[])
{
	int ret = 0;
	size_t size = PTHREAD_STACK_MIN;
	void *result = NULL;
	void *stack = NULL;
	pthread_t t1;
	pthread_attr_t attr;

	stack = calloc(sizeof(char), size);
	if (!stack) {
		printf("Malloc failed");
		exit(EXIT_FAILURE);
	}

	printf("stack address: %p\n", stack);

	ret = pthread_attr_init(&attr);
	if (ret)
		error_en_free(ret, "Thread attribute init failed");

	ret = pthread_attr_setstack(&attr, stack, size);
	if (ret)
		error_en_free(ret, "Set stack failed");

	ret = pthread_create(&t1, &attr, thread_t1, "Thread t1");
	if (ret)
		error_en_free(ret, "Thread create failed");

	ret = pthread_join(t1, &result);
	if (ret)
		error_en_free(ret, "Thread join failed");

	printf("Thread return: %zu\n", (size_t)result);

	ret = pthread_attr_destroy(&attr);
	if (ret)
		error_en_free(ret, "Thread attribute destroy failed");
free_stack:
	free(stack);
	stack = NULL;

	if (ret)
		exit(EXIT_FAILURE);

	exit(EXIT_SUCCESS);
}
