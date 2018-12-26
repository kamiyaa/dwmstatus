
/* Refresh rates of status bar */
#define STATUS_REFRESH_RATE_REG	3
#define STATUS_REFRESH_RATE_LOW	30

/* File indicating AC is present */
#define AC_FILE		"/sys/class/power_supply/AC/online"

/* Directory for battery presence */
#define BAT_DIR		"/sys/class/power_supply/BAT0"

/* File with battery value */
#define BAT_CAPFILE	BAT_DIR "/capacity"

/* File with battery drain information in milliwatts */
#define BAT_DRAIN_FILE	BAT_DIR "/power_now"

/* File with current frequency of first core */
#define CPU_FREQFILE	"/sys/devices/system/cpu/cpufreq/policy0/scaling_cur_freq"

/* File with current temperature of first core */
#define CPU_TEMPFILE	"/sys/devices/platform/coretemp.0/hwmon/hwmon0/temp1_input"

/* File with current connectivity status of (wired) network interface */
#define ETH_CARFILE	"/sys/class/net/enp3s0/carrier"

/* File with current connectivity status of (wireless) network interface */
#define WLAN_CARFILE	"/sys/class/net/wlp5s0/carrier"

/* Which sound card volume to display */
#define SOUNDCARD	"default"

#define SOUNDCONTROL	"Master"
/* Wich wireless to use */
#define IW_INTERFACE    "wlp2s0"
