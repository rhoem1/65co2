#ifndef __APPLE1_H__
#define __APPLE1_H__

#include <memory>

// cpu emulation
#include "Cpu/65co2.h"


/**
 * Apple One Emulation
 * child of SixtyFiveCeeOhTwo cpu emulation
 * 
 * Memory Map:
 * $D000-$DFFF I/O range
 * 
 */
struct AppleOne
{
		std::unique_ptr<SixtyFiveCeeOhTwo> cpu;
    bool running = true;

    /**
     * stop emulation memory intercept
     */
    struct memoryInterceptStopEmulation : memoryIntercept
    {
      AppleOne *apple;
      memoryInterceptStopEmulation(AppleOne *_apple);
      virtual unsigned char read(unsigned short address);
      virtual void write(unsigned char value, unsigned short address);
    };

    /**
     * the PIA is made up of 4 memory intercepts
     *
     * KB    (keyboard input)
     * KBcr  (keyboard control, status)
     * DSP   (display output)
     * DSPcr (display control, status)
     */
    struct memoryInterceptPIA : memoryIntercept
    {
      int keyin = 0;
      int keycontrol = 0;
      int dspout = 0;
      int dspcontrol = 0;
      AppleOne *apple;
      memoryInterceptPIA(AppleOne *_apple);
      virtual unsigned char read(unsigned short address);
      virtual void write(unsigned char value, unsigned short address);
    };

    /*
	 * Random number generation
	 */
    struct memoryInterceptRNG : memoryIntercept
    {
        AppleOne *apple;
        memoryInterceptRNG(AppleOne *_apple);
        virtual unsigned char read(unsigned short address);
        virtual void write(unsigned char value, unsigned short address);
    };

    // ioPages is used to block out all of $D000
    // so things like msbasic can autodetect available memory
    std::unique_ptr<romIntercept> ioPages;
    std::unique_ptr<memoryInterceptStopEmulation> stopEmulation;
    std::unique_ptr<memoryInterceptRNG> miRNG;
    std::unique_ptr<memoryInterceptPIA> pia;


    AppleOne();

    virtual void outputDsp(unsigned char value) = 0;
    virtual void checkKeyboard(bool reading) = 0;
};

#endif
