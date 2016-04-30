// Version 0.02

#include <stdio.h>
#include <time.h>
#include <unistd.h>
//#include <dirent.h>

#include "config.h"

/* Network Connections */
char *net(void) {
	FILE *west;
	char wlan;

	west = fopen(WLPCARRIERFILE, "r");
	wlan = fgetc(west);
	fclose(west);

	if (wlan == '1')
		return "<--->";
	else {
		west = fopen(ENPCARRIERFILE, "r");
		wlan = (int)fgetc(west);
		fclose(west);
		if (wlan == '1')
			return "[---]";
		else
			return "--/--";
	}
}

/* Memory used */
long long unsigned int memused(void) {
	FILE *meminfo;
	meminfo = fopen(PROCMEMINFOFILE, "r");
	char buffer[128];
	long long unsigned int memtotal, memfree, buffers, cached;
	fgets(buffer, 128, meminfo);
	sscanf(buffer, "MemTotal: %32llu", &memtotal);
	fgets(buffer, 128, meminfo);
	sscanf(buffer, "MemFree: %32llu", &memfree);
	fgets(buffer, 128, meminfo);
	fgets(buffer, 128, meminfo);
	sscanf(buffer, "Buffers: %32llu", &buffers);
	fgets(buffer, 128, meminfo);
	sscanf(buffer, "Cached: %32llu", &cached);
	fclose(meminfo);
	return (memtotal - memfree - buffers - cached) / 1024;
}

/* CPU (core0) freq */
/*float freq(void) {
	FILE *freq;
	float corefreq;

	freq = fopen(CPU0FREQFILE, "r");
	fscanf(freq, "%f", &corefreq);
	corefreq = corefreq * 0.000001;
	fclose(freq);
	return corefreq;
}*/

/* CPU (core0) temp */
/*unsigned short temp(void) {
	FILE *temps;
	unsigned int coretemp;

	temps = fopen(CPU0TEMPFILE, "r");
	fscanf(temps, "%ud", &coretemp);
	coretemp = coretemp * 0.001;
	fclose(temps);
	return coretemp;
}*/

/* Power */
unsigned short power(void) {
	FILE *ac;
	unsigned short supply;

	ac = fopen(ACFILE, "r");
	supply = fgetc(ac);
	fclose(ac);
	if (supply == 49)
		return 0;
	else {
		ac = fopen(BATTERYCAPFILE, "r");
		fscanf(ac, "%hu", &supply);
		fclose(ac);
		return supply;
	}
}

/* Date/time */
char *unixtime(void) {
	static char buffer[22];
	time_t date;
	struct tm *tm_info;

	time(&date);
	tm_info = localtime(&date);
	strftime(buffer, sizeof(buffer), "%A %d/%m %I:%M", tm_info);

	return buffer;
}

int main(void) {
	printf("%s ┃ %lluMB ┃ [%u%%] ┃ %s\n", net(), memused(), power(), unixtime());
//	sleep(3);

//	printf("%s ┃ %uM ┃ %0.1fGHz ┃ %hu°C ┃ [%u%%] ┃ %s\n", net(), mem(), freq(), temp(), power(), unixtime());
//	printf("%s ┃ %0.1fGHz ┃ %u°C ┃ [%u%%] ┃ %s\n", net(), freq(), temp(), power(), unixtime());
	return 0;
}

//#include <sys/types.h>
//#include <linux/sysinfo.h>

//#define MB 1048576
//#define GB 1073741824
