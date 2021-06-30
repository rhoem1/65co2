#makefile

CC=g++
DEFINES=
EXTRA_DEFINES=
CPPFLAGS= -g -std=c++11 -o3 -Wall
LDFLAGS=
DASM=/opt/dasm/bin/dasm
EXE=


OBJS= apple1.o 65co2.o

all: apple1

%.bin: %.asm
	$(DASM) $< -o$@ -f3 -s$*.sym

%.mon: %.bin 
	bintomon -l 8192 $< > $*.mon

apple1: $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) 

wozmon.bin: wozmon.s
	$(DASM) $< -o$@ -f3

wozmon_code.h: wozmon.bin
	php import_bin wozmon.bin WOZMON monitor > wozmon_code.h

intout.bin: asm/intout.s
	$(DASM) $< -o$@ -f3

# depends
apple1.o:apple1.cpp apple1.h 65co2.h apple1basic.h applesoft_lite_0_4.h msbasic.h msbasic_code.h wozmon_code.h wozmon.h
	$(CC) $(CPPFLAGS) -o $@ -c $<

65co2.o:65co2.cpp 65co2.h
	$(CC) $(CPPFLAGS) -o $@ -c $<

msbasic_code.h:msbasic/tmp/applesoft.bin
	php import_bin msbasic/tmp/applesoft.bin MSBASIC tmp_applesoft_bin > msbasic_code.h

msbasic/tmp/applesoft.bin:msbasic/*.s
	/opt/cc65/bin/ca65 -D applesoft msbasic/msbasic.s -o msbasic/tmp/applesoft.o -l msbasic/tmp/applesoft.lst
	/opt/cc65/bin/ld65 -C msbasic/applesoft.cfg msbasic/tmp/applesoft.o -o msbasic/tmp/applesoft.bin -Ln msbasic/tmp/applesoft.lbl

clean: 
	rm -f $(OBJS) *.bin apple1

