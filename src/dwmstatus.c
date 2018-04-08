#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <sys/sysinfo.h>

#include "dwmstatus.h"
#include "config.h"

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
char *get_network_status()
{
	unsigned char iface_status;
	FILE *carrier_fd;
	char *status;
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
long get_free_mem(struct sysinfo *s_info)
{
	unsigned long free_ram = s_info->freeram;
	return free_ram;
}

/**
 * get and return the total amount of ram in KBs
 */
unsigned long get_total_mem(struct sysinfo *s_info)
{
	unsigned long total_mem = s_info->totalram;
	return total_mem;
}

/**
 * get and return the current amount of ram used in KBs
 */
unsigned long get_used_mem(struct sysinfo *s_info)
{
	unsigned long total_mem = get_total_mem(s_info);
	unsigned long free_mem = get_free_mem(s_info);
	unsigned long used_mem = total_mem - free_mem;
	return used_mem;
}


/**
 * get and return the current frequency of the core
 */
float get_freq()
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
short get_temp()
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

/* Power */
unsigned short get_power()
{
	/* Open up the sysfs file for battery info */
	FILE *power_fd;
	power_fd = fopen(BAT_CAPFILE, "r");
	unsigned short battery_charge = 0;
	/* If battery exists get battery charge*/
	if (power_fd) {
		fscanf(power_fd, "%hu", &battery_charge);
		fclose(power_fd);
		power_fd = fopen(AC_FILE, "r");
		char ac_on = fgetc(power_fd);
		fclose(power_fd);
		/* refresh rate with change depending on if we are
		 * on ac or battery
		 */
		if (ac_on == '1' && status_rrate != rrate_ac) {
			status_rrate = rrate_ac;
		}
		else if (status_rrate != rrate_battery) {
			status_rrate = rrate_battery;
		}
	}
	/* We must be connected to ac then */
	else if (status_rrate != rrate_ac)
		status_rrate = rrate_ac;

	return battery_charge;
}

/**
 * get and return an array of chars representing the time of the system:
 * day_of_week month/day  hour:minutes
 */
char *unixtime()
{
	static char buffer[22];
	time_t date;
	struct tm *tm_info;

	const char *format = "%a %m/%d  %I:%M";
	time(&date);
	tm_info = localtime(&date);
	strftime(buffer, sizeof(buffer), format, tm_info);

	return buffer;
}
