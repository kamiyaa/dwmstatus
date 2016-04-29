# dwmstatus
A hardlinked status bar for dwm, written in C.

Should output something like this:
<---> ┃ 440MB ┃ [0%] ┃ Friday 29/10 09:10

Features:
 - Displays date

 - Displays battery percentage
   - only when unplugged

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
