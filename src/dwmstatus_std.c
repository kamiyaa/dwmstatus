#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

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

	/* format the uptime into minutes */
	unsigned int up_minutes, up_hours, volume;
	long uptime, alsa_max_vol;
	char *system_time, *battery_status;

	static struct sysinfo s_info;

         /* use a counter to update less important info less often */
	unsigned int counter = status_lirate;

	snd_mixer_t *alsa_handle = create_alsa_handle();
	alsa_max_vol = alsa_get_max_vol(alsa_handle);
	volume = alsa_volume(alsa_handle) / alsa_max_vol;

	while (keep_running) {
		int res = snd_mixer_wait(alsa_handle, status_rrate * 1000);
		if (res == 0) {
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

		/* output and flush status to stdout */
		printf("%s \u2502 %0.02fGHz \u2502 %u\u00B0C \u2502 [%s] \u2502 Vol: %d%% \u2502 %d:%d \u2502 %s ",
			network_status(), cpufreq(), cputemp(), battery_status, volume, up_hours, up_minutes, system_time);
		fflush(stdout);

		/* refresh rate of status bar */
		counter += status_rrate;
	}

	snd_mixer_close(alsa_handle);

	return 0;
}
