#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define error_en(msg) \
	do { \
		perror(msg); \
		exit(EXIT_FAILURE); \
	} while (0)

int main(int argc, char *argv[])
{
	struct termios tm;

	if (tcgetattr(STDIN_FILENO, &tm) == -1)
		error_en("Get terminal attribute failed");

	printf("c_iflag: 0x%x\n", tm.c_iflag);
	printf("c_oflag: 0x%x\n", tm.c_oflag);
	printf("c_cflag: 0x%x\n", tm.c_cflag);
	printf("c_lflag: 0x%x\n", tm.c_lflag);
	printf("c_line: 0x%x\n", tm.c_line);
	printf("c_cc[VINTR]: 0x%x\n", tm.c_cc[VINTR]);
	printf("c_cc[VQUIT]: 0x%x\n", tm.c_cc[VQUIT]);
	printf("c_cc[VERASE]: 0x%x\n", tm.c_cc[VERASE]);
	printf("c_cc[VKILL]: 0x%x\n", tm.c_cc[VKILL]);
	printf("c_cc[VEOF]: 0x%x\n", tm.c_cc[VEOF]);
	printf("c_cc[VTIME]: 0x%x\n", tm.c_cc[VTIME]);
	printf("c_cc[VMIN]: 0x%x\n", tm.c_cc[VMIN]);
	printf("c_cc[VSWTC]: 0x%x\n", tm.c_cc[VSWTC]);
	printf("c_cc[VSTART]: 0x%x\n", tm.c_cc[VSTART]);
	printf("c_cc[VSTOP]: 0x%x\n", tm.c_cc[VSTOP]);
	printf("c_cc[VSUSP]: 0x%x\n", tm.c_cc[VSUSP]);
	printf("c_cc[VEOL]: 0x%x\n", tm.c_cc[VEOL]);
	printf("c_cc[VREPRINT]: 0x%x\n", tm.c_cc[VREPRINT]);
	printf("c_cc[VDISCARD]: 0x%x\n", tm.c_cc[VDISCARD]);
	printf("c_cc[VWERASE]: 0x%x\n", tm.c_cc[VWERASE]);
	printf("c_cc[VLNEXT]: 0x%x\n", tm.c_cc[VLNEXT]);
	printf("c_cc[VEOL2]: 0x%x\n", tm.c_cc[VEOL2]);
	printf("c_ispeed: 0x%x\n", tm.c_ispeed);
	printf("c_ospeed: 0x%x\n", tm.c_ospeed);

	return 0;
}
