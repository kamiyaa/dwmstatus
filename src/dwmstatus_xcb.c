#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#include <sys/sysinfo.h>

#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>

#include "dwmstatus.h"

static unsigned short keep_running = 1;
static xcb_connection_t *connection;

/**
 * handles all memory cleanups when program is told to stop
 */
void exit_cleanup(int opt_code)
{
	keep_running = 0;
	/* disconnect from X server */
	xcb_disconnect(connection);
}


int main(void)
{
	/* display number */
	int screen_default_nbr;
	/* connect to display */
	connection = xcb_connect(NULL, &screen_default_nbr);

	/* get the screen and the root window */
	xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
	xcb_window_t root_window = 0;
	if (screen) {
		root_window = screen->root;
	}

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
	unsigned short cpu_temp;

	unsigned short status_len = 70;
	char status[status_len];

	signal(SIGINT, exit_cleanup);

	static struct sysinfo s_info;

	while (keep_running) {
		/* setup sysinfo with values */
		initialize_sysinfo(&s_info);

		/* get the battery life */
		battery_life = get_power();
		/* get the uptime of machine in minutes */
		uptime = s_info.uptime / 60;
		/* format the uptime into minutes */
		up_hours = uptime / 60;
		up_minutes = uptime % 60;
		/* get the network status */
		net_status = get_network_status();
		/* get the system time */
		system_time = unixtime();
		/* get the temperature of cpu */
		cpu_temp = get_temp();

		/* if update delay is greater than 5, then we are on battery mode */
		if (status_rrate == rrate_battery) {
			snprintf(status, status_len,
				"%s \u2502 %u\u00B0C \u2502 [%u%%] \u2502 %d:%d \u2502 %s ",
				net_status, cpu_temp, battery_life, up_hours, up_minutes, system_time);
		}
		/* otherwise, we are in normal mode */
		else {
			snprintf(status, status_len,
				"%s \u2502 %0.1fGHz \u2502 %u\u00B0C \u2502 [%u%%] \u2502 %d:%d \u2502 %s ",
				net_status, get_freq(), cpu_temp, battery_life, up_hours, up_minutes, system_time);
		}

		/* changed root window name */
		xcb_change_property(connection,
			XCB_PROP_MODE_REPLACE,
			root_window,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING,
			8,
			status_len,
			status);

		/* update display */
		xcb_flush(connection);

		/* refresh rate */
		sleep(status_rrate);
	}

	return 0;
}

//		printf("%s \u2502 %uMB \u2502 %u°C \u2502 [%u%%] \u2502 %s \n",
//			get_net_carrier(), get_meminfo(), get_temp(), battery_life, unixtime());
