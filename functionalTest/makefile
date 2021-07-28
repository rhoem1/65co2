#makefile

CC=g++
DEFINES=
EXTRA_DEFINES=
CPPFLAGS= -g -std=c++11 -o3
LDFLAGS=
DASM=/opt/dasm/bin/dasm
AS65=/opt/as65/as65
EXE=


OBJS= \
	  test.o 

all: test

test: $(OBJS) 6502_functional_test.bin
	$(CC) $(CPPFLAGS) -o $@ $(OBJS) $(LDFLAGS) 


6502_functional_test.bin:6502_functional_test.a65 report.i65
	$(AS65) -l -m -w -h0 $<



# depends
test.o:test.cpp ../65co2.h
	$(CC) $(CPPFLAGS) -o $@ -c $<


clean: 
	rm -f $(OBJS) test

