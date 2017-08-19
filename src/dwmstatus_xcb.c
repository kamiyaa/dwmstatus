#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#include <sys/sysinfo.h>

#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>

#include "config.h"


/* Status bar refresh rate */
static unsigned int status_rrate = 1;

static struct sysinfo s_info;
static unsigned short keep_running = 1;
static xcb_connection_t *connection;

/**
 * grabs sysinfo
 */
void initialize_sysinfo(void)
{
	int error = sysinfo(&s_info);
	if (error)
		printf("Error: error code: %d\n", error);
}

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
	int retval = fscanf(freq_fd, "%f", &corefreq);
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
	int retval = fscanf(temps, "%ud", &coretemp);
	fclose(temps);
	coretemp = coretemp * 0.001;
	return coretemp;
}

/* Volume (Hexadecimal) */
/* unsigned int get_volume(void)
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
unsigned short get_power(void)
{
	/* Open up the sysfs file for battery info */
	FILE *power_fd;
	power_fd = fopen(BAT_CAPFILE, "r");
	unsigned short battery_charge = 0;
	/* If battery exists get battery charge*/
	if (power_fd) {
		int retval = fscanf(power_fd, "%hu", &battery_charge);
		fclose(power_fd);
		power_fd = fopen(AC_FILE, "r");
		char ac_on = fgetc(power_fd);
		fclose(power_fd);
		/* If connected to AC, refresh rate will be set to 3
		 * seconds
		 */
		if (ac_on == '1')
			status_rrate = rrate_ac;
		/* Else, change the refresh rate to 30 seconds to save
		 * battery
		 */
		else if (status_rrate != rrate_battery)
			status_rrate = rrate_battery;
	}
	/* We must be connected to ac then */
	else if (status_rrate != rrate_ac)
		status_rrate = rrate_ac;

	return battery_charge;
}

/**
 * get and return the total uptime of machine in seconds
 */
long get_uptime(void)
{
	return s_info.uptime;
}

/**
 * get and return an array of chars representing the time of the system:
 * day_of_week month/day  hour:minutes
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

/**
 * handles all memory cleanups when program is told to stop
 */
void exit_cleanup(int opt_code)
{
	keep_running = 0;
	/* disconnect from X server */
	xcb_disconnect(connection);
}


int main(void)
{
	/* display number */
	int screen_default_nbr;
	/* connect to display */
	connection = xcb_connect(NULL, &screen_default_nbr);

	/* get the screen and the root window */
	xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
	xcb_window_t root_window = 0;
	if (screen) {
		root_window = screen->root;
	}

	/* get the battery life */
	unsigned int battery_life;
	/* get the uptime of machine in minutes */
	long uptime;
	/* format the uptime into minutes */
	unsigned int up_hours;
	unsigned int up_minutes;
	/* get the network status */
	char *net_status;
	/* get the system time */
	char *system_time;
	/* get the temperature of cpu */
	unsigned short cpu_temp;

	unsigned short status_len = 70;
	char status[status_len];

	signal(SIGINT, exit_cleanup);

	while (keep_running) {
		/* setup sysinfo with values */
		initialize_sysinfo();

		/* get the battery life */
		battery_life = get_power();
		/* get the uptime of machine in minutes */
		uptime = get_uptime() / 60;
		/* format the uptime into minutes */
		up_hours = uptime / 60;
		up_minutes = uptime % 60;
		/* get the network status */
		net_status = get_network_status();
		/* get the system time */
		system_time = unixtime();
		/* get the temperature of cpu */
		cpu_temp = get_temp();

		/* if update delay is greater than 5, then we are on battery mode */
		if (status_rrate == rrate_battery) {
			snprintf(status, status_len,
				"%s \u2502 %u\u00B0C \u2502 [%u%%] \u2502 %d:%d \u2502 %s ",
				net_status, cpu_temp, battery_life, up_hours, up_minutes, system_time);
		}
		/* otherwise, we are in normal mode */
		else {
			snprintf(status, status_len,
				"%s \u2502 %0.1fGHz \u2502 %u\u00B0C \u2502 [%u%%] \u2502 %d:%d \u2502 %s ",
				net_status, get_freq(), cpu_temp, battery_life, up_hours, up_minutes, system_time);
		}

		/* changed root window name */
		xcb_change_property(connection,
			XCB_PROP_MODE_REPLACE,
			root_window,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING,
			8,
			status_len,
			status);

		/* update display */
		xcb_flush(connection);

		/* refresh rate */
		sleep(status_rrate);
	}

	return 0;
}

//		printf("%s \u2502 %uMB \u2502 %u°C \u2502 [%u%%] \u2502 %s \n",
//			get_net_carrier(), get_meminfo(), get_temp(), battery_life, unixtime());
