# Makefile

include config.mk

all: config dwmstatus

config:
	@echo creating config.h from config.def.h
	cp config.def.h config.h

dwmstatus: dwmstatus.c Makefile
	@echo ${CC} ${CFLAGS} dwmstatus.c -o dwmstatus
	${CC} ${CFLAGS} dwmstatus.c -o dwmstatus
devel: dwmstatus-devel.c Makefile
	${CC} ${CFLAGS} dwmstatus-devel.c -o dwmstatus-devel


USER = $(shell whoami)
install:
	rm -vrf /tmp/$(USER)
	mkdir -v /tmp/$(USER)
	cp -v dwmstatus /tmp/$(USER)/dwmstatus
clean:
	rm -f *.o dwmstatus
