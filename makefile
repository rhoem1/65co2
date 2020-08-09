#makefile

CC=g++
DEFINES=
EXTRA_DEFINES=
CPPFLAGS= -g -std=c++11 -o3 -Wall
LDFLAGS=
DASM=/opt/dasm/bin/dasm
EXE=


OBJS= apple1.o 

all: apple1

apple1: $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) 

wozmon.bin: wozmon.s
	$(DASM) $< -o$@ -f3

intout.bin: asm/intout.s
	$(DASM) $< -o$@ -f3

# depends
apple1.o:apple1.cpp apple1.h 65co2.h apple1basic.h applesoft_lite_0_4.h msbasic.h msbasic_code.h
	$(CC) $(CPPFLAGS) -o $@ -c $<


clean: 
	rm -f $(OBJS) *.bin apple1

