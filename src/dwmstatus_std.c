#include <stdio.h>
#include <unistd.h>

#include "dwmstatus.h"

static unsigned char keep_running = 1;

int main()
{
	/* get the battery life */
	char *battery_status;
	long uptime;
	/* format the uptime into minutes */
	unsigned int up_hours;
	unsigned int up_minutes;

	char *net_status;
	char *system_time;
	short cpu_temp;
	float cpu_freq;

	static struct sysinfo s_info;

        /* use a counter to update less important info less often */
	unsigned int counter = status_lirate;
	while (keep_running) {
		if (counter >= status_lirate) {
			counter = 0;

			/* setup sysinfo with values */
			sysinfo(&s_info);

			/* get the uptime of machine in minutes */
			uptime = s_info.uptime / 60;
			/* format the uptime into minutes */
			up_hours = uptime / 60;
			up_minutes = uptime % 60;

			/* get the battery life */
			battery_status = get_power();

			/* get the system time */
			system_time = unixtime();
		}
		/* get the network status */
		net_status = get_network_status();
		/* get the temperature of cpu */
		cpu_temp = get_temp();
		/* get the frequency of cpu */
		cpu_freq = get_freq();

		/* output and flush status to stdout */
		printf("%s \u2502 %0.1fGHz \u2502 %u\u00B0C \u2502 [%s] \u2502 %d:%d \u2502 %s \n",
			net_status, cpu_freq, cpu_temp, battery_status, up_hours, up_minutes, system_time);
		fflush(stdout);

		/* refresh rate of status bar */
		sleep(status_rrate);
		counter += status_rrate;
	}

	return 0;
}
