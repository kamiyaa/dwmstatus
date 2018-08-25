#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/sysinfo.h>

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

#include "dwmstatus.h"
#include "config.h"

#define alloca(x)  __builtin_alloca(x)

/* Status bar refresh rates on battery and on AC */
static const unsigned int rrate_battery	= 30;
static const unsigned int rrate_ac	= 3;

/**
 * grabs sysinfo
 */
void initialize_sysinfo(struct sysinfo *s_info)
{
	int error = sysinfo(s_info);
	if (error)
		fprintf(stderr, "Error: error code: %d\n", error);
}

/**
 * get the connectivity of network interfaces and return string
 * indicating the status of them:
 * <---> - wireless connection to network
 * [---] - wired connection to network
 * --/-- - no connection to network
 */
char *network_status()
{
	FILE *carrier_fd;
	/* If the file exists, get and return its state */
	if ((carrier_fd = fopen(WLAN_CARFILE, "r"))) {
		if (fgetc(carrier_fd) == '1') {
			fclose(carrier_fd);
			return "<--->";
		}
		fclose(carrier_fd);
	}
	/* Else, check if the wired interface carrier file exists */
	if ((carrier_fd = fopen(ETH_CARFILE, "r"))) {
		if (fgetc(carrier_fd) == '1') {
			fclose(carrier_fd);
			return "[---]";
		}
		fclose(carrier_fd);
	}
	return "--/--";
}

/**
 * get and return the current amount of free ram in KBs
 */
long memfree(struct sysinfo *s_info)
{
	unsigned long free_ram = s_info->freeram;
	return free_ram;
}

/**
 * get and return the total amount of ram in KBs
 */
unsigned long memtotal(struct sysinfo *s_info)
{
	unsigned long total_mem = s_info->totalram;
	return total_mem;
}

/**
 * get and return the current amount of ram used in KBs
 */
unsigned long memused(struct sysinfo *s_info)
{
	unsigned long total_mem = memtotal(s_info);
	unsigned long free_mem = memfree(s_info);
	unsigned long used_mem = total_mem - free_mem;
	return used_mem;
}

long alsa_get_max_vol(snd_mixer_t *handle)
{
	snd_mixer_selem_id_t *sid;

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, "Master");

	long min, max;

	snd_mixer_elem_t *elem = snd_mixer_find_selem(handle, sid);

        snd_mixer_selem_get_playback_volume_range(elem, &min, &max);

//	snd_mixer_close(handle);

	return max;
}

snd_mixer_t *create_alsa_handle()
{
	snd_mixer_t *handle;

	snd_mixer_open(&handle, 0);
	snd_mixer_attach(handle, SOUNDCARD);
	snd_mixer_selem_register(handle, NULL, NULL);
	snd_mixer_load(handle);

	return handle;
}

unsigned int alsa_volume(snd_mixer_t *handle)
{
	long volume;
	snd_mixer_selem_id_t *sid;
	snd_mixer_elem_t *elem;

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, "Master");

	elem = snd_mixer_find_selem(handle, sid);

	snd_mixer_selem_get_playback_volume(elem,
		SND_MIXER_SCHN_FRONT_LEFT, &volume);

	return volume * 100;
}

/**
 * get and return the current frequency of the core
 */
float cpufreq()
{
	/* Open up core0 frequency sysfs file for parsing current
	 * frequency
	 */
	unsigned int raw_freq;
	float core_freq = 0;

	FILE *freq_fd;
	if ((freq_fd = fopen(CPU_FREQFILE, "r"))) {
		if (fscanf(freq_fd, "%ud", &raw_freq) == 1) {
			/* Format the frequency to GHz */
			core_freq = raw_freq * 0.000001;
		}
		fclose(freq_fd);
	}
	return core_freq;
}

/**
 * get and return the temperature of the core in celsius
 */
short cputemp()
{
	FILE *temp_fd;
	unsigned int raw_temp;
	short core_temp = 0;

	if ((temp_fd = fopen(CPU_TEMPFILE, "r"))) {
		if (fscanf(temp_fd, "%ud", &raw_temp) == 1) {
			core_temp = raw_temp * 0.001;
		}
		fclose(temp_fd);
	}
	return core_temp;
}

/* Volume (Hexadecimal) */
/* unsigned int get_volume()
{
	FILE *codec;
	codec = fopen(SOUNDFILE, "r");
	char buffer[64];
	unsigned int hexvoll, hexvolr;

	for (int i = 0; i < 41; i++)
		fgets(buffer, 64, codec);
	fgets(buffer, 64, codec);
	sscanf(buffer, "  Amp-Out vals:  [%x %x]\n", &hexvoll, &hexvolr);
	fclose(codec);
	return hexvoll;
}*/

float battery_watt_drain()
{
	FILE *fp;

	if ((fp = fopen(BAT_DRAIN_FILE, "r")) == NULL)
		return -1;

	unsigned int milliwatts;
	if (fscanf(fp, "%u", &milliwatts) != 1)
		return -1;

	return milliwatts / 10000000;
}

/* Power */
char *power_status()
{
	static char ac_on;
	static char power_buf[7];
	static unsigned short battery_charge;

	/* Open up the sysfs file for battery info */
	FILE *fp;
	/* If battery exists get battery charge*/
	unsigned short tmp_charge;
	int tmp_on;

	/* if we can't open battery file, then we are on AC */
	if ((fp = fopen(BAT_CAPFILE, "r")) == NULL) {
		return "AC";
	}

	/* error if we are on battey, but can't retrieve
	 * battery life information */
	if (fscanf(fp, "%hu", &tmp_charge) != 1) {
		fclose(fp);
		return "Failed to get battery";
	}
	fclose(fp);

	if ((fp = fopen(AC_FILE, "r"))) {
		tmp_on = fgetc(fp);
		fclose(fp);
		if (ac_on != tmp_on)
			ac_on = tmp_on;
	}

	if (battery_charge != tmp_charge) {
		battery_charge = tmp_charge;
		snprintf(power_buf, sizeof(power_buf),
			"%hu%%", battery_charge);
		if (ac_on == '1')
			strcat(power_buf, "+");
	}

	/* refresh rate with change depending on if we are
	 * on ac or battery
	 */
	return power_buf;
}

/**
 * get and return an array of chars representing the time of the system:
 * day_of_week month/day  hour:minutes
 */
char *unixtime()
{
	static char time_buf[22];
	static const char *format = "%a %m/%d  %I:%M";

	time_t date;
	time(&date);

	struct tm *tm_info = localtime(&date);
	strftime(time_buf, sizeof(time_buf), format, tm_info);

	return time_buf;
}
