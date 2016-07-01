# Makefile

#LIBS = -lX11
STD = -std=c11
LIBS = 

all: dwmstatus

dwmstatus: dwmstatus.c Makefile
	gcc -ggdb3 -O0 -Wall -Werror -Wno-unused-variable $(STD) $(LIBS) dwmstatus.c -o dwmstatus

clean:
	rm -f *.o dwmstatus
