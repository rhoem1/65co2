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
#include <memory>


// emulation
#include "apple1.h"

AppleOne::AppleOne()
{
	cpu = std::make_unique<SixtyFiveCeeOhTwo>();
	ioPages = std::make_unique<romIntercept>(cpu.get());

	pia = std::make_unique<memoryInterceptPIA>(this);

	stopEmulation = std::make_unique<memoryInterceptStopEmulation>(this);
	miRNG = std::make_unique<memoryInterceptRNG>(this);

	cpu->addInterceptRange(0xD000, 0x1000, ioPages.get());

	// wire up pia intercepts
	cpu->addInterceptRange(0xD010, 0x4, pia.get());

	cpu->addIntercept(0xCFFF, miRNG.get());

	cpu->addIntercept(0xD01F, stopEmulation.get());
}

AppleOne::memoryInterceptStopEmulation::memoryInterceptStopEmulation(AppleOne *_apple)
{
  apple = _apple;
}
unsigned char AppleOne::memoryInterceptStopEmulation::read(unsigned short address)
{
  apple->running = false;
  return 0;
}
void AppleOne::memoryInterceptStopEmulation::write(unsigned char value, unsigned short address)
{
  apple->running = false;
}

AppleOne::memoryInterceptPIA::memoryInterceptPIA(AppleOne *_apple)
{
  apple = _apple;
}
unsigned char AppleOne::memoryInterceptPIA::read(unsigned short address)
{
  switch (address & 0x03)
  {
  // KB
  case 0x0:
    apple->checkKeyboard(true);
    keycontrol = 0;
    return keyin;
  // KBcr
  case 0x1:
    apple->checkKeyboard(false);
    return keycontrol;
  case 0x2:
    return 0; // always ready to accept a char
  case 0x3:
    return dspcontrol;
  default:
    return 0;
  }
}

void AppleOne::memoryInterceptPIA::write(unsigned char value, unsigned short address)
{
  switch (address & 0x03)
  {
  case 0x0:
    keyin = value;
    break;
  case 0x1:
    keycontrol = value;
    break;
  case 0x2:
    dspout = value;
    apple->outputDsp(value);
    break;
  case 0x3:
    dspcontrol = value;
    break;
  }
}

AppleOne::memoryInterceptRNG::memoryInterceptRNG(AppleOne *_apple)
{
  srand(time(NULL));
  apple = _apple;
}
unsigned char AppleOne::memoryInterceptRNG::read(unsigned short address)
{
  return rand() & 0xFF;
}
void AppleOne::memoryInterceptRNG::write(unsigned char value, unsigned short address)
{
  srand(value);
}