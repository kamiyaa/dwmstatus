# compiler and linker
CC = cc

# flags
CFLAGS = -ggdb3 -Os -Wall -Wno-unused-variable -march=native -fno-stack-protector -D_GNU_SOURCE

# X11 export support
DEP_XCB = `pkg-config --cflags --libs xcb xcb-atom`

# output volume support
DEP_ALSA = `pkg-config --cflags --libs alsa`

LIBS = ${DEP_XCB} ${DEP_ALSA}

PREFIX=/usr/local
