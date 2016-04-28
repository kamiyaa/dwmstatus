#define SYSDIR "/sys"
#define PROCDIR "/proc"

/* File to check if you are connected to power supply */
#define ACFILE		SYSDIR "/class/power_supply/AC/online"

/* File to get battery percentage */
#define BATTERYCAPFILE	SYSDIR "/class/power_supply/BAT0/capacity"

/* File to get current frequency of a single cpu */
#define CPU0FREQFILE	SYSDIR "/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"

/* File to get current temperature of a single cpu */
#define CPU0TEMPFILE	SYSDIR "/class/hwmon/hwmon1/temp1_input"

/* File to see if we are connected to a network */
#define ENPCARRIERFILE	SYSDIR "/class/net/enp0s25/carrier"

/* File to see if we are connected to a network wirelessly */
#define WLPCARRIERFILE	SYSDIR "/class/net/wlp4s0/carrier"

/* Memory information file (Should usually not be changed)*/
#define PROCMEMINFOFILE	PROCDIR "/meminfo"

//
//#define BATTERYDIR "/sys/class/power_supply/BAT0"

