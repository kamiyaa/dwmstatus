/* dwmstatus-0.06 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <linux/unistd.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>

#include "config.h"

static struct sysinfo s_info;

/**
 * get the connectivity of network interfaces and return string
 * indicating the status of them:
 * <---> - wireless connection to network
 * [---] - wired connection to network
 * --/-- - no connection to network
 */
char *get_network_status(void)
{
	/* Open up the wireless interface carrier file to check if
	 * it is connected to a network.
	 *
	 */
	FILE *carrier_fd;
	carrier_fd = fopen(WLAN_CARFILE, "r");
	unsigned char iface_status;

	/* If the file exists, get and return its state */
	if (carrier_fd) {
		iface_status = fgetc(carrier_fd);
		fclose(carrier_fd);
		if (iface_status == '1')
			return "<--->\0";
	}
	/* Else, check if the wired interface carrier file exists */
	else {
		carrier_fd = fopen(ETH_CARFILE, "r");
        	if (carrier_fd) {
    			iface_status = fgetc(carrier_fd);
		    	fclose(carrier_fd);
		    	if (iface_status == '1')
		    		return "[---]\0";
	        }
	}
	return "--/--\0";
}

/**
 * get and return the current amount of free ram in KBs
 */
long get_free_mem(void)
{
	unsigned long free_ram = s_info.freeram;
	return free_ram;
}

/**
 * get and return the total amount of ram in KBs
 */
unsigned long get_total_mem(void)
{
	unsigned long total_mem = s_info.totalram;
	return total_mem;
}

/**
 * get and return the current amount of ram used in KBs
 */
unsigned long get_used_mem(void)
{
	unsigned long total_mem = get_total_mem();
	unsigned long free_mem = get_free_mem();
	unsigned long used_mem = total_mem - free_mem;
	return used_mem;
}

/**
 * get and return the current frequency of the core
 */
float get_freq(void)
{
	/* Open up core0 frequency sysfs file for parsing current
	 * frequency
	 */
	FILE *freq_fd;
	freq_fd = fopen(CPU_FREQFILE, "r");

	float corefreq;
	fscanf(freq_fd, "%f", &corefreq);
	fclose(freq_fd);
	/* Format the frequency to GHz */
	corefreq = corefreq * 0.000001;
	return corefreq;
}

/**
 * get and return the temperature of the core in celsius
 */
unsigned short get_temp(void)
{
	FILE *temps;
	unsigned int coretemp;

	temps = fopen(CPU_TEMPFILE, "r");
	fscanf(temps, "%ud", &coretemp);
	fclose(temps);
	coretemp = coretemp * 0.001;
	return coretemp;
}

/* Volume (Hexadecimal) */
unsigned int volume(void)
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
}

/* Power */
unsigned short get_power(void)
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
		/* If connected to AC, refresh rate will be set to 3
		 * seconds
		 */
		if (ac_on == '1')
			status_rrate = 3;
		/* Else, change the refresh rate to 30 seconds to save
		 * battery
		 */
		else if (status_rrate != 30)
			status_rrate = 30;
	}
	/* We must be connected to ac then */
	else if (status_rrate != 3)
		status_rrate = 3;

	return battery_charge;
}

/**
 * get and return the total uptime of machine in seconds
 */
long get_uptime(void)
{
	int error = sysinfo(&s_info);
	if (error != 0) {
		printf("code error = %d\n", error);
	}
	return s_info.uptime;
}

/**
 * get and return an array of chars representing the time of the system:
 * day_of_week month/day hour:minutes
 */
char *unixtime(void)
{
	static char buffer[22];
	time_t date;
	struct tm *tm_info;

	char *format = "%a %m/%d  %I:%M";
	time(&date);
	tm_info = localtime(&date);
	strftime(buffer, sizeof(buffer), format, tm_info);

	return buffer;
}

int main(void) {
	unsigned int battery_life = get_power();

	long uptime = get_uptime() / 60;
	unsigned int up_hours = uptime / 60;
	unsigned int up_minutes = uptime % 60;
	char *net_status = get_network_status();

	if (status_rrate > 5) {
		printf("%s \u2502 %u°C \u2502 [%u%%] \u2502 %d:%d \u2502 %s \n",
			net_status, get_temp(), battery_life, up_hours, up_minutes, unixtime());
	}
	else {
		printf("%s \u2502 %0.1fGHz \u2502 %u°C \u2502 [%u%%] \u2502 %d:%d \u2502 %s \n",
			net_status, get_freq(), get_temp(), battery_life, up_hours, up_minutes, unixtime());
	}
	sleep(status_rrate);

	return 0;
}

//		printf("%s \u2502 %uMB \u2502 %u°C \u2502 [%u%%] \u2502 %s \n",
//			get_net_carrier(), get_meminfo(), get_temp(), battery_life, unixtime());
