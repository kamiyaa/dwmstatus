#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>

#include "dwmstatus.h"

static unsigned char keep_running = 1;

/**
 * handles all memory cleanups when program is told to stop
 */
void sigint_handler()
{
	keep_running = 0;
}


int main()
{
	/* format the uptime into minutes */
	unsigned int up_minutes, up_hours, volume;
	char *battery_status, *system_time;
	long uptime, alsa_vol_unit;
	static char status[100];
	struct sysinfo s_info;

	/* display number */
	int screen_default_nbr;
	/* connect to display */
	xcb_connection_t *connection = xcb_connect(NULL, &screen_default_nbr);

	/* get the screen and the root window */
	xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
	xcb_window_t root_window = 0;

	signal(SIGINT, sigint_handler);

	if (screen)
		root_window = screen->root;

	snd_mixer_t *alsa_handle = create_alsa_handle();
	alsa_vol_unit = alsa_get_max_vol(alsa_handle) / 100;
	volume = alsa_volume_percent(alsa_handle, alsa_vol_unit);

	/* use a counter to update less important info less often */
	unsigned int counter = STATUS_REFRESH_RATE_LOW;
	while (keep_running) {
		if (snd_mixer_wait(alsa_handle, STATUS_REFRESH_RATE_REG * 1000) == 0) {
			snd_mixer_handle_events(alsa_handle);
			volume = alsa_get_max_vol(alsa_handle) / 100;
		}

		if (counter >= STATUS_REFRESH_RATE_LOW) {
			counter = 0;

			/* setup sysinfo with values */
			sysinfo(&s_info);

			/* get the uptime of machine in minutes */
			uptime = s_info.uptime / 60;
			/* format the uptime into minutes */
			up_hours = uptime / 60;
			up_minutes = uptime % 60;

			/* get the battery life */
			battery_status = power_status();

			/* get the system time */
			system_time = unixtime();
		}

		snprintf(status, sizeof(status),
			"%s \u2502 %0.02fGHz \u2502 %u\u00B0C \u2502 [%s] \u2502 vol: %d \u2502 %d:%02d \u2502 %s ",
			network_status(), cpufreq(), cputemp(), battery_status, volume, up_hours, up_minutes, system_time);

		/* changed root window name */
		xcb_change_property(connection,
			XCB_PROP_MODE_REPLACE,
			root_window,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING,
			8,
			sizeof(status),
			status);

		/* update display */
		xcb_flush(connection);

		/* refresh rate */
		counter += STATUS_REFRESH_RATE_REG;
	}

	snd_mixer_close(alsa_handle);
	/* disconnect from X server */
	xcb_disconnect(connection);

	return 0;
}

//		printf("%s \u2502 %uMB \u2502 %u°C \u2502 [%u%%] \u2502 %s \n",
//			get_net_carrier(), get_meminfo(), get_temp(), battery_life, unixtime());
