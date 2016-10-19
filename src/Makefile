# Makefile

include config.mk

all: config dwmstatus

config:
	@echo creating config.h from config.def.h
	@cp config.def.h config.h

dwmstatus: dwmstatus.c Makefile
	@echo "    CC = ${CC}"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "    CC      dwmstatus.o"
	@${CC} ${CFLAGS} -c dwmstatus.c -o dwmstatus.o
	@echo "    CC      dwmstatus"
	@${CC} ${CFLAGS} dwmstatus.o -o dwmstatus

USER = $(shell whoami)
install:
	rm -vrf /tmp/$(USER)
	mkdir -v /tmp/$(USER)
	cp -v dwmstatus /tmp/$(USER)/dwmstatus
clean:
	rm -f *.o dwmstatus
