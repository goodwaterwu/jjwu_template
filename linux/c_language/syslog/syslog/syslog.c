#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

void write_to_syslog(void)
{
	syslog(LOG_EMERG, "%s", "This is LOG_EMERG");
	syslog(LOG_ALERT, "%s", "This is LOG_ALERT");
	syslog(LOG_CRIT, "%s", "This is LOG_CRIT");
	syslog(LOG_ERR, "%s", "This is LOG_ERR");
	syslog(LOG_WARNING, "%s", "This is LOG_WARNING");
	syslog(LOG_NOTICE, "%s", "This is LOG_NOTICE");
	syslog(LOG_INFO, "%s", "This is LOG_INFO");
	syslog(LOG_DEBUG, "%s", "This is LOG_DEBUG");
}

int main(int argc, char *argv[])
{
	int option = LOG_CONS | LOG_NDELAY | LOG_PID;
	int mask = LOG_MASK(LOG_EMERG) | LOG_MASK(LOG_ALERT) |
			LOG_MASK(LOG_CRIT) | LOG_MASK(LOG_ERR);
	int old_mask = 0;

	openlog(argv[0], option, LOG_USER);
	
	write_to_syslog();
	old_mask = setlogmask(mask);
	syslog(LOG_EMERG, "Old log mask: 0x%x new: 0x%x", old_mask, mask);
	write_to_syslog();
	closelog();

	return 0;
}
