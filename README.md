# dwmstatus
## Description
A hardlinked status bar for Dynamic Window Manager(dwm), written in C.

## Usage
To run, just execute the binary from a console:
./dwmstatus

## Configuration
All configurations are done through config.def.h/config.h
to keep the program simple and fast(?).
Note: some functions may be commented from my usages.
Uncomment to use them.

Output should look something like this:
<---> ┃ 427MB ┃ [0%] ┃ Uptime: 0:17 ┃ Monday 16/05 08:03

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
