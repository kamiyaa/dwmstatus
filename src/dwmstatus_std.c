#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "dwmstatus.h"

static unsigned short keep_running = 1;

int main(void)
{
	/* get the battery life */
	unsigned int battery_life;
	/* get the uptime of machine in minutes */
	long uptime;
	/* format the uptime into minutes */
	unsigned int up_hours;
	unsigned int up_minutes;
	/* get the network status */
	char *net_status;
	/* get the system time */
	char *system_time;
	/* get the temperature of cpu */
	short cpu_temp;
	static struct sysinfo s_info;


	unsigned int counter = 60;
	while (keep_running) {
		if (counter >= 60) {
			counter = 0;

			/* setup sysinfo with values */
			sysinfo(&s_info);

			/* get the uptime of machine in minutes */
			uptime = s_info.uptime / 60;
			/* format the uptime into minutes */
			up_hours = uptime / 60;
			up_minutes = uptime % 60;

			/* get the battery life */
			battery_life = get_power();

			/* get the system time */
			system_time = unixtime();
		}
		/* get the network status */
		net_status = get_network_status();
		/* get the temperature of cpu */
		cpu_temp = get_temp();

		/* output and flush status to stdout */
		printf("%s \u2502 %0.1fGHz \u2502 %u\u00B0C \u2502 [%u%%] \u2502 %d:%d \u2502 %s \n",
			net_status, get_freq(), cpu_temp, battery_life, up_hours, up_minutes, system_time);
		fflush(stdout);

		/* refresh rate of status bar */
		sleep(status_rrate);
		counter += status_rrate;
	}

	return 0;
}
