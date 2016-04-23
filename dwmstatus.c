// Version 0.01

#include <stdio.h>
#include <time.h>
#include <unistd.h>
//#include <dirent.h>

#define AC_ON "/sys/class/power_supply/AC/online"
#define BAT0_CAP "/sys/class/power_supply/BAT0/capacity"
#define CPU0_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"
#define CPU0_TEMP "/sys/class/hwmon/hwmon1/temp1_input"
#define ENP_CAR "/sys/class/net/enp0s25/carrier"
#define WLP_CAR "/sys/class/net/wlp4s0/carrier"
//#define BAT0_DIR "/sys/class/power_supply/BAT0"
//#define MEMINFO "/proc/meminfo"

/* Network Connections */
char *net(void) {
	FILE *west;
	char wlan;

	west = fopen(WLP_CAR, "r");
	wlan = fgetc(west);
	fclose(west);

	if (wlan == '1')
		return "<--->";
	else {
		west = fopen(ENP_CAR, "r");
		wlan = (int)fgetc(west);
		fclose(west);
		if (wlan == '1')
			return "[---]";
		else
			return "--/--";
	}
}

/* CPU (core0) freq */
/*float freq(void) {
	FILE *freq;
	float corefreq;

	freq = fopen(CPU0_FREQ, "r");
	fscanf(freq, "%f", &corefreq);
	corefreq = corefreq * 0.000001;
	fclose(freq);
	return corefreq;
}*/

/* CPU (core0) temp */
/*unsigned int temp(void) {
	FILE *temps;
	unsigned int coretemp;

	temps = fopen(CPU0_TEMP, "r");
	fscanf(temps, "%ud", &coretemp);
	coretemp = coretemp * 0.001;
	fclose(temps);
	return coretemp;
}*/

/* Power */
unsigned short power(void) {
	FILE *ac;
	unsigned short supply;

	ac = fopen(AC_ON, "r");
	supply = fgetc(ac);
	fclose(ac);
	if (supply == 49)
		return 0;
	else {
		ac = fopen(BAT0_CAP, "r");
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
	printf("%s ┃ [%u%%] ┃ %s\n", net(), power(), unixtime());
//	sleep(3);

//	printf("%s ┃ %uM ┃ %0.1fGHz ┃ %u°C ┃ [%u%%] ┃ %s\n", net(), mem(), freq(), temp(), power(), unixtime());
//	printf("%s ┃ %0.1fGHz ┃ %u°C ┃ [%u%%] ┃ %s\n", net(), freq(), temp(), power(), unixtime());
	return 0;
}

//#include <sys/types.h>
//#include <linux/sysinfo.h>

//#define MB 1048576
//#define GB 1073741824

/*unsigned int mem(void) {
	FILE * meminfo = fopen(MEMINFO, "r");

	if (meminfo == NULL)
		return 1;

	unsigned int linen = 0, memuse, factor = 1, add;
	unsigned int memt = 15, memtotal = 0, memava = 0;
	// , unit = 2;
	static char checks;

	for (int i = 0; i < 9; i++) {
		fgetc(meminfo);
	}

	while ((int)checks < 48 || (int)checks > 57) {
		checks = fgetc(meminfo);
		if (checks == ' ')
			memt--;
	}

	for (int i = 0, n = memt - 1; i < n; i++) {
		factor = factor * 10;
	}


	for (int i = 0; i < memt; i++)  {
		add = checks - '0';
		memtotal = memtotal + add * factor;
		factor = factor / 10;
		checks = fgetc(meminfo);
	}

	while (linen < 2) {
		checks = fgetc(meminfo);
		if (checks == '\n')
			linen++;
	}

	for (int i = 0; i < 12; i++) {
		fgetc(meminfo);
	}

	memt = 11;
	checks = fgetc(meminfo);

	while ((int)checks < 48 || (int)checks > 57) {
		checks = fgetc(meminfo);
		if (checks == ' ') {
			memt--;
		}
	}

	factor = 1;
	for (int i = 0, n = memt - 1; i < n; i++) {
		factor = factor * 10;
	}

	for (int i = 0; i < memt; i++)  {
		add = checks - '0';
		memava = memava + add * factor;
		factor = factor / 10;
		checks = fgetc(meminfo);
	}

	fclose(meminfo);
	memuse = (memtotal - memava) / 1024;

	return memuse;

}*/

