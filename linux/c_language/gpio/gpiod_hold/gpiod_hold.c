#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <gpiod.h> /* Install libgpiod first. */
#include <sys/time.h>

#define error_log(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

#define GPIO_CHIP 0
#define GPIO_LINE 38
#define GPIO_CONSUMER "consumer"
#define KEY_HOLD_TIME 3

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
	int ret = 0;
	double diff = 0;
	struct gpiod_line_event event;
	struct timeval last;
	struct timeval now;
	struct timespec tspec = {
		.tv_sec = 1,
		.tv_nsec = 0
	};

	if (!recovery_gpio_init(GPIO_CHIP))
		error_log("Init GPIO failed");

	if (gpiod_line_request_both_edges_events(line, GPIO_CONSUMER) < 0)
		error_log("Request event failed");

	while (1) {
		while (ret != 1) {
			ret = gpiod_line_event_wait(line, &tspec);
			if (ret < 0)
				error_log("Wait for event failed");
		}

		if (gpiod_line_event_read(line, &event) < 0)
			error_log("Read event failed");

		if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE)
			gettimeofday(&last, NULL);

		if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
			gettimeofday(&now, NULL);
			diff = difftime(now.tv_sec, last.tv_sec);
			if (diff >= KEY_HOLD_TIME) {
				printf("Key hold\n");
				break;
			}
		}
	}

	recovery_gpio_deinit();

	return 0;
}
