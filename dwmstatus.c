/* Version 0.05 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>

// #include <X11/Xlib.h>
// #include <dirent.h>

#include "config.h"

unsigned int delay = 3;

/* Network Connections */
char *net(void) {
	FILE *carrier;
	char wlan;

	carrier = fopen(WLAN_CARFILE, "r");
	wlan = fgetc(carrier);
	fclose(carrier);

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
long long unsigned int memused(void) {
	FILE *meminfo;
	meminfo = fopen(MEMINFOFILE, "r");
	char buffer[48];
	long long unsigned int memtotal, memfree, buffers, cached;

	fgets(buffer, 48, meminfo);
	sscanf(buffer, "MemTotal: %32llu", &memtotal);
	fgets(buffer, 48, meminfo);
	sscanf(buffer, "MemFree: %32llu", &memfree);
	fgets(buffer, 48, meminfo);
	fgets(buffer, 48, meminfo);
	sscanf(buffer, "Buffers: %32llu", &buffers);
	fgets(buffer, 48, meminfo);
	sscanf(buffer, "Cached: %32llu", &cached);
	fclose(meminfo);
	return (memtotal - memfree - buffers - cached) / 1024;
}

/* CPU (core0) freq */
float freq(void) {
	FILE *freq;
	float corefreq;

	freq = fopen(CPU_FREQFILE, "r");
	fscanf(freq, "%f", &corefreq);
	corefreq = corefreq * 0.000001;
	fclose(freq);
	return corefreq;
}

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

	ac = fopen(AC_FILE, "r");
	supply = fgetc(ac);
	fclose(ac);
	if (supply == 49)
		return 0;
	else {
		ac = fopen(BAT_CAPFILE, "r");
		fscanf(ac, "%hu", &supply);
		if (delay == 3)
			delay = 5;
		fclose(ac);
		return supply;
	}
}

/* Uptime */
unsigned int uptime(char hm) {
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
	sleep(delay);
	printf("%s ┃ %lluMB ┃ %0.1fGHz ┃ [%u%%] ┃ Uptime: %u:%u ┃ %s",
		net(), memused(), freq(), power(), uptime('h'), uptime('m'), unixtime());
	return 0;
}

//#include <sys/types.h>
//#include <linux/sysinfo.h>

//#define MB 1048576
//#define GB 1073741824

/*	Display *dpy;
	Window root;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fprintf(stderr, "ERROR: could not open display\n");
		return 1;
	}
	root = XRootWindow(dpy,DefaultScreen(dpy));
	for (;;) {
		sprintf(status, "%s ┃ %lluMB ┃ [%u%%] ┃ Uptime: %u:%u ┃ %s", net(), memused(), power(), uptime('h'), uptime('m'), unixtime());
		XStoreName(dpy,root,status);
		XFlush(dpy);
		sleep(3);
	}*/
