/* dwmstatus-0.06 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "config.h"

static unsigned int delay = 3;

/* Network Connections */
char *net(void)
{
	FILE *carrier;
	unsigned char wlan;

	carrier = fopen(WLAN_CARFILE, "r");
	wlan = fgetc(carrier);
	fclose(carrier);
	char netstate[6];

	if (wlan == '1')
		return "<--->\0";
	else {
		carrier = fopen(ETH_CARFILE, "r");
		wlan = (int)fgetc(carrier);
		fclose(carrier);
		if (wlan == '1')
			return "[---]\0";
		else
			return "--/--\0";
	}
}

/* Memory used */
unsigned int memused(void)
{
	FILE *meminfo;
	meminfo = fopen(MEMINFOFILE, "r");
	char buffer[48];
	/* For people with more than 2047GB Ram */
	/* long long unsigned int memtotal, memfree, buffers, cached; */
	/* For the rest of us */
	unsigned int memtotal, memfree, buffers, cached;


	fgets(buffer, 48, meminfo);
	sscanf(buffer, "MemTotal: %u", &memtotal);
	fgets(buffer, 48, meminfo);
	sscanf(buffer, "MemFree: %u", &memfree);
	fgets(buffer, 48, meminfo);
	fgets(buffer, 48, meminfo);
	sscanf(buffer, "Buffers: %u", &buffers);
	fgets(buffer, 48, meminfo);
	sscanf(buffer, "Cached: %u", &cached);
	fclose(meminfo);
	unsigned int memused = (memtotal - memfree - buffers - cached) / 1024;
	return memused;
}

/* CPU (core0) freq */
float freq(void)
{
	FILE *freq;
	float corefreq;

	freq = fopen(CPU_FREQFILE, "r");
	fscanf(freq, "%f", &corefreq);
	corefreq = corefreq * 0.000001;
	fclose(freq);
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
	FILE *ac;
	unsigned short supply = 0;
	if (fopen(BAT_CAPFILE, "r")) {
		if (delay != 30)
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
	strftime(buffer, sizeof(buffer), "%a  %m/%d  %I:%M", tm_info);

	return buffer;
}

int main(void) {
	unsigned int battery_life = power();
	if (delay > 5)
		printf("%s  \u2502  [%u%%]  \u2502  %s ",
			net(),       battery_life,       unixtime());
	else
		printf("%s  \u2502  %uMB  \u2502  %0.1fGHz  \u2502  [%u%%]  \u2502  %s ",
			net(),      memused(),    freq(),            battery_life,       unixtime());
	sleep(delay);

	return 0;
}


