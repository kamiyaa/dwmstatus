# Makefile

include config.mk

all: options dwmstatus_xcb dwmstatus_std

options:

	@echo "    CC = ${CC}"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "  LIBS = ${LIBS}"

config.def.h:

	@echo "cp config.def.h -> config.h"
	cp config.def.h config.h

dwmstatus_xcb.o: config.def.h dwmstatus_xcb.c
	@echo CC dwmstatus_xcb.c -o $@
	@${CC} ${CFLAGS} -c dwmstatus_xcb.c -o $@

dwmstatus_xcb: dwmstatus_xcb.o dwmstatus_xcb.c

	@echo CC dwmstatus_xcb.o -o $@
	@${CC} ${CFLAGS} $@.o -o $@ ${LIBS}

dwmstatus_std.o: config.def.h dwmstatus_std.c
	@echo CC dwmstatus_std.c -o $@
	@${CC} ${CFLAGS} -c dwmstatus_std.c -o $@

dwmstatus_std: dwmstatus_std.o dwmstatus_std.c

	@echo CC dwmstatus_std.o -o $@
	@${CC} ${CFLAGS} $@.o -o $@

install:

	cp dwmstatus /usr/local/bin/dwmstatus

clean:

	rm -f *.o dwmstatus_std dwmstatus_xcb
