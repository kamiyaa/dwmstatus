# dwmstatus
## Description
A simple, customizable status bar for Dynamic Window Manager(dwm), written in C.

Exports status bar to X11 root window via xcb instead of xlib

Output:
```
<---> │ 0.8GHz │ 41°C │ [0%] │ 2:40 │ Mon 08/07  12:32 
```

## Requirements
 - clang/gcc
 - libxcb [For exporting status bar to root X11 window]
 - Kernel options:
    - CONFIG_PROC_FS: /proc file system support [For audio info]
   - CONFIG_SYSFS: sysfs file system support [Where most information will be parsed from]
   - CONFIG_THERMAL_HWMON: Expose thermal sensors as hwmon device [For getting cpu temps]

## Installation
```
git clone git@github.com:Kamiyaa/dwmstatus.git
cd dwmstatus/src
make
```

## Running
```
./dwmstatus_std  # for outputting to stdout
./dwmstatus_xcb  # for outputting to X11 root window
```

## Configuration
All configurations are done through config.def.h/config.h
to keep the program simple and fast(?).
Note: some functions may be commented from my usages.
Uncomment to use them.

## Features
 - Displays date, uptime, battery percentage
 - Displays a single cpu's frequency and temperature
 - Displays network connections
   - only if connected to access point, not internet access
