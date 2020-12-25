#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

unsigned int get_speed(speed_t speed)
{
	unsigned int baud = 0;

	switch (speed) {
	case B0:
		baud = 0;
		break;
	case B9600:
		baud = 9600;
		break;
	case B19200:
		baud = 19200;
		break;
	case B38400:
		baud = 38400;
		break;
	case B57600:
		baud = 57600;
		break;
	case B115200:
		baud = 115200;
		break;
	case B230400:
		baud = 230400;
		break;
	case B460800:
		baud = 460800;
		break;
	}

	return baud;
}

int main(int argc, char *argv[])
{
	speed_t speed;
	speed_t new_speed = B115200;
	struct termios tm;

	if (tcgetattr(STDIN_FILENO, &tm) == -1)
		error_en("Get terminal attribute failed");

	speed = cfgetispeed(&tm);
	printf("Input speed: %u\n", get_speed(speed));
	speed = cfgetospeed(&tm);
	printf("Output speed: %u\n", get_speed(speed));
	printf("Set speed to %u\n", get_speed(new_speed));

	if (cfsetispeed(&tm, new_speed) == -1)
		error_en("Set terminal input speed failed");

	if (cfsetospeed(&tm, new_speed) == -1)
		error_en("Set terminal output speed failed");

	new_speed = cfgetispeed(&tm);
	printf("Input speed: %u\n", get_speed(new_speed));
	new_speed = cfgetospeed(&tm);
	printf("Output speed: %u\n", get_speed(new_speed));
	sleep(10);
	printf("Set speed to default %u\n", get_speed(speed));

	if (cfsetispeed(&tm, speed) == -1)
		error_en("Set terminal input speed failed");

	if (cfsetospeed(&tm, speed) == -1)
		error_en("Set terminal output speed failed");

	return 0;
}
