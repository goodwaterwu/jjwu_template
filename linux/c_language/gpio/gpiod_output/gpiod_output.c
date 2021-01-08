#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <gpiod.h> /* Install libgpiod first. */

#define error_log(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define GPIO_CHIP 0
#define GPIO_LINE 38
#define GPIO_CONSUMER "consumer"

static struct gpiod_chip *chip;
static struct gpiod_line *line;

static void recovery_gpio_deinit(void)
{
	gpiod_line_release(line);
	gpiod_chip_close(chip);
}

static bool recovery_gpio_init(unsigned int num)
{
	chip = gpiod_chip_open_by_number(num);
	if (!chip)
		return false;

	line = gpiod_chip_get_line(chip, GPIO_LINE);
	if (!line) {
		gpiod_chip_close(chip);
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{
	int value = 0;

	if (!recovery_gpio_init(GPIO_CHIP))
		error_log("Init GPIO failed");

	if (gpiod_line_request_output(line, GPIO_CONSUMER, 1))
		error_log("Request input failed");

	for (int i = 0; i != 10; i++) {
		int ret = 0;

		if (gpiod_line_set_value(line, value) < 0)
			error_log("Set value failed");

		ret = gpiod_line_get_value(line);
		if (ret < 0)
			error_log("Get value failed");

		printf("value: %d\n", ret);
		value = (value + 1) % 2;
		sleep(1);
	}

	recovery_gpio_deinit();

	return 0;
}
