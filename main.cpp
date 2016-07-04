/*
 * 65o2 test
 *
 * an apple i emulator
 *
 * since the apple 1 didn't do graphics
 * it's basically just a 6502 and a 6521 PIA
 *
 * so port A on the PIA talks to a display
 * and port B on the PIA takes ASCII from a ASCII keyboard
 *
 * screen is 40 x 24 (960) chars
 *
 *
 * (c) 2013 Robert Hoem
 *
 */


#include <string>
#include <iostream>
#include <iomanip>
using namespace std;
#include <ncurses.h>

//#include "SoundStream.h"
//#include "TransferMethods.h"

#include <stdio.h>

#include "cpu.h"


#define DEBUG


#ifdef DEBUG
// shows all printable characters
//string test_input = "300:A9 0 AA 20 EF FF E8 8A 4C 2 3\n300.30A\n300R\n";

// shows hex val of what is typed
//string test_input = "0700: A9 8D 20 EF FF AD 11 D0 10 FB AD 10 D0 AA 20 EF FF A9 A0 20 EF FF 8A 20 DC FF 4C 00 07\n700R";

/*
// displays all printable characters with hex codes
string test_input =
"0750: A2 A0 A9 8D 20 EF FF 8A\n"
"0758: 20 EF FF A9 A0 20 EF FF\n"
"0760: 8A 20 DC FF E8 A9 A0 20\n"
"0768: EF FF 20 EF FF E0 00 F0\n"
"0770: 03 4C 54 07 4C 24 FF\n"
"0750R\n";
*/

/*
 * source for above
 * org $0750
 * ldx #$A0
 * lda #$8D
 * loop JSR $FFEF
 * txa
 * jsr $FFEF
 * lda #$A0
 * jsr $FFEF
 * inx
 * lda #$a0
 * jsr $FFEF
 * jsr $FFEF
 * cpx #$00
 * beq stop
 * jmp loop
 * stop jmp $FF1F
 *
 *
 *
 *
 */






// basic test

string test_input = "E000R\n"
"10 PRINT \"HELLO WORLD\"\n"
"20 GOTO 10\n"
"RUN\n";

#define MINWIDTH 110
#define MINHEIGHT 50
#else
#define MINWIDTH 80
#define MINHEIGHT 24


#endif

struct memoryInterceptPIAD010;
struct memoryInterceptPIAD011;
struct memoryInterceptPIAD012;
struct memoryInterceptPIAD013;



// the apple 1 used a funny character encoding into the character rom
// so it probably didn't output ascii? I dunno.
char char_map[65] = "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ !\"#$%&'()*+,-./0123456789:;<=>?";


struct AppleOne : public SixtyFiveOhTwo {

	// WOZMON for Apple I
	unsigned char monitor[256] = {

0xd8, 0x58, 0xa0, 0x7f, 0x8c, 0x12, 0xd0, 0xa9, 0xa7, 0x8d, 0x11, 0xd0, 0x8d, 0x13, 0xd0, 0xc9,
0xdf, 0xf0, 0x13, 0xc9, 0x9b, 0xf0, 0x03, 0xc8, 0x10, 0x0f, 0xa9, 0xdc, 0x20, 0xef, 0xff, 0xa9,
0x8d, 0x20, 0xef, 0xff, 0xa0, 0x01, 0x88, 0x30, 0xf6, 0xad, 0x11, 0xd0, 0x10, 0xfb, 0xad, 0x10,
0xd0, 0x99, 0x00, 0x02, 0x20, 0xef, 0xff, 0xc9, 0x8d, 0xd0, 0xd4, 0xa0, 0xff, 0xa9, 0x00, 0xaa,
0x0a, 0x85, 0x2b, 0xc8, 0xb9, 0x00, 0x02, 0xc9, 0x8d, 0xf0, 0xd4, 0xc9, 0xae, 0x90, 0xf4, 0xf0,
0xf0, 0xc9, 0xba, 0xf0, 0xeb, 0xc9, 0xd2, 0xf0, 0x3b, 0x86, 0x28, 0x86, 0x29, 0x84, 0x2a, 0xb9,
0x00, 0x02, 0x49, 0xb0, 0xc9, 0x0a, 0x90, 0x06, 0x69, 0x88, 0xc9, 0xfa, 0x90, 0x11, 0x0a, 0x0a,
0x0a, 0x0a, 0xa2, 0x04, 0x0a, 0x26, 0x28, 0x26, 0x29, 0xca, 0xd0, 0xf8, 0xc8, 0xd0, 0xe0, 0xc4,
0x2a, 0xf0, 0x97, 0x24, 0x2b, 0x50, 0x10, 0xa5, 0x28, 0x81, 0x26, 0xe6, 0x26, 0xd0, 0xb5, 0xe6,
0x27, 0x4c, 0x44, 0xff, 0x6c, 0x24, 0x00, 0x30, 0x2b, 0xa2, 0x02, 0xb5, 0x27, 0x95, 0x25, 0x95,
0x23, 0xca, 0xd0, 0xf7, 0xd0, 0x14, 0xa9, 0x8d, 0x20, 0xef, 0xff, 0xa5, 0x25, 0x20, 0xdc, 0xff,
0xa5, 0x24, 0x20, 0xdc, 0xff, 0xa9, 0xba, 0x20, 0xef, 0xff, 0xa9, 0xa0, 0x20, 0xef, 0xff, 0xa1,
0x24, 0x20, 0xdc, 0xff, 0x86, 0x2b, 0xa5, 0x24, 0xc5, 0x28, 0xa5, 0x25, 0xe5, 0x29, 0xb0, 0xc1,
0xe6, 0x24, 0xd0, 0x02, 0xe6, 0x25, 0xa5, 0x24, 0x29, 0x07, 0x10, 0xc8, 0x48, 0x4a, 0x4a, 0x4a,
0x4a, 0x20, 0xe5, 0xff, 0x68, 0x29, 0x0f, 0x09, 0xb0, 0xc9, 0xba, 0x90, 0x02, 0x69, 0x06, 0x2c,
0x12, 0xd0, 0x30, 0xfb, 0x8d, 0x12, 0xd0, 0x60, 0x00, 0x00, 0x00, 0x0f, 0x00, 0xff, 0x00, 0x00

	};

	bool kbdInterrupts;
	bool dspOutput;


	int indexX;
	int indexY;
	int cursorX;
	int cursorY;
	//char screenTbl[80][24];

	WINDOW * screenWindow;
	WINDOW * opcodeTraceWindow;


	int input_idx = 0;

	int screenW;
	int screenH;

	enum { 
		NORMAL = 1,
		SCREEN,
		CURSOR,
		TRACE,
		PAGEDATA,
		STACKHERE
	};

	struct pageDisplay : public memoryIntercept {

		AppleOne * cpu;

		WINDOW * titleWindow;
		WINDOW * pageWindow;

		unsigned short last_address;

		unsigned short page;
		string title;

		pageDisplay(
				AppleOne *CPU,
				unsigned short PAGE,
				string TITLE,
				int top,
				int left
				) {
			cpu = CPU;

			page = PAGE;
			title = TITLE;

			last_address = page;

			titleWindow = newwin(1, 50, top, left);

			pageWindow = newwin(16, 50, top + 1, left);
			
		}

		~pageDisplay() {

		}

		void redraw() {

			wattron(pageWindow, COLOR_PAIR(PAGEDATA));
			wclear(pageWindow);
			for(char i = 0; i < 16; i++) {
				for(char j = 0; j < 16; j++) {
					unsigned char offs = i * 16 + j;

					wattron(pageWindow, COLOR_PAIR(PAGEDATA));
					if(page + offs == last_address & 0xFF)
						wattron(pageWindow, COLOR_PAIR(STACKHERE));

					mvwprintw(pageWindow,i,j * 3,"%02X",cpu->memory[page + offs]);

					wattron(pageWindow, COLOR_PAIR(NORMAL));
					mvwaddch(pageWindow,i,j * 3 + 2,' ');

				}
			}
			wrefresh(pageWindow);

			wattron(titleWindow, COLOR_PAIR(NORMAL));
			wclear(titleWindow);
			mvwprintw(titleWindow , 0, 0, "%s", title.c_str());
			wrefresh(titleWindow);
		}

		unsigned char read(unsigned short address) {
			return cpu->memory[address]; 
		};

		unsigned char row(unsigned short address) {
			return (address & 0xF0) >> 4;
		}
		unsigned char col(unsigned short address) {
			return (address & 0x0F) * 3;
		}

		void write(unsigned char value, unsigned short address) {
			cpu->memory[address] = value;

			wattron(pageWindow, COLOR_PAIR(PAGEDATA));
			mvwprintw(pageWindow, row(last_address), col(last_address), "%02X", cpu->memory[last_address]);

			wattron(pageWindow, COLOR_PAIR(STACKHERE));
			mvwprintw(pageWindow, row(address), col(address), "%02X", value);

			last_address = address;

			wrefresh(pageWindow);
		};
	};


	pageDisplay *zeroPageDisplay;
	pageDisplay *stackPageDisplay;
	pageDisplay *inbufPageDisplay;




	AppleOne() {
		SixtyFiveOhTwo();

		indexX = 0;
		indexY = 0;
		cursorX = 1;
		cursorY = 0;

		for(int i = 0; i < 65536; i++) {
			memory[i] = 0;
		}

		for(int i = 0; i < 256; i++)
			memory[0xFF00 + i] = monitor[i];

		kbdInterrupts = true;
		dspOutput = false;

		initscr();
		start_color();
		curs_set(0);
		keypad(stdscr, TRUE);
		nodelay(stdscr, TRUE);
		nonl();
		cbreak();
		noecho();
		getmaxyx(stdscr, screenH, screenW);
		//Make sure the window is big enough
		if((screenH < MINHEIGHT) || (screenW < MINWIDTH)) {

			endwin();
			fprintf(stderr, "\nMinimum window size is %dx%d (Yours: %dx%d)\nPlease resize your window/resolution and re-run the game.\n\n",
					MINWIDTH,MINHEIGHT, screenW,screenH);
			exit(0);
		}

		init_pair(  NORMAL,    COLOR_WHITE, COLOR_BLUE   );
		init_pair(  SCREEN,    COLOR_GREEN, COLOR_BLACK  );
		init_pair(  CURSOR,    COLOR_BLACK, COLOR_GREEN  );
		init_pair(  TRACE,     COLOR_BLACK, COLOR_WHITE  );
		init_pair(  PAGEDATA,  COLOR_BLACK, COLOR_WHITE  );
		init_pair(  STACKHERE, COLOR_WHITE, COLOR_RED    );

		attron(COLOR_PAIR(NORMAL));

		clear();

		mvprintw( 0,0,"+-- SCREEN ------------------------------+");
		screenWindow = newwin(24,41,1,0);
		clearok(screenWindow,true);
		scrollok(screenWindow,true);

#ifdef DEBUG		

		mvprintw(25,0, "PC     A  X  Y SP  mem  alu addr flags   + OP data OPR MODE");
		opcodeTraceWindow = newwin(26,61,26,0);
		scrollok(opcodeTraceWindow,true);
		idlok(opcodeTraceWindow, true);

		refresh();

		// hook in page display intercepts

		zeroPageDisplay  = new pageDisplay(this,0x0000,"-- ZERO PAGE -----------------------------------", 0,62);
		addInterceptRange(0x0000,0x100, zeroPageDisplay);
		zeroPageDisplay->redraw();

		stackPageDisplay = new pageDisplay(this,0x0100,"-- STACK ---------------------------------------",17,62);
		addInterceptRange(0x0100,0x100, stackPageDisplay);
		stackPageDisplay->redraw();

		inbufPageDisplay   = new pageDisplay(this,0x0200,"-- PAGE $200 -----------------------------------",34,62);
		addInterceptRange(0x0200,0x100, inbufPageDisplay);
		inbufPageDisplay->redraw();

#endif

	}

	~AppleOne() {
		endwin();
	}
	void outputDsp(unsigned char value) {
		
		// clear cursor

		wattron(screenWindow, COLOR_PAIR(NORMAL));
		mvwaddch(screenWindow,cursorY,cursorX,' ');

		// clear ?
		if(value == 0x7F)
			wclear(screenWindow);

		value &= 0x7F;

		switch (value) {
			case 0x00:
				// Nothing
				break;
			case 0x7F:
				// Nothing (Dsp Register Mask)
				break;

			case 0x5F:
				// Backspace
				if (indexX == 0) {
					indexY--;
					indexX = 39;
				} else {
					indexX--;
				}
				//screenTbl[indexX][indexY] = 0x00;
				mvwaddch(screenWindow,indexY,indexX,' ');
				break;
			case 0x0D:
			case 0x0A:
				// End of Line
				indexX = 0;
				cursorX = 1;
				indexY++;
				cursorY++;
				// maybe erase to eor?
				break;
			default:
				// Character

				indexX++;
				cursorX++;

				// probably a more direct way of doing this set
				mvwaddch(screenWindow,indexY,indexX,value);


				break;
		}


		if (indexX == 40) {
			indexX = 0;
			indexY++;
		}
		if (indexY == 24) {
			indexY--;
		}
		if (cursorX == 40) {
			cursorX = 0;
			cursorY++;
		}
		if (cursorY == 24) {
			cursorY--;
		}

		// show cursor
		wattron(screenWindow, COLOR_PAIR(CURSOR));
		mvwaddch(screenWindow,cursorY,cursorX,'@');

		wrefresh(screenWindow);
	}

	void updateOpcodeTraceWindow() {
		wprintw(opcodeTraceWindow,"%04X: %02X %02X %02X %02X %04X %04X %04X %c%cU%c%c%c%c%c: ",
				r.old_PC,
				r.A, 
				r.X,
				r.Y, 
				r.SP,
				old_alu & 0xFFFF, 
				alu & 0xFFFF, 
				address & 0xFFFF,
				(r.SR_NEGATIVE)?'N':' ',
				(r.SR_OVERFLOW)?'O':' ',
				(r.SR_BREAK)?'B':' ',
				(r.SR_DECIMAL)?'D':' ',
				(r.SR_INTERRUPT)?'I':' ',
				(r.SR_ZERO)?'Z':' ',
				(r.SR_CARRY)?'C':' '
			   );
		switch(opcodes[opcode].step) {
			case 0:
				wprintw(opcodeTraceWindow,"%02X      ", opcode);
				break;
			case 1:
				wprintw(opcodeTraceWindow,"%02X %02X   ", opcode, zpage);
				break;
			case 2:
				wprintw(opcodeTraceWindow,"%02X %02X%02X ", opcode, zpage, zpageh);
				break;
		}
		wprintw(opcodeTraceWindow, "%s %s\n",opcodes[opcode].nmem, MODES[opcodes[opcode].mode]);

		wrefresh(opcodeTraceWindow);
	}

	bool getKeyboardReady(bool wait) {
		// excellent peek stream, waits for a second
		fd_set wk_set;          /* Working set */
		struct timeval tv;    /* Timeout value */

		FD_ZERO(&wk_set);
		FD_SET(0,&wk_set); // 0 = stdin
		tv.tv_sec = 0;
		tv.tv_usec = 17;
		if(wait) {
			tv.tv_sec = 1;
			tv.tv_usec = 0;
		}
		int mk = 1;
		int n = select(mk,&wk_set,NULL,NULL,&tv);
		if ( n == -1 ) {
			return false;
		} else if ( !n ) {
			return false;
		}

		if ( FD_ISSET(0,&wk_set) ) { // something there
			return true;
		}
		return false;


	}

};

// pia registers

// KB
struct memoryInterceptPIAD010 : memoryIntercept {
	unsigned char reg;
	AppleOne *cpu;
	memoryInterceptPIAD010() {
		reg = 0;
	};
	virtual unsigned char read(unsigned short address) {


#ifdef DEBUG		
		reg = test_input[cpu->input_idx];
		if(cpu->input_idx < test_input.length())
			cpu->input_idx++;
		//cpu->input_idx = 0;
#else
		reg = toupper(getch());
#endif
		reg = translate(reg);

		// Bit 7 enable Monitor Reading

		return reg;
	}
	virtual void write(unsigned char value, unsigned short address) {
		reg = value;
	}

	unsigned char translate(unsigned char in) {

		// mapping
		switch(in) {
			case 0x03:
				return 0x83;
			case 0x0A:
				return 0x8D;
			case ' ':
				return 0xA0;
		}

		if((in > 32) && (in < 94))
			return in + 0x80;

		return 0;
	}
};

// KBcr
struct memoryInterceptPIAD011 : memoryIntercept {
	unsigned char reg;
	AppleOne *cpu;
	memoryInterceptPIAD011() {
		reg = 0;
	};
	virtual unsigned char read(unsigned short address) {

#ifdef DEBUG		
		//if(cpu->input_idx < test_input.length())
			return 0xA7;
#endif

		if(cpu->getKeyboardReady(false)) {
			//if(reg >= 0x80) {
				return 0xA7;
			//}
		}
		return 0;
	}
	virtual void write(unsigned char value, unsigned short address) {
		if(!cpu->kbdInterrupts) {
			if(reg >= 0x80) {
				cpu->kbdInterrupts = true;
				reg = 0x00;
			}
		} else {
			reg = value;
		}
	}
};

// DSP
struct memoryInterceptPIAD012 : memoryIntercept {
	unsigned char reg;
	AppleOne *cpu;
	memoryInterceptPIAD012() {
		reg = 0;
	};
	unsigned char read(unsigned short address) {
		return 0;
	}
	void write(unsigned char value, unsigned short address) {
		reg = value;
		cpu->outputDsp(value);
	}
};

// DSPcr
struct memoryInterceptPIAD013 : memoryIntercept {
	unsigned char reg;
	AppleOne *cpu;
	memoryInterceptPIAD013() {
		reg = 0;
	};
	virtual unsigned char read(unsigned short address) {
		return reg;
	}
	virtual void write(unsigned char value, unsigned short address) {
		if(value >= 0x80)
			cpu->dspOutput = true;
		reg = value;
	}
};


#define base 0xE000


int main(int argc, char *argv[]) {


	// do stuff here
	//

	FILE *fp;


	AppleOne *cpu = new AppleOne();

	memoryInterceptPIAD010 piaKB;
	memoryInterceptPIAD011 piaKBcr;
	memoryInterceptPIAD012 piaDSP;
	memoryInterceptPIAD013 piaDSPcr;

	piaKB.cpu = cpu;
	piaKBcr.cpu = cpu;
	piaDSP.cpu = cpu;
	piaDSPcr.cpu = cpu;

	// wire up pia intercepts
	cpu->addIntercept(0xD010,&piaKB);
	cpu->addIntercept(0xD011,&piaKBcr);
	cpu->addIntercept(0xD012,&piaDSP);
	cpu->addIntercept(0xD013,&piaDSPcr);

	/*
	// prep test memory
	for(int p = 0; p < 256; p++)
		for(int i = 0; i < 256; i++)
			cpu->memory[(p << 8) + i] = i;
			*/


	// load test rom
		/*
#ifdef DEBUG		
		if(fp = fopen("opcode_test.bin","r")) {
			unsigned short i = 0;
			while(!feof(fp)) {
				char c = fgetc(fp);
				if(base + i < 65536) {
					cpu->memory[base + i] =  c;
				}
				i++;
			}
		}
		cpu->paused = true;
#endif
		*/
	
	// load basic
		if(fp = fopen("rom/basic.rom","r")) {
			unsigned short i = 0;
			while(!feof(fp)) {
				char c = fgetc(fp);
				if(base + i < 65536) {
					cpu->memory[base + i] =  c;
				}
				i++;
			}
		}


	cpu->reset_cpu();

	


	while(true) 
	{
		char c;

		if(!cpu->paused)
			cpu->do_op();


#ifdef DEBUG		

		if(!cpu->paused)
			cpu->updateOpcodeTraceWindow();

		if(cpu->getKeyboardReady(false)) {
			c = getc(stdin);

			if(c == 0x20)
				cpu->paused = !cpu->paused;

			if(cpu->paused) {
				if(c == 's') {
					cpu->do_op();
					cpu->updateOpcodeTraceWindow();
				}
			}
		}


		// breakpoint
		//if(cpu->r.PC == 0x0724) cpu->paused = true;
#endif
	}


	delete cpu;


	fclose(fp);
	return 0;


}

