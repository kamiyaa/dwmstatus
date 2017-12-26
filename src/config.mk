# compiler and linker
CC = cc

# flags
CFLAGS = -ggdb3 -Os -Wall -Wno-unused-variable -march=native -fno-stack-protector

LIBS = `pkg-config --cflags --libs xcb xcb-atom`
