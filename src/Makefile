# Makefile

include config.mk

all: config dwmstatus

config:
	@echo creating config.h from config.def.h
	cp config.def.h config.h

dwmstatus: dwmstatus.c Makefile
	@echo ${CC} ${CFLAGS} dwmstatus.c -o dwmstatus
	${CC} ${CFLAGS} -c dwmstatus.c -o dwmstatus.o
	${CC} ${CFLAGS} dwmstatus.o -o dwmstatus

USER = $(shell whoami)
install:
	rm -vrf /tmp/$(USER)
	mkdir -v /tmp/$(USER)
	cp -v dwmstatus /tmp/$(USER)/dwmstatus
clean:
	rm -f *.o dwmstatus
