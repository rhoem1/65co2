#ifndef __APPLE1_H__
#define __APPLE1_H__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

// cpu emulation
#include "65co2.h"

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
    SixtyFiveCeeOhTwo *cpu;
    bool running = true;

    /**
	 * stop emulation memory intercept
	 */
    struct memoryInterceptStopEmulation : memoryIntercept
    {
        AppleOne *apple;
        memoryInterceptStopEmulation(AppleOne *_apple) { apple = _apple; }
        virtual unsigned char read(unsigned short address)
        {
            apple->running = false;
            return 0;
        }
        virtual void write(unsigned char value, unsigned short address)
        {
            apple->running = false;
        }
    };

    /**
	 * cycle count reporting intercept
	 */
    struct memoryInterceptCycleCount : memoryIntercept
    {
        int reg;
        AppleOne *apple;
        memoryInterceptCycleCount(AppleOne *_apple)
        {
            memoryInterceptCycleCount();
            apple = _apple;
        }
        memoryInterceptCycleCount()
        {
            reg = 0;
        };
        virtual unsigned char read(unsigned short address)
        {
            return reg;
        }
        virtual void write(unsigned char value, unsigned short address)
        {
        }

        void update(int cycles)
        {
            reg += cycles;
            reg &= 0xFF;
        }
    };

    /**
	 * the PIA is made up of 4 memory intercepts
	 *
	 * KB    (keyboard input)
	 * KBcr  (keyboard control, status)
	 * DSP   (display output)
	 * DSPcr (display control, status)
	 */
    struct memoryInterceptPIAKB : memoryIntercept
    {
        int reg;
        AppleOne *apple;
        memoryInterceptPIAKB()
        {
            reg = 0;
        };
        memoryInterceptPIAKB(AppleOne *_apple)
        {
            memoryInterceptPIAKB();
            apple = _apple;
        }
        virtual unsigned char read(unsigned short address)
        {
            apple->checkKeyboard(true);

            apple->piaKBcr->reg = 0;
            apple->piaKBcr->counter = 10;

            return reg;
        }
        virtual void write(unsigned char value, unsigned short address)
        {
            reg = value;
        }
    };
    struct memoryInterceptPIAKBcr : memoryIntercept
    {
        unsigned char reg;
        int counter;
        AppleOne *apple;
        memoryInterceptPIAKBcr(AppleOne *_apple)
        {
            memoryInterceptPIAKBcr();
            apple = _apple;
        }
        memoryInterceptPIAKBcr()
        {
            reg = 0;
            counter = 10;
        };
        virtual unsigned char read(unsigned short address)
        {
            counter--;
            apple->checkKeyboard(false);
            if (counter == 0)
            {
                usleep(100);
                counter = 1;
            }
            //if(reg == 0) usleep(100);
            return reg;
        }
        virtual void write(unsigned char value, unsigned short address)
        {
            reg = value;
        }
    };
    struct memoryInterceptPIADSP : memoryIntercept
    {
        AppleOne *apple;
        memoryInterceptPIADSP(AppleOne *_apple)
        {
            memoryInterceptPIADSP();
            apple = _apple;
        }
        memoryInterceptPIADSP(){};
        void write(unsigned char value, unsigned short address)
        {
            apple->outputDsp(value);
        }
    };
    struct memoryInterceptPIADSPcr : memoryIntercept
    {
        unsigned char reg;
        AppleOne *apple;
        memoryInterceptPIADSPcr(AppleOne *_apple)
        {
            memoryInterceptPIADSPcr();
            apple = _apple;
        }
        memoryInterceptPIADSPcr()
        {
            reg = 0;
        };
        virtual unsigned char read(unsigned short address)
        {
            return 0;
        }
        virtual void write(unsigned char value, unsigned short address)
        {
            reg = value;
        }
    };

    /*
	 * Random number generation
	 */
    struct memoryInterceptRNG : memoryIntercept
    {
        AppleOne *apple;
        memoryInterceptRNG(AppleOne *_apple)
        {
            memoryInterceptRNG();
            apple = _apple;
        }
        memoryInterceptRNG()
        {
            srand(time(NULL));
        };
        virtual unsigned char read(unsigned short address)
        {
            return rand() & 0xFF;
        }
        virtual void write(unsigned char value, unsigned short address)
        {
            srand(value);
        }
    };

    // ioPages is used to block out all of $D000
    // so things like msbasic can autodetect available memory
    romIntercept *ioPages;
    memoryInterceptStopEmulation *stopEmulation;
    memoryInterceptPIAKB *piaKB;
    memoryInterceptPIAKBcr *piaKBcr;
    memoryInterceptPIADSP *piaDSP;
    memoryInterceptPIADSPcr *piaDSPcr;

    memoryInterceptCycleCount *cycleCounter;
    memoryInterceptRNG *miRNG;

    AppleOne()
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

        cpu->addIntercept(0xD01E, miRNG);
        cpu->addIntercept(0xD01F, cycleCounter);

        cpu->addIntercept(0xDFFF, stopEmulation);
    }

    ~AppleOne()
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

    virtual void outputDsp(unsigned char value) = 0;
    virtual void checkKeyboard(bool reading) = 0;
};

#endif
