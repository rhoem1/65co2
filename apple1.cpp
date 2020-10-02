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

// roms
#include "wozmon.h"
#include "apple1basic.h"
#include "krusader13.h"
#include "applesoft_lite_0_4.h"
#include "msbasic.h"

struct AppleOneStdio : public AppleOne
{
	bool debugging = false;
	bool stepping = false;
	bool quiet = false;

	bool width40 = true;
	int cursorX = 0;

	/**
	 * output something to stdout from DSP
	 */
	void outputDsp(unsigned char value) override
	{

		// only quiet during file load
		quiet = quiet && (fpLoadFile != NULL);

		if (quiet)
			return;

		// ignore any value that does not have bit 8 set
		if (value == 0x7F)
			return;

		value &= 0x7F;

		switch (value)
		{
		case 0x7F:
		case 0x08:
		case 0x5F:
			// Backspace, del, underline
			// apple 1 used underline for backspace
			printf("%c %c", 0x08, 0x08);
			fflush(stdout);
			cursorX--;
			break;

		case 0x0A:
			break;
		case 0x0D:
			// End of Line
			printf("\r\n");
			cursorX = 0;
			break;
		default:
			// Character
			printf("%c", value);
			fflush(stdout);
			cursorX++;
			break;
		}
		if (cursorX == 40)
		{
			cursorX = 0;
			if (width40)
				printf("\r\n");
		}
	}

	FILE *fpLoadFile;
	/**
	 * open the loadfile
	 */
	void readFile(char *filename)
	{

		struct stat sb;

		if (stat(filename, &sb) == -1)
		{
			perror("stat");
			exit(EXIT_FAILURE);
		}

		fpLoadFile = fopen(filename, "r");
	}

	/**
	 * check for input on stdin or fpLoadFile
	 */
	bool getKeyboardReady()
	{

		// use select to check stdin for data
		fd_set wk_set;
		struct timeval tv;

		FD_ZERO(&wk_set);
		FD_SET(STDIN_FILENO, &wk_set);

		tv.tv_sec = 0;
		tv.tv_usec = 0; // poll mode
		int mk = 1;
		int n = select(mk, &wk_set, NULL, NULL, &tv);
		if (n == -1)
		{
			// time out
			return false;
		}
		else if (!n)
		{
			// not selected
			return false;
		}

		// something there?
		if (FD_ISSET(STDIN_FILENO, &wk_set))
		{
			return true;
		}
		return false;
	}

	/**
	 * read from stdin
	 */
	unsigned char getKeyboardData()
	{
		unsigned char c;
		read(STDIN_FILENO, &c, 1);

		return c;
	}

	// buffer to hold keyboard data
	std::string keyboardBuffer;

	/**
	 * check for input, set PIA KB/KBcr status
	 * called between opcodes, when KBcr is accessed
	 * and when KB is accessed
	 */
	void checkKeyboard(bool reading) override
	{
		unsigned char c = 0;

		// if we're reading from a file
		if (fpLoadFile)
		{
			// if still something left
			if (!feof(fpLoadFile))
			{
				// set KBcr to character available
				piaKBcr->reg = 0xA7;
				// did we request a read?
				if (reading)
				{
					c = fgetc(fpLoadFile);
					c = toupper(c);
					if (c == 0x0A) // turn line feeds into carriage returns
						c = 0x0D;
					if (feof(fpLoadFile))
					{
						// stop reading
						fclose(fpLoadFile);
						fpLoadFile = NULL;
						c = 0x0D; // end of file, always return a carriage return
					}
					c |= 0x80;
				}
			}
		}
		else
		{

			if (getKeyboardReady())
			{
				// set KBcr to character available
				piaKBcr->reg = 0xA7;

				// this only works when we're reading
				if (reading)
				{
					c = getKeyboardData();

					// this is a very posix oriented thing

					if (c == 27)
					{ // escape? soak up extended key codes
						keyboardBuffer.erase(0, keyboardBuffer.size());

						while (getKeyboardReady())
							keyboardBuffer += getKeyboardData();

						// F12 to break
						if (keyboardBuffer == "[24~")
						{
							c = 0;
							cpu->non_maskable_interrupt();
						}
						else
							// F2 to reset
							if (keyboardBuffer == "OQ")
						{
							c = 0;
							cpu->reset_cpu();
						}
						else
							c |= 0x80;
					}
					else
					{

						// mapping
						switch (c)
						{
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
		if (c > 127)
		{ // if we set bit 8, update KB
			piaKB->reg = c;
		}
	}
};

int main(int argc, char *argv[])
{

	bool interactive = true;

	struct termios tp, save;

	// create computer emulation
	AppleOneStdio *apple = new AppleOneStdio();

	// various roms
	memoryInterceptWozMon monitorRomIntercept;
	memoryInterceptIntBasic BasicRom;
	memoryInterceptApplesoftLite ApplesoftLiteRom;
	memoryInterceptKrusader Krusader;
	memoryInterceptMSBasic MSBasic;

	// make monitor page act like a rom
	apple->cpu->addInterceptRange(WOZMON, WOZMON_LENGTH, &monitorRomIntercept);
	// clear IRQ and NMI intercepts
	// allow IRQ and NMI to be set by programs
	apple->cpu->addInterceptRange(0xFFF8, 4, NULL);
	apple->cpu->addInterceptRange(0xFFFE, 2, NULL);

	// mix in IntBasic
	apple->cpu->addInterceptRange(BASIC, BASIC_LENGTH, &BasicRom);

	// reset the state
	apple->cpu->reset_cpu();

	// file to load
	char *filename = NULL;
	opterr = 0;

	// process command line
	int c;
	while ((c = getopt(argc, argv, "abdkmqsw")) != -1)
		switch (c)
		{
		case 'a':
			// mix in Applesoft Lite
			apple->cpu->addInterceptRange(APPLESOFTLITE, APPLESOFTLITE_LENGTH, &ApplesoftLiteRom);
			apple->cpu->setPC(APPLESOFTLITE);
			break;
		case 'b': // start in basic
			apple->cpu->setPC(BASIC);
			break;
		case 'k':
			// mix in Krusader
			apple->cpu->addInterceptRange(KRUSADER, KRUSADER_LENGTH, &Krusader);
			// set NMI to hop into Krusader's debugger
			apple->cpu->write(0xFFFE, 0x19);
			apple->cpu->write(0xFFFF, 0xFE);
			apple->cpu->setPC(KRUSADER);
			break;
		case 'm':
			// mix in MSBasic
			apple->cpu->addInterceptRange(MSBASIC, MSBASIC_LENGTH, &MSBasic);
			apple->cpu->setPC(MSBASIC);
			break;

		case 'q':
			apple->quiet = true;
			break;
		case 'd':
			apple->debugging = true;
			break;
		case 's':
			apple->debugging = true;
			apple->stepping = true;
			break;
		case 'w':
			apple->width40 = false;
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

	if (optind > 0)
	{
		filename = argv[optind];
	}

	if (filename)
	{
		apple->readFile(filename);
		printf("Loading %s\n", filename);
	}

	// check if we are interactive or not
	interactive = true;
	if (tcgetattr(STDIN_FILENO, &tp) == -1)
	{
		interactive = false;
	}

	if (interactive)
	{
		// if we are then change io to RAW
		save = tp;

		// make a lot of the input raw, catch ctrl-c for break

		tp.c_iflag &= ~(ISTRIP | INLCR | IGNCR | ICRNL | IXON);
		tp.c_oflag &= ~OPOST;
		tp.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN);
		tp.c_cflag &= ~(CSIZE | PARENB);
		tp.c_cflag |= CS8;

		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) == -1)
		{
			printf("could not set term attributes\n");
			exit(1);
		}
	}

	// display initial cpu state
	if (apple->debugging)
	{
		apple->cpu->printCpuStateHeader();
		apple->cpu->dumpCpuState();
	}

	// the main loop
	while (apple->running)
	{
		// run an op, get the cycles used
		int cycles = apple->cpu->do_cycle();

		// pass the cycles used to things that need to know
		apple->cycleCounter->update(cycles);

		// check the keyboard hardware
		apple->checkKeyboard(false);

		/*
		if(cpu->r.PC == 0x300) {
			cpu->debugging = true;
			cpu->stepping = true;
		}
		*/

		// TODO: improve the debugging capabilities beyond stepping
		if (apple->debugging)
		{
			apple->cpu->dumpCpuState();

			//if(cpu->r.SR_BREAK)
			//	cpu->stepping = true;
			if (apple->stepping)
			{
				char c = getchar();
				switch (c)
				{
				case ' ':
					apple->stepping = false;
					break;
				case '?':
					apple->cpu->printCpuStateHeader();
					break;
				case 'c':
					apple->stepping = false;
					apple->debugging = false;
					break;
				}
			}
		}
	}

	// all done? time to clean up

	// reset io mode back to whatever it was when we started
	if (interactive)
		tcsetattr(STDIN_FILENO, TCSANOW, &save);

	return 0;
}
