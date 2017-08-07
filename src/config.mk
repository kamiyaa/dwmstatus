# compiler and linker
CC = clang

# flags
CFLAGS = -ggdb3 -O2 -Wall -Werror -Wno-unused-variable -march=native

LIBS = -lX11 `pkg-config --cflags --libs xcb xcb-atom`
