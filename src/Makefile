# Makefile

include config.mk

all: config dwmstatus

config: config.def.h
	@echo creating config.h from config.def.h
	@cp config.def.h config.h

dwmstatus: config.def.h dwmstatus.c Makefile
	@echo "    CC = ${CC}"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "  LIBS = ${LIBS}"
	@echo "    CC      dwmstatus.o"
	@${CC} ${CFLAGS} -c dwmstatus.c -o dwmstatus.o
	@echo "    CC      dwmstatus"
	@${CC} ${CFLAGS} dwmstatus.o -o dwmstatus ${LIBS}

install:
	cp dwmstatus /usr/local/bin/dwmstatus

clean:
	rm -f *.o dwmstatus
