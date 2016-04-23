# Makefile

all: dwmstatus

dwmstatus: dwmstatus.c Makefile
	gcc -ggdb3 -O0 -Wall -Werror -Wno-unused-variable dwmstatus.c -o dwmstatus

clean:
	rm -f *.o dwmstatus
