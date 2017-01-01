/* dwmstatus-0.06 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "config.h"

/* Network Connections */
char *get_net_carrier(void)
{
	/* Open up the wireless interface carrier file to check if
	 * it is connected to a network.
	 *
	 */
	FILE *carrier;
	carrier = fopen(WLAN_CARFILE, "r");
	unsigned char wlan;

	/* If the file exists, get and return its state */
	if (carrier) {
		wlan = fgetc(carrier);
		fclose(carrier);
		if (wlan == '1')
			return "<--->\0";
	}
	/* Else, check if the wired interface carrier file exists */
	else if (fopen(ETH_CARFILE, "r")) {
		carrier = fopen(ETH_CARFILE, "r");
		wlan = fgetc(carrier);
		fclose(carrier);
		if (wlan == '1')
			return "[---]\0";
	}

	return "--/--\0";
}

/* Memory used */
void get_meminfo(unsigned int *list)
{
	FILE *meminfo;
	meminfo = fopen(MEMINFOFILE, "r");
	char buffer[48];
	/* For people with more than 2047GB Ram */
	/* long long unsigned int memtotal, memfree, buffers, cached; */
	/* For the rest of us */
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
	sscanf(buffer, "Buffers: %u", &buffers);
	fgets(buffer, 48, meminfo);
	/* Get the amount of cache is being used */
	sscanf(buffer, "Cached: %u", &cached);
	fclose(meminfo);
	/* Get the amount of active memory */
	list[0] = (memtotal - memfree - buffers - cached) / 1024;
	/* Get the amount of total memory used */
	list[1] = (memtotal - memfree) / 1024;
}


/* CPU (core0) freq */
float freq(void)
{
	/* Open up core0 frequency sysfs file for parsing current
	 * frequency
	 */
	FILE *freq;
	freq = fopen(CPU_FREQFILE, "r");

	float corefreq;
	fscanf(freq, "%f", &corefreq);
	fclose(freq);
	/* Format the frequency to GHz */
	corefreq = corefreq * 0.000001;
	return corefreq;
}

/* CPU (core0) temp */
/*unsigned short temp(void)
{
	FILE *temps;
	unsigned int coretemp;

	temps = fopen(CPU_TEMPFILE, "r");
	fscanf(temps, "%ud", &coretemp);
	coretemp = coretemp * 0.001;
	fclose(temps);
	return coretemp;
}*/

/* Volume (Hexadecimal) */
/*unsigned int volume(void)
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
unsigned short power(void)
{
	/* Open up the sysfs file for AC power */
	FILE *ac;
	unsigned short supply = 0;
	if (fopen(BAT_CAPFILE, "r")) {
		ac = fopen(AC_FILE, "r");
		char ac_on = fgetc(ac);
		fclose(ac);
		/* If connected to AC, refresh rate will be set to 3
		 * seconds
		 */
		if (ac_on == '1')
			delay = 3;
		/* Else, change the refresh rate to 30 seconds to save
		 * battery
		 */
		else if (delay != 30)
			delay = 30;
		ac = fopen(BAT_CAPFILE, "r");
		fscanf(ac, "%hu", &supply);
		fclose(ac);
	}
	else if (delay != 3)
		delay = 3;

	return supply;
}

/* Uptime */
/*unsigned int uptime(char hm) {
	long long unsigned int timeup;
	FILE *fuptime;

	fuptime = fopen(UPTIMEFILE, "r");
	char buffer[16];
	fgets(buffer, 16, fuptime);
	sscanf(buffer, "%32llu", &timeup);
	fclose(fuptime);
	unsigned int times[2];
	times[0] = 0;
	if (timeup > 3600) {
		times[0] = timeup / 3600;
		timeup = timeup - 3600 * times[0];
	}
	times[1] = timeup / 60;
	if (hm == 'h')
		return times[0];
	else
		return times[1];
}*/

/* Date/time */
char *unixtime(void)
{
	static char buffer[22];
	time_t date;
	struct tm *tm_info;

	time(&date);
	tm_info = localtime(&date);
	strftime(buffer, sizeof(buffer), "%a %m/%d  %I:%M", tm_info);

	return buffer;
}

int main(void) {
	unsigned int battery_life = power();

	if (delay > 5) {
		printf("%s \u2502 [%u%%] \u2502 %s \n",
			get_net_carrier(), battery_life, unixtime());
	}
	else {
		unsigned int ram_usage[2];
		get_meminfo(ram_usage);
		printf("%s \u2502 %uMB/%uMB \u2502 %0.1fGHz \u2502 [%u%%] \u2502 %s \n",
			get_net_carrier(), ram_usage[0], ram_usage[1], freq(), battery_life, unixtime());
	}
	sleep(delay);

	return 0;
}
