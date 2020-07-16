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
 *
 * includes several different BASIC interperters
 * and krusader v1.3
 *
 *
 *
 * (c) 2015 Robert Hoem
 *
 */


#include <string>
#include <list>
#include <iostream>
#include <iomanip>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <termios.h>

// cpu emulation
#include "65co2.h"

// roms
#include "wozmon.h"
#include "apple1basic.h"
#include "krusader13.h"
#include "applesoft_lite_0_4.h"
#include "msbasic.h"


/**
 * Apple One Emulation
 * child of SixtyFiveCeeOhTwo cpu emulation
 */
struct AppleOne : public SixtyFiveCeeOhTwo {
	bool running = true;
	bool debugging = false;
	bool stepping = false;
	bool quiet = false;

	/**
	 * stop emulation memory intercept
	 */ 
	struct memoryInterceptStopEmulation : memoryIntercept {
		AppleOne *cpu;
		virtual unsigned char read(unsigned short address) {
			cpu->running = false;
			return 0;
		}
		virtual void write(unsigned char value, unsigned short address) {
			cpu->running = false;
		}
	};

	memoryInterceptStopEmulation stopEmulation;


	/**
	 * cycle count reporting intercept
	 */
	struct memoryInterceptCycleCount : memoryIntercept {
		int reg;
		memoryInterceptCycleCount() {
			reg = 0;
		};
		virtual unsigned char read(unsigned short address) {
			return reg;
		}
		virtual void write(unsigned char value, unsigned short address) {
		}

		void update(int cycles) {
			reg += cycles;
			reg &= 0xFF;
		}

	};

	memoryInterceptCycleCount cycleCounter;

	/**
	 * the PIA is made up of 4 memory intercepts
	 *
	 * KB    (keyboard input)
	 * KBcr  (keyboard control, status)
	 * DSP   (display output)
	 * DSPcr (display control, status)
	 */
	struct memoryInterceptPIAKB : memoryIntercept {
		int reg;
		AppleOne *cpu;
		memoryInterceptPIAKB() {
			reg = 0;
		};
		virtual unsigned char read(unsigned short address) {
			cpu->checkKeyboard(true);

			cpu->piaKBcr.reg = 0;
			cpu->piaKBcr.counter = 10;

			return reg;
		}
		virtual void write(unsigned char value, unsigned short address) {
			reg = value;
		}

	};
	struct memoryInterceptPIAKBcr : memoryIntercept {
		unsigned char reg;
		int counter;
		AppleOne *cpu;
		memoryInterceptPIAKBcr() {
			reg = 0;
			counter = 10;
		};
		virtual unsigned char read(unsigned short address) {
			counter--;
			cpu->checkKeyboard(false);
			if(counter == 0) {
				usleep(100);
				counter = 1;
			}
			//if(reg == 0) usleep(100);
			return reg;
		}
		virtual void write(unsigned char value, unsigned short address) {
			reg = value;
		}
	};
	struct memoryInterceptPIADSP : memoryIntercept {
		AppleOne *cpu;
		memoryInterceptPIADSP() {
		};
		void write(unsigned char value, unsigned short address) {
			cpu->outputDsp(value);
		}
	};
	struct memoryInterceptPIADSPcr : memoryIntercept {
		unsigned char reg;
		AppleOne *cpu;
		memoryInterceptPIADSPcr() {
			reg = 0;
		};
		virtual unsigned char read(unsigned short address) {
			return 0;
		}
		virtual void write(unsigned char value, unsigned short address) {
			reg = value;
		}
	};
	memoryInterceptPIAKB    piaKB;
	memoryInterceptPIAKBcr  piaKBcr;
	memoryInterceptPIADSP   piaDSP;
	memoryInterceptPIADSPcr piaDSPcr;


	/*
	 * Random number generation
	 */
	struct memoryInterceptRNG : memoryIntercept {
		memoryInterceptRNG() {
			srand(time(NULL));
		};
		virtual unsigned char read(unsigned short address) {
			return rand() & 0xFF;
		}
		virtual void write(unsigned char value, unsigned short address) {
			srand(value);
		}
	};

	memoryInterceptRNG miRNG;


	// ioPages is used to block out all of $D000
	// so things like msbasic can autodetect available memory
	romIntercept ioPages;

	// various roms
	memoryInterceptWozMon monitorRomIntercept;
	memoryInterceptIntBasic BasicRom;
	memoryInterceptApplesoftLite ApplesoftLiteRom;
	memoryInterceptKrusader Krusader;
	memoryInterceptMSBasic MSBasic;

	bool width40 = true;
	int cursorX = 0;

	AppleOne() {
		SixtyFiveCeeOhTwo();

		addInterceptRange(0xD000,0x1000, &ioPages);

		// make monitor page act like a rom
		addInterceptRange(WOZMON,WOZMON_LENGTH, &monitorRomIntercept);
		// clear IRQ and NMI intercepts
		// allow IRQ and NMI to be set by programs
		addInterceptRange(0xFFF8,4,NULL);
		addInterceptRange(0xFFFE,2,NULL); 

		cursorX = 0;

		piaKB.cpu = this;
		piaKBcr.cpu = this;
		piaDSP.cpu = this;
		piaDSPcr.cpu = this;
		ioPages.cpu = this;
		stopEmulation.cpu = this;


		// wire up pia intercepts
		addIntercept(0xD010,&piaKB);
		addIntercept(0xD011,&piaKBcr);
		addIntercept(0xD012,&piaDSP);
		addIntercept(0xD013,&piaDSPcr);

		addIntercept(0xD01E,&miRNG);
		addIntercept(0xD01F,&cycleCounter);
		
		addIntercept(0xDFFF,&stopEmulation);



	}

	void selectIntBasic() {
		// mix in IntBasic
		addInterceptRange(BASIC, BASIC_LENGTH, &BasicRom);
	}

	void selectApplesoftLite() {
		// mix in Applesoft Lite
		addInterceptRange(APPLESOFTLITE, APPLESOFTLITE_LENGTH, &ApplesoftLiteRom);
		r.PC = APPLESOFTLITE;
	}

	void selectKrusader() {
		// mix in Krusader
		addInterceptRange(KRUSADER, KRUSADER_LENGTH, &Krusader);
		// set NMI to hop into Krusader's debugger
		memory[0xFFFE] = 0x19;
		memory[0xFFFF] = 0xFE;
	}

	void selectMSBasic() {
		// mix in MSBasic
		addInterceptRange(MSBASIC, MSBASIC_LENGTH, &MSBasic);
		r.PC = MSBASIC;
	}


	FILE *fpLoadFile;
	/**
	 * open the loadfile
	 */
	void readFile(char *filename) {

		struct stat sb;

		if (stat(filename, &sb) == -1) {
			perror("stat");
			exit(EXIT_FAILURE);
		}      

		fpLoadFile = fopen(filename, "r");
	}

	/**
	 * output something to stdout from DSP
	 */
	void outputDsp(unsigned char value) {

		// only quiet during file load
		quiet = quiet && (fpLoadFile != NULL);

		if(quiet) 
			return;


		// ignore any value that does not have bit 8 set
		if(value == 0x7F) 
			return;
		


		value &= 0x7F;

		switch (value) {
			case 0x7F:
			case 0x08:
			case 0x5F:
				// Backspace, del, underline
				// apple 1 used underline for backspace
				printf("%c %c",0x08,0x08);
				fflush(stdout);
				cursorX--;
				break;

			case 0x0A:
				break;
			case 0x0D:
				// End of Line
				printf("\r\n");
				cursorX=0;
				break;
			default:
				// Character
				printf("%c",value);
				fflush(stdout);
				cursorX++;
				break;
		}
		if(cursorX == 40) {
			cursorX = 0;
			if(width40)
				printf("\r\n");
		}
	}


	/**
	 * check for input on stdin or fpLoadFile
	 */
	bool getKeyboardReady() {

		// use select to check stdin for data
		fd_set wk_set;
		struct timeval tv;

		FD_ZERO(&wk_set);
		FD_SET(STDIN_FILENO,&wk_set);

		tv.tv_sec = 0;
		tv.tv_usec = 0; // poll mode
		int mk = 1;
		int n = select(mk,&wk_set,NULL,NULL,&tv);
		if ( n == -1 ) {
			// time out
			return false;
		} else if ( !n ) {
			// not selected
			return false;
		}

		// something there?
		if ( FD_ISSET(STDIN_FILENO,&wk_set) ) {
			return true;
		}
		return false;


	}

	/**
	 * read from stdin
	 */
	unsigned char getKeyboardData() {
		unsigned char c;
		read(STDIN_FILENO,&c,1);
		
		return c;
	}

	// buffer to hold keyboard data
	std::string keyboardBuffer;

	/**
	 * check for input, set PIA KB/KBcr status
	 * called between opcodes, when KBcr is accessed
	 * and when KB is accessed
	 */
	void checkKeyboard(bool reading) {
		unsigned char c = 0;

		// if we're reading from a file
		if(fpLoadFile) {
			// if still something left
			if(!feof(fpLoadFile)) {
				// set KBcr to character available
				piaKBcr.reg = 0xA7;
				// did we request a read?
				if(reading) {
					c = fgetc(fpLoadFile);
					c = toupper(c);
					if(c == 0x0A) // turn line feeds into carriage returns
						c = 0x0D;
					if(feof(fpLoadFile)) {
						// stop reading
						fclose(fpLoadFile);
						fpLoadFile = NULL;
						c = 0x0D; // end of file, always return a carriage return
					}
					c |= 0x80;
				}
			}
		} else {

			if(getKeyboardReady()) {
				// set KBcr to character available
				piaKBcr.reg = 0xA7;

				// this only works when we're reading
				if(reading) {
					c = getKeyboardData();

					// this is a very posix oriented thing
					
					if(c == 27) { // escape? soak up extended key codes
						keyboardBuffer.erase(0,keyboardBuffer.size());

						while(getKeyboardReady())
							keyboardBuffer += getKeyboardData();


						// F12 to break
						if(keyboardBuffer == "[24~") {
							c = 0;
							nmi_interrupt_cpu();
						} else
					    // F2 to reset
						if(keyboardBuffer == "OQ") {
							c = 0;
							reset_cpu();
						} else
						c |= 0x80;
							
					} else {

						// mapping
						switch(c) {
							case 0x0A: // linefeed = carriage return
								c = 0x0D;
								break;
							case 0x7F: // delete, backspace = underline
							case 0x08:
								c = 0x5F;
								break;
							default: // uppercase it
								c = toupper(c); 
								break;
						}

						c |= 0x80;
					}
				}
			}
		}
		if(c > 127) { // if we set bit 8, update KB
			piaKB.reg = c;
		}
	}

};


int main(int argc, char *argv[]) {

	bool interactive = true;

	struct termios tp, save;

	// create computer emulation
	AppleOne *cpu = new AppleOne();
	// reset the state
	cpu->reset_cpu();
	// load integer basic
	cpu->selectIntBasic();

	// file to load
	char *filename = NULL;
	opterr = 0;

	// process command line
	int c;
	while ((c = getopt (argc, argv, "abdkmqsw")) != -1)
		switch (c) {
			case 'a':
				cpu->selectApplesoftLite();
				break;
			case 'b': // start in basic
				cpu->r.PC = 0xE000;
				break;
			case 'd':
				cpu->debugging = true;
				break;
			case 'k':
				cpu->selectKrusader();
				break;
			case 'm':
				cpu->selectMSBasic();
				break;
			case 'q':
				cpu->quiet = true;
				break;
			case 's':
				cpu->debugging = true;
				cpu->stepping = true;
				break;
			case 'w':
				cpu->width40 = false;
				break;
			case '?': // don't care about other args
				printf("apple1 [-abkmqdsw] [loadfile]\n");
				printf("  -a install ApplesoftLite @ E000\n");
				printf("  -b start Woz Basic\n");
				printf("  -k install Krusader13 @ F000\n");
				printf("  -m install MSBasic @ 400\n");
				printf("  -q quiet until file loaded\n");

				printf("  -d turn on opcode trace\n");
				printf("  -s turn on opcode trace in step mode\n");

				printf("  -w turn off 40col width limitation\n");
				printf(" ctrl-c to quit, f2 to reset, f12 to nmi\n");
				exit(0);
				break;
			default:
				abort();
		}

	if(optind > 0) {
		filename = argv[optind];
	}

	if(filename) {
		cpu->readFile(filename);
		printf("Loading %s\n",filename);
	}

	// check if we are interactive or not
	interactive = true;
	if(tcgetattr(STDIN_FILENO, &tp) == -1) {
		interactive = false;
	}

	if(interactive) {
		// if we are then change io to RAW
		save = tp;

		// make a lot of the input raw, catch ctrl-c for break
		
		tp.c_iflag &= ~(  ISTRIP | INLCR | IGNCR | ICRNL | IXON);
		tp.c_oflag &= ~OPOST;
		tp.c_lflag &= ~(ECHO | ECHONL | ICANON |  IEXTEN);
		tp.c_cflag &= ~(CSIZE | PARENB);
		tp.c_cflag |= CS8;

		if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) == -1) {
			printf("could not set term attributes\n");
			exit(1);
		}
	}

	// display initial cpu state
	if(cpu->debugging) {
		cpu->printCpuStateHeader();
		cpu->dumpCpuState();
	}

	// the main loop
	while(cpu->running)  {
		// run an op, get the cycles used
		int cycles = cpu->do_op();

		// pass the cycles used to things that need to know
		cpu->cycleCounter.update(cycles);

		// check the keyboard hardware
		cpu->checkKeyboard(false);

// TODO: improve the debugging capabilities beyond stepping
		if(cpu->debugging) {
			cpu->dumpCpuState();

			if(cpu->r.SR_BREAK)
				cpu->stepping = true;
			if(cpu->stepping) {
				char c = getchar();
				switch(c) {
					case ' ':
						cpu->stepping = false;
						break;
					case '?':
						cpu->printCpuStateHeader();
						break;
					case 'c':
						cpu->stepping = false;
						cpu->debugging = false;
						break;
				}
			}
		}
	}

	// all done? time to clean up
	
	// reset io mode back to whatever it was when we started
	if(interactive)
		tcsetattr(STDIN_FILENO, TCSANOW, &save);

	// tear down emulated computer
	delete cpu;

	return 0;

}

