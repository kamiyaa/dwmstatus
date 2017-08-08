# Makefile

include config.mk

all: dwmstatus

dwmstatus: config.def.h dwmstatus.c
	@echo "cp config.def.h -> config.h"
	@cp config.def.h config.h

	@echo "    CC = ${CC}"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "  LIBS = ${LIBS}"
	@echo "    CC    dwmstatus.o"
	@${CC} ${CFLAGS} -c dwmstatus.c -o dwmstatus.o
	@echo "    CC    dwmstatus"
	@${CC} ${CFLAGS} dwmstatus.o -o dwmstatus ${LIBS}

install:
	cp dwmstatus /usr/local/bin/dwmstatus

clean:
	rm -f *.o dwmstatus
