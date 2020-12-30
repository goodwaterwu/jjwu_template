#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h> /* Install libi2c-dev first. */

#define exit_on_error(msg) \
	do { \
		printf("%s\n", msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define error_log(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

static int fd;

int main(int argc, char *argv[])
{
	char dev[32] = {0};
	char ch = 0;

	if (argc < 3)
		exit_on_error("Invalid parameters");

	snprintf(dev, sizeof(dev), "/dev/i2c-%lu", strtoul(argv[1], NULL, 10));
	fd = open(dev, O_RDWR);
	if (fd == -1)
		error_log("Open i2c failed");

	if (ioctl(fd, I2C_SLAVE, strtoul(argv[2], NULL, 16)) == -1)
		error_log("Set slave address failed");

	printf("Open i2c succeeded\n");

	ch = i2c_smbus_read_byte(fd);
	printf("ch: 0x%02x\n", ch);
	ch = i2c_smbus_read_byte_data(fd, 0x0);
	printf("cmd: %d ch: 0x%02x\n", 0x0, ch);
	ch = i2c_smbus_read_byte_data(fd, 0x1);
	printf("cmd: %d ch: 0x%02x\n", 0x1, ch);
	ch = i2c_smbus_read_byte_data(fd, 0x2);
	printf("cmd: %d ch: 0x%02x\n", 0x2, ch);

	close(fd);
	fd = 0;

	return 0;
}
