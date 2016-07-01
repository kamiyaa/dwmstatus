/* Header file for dwmstatus.c */

unsigned int delay = 3;

/* Definitions */
#define AC_FILE		"/sys/class/power_supply/AC/online"			/* File stating if connection to AC is present */
#define BAT_CAPFILE	"/sys/class/power_supply/BAT0/capacity"			/* File stating battery value */
#define CPU_FREQFILE	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"	/* File stating the current frequency of one core */
#define CPU_TEMPFILE	"/sys/class/hwmon/hwmon1/temp1_input"			/* File stating current temperature of one core */
#define ETH_CARFILE	"/sys/class/net/enp0s25/carrier"		/* File stating connectivity of (wired) network interface */
#define WLAN_CARFILE	"/sys/class/net/wlp4s0/carrier"			/* File stating connectivity of (wireless) network interface */
#define MEMINFOFILE	"/proc/meminfo"					/* File stating all memory information */
#define UPTIMEFILE	"/proc/uptime"					/* File stating uptime of computer */
#define SOUNDFILE	"/proc/asound/card1/codec#0"			/* File stating volume of speakers (hex value) */

/* Functions */
char *net(void);
long long unsigned int memused(void);
float freq(void);
unsigned short temp(void);
unsigned int volume(void);
unsigned short power(void);
unsigned int uptime(char hm);
char *unixtime(void);

