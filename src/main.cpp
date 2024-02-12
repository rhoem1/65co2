#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "apple1.h"
#include "apple1stdio.h"

// roms
#include "Roms/wozmon.h"
#include "Roms/apple1basic.h"
#include "Roms/krusader13.h"
#include "Roms/applesoft_lite_0_4.h"
#include "Roms/msbasic.h"

const uint64_t MAX_CYCLES_PER_FLUSH = 2*1024*1024/8;

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


	// reset the state
	apple->cpu->reset_cpu();

	bool loadRoms = true;

	// file to load
	char *filename = NULL;
	opterr = 0;

	// process command line
	int c;
	while ((c = getopt(argc, argv, "qvabkmdsrwl8")) != -1)
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
			apple->cpu->setPC(KRUSADER);
			break;
		case 'm':
			// mix in MSBasic
			apple->cpu->addInterceptRange(MSBASIC, MSBASIC_LENGTH, &MSBasic);
			apple->cpu->setPC(MSBASIC);
			apple->width40 = false;
			break;
		case 'w':
			apple->width40 = false;
			break;
    case 'l':
      apple->alwaysUppercase = false;
      break;
    case '8':
      apple->eightBitOutput = true;
      break;


		case 'r':
			loadRoms = false;
			break;

		case 'q':
			apple->quiet = true;
			break;
      
    case 'v':
      apple->verbose = true;
      break;

		case 'd':
			apple->debugging = true;
			break;
		case 's':
			apple->debugging = true;
			apple->stepping = true;
			break;
		case '?': // don't care about other args
			printf("apple1 [-abkmqds] [loadfile]\n");
			printf("  -a install ApplesoftLite @ E000\n");
			printf("  -b start Woz Basic\n");
			printf("  -k install Krusader13 @ F000\n");
			printf("  -m install MSBasic @ 400\n");
			printf("  -v verbose while file loaded\n");
      printf("  -q quiet mode");
			printf("  -w turn off 40 col mode\n");

			printf("  -d turn on opcode trace\n");
			printf("  -s turn on opcode trace in step mode\n");

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

	if(loadRoms) {
		// mix in IntBasic
		apple->cpu->addInterceptRange(BASIC, BASIC_LENGTH, &BasicRom);

		// mix in Krusader
		apple->cpu->addInterceptRange(KRUSADER, KRUSADER_LENGTH, &Krusader);
		// set NMI to hop into Krusader's debugger
		apple->cpu->write(0xFFFE, 0x19);
		apple->cpu->write(0xFFFF, 0xFE);
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
  
  uint64_t cyclecount = 0;

	// display initial cpu state
	if (apple->debugging)
	{
		apple->cpu->printCpuStateHeader();
		apple->cpu->dumpCpuState();
		// the main loop
		while (apple->running)
		{
			// run an op, get the cycles used
			int cycles = apple->cpu->do_operation();

			// check the keyboard hardware
			apple->checkKeyboard(false);

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
	}
	else
	{
		// the main loop
		while (apple->running && !apple->cpu->r.stopped)
		{
			// run an op, get the cycles used
			int cycles = apple->cpu->do_operation();
      cyclecount += cycles;
      if(cyclecount > MAX_CYCLES_PER_FLUSH) {
        cyclecount = 0;
        apple->flush();
      }
      
      // check the keyboard once every 4096 cycles
      if(!apple->quiet && (cyclecount & 0x0800))
        // check the keyboard hardware
        apple->checkKeyboard(false);

		}
	}


	// all done? time to clean up

	// reset io mode back to whatever it was when we started
	if (interactive)
		tcsetattr(STDIN_FILENO, TCSANOW, &save);

	return 0;
}
