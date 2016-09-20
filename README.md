# dwmstatus
A hardlinked status bar for dwm, written in C.

Output should look something like this:
<---> ┃ 427MB ┃ [0%] ┃ Uptime: 0:17 ┃ Monday 16/05 08:03

Features:
 - Displays date

 - Displays uptime

 - Displays battery percentage

 - Displays memory used
   - will only display in Megabytes

 - Displays cpu frequency
   - currently commented 
   - will only get cpu0 temperature

 - Displays cpu temperature
   - currently commented
   - will only get cpu0 temperature
   - requires the kernel to expose hwmon in sysfs

 - Displays network connections
   - only displays if connected or not
   - cannot check for internet access
