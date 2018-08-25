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
	signal(SIGINT, sigint_handler);

	/* display number */
	int screen_default_nbr;
	/* connect to display */
	xcb_connection_t *connection = xcb_connect(NULL, &screen_default_nbr);

	/* get the screen and the root window */
	xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
	xcb_window_t root_window = 0;
	if (screen)
		root_window = screen->root;

	/* format the uptime into minutes */
	unsigned int up_minutes, up_hours, volume;
	char *battery_status, *system_time;
	long uptime, alsa_max_vol;
	static char status[100];

	struct sysinfo s_info;

	snd_mixer_t *alsa_handle = create_alsa_handle();
	alsa_max_vol = alsa_get_max_vol(alsa_handle);
	volume = alsa_volume(alsa_handle) / alsa_max_vol;

	/* use a counter to update less important info less often */
	unsigned int counter = status_lirate;
	while (keep_running) {
		int res = snd_mixer_wait(alsa_handle, status_rrate * 1000);
		if (res >= 0) {
			res = snd_mixer_handle_events(alsa_handle);
			volume = alsa_volume(alsa_handle) / alsa_max_vol;
		}

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
			battery_status = power_status();

			/* get the system time */
			system_time = unixtime();
		}

		snprintf(status, sizeof(status),
			"%s \u2502 %0.02fGHz \u2502 %u\u00B0C \u2502 [%s] \u2502 vol: %d \u2502 %d:%d \u2502 %s ",
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
		counter += status_rrate;
	}

	/* disconnect from X server */
	xcb_disconnect(connection);

	return 0;
}

//		printf("%s \u2502 %uMB \u2502 %u°C \u2502 [%u%%] \u2502 %s \n",
//			get_net_carrier(), get_meminfo(), get_temp(), battery_life, unixtime());
