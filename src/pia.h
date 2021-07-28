#ifndef __PIA_H__
#define __PIA_H__

#include "65co2.h"

/*
 * The Peripherial Interface Adapter
 * 6520
 *
 */


struct SixtyFiveTwenty {

	SixtyFiveCeeOhTwo *cpu;

	typedef struct __controlreg {
		bool irq1;
		bool irq2;
		unsigned char control2;
		bool output;
		unsigned char control1;

		unsigned char getByte() {
			return (0x80 & irq1)
				| (0x40 & irq2)
				| ((control2 & 0x07) << 3)
				| (0x40 & output)
				| (control1 & 0x3);
		}

		void setByte(unsigned char b) {
			irq1 = b & 0x80;
			irq2 = b & 0x40;
			control2 = (b & 0x28) >> 3;
			output = b & 0x40;
			control1 = b & 0x03;
		}

		__controlreg() {
			irq1 = false;
			irq2 = false;
			control2 = 0;
			control1 = 0;
			output = false;
		}
	} PortControl;

	PortControl CA;
	PortControl CB;
	unsigned char DA; // data A 
	unsigned char DB; // data B

	struct piaMemoryInterceptCA : public memoryIntercept {
		SixtyFiveTwenty *pia;
		SixtyFiveCeeOhTwo *cpu;
		piaMemoryInterceptCA(SixtyFiveCeeOhTwo *CPU, SixtyFiveTwenty *PIA) {
			cpu = CPU;
			pia = PIA;
		}
		// write a byte to a register from cpu
		void write(unsigned char value) {
			pia->CA.setByte(value);
		}

		// read a register from cpu
		unsigned char read() {
			return pia->CA.getByte();
		}

	};
	struct piaMemoryInterceptDA : public memoryIntercept {
		SixtyFiveTwenty *pia;
		SixtyFiveCeeOhTwo *cpu;
		piaMemoryInterceptDA(SixtyFiveCeeOhTwo *CPU, SixtyFiveTwenty *PIA) {
			cpu = CPU;
			pia = PIA;
		}
		// write a byte to a register from cpu
		void write(unsigned char value) {
			pia->CB.setByte(value);
		}

		// read a register from cpu
		unsigned char read() {
			return pia->CB.getByte();
		}

	};
	struct piaMemoryInterceptCB : public memoryIntercept {
		SixtyFiveTwenty *pia;
		SixtyFiveCeeOhTwo *cpu;
		piaMemoryInterceptCB(SixtyFiveCeeOhTwo *CPU, SixtyFiveTwenty *PIA) {
			cpu = CPU;
			pia = PIA;
		}
		// write a byte to a register from cpu
		void write(unsigned char value) {
			pia->DA = value;
		}

		// read a register from cpu
		unsigned char read() {
			return pia->DA;
		}

	};
	struct piaMemoryInterceptDB : public memoryIntercept {
		SixtyFiveTwenty *pia;
		SixtyFiveCeeOhTwo *cpu;
		piaMemoryInterceptDB(SixtyFiveCeeOhTwo *CPU, SixtyFiveTwenty *PIA) {
			cpu = CPU;
			pia = PIA;
		}
		// write a byte to a register from cpu
		void write(unsigned char value) {
			pia->DB = value;
		}

		// read a register from cpu
		unsigned char read() {
			return pia->DB;
		}

	};


	piaMemoryInterceptCA *pMICA;
	piaMemoryInterceptCB *pMICB;
	piaMemoryInterceptDA *pMIDA;
	piaMemoryInterceptDB *pMIDB;



	SixtyFiveTwenty(SixtyFiveCeeOhTwo *CPU, unsigned short address) {
		cpu = CPU;
		pMICA = new piaMemoryInterceptCA(CPU, this);
		pMICB = new piaMemoryInterceptCB(CPU, this);
		pMIDA = new piaMemoryInterceptDA(CPU, this);
		pMIDB = new piaMemoryInterceptDB(CPU, this);
		cpu->addIntercept(address, pMICA);
		cpu->addIntercept(address + 1, pMICB);
		cpu->addIntercept(address + 2, pMIDA);
		cpu->addIntercept(address + 3, pMIDB);

	}

	~SixtyFiveTwenty() {
		delete pMICA;
		delete pMICB;
		delete pMIDA;
		delete pMIDB;
	}



	//  read and write to Port A and B
	void writePortA(unsigned char b) {
	}

	unsigned char readPortA() {
	}

	void writePortB(unsigned char b) {
	}

	unsigned char readPortB() {
	}

	// interrupts
	void setIrqA1() {
		CA.irq1 = true;
		cpu->maskable_interrupt();
	}

	void setIrqA2() {
		CA.irq2 = true;
		cpu->maskable_interrupt();
	}

	void setIrqB1() {
		CB.irq1 = true;
		cpu->maskable_interrupt();
	}

	void setIrqB2() {
		CB.irq2 = true;
		cpu->maskable_interrupt();
	}

	void clearIrqA1() {
		CA.irq1 = false;
		cpu->clear_maskable_interrupt();
	}

	void clearIrqA2() {
		CA.irq2 = false;
		cpu->clear_maskable_interrupt();
	}

	void clearIrqB1() {
		CB.irq1 = false;
		cpu->clear_maskable_interrupt();
	}

	void clearIrqB2() {
		CB.irq2 = false;
		cpu->clear_maskable_interrupt();
	}
	

};

#endif
