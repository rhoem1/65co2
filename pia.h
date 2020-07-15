#ifndef __PIA_H__
#define __PIA_H__


/*
 * The Peripherial Interface Adapter
 * 6520
 *
 */


struct SixtyFiveTwenty {

	SixtyFiveOhTwo *cpu;

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


	struct piaMemoryInterceptCA *pMICA;
	struct piaMemoryInterceptCB *pMICB;
	struct piaMemoryInterceptDA *pMIDA;
	struct piaMemoryInterceptDB *pMIDB;



	SixtyFiveTwenty(SixtyFiveOhTwo *CPU, unsigned short address) {
		cpu = CPU;
		pMICA = new piaMemoryInterceptCA(CPU,this);
		pMICB = new piaMemoryInterceptCB(CPU,this);
		pMIDA = new piaMemoryInterceptDA(CPU,this);
		pMIDB = new piaMemoryInterceptDB(CPU,this);
		cpu->intercepts[address] = &pMICA;
		cpu->intercepts[address + 1] = &pMICB;
		cpu->intercepts[address + 2] = &pMIDA;
		cpu->intercepts[address + 3] = &pMIDB;

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
		cpu->interrupt_cpu();
	}

	void setIrqA2() {
		CA.irq2 = true;
		cpu->interrupt_cpu();
	}

	void setIrqB1() {
		CB.irq1 = true;
		cpu->interrupt_cpu();
	}

	void setIrqB2() {
		CB.irq2 = true;
		cpu->interrupt_cpu();
	}

	



};


struct piaMemoryInterceptCA : public memoryIntercept {
	SixtyFiveTwenty *pia;
	piaMemoryInterceptCA(SixtyFiveOhTwo *CPU, SixtyFiveTwenty *PIA) {
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

}
struct piaMemoryInterceptDA : public memoryIntercept {
	SixtyFiveTwenty *pia;
	piaMemoryInterceptDA(SixtyFiveOhTwo *CPU, SixtyFiveTwenty *PIA) {
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

}
struct piaMemoryInterceptCB : public memoryIntercept {
	SixtyFiveTwenty *pia;
	piaMemoryInterceptCB(SixtyFiveOhTwo *CPU, SixtyFiveTwenty *PIA) {
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

}
struct piaMemoryInterceptDB : public memoryIntercept {
	SixtyFiveTwenty *pia;
	piaMemoryInterceptDB(SixtyFiveOhTwo *CPU, SixtyFiveTwenty *PIA) {
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

}

#endif
