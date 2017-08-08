# compiler and linker
CC = clang

# flags
CFLAGS = -ggdb3 -O0 -Wall -Werror -Wno-unused-variable -march=native

LIBS = `pkg-config --cflags --libs xcb xcb-atom`
