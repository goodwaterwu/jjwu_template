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
	if (!recovery_gpio_init(GPIO_CHIP))
		error_log("Init GPIO failed");

	if (gpiod_line_request_input(line, GPIO_CONSUMER))
		error_log("Request input failed");

	for (int i = 0; i != 10; i++) {
		int value = gpiod_line_get_value(line);

		if (value < 0)
			error_log("Get value failed");

		printf("value: %d\n", value);
		sleep(1);
	}

	recovery_gpio_deinit();

	return 0;
}
