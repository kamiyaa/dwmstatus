# Makefile

#LIBS = -lX11
STD = -std=c11

all: config dwmstatus

config:
	@echo creating config.h from config.def.h
	cp config.def.h config.h

dwmstatus: dwmstatus.c Makefile
	gcc -ggdb3 -O0 -Wall -Werror -Wno-unused-variable $(STD) dwmstatus.c -o dwmstatus
devel: dwmstatus-devel.c Makefile
	gcc -ggdb3 -O0 -Wall -Werror -Wno-unused-variable $(STD) dwmstatus-devel.c -o dwmstatus-devel


USER = $(shell whoami)
install:
	rm -vrf /tmp/$(USER)
	mkdir -v /tmp/$(USER)
	cp -v dwmstatus /tmp/$(USER)/dwmstatus
clean:
	rm -f *.o dwmstatus
