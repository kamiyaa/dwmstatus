#include <sys/sysinfo.h>

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

#include "config.h"

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
char *network_status();

/**
 * get and return the current amount of free ram in KBs
 */
long memfree(struct sysinfo *s_info);

/**
 * get and return the total amount of ram in KBs
 */
unsigned long memtotal(struct sysinfo *s_info);

/**
 * get and return the current amount of ram used in KBs
 */
unsigned long memused(struct sysinfo *s_info);

long alsa_get_max_vol(snd_mixer_t *handle);
snd_mixer_t *create_alsa_handle();
unsigned int alsa_volume(snd_mixer_t *handle);

/**
 * get and return the current frequency of the core
 */
float cpufreq();

/**
 * get and return the temperature of the core in celsius
 */
short cputemp();

/* Volume (Hexadecimal) */

/* get battery drain in watts */
float battery_watt_drain();

/* Power */
char *power_status();

/**
 * get and return an array of chars representing the time of the system:
 * day_of_week month/day  hour:minutes
 */
char *unixtime(void);

/* get wireless SSID */

char *wifi_ssid();
