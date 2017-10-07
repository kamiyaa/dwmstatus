#include <sys/sysinfo.h>

/* Status bar refresh rates on battery and on AC */
static const unsigned int rrate_battery	= 30;
static const unsigned int rrate_ac	= 3;
/* Status bar refresh rate */
static unsigned int status_rrate	= 1;

/**
 * grabs sysinfo
 */
void initialize_sysinfo(struct sysinfo *s_info);

/**
 * get the connectivity of network interfaces and return string
 * indicating the status of them:
 * <---> - wireless connection to network
 * [---] - wired connection to network
 * --/-- - no connection to network
 */
char *get_network_status(void);

/**
 * get and return the current amount of free ram in KBs
 */
long get_free_mem(struct sysinfo *s_info);

/**
 * get and return the total amount of ram in KBs
 */
unsigned long get_total_mem(struct sysinfo *s_info);

/**
 * get and return the current amount of ram used in KBs
 */
unsigned long get_used_mem(struct sysinfo *s_info);

/**
 * get and return the current frequency of the core
 */
float get_freq(void);

/**
 * get and return the temperature of the core in celsius
 */
short get_temp(void);

/* Volume (Hexadecimal) */

/* Power */
unsigned short get_power(void);

/**
 * get and return an array of chars representing the time of the system:
 * day_of_week month/day  hour:minutes
 */
char *unixtime(void);
