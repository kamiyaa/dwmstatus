# Makefile

include config.mk

all: options dwmstatus_xcb dwmstatus_std

options:

	@echo "    CC = ${CC}"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "  LIBS = ${LIBS}"

config.h: config.def.h

	@echo "cp config.def.h -> config.h"
	cp config.def.h config.h

dwmstatus_xcb.o: config.h dwmstatus_xcb.c dwmstatus.c
	${CC} ${CFLAGS} -c dwmstatus_xcb.c -o $@

dwmstatus_xcb: dwmstatus_xcb.o dwmstatus_xcb.c dwmstatus.o
	${CC} ${CFLAGS} dwmstatus.o $@.o -o $@ ${LIBS}

dwmstatus_std.o: config.h dwmstatus_std.c dwmstatus.c
	${CC} ${CFLAGS} -c dwmstatus_std.c -o $@

dwmstatus_std: dwmstatus_std.o dwmstatus_std.c dwmstatus.o
	${CC} ${CFLAGS} $@.o dwmstatus.o -o $@

install:
	cp dwmstatus_std /usr/local/bin/dwmstatus_std
	cp dwmstatus_xcb /usr/local/bin/dwmstatus_xcb

clean:
	rm -f *.o dwmstatus_std dwmstatus_xcb
