# dwmstatus
## Description
A hardlinked status bar for Dynamic Window Manager(dwm), written in C.

## Requirements
 - clang/gcc
 - Kernel options:
  - Expose thermal sensors as hwmon device (CONFIG_THERMAL_HWMON) [For getting cpu temps]
  - sysfs file system support (CONFIG_SYSFS) [Where most information will be parsed from]
  - /proc file system support (CONFIG_PROC_FS) [For memory, uptime and audio info]

## Installation
```
git clone git@github.com:Kamiyaa/dwmstatus.git
cd dwmstatus/src
make
```
## Configuration
All configurations are done through config.def.h/config.h
to keep the program simple and fast(?).
Note: some functions may be commented from my usages.
Uncomment to use them.

Output should look something like this:

<---> │ 624MB/1444MB │ 0.5GHz │ [0%] │ Mon 01/02  10:19

## Features
 - Displays date

 - Displays uptime

 - Displays battery percentage

 - Displays memory used (in megabytes only)

 - Displays cpu frequency
   - will only get cpu0 temperature

 - Displays cpu temperature
   - will only get cpu0 temperature
   - requires the kernel to expose hwmon in sysfs

 - Displays network connections
   - only if connected to access point
   - does not check for internet access,
