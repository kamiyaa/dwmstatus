/* dwmstatus-0.06 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <linux/unistd.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>

#include "config.h"

/* Network Connections */
char *get_net_carrier(void)
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

/* Memory used */
unsigned int get_meminfo(void)
{
	FILE *meminfo;
	meminfo = fopen(MEMINFOFILE, "r");
	char buffer[48];
	/* for memory total, memory free, buffers and cached */
	unsigned int memtotal, memfree, buffers, cached;

	/* Get the system memory amount */
	fgets(buffer, 48, meminfo);
	sscanf(buffer, "MemTotal: %u", &memtotal);
	/* Get the amount of memory free */
	fgets(buffer, 48, meminfo);
	sscanf(buffer, "MemFree: %u", &memfree);
	fgets(buffer, 48, meminfo);
	fgets(buffer, 48, meminfo);
	/* Get the amount of buffers are being used */
//	sscanf(buffer, "Buffers: %u", &buffers);
	fgets(buffer, 48, meminfo);
	/* Get the amount of cache is being used */
	sscanf(buffer, "Cached: %u", &cached);
	fclose(meminfo);
	/* Get the amount of active memory */
//	list[0] = (memtotal - memfree - buffers - cached) / 1024;
	/* Get the amount of total memory used */
	return (memtotal - memfree - cached) / 1024;
}


/* CPU (core0) freq */
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

/* CPU (core0) temp */
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
unsigned short power(void)
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

/* Uptime */
long get_uptime(void) {
	struct sysinfo s_info;
	int error = sysinfo(&s_info);
	if (error != 0) {
		printf("code error = %d\n", error);
	}
	return s_info.uptime;
}
/* Date/time */
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
	unsigned int battery_life = power();

	long uptime = get_uptime() / 60;

	if (status_rrate > 5) {
//		printf("%s \u2502 %uMB \u2502 %u°C \u2502 [%u%%] \u2502 %s \n",
//			get_net_carrier(), get_meminfo(), get_temp(), battery_life, unixtime());
		printf("%s \u2502 %u°C \u2502 [%u%%] \u2502 %s \n",
			get_net_carrier(), get_temp(), battery_life, unixtime());
	}
	else {
		unsigned int up_hours = uptime / 60;
		unsigned int up_minutes = uptime % 60;
		printf("%s \u2502 %0.1fGHz \u2502 %u°C \u2502 [%u%%] \u2502 %d:%d \u2502 %s \n",
			get_net_carrier(), get_freq(), get_temp(), battery_life, up_hours, up_minutes, unixtime());
	}
	sleep(status_rrate);

	return 0;
}
