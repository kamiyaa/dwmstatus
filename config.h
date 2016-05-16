/* File to check if you are connected to power supply */
#define AC_FILE		"/sys/class/power_supply/AC/online"
/* File to get battery percentage */
#define BAT_CAPFILE	"/sys/class/power_supply/BAT0/capacity"

/* File to get current frequency of a single cpu */
#define CPU_FREQFILE	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"

/* File to get current temperature of a single cpu */
#define CPU_TEMPFILE	"/sys/class/hwmon/hwmon1/temp1_input"

/* File to see if we are connected to a network */
#define ETH_CARFILE	"/sys/class/net/enp0s25/carrier"

/* File to see if we are connected to a network wirelessly */
#define WLAN_CARFILE	"/sys/class/net/wlp4s0/carrier"

/* Memory information file (Usually can be left unchanged) */
#define MEMINFOFILE	"/proc/meminfo"

/* Uptime file (Usually can be left unchanged) */
#define UPTIMEFILE	"/proc/uptime"


//
//#define BATTERYDIR "/sys/class/power_supply/BAT0"

