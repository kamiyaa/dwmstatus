/* Status bar refresh rate */
static unsigned int status_rrate = 3;

/* Hardcoded file locations */

/* File stating if connection to AC is present */
#define AC_FILE		"/sys/class/power_supply/AC/online"

/* File with battery value */
#define BAT_CAPFILE	"/sys/class/power_supply/BAT0/capacity"

/* File with current frequency of first core */
#define CPU_FREQFILE	"/sys/devices/system/cpu/cpufreq/policy0/scaling_cur_freq"

/* File with current temperature of first core */
#define CPU_TEMPFILE	"/sys/class/hwmon/hwmon0/temp1_input"

/* File with current connectivity status of (wired) network interface */
#define ETH_CARFILE	"/sys/class/net/enp0s25/carrier"

/* File with current connectivity status of (wireless) network interface */
#define WLAN_CARFILE	"/sys/class/net/wlp4s0/carrier"

/* File with all memory information */
#define MEMINFOFILE	"/proc/meminfo"

/* File with uptime of computer */
#define UPTIMEFILE	"/proc/uptime"

/* File with volume of speakers (hex value) */
#define SOUNDFILE	"/proc/asound/card1/codec#0"

/* Directory for battery presence */
#define BAT_DIR		"/sys/class/power_supply/BAT0"
