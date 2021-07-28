/*
 * 65co2 example
 *
 * an apple i emulator
 *
 * since the apple 1 didn't do graphics
 * it's basically just a 6502 and a 6521 PIA
 *
 * port A on the PIA talks to a display and port B on the PIA 
 * takes ASCII from a ASCII keyboard.
 * this lets us wire up port A to stdout and port B to stdin.
 * in fact we don't even need to emulate the PIA as the interrupt lines
 * are not connected -- just push and pull io
 *
 * includes several different BASIC interperters
 * and krusader v1.3
 *
 *
 *
 * (c) 2020 Robert Hoem
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

// emulation
#include "apple1.h"

AppleOne::AppleOne()
{
	cpu = new SixtyFiveCeeOhTwo();
	ioPages = new romIntercept(cpu);

	piaKB = new memoryInterceptPIAKB(this);
	piaKBcr = new memoryInterceptPIAKBcr(this);
	piaDSP = new memoryInterceptPIADSP(this);
	piaDSPcr = new memoryInterceptPIADSPcr(this);

	stopEmulation = new memoryInterceptStopEmulation(this);
	cycleCounter = new memoryInterceptCycleCount(this);
	miRNG = new memoryInterceptRNG(this);

	cpu->addInterceptRange(0xD000, 0x1000, ioPages);

	// wire up pia intercepts
	cpu->addIntercept(0xD010, piaKB);
	cpu->addIntercept(0xD011, piaKBcr);
	cpu->addIntercept(0xD012, piaDSP);
	cpu->addIntercept(0xD013, piaDSPcr);

	cpu->addIntercept(0xCFFF, miRNG);

	cpu->addIntercept(0xD01F, stopEmulation);
}

AppleOne::~AppleOne()
{
	delete miRNG;
	delete cycleCounter;
	delete stopEmulation;
	delete piaDSPcr;
	delete piaDSP;
	delete piaKBcr;
	delete piaKB;
	delete ioPages;
	delete cpu;
}


