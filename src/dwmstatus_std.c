#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <sys/sysinfo.h>

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

	while (keep_running) {
		/* setup sysinfo with values */
		initialize_sysinfo(&s_info);

		/* get the battery life */
		battery_life = get_power();
		/* get the uptime of machine in minutes */
		uptime = get_uptime(&s_info) / 60;
		/* format the uptime into minutes */
		up_hours = uptime / 60;
		up_minutes = uptime % 60;
		/* get the network status */
		net_status = get_network_status();
		/* get the system time */
		system_time = unixtime();
		/* get the temperature of cpu */
		cpu_temp = get_temp();

		printf("%s \u2502 %0.1fGHz \u2502 %u\u00B0C \u2502 [%u%%] \u2502 %d:%d \u2502 %s \n",
			net_status, get_freq(), cpu_temp, battery_life, up_hours, up_minutes, system_time);
		fflush(stdout);
		sleep(3);
	}

	return 0;
}
