
#include "apple1.h"
#include "apple1stdio.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>


AppleOneStdio::AppleOneStdio()
{
  quietOutput = std::make_unique<memoryInterceptQuietOutput>(this);
	cpu->addIntercept(0xD01E, quietOutput.get());
}
void AppleOneStdio::outputDsp(unsigned char value) 
{

	// only quiet during file load
  if (fpLoadFile != NULL && !verbose)
    return;

	if (quiet)
		return;

	//if (value == 0x7F) return;

  if(!eightBitOutput)
    value &= 0x7F;

	switch (value)
	{
		case 0x7F:
		case 0x08:
		case 0x5F:
			// Backspace, del, underline
			// apple 1 used underline for backspace
			printf("%c %c", 0x08, 0x08);
			cursorX--;
			break;

		case 0x0A:
			break;
		case 0x0D:
			// End of Line
			cursorX = 0;
			printf("\r\n");
      flush();
			break;
		default:
			// Character
			printf("%c", value);
			cursorX++;
			break;
	}
  if (width40)
  {
    if (cursorX == 40)
    {
      cursorX = 0;
      printf("\r\n");
      flush();
    }
  }
}
void AppleOneStdio::flush()
{
  if(quiet) return;
  fflush(stdout);
  usleep(1);
}
void AppleOneStdio::readFile(char *filename)
{

	struct stat sb;

	if (stat(filename, &sb) == -1)
	{
		perror("stat");
		exit(EXIT_FAILURE);
	}

	fpLoadFile = fopen(filename, "r");
}
bool AppleOneStdio::getKeyboardReady()
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
unsigned char AppleOneStdio::getKeyboardData()
{
	unsigned char c;
	read(STDIN_FILENO, &c, 1);

	return c;
}
void AppleOneStdio::checkKeyboard(bool reading)
{
	unsigned char c = 0;

  // if we're reading from a file
	if (fpLoadFile)
	{
		// if still something left
		if (!feof(fpLoadFile))
		{
			// set KBcr to character available
			pia->keycontrol = 0xA7;
			// did we request a read?
			if (reading)
			{
				c = fgetc(fpLoadFile);
        if(alwaysUppercase)
          c = toupper(c);
				if (c == 0x0A) // turn line feeds into carriage returns
					c = 0x0D;
				if (feof(fpLoadFile))
				{
					// stop reading
					fclose(fpLoadFile);
					fpLoadFile = NULL;
					c = 0x0D; // end of file, always return a carriage return
          goInteractive();
				}
				c |= 0x80;
			}
    }
	}
	else
	{
    if(!reading) {
      counter = MAX_KEYBOARD_CHECKS;
    }
    else
    {
      counter--;
      if (counter == 0)
      {
        flush();
        counter = MAX_KEYBOARD_CHECKS;
      }
    }

		if (getKeyboardReady())
		{
			// set KBcr to character available
			pia->keycontrol = 0xA7;

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

          // F2 to reset
          if (keyboardBuffer == "OQ")
          {
            cpu->reset_cpu();
            return;
          }
					// F3 to break
					if (keyboardBuffer == "OR")
					{
						cpu->non_maskable_interrupt();
            return;
					}
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
						default: 
              // uppercase it
              if(alwaysUppercase)
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
		pia->keyin = c;
    flush();
  }
}
void AppleOneStdio::goInteractive()
{
	// check if we are interactive or not
	if (tcgetattr(STDIN_FILENO, &tp) == -1)
	{
    return;
	}
  if(interactive)
    return;
  // if we are then change io to RAW
  save = tp;
  // make a lot of the input raw, catch ctrl-c for break

  tp.c_iflag |= IGNBRK;
  tp.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF | ISTRIP | IGNCR );
  tp.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
  tp.c_cc[VMIN] = 1;
  tp.c_cc[VTIME] = 0;
  tp.c_cflag |= CS8;
  tp.c_cflag &= ~(OPOST | CSIZE | PARENB);

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) == -1)
  {
    printf("could not set term attributes\n");
  }
  interactive = true;
}

void AppleOneStdio::leaveInteractive()
{
		tcsetattr(STDIN_FILENO, TCSANOW, &save);
    interactive = false;
}

AppleOneStdio::memoryInterceptQuietOutput::memoryInterceptQuietOutput(AppleOneStdio *_apple)
{
  apple = _apple;
}
unsigned char AppleOneStdio::memoryInterceptQuietOutput::read(unsigned short address)
{
  return apple->quiet ? 0x80 : 0x00;
}
void AppleOneStdio::memoryInterceptQuietOutput::write(unsigned char value, unsigned short address)
{
  apple->quiet = value != 0x00;
}
