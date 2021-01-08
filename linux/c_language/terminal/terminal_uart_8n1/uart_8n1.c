#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <termios.h>
#include <pthread.h>

#define error_en(en, msg) \
	do { \
		errno = en; \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define error_log(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define DEV "/dev/ttyACM0"
#define SPEED B115200
#define BUF_SIZE 1024
#define CANONICAL 1

static int fd;
static struct termios old_tm;

static void signal_handler(int signal)
{
	if (tcsetattr(fd, TCSAFLUSH, &old_tm) == -1)
		error_log("Set terminal attribute failed");

	close(fd);
	fd = 0;
}

static void *thread_t1(void *arg)
{
	while (fd <= 0)
		sleep(1);

	while (1) {
		char buf[BUF_SIZE] = {0};

		if (fgets(buf, BUF_SIZE - 1, stdin)) {
			if (write(fd, buf, strlen(buf) + 1) < 0)
				pthread_exit(NULL);

			tcdrain(fd);
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	pthread_t t1;
	struct sigaction act;
	struct sigaction old_act;
	struct termios tm;

	ret = pthread_create(&t1, NULL, thread_t1, "Thread t1");
	if (ret)
		error_en(ret, "Thread create failed");

	if (sigaction(SIGTERM, NULL, &old_act))
		error_log("Get signal failed");

	act = old_act;
	act.sa_handler = signal_handler;

	if (sigaction(SIGTERM, &act, NULL))
		error_log("Set signal failed");

	fd = open(DEV, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
		error_log("Open uart failed");

	fcntl(fd, F_SETFL, FNDELAY);

	if (tcgetattr(fd, &old_tm) == -1)
		error_log("Get uart attribute failed");

	tm = old_tm;

	cfsetispeed(&tm, SPEED);
	cfsetospeed(&tm, SPEED);

	tm.c_cc[VMIN] = 1;
	tm.c_cc[VTIME] = 0;
	tm.c_iflag |= (IGNPAR | IXON | IXOFF);
	tm.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | INPCK | ISTRIP | INLCR
			| IGNCR | ICRNL | IXANY | IUCLC | IMAXBEL | IUTF8);
	tm.c_oflag &= ~(OPOST | OLCUC | OCRNL | ONLCR |
			ONOCR | ONLRET | OFILL | OFDEL);
	tm.c_cflag |= (CS8 | HUPCL | CREAD | CLOCAL);
	tm.c_cflag &= ~(PARENB | PARODD | CMSPAR | CSTOPB | CRTSCTS);
	if (CANONICAL) {
		tm.c_lflag |= (ISIG | ICANON);
		tm.c_lflag &= ~(IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL |
				ECHOKE | ECHONL | NOFLSH | XCASE | TOSTOP |
				ECHOPRT | FLUSHO | EXTPROC);
	} else {
		tm.c_lflag &= ~(ISIG | ICANON | IEXTEN | ECHO | ECHOE |
				ECHOK | ECHONL | NOFLSH | XCASE | TOSTOP |
				ECHOPRT | ECHOCTL | ECHOKE | FLUSHO | EXTPROC);
	}

	tcflush(fd, TCIFLUSH);

	if (tcsetattr(fd, TCSANOW, &tm) == -1)
		error_log("Set terminal attribute failed");

	while (1) {
		char buf[BUF_SIZE] = {0};
		int bytes = 0;

		bytes = read(fd, buf, BUF_SIZE - 1);
		if (bytes == 1 && buf[0] == '\n')
			continue;
		if (bytes == 2 && buf[0] == '\r' && buf[1] == '\n')
			continue;
		else if (bytes > 0)
			printf("%s", buf);
		else
			continue;
	}

	if (tcsetattr(fd, TCSAFLUSH, &old_tm) == -1)
		error_log("Set terminal attribute failed");

	close(fd);
	fd = 0;

	return 0;
}
