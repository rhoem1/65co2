
#include "apple1.h"
#include "apple1stdio.h"

void AppleOneStdio::outputDsp(unsigned char value) 
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
