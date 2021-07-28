
// emulation
#include "apple1.h"

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
	void outputDsp(unsigned char value) override;

	FILE *fpLoadFile;
	/**
	 * open the loadfile
	 */
	void readFile(char *filename);

	/**
	 * check for input on stdin or fpLoadFile
	 */
	bool getKeyboardReady();

	/**
	 * read from stdin
	 */
	unsigned char getKeyboardData();

	// buffer to hold keyboard data
	std::string keyboardBuffer;

	/**
	 * check for input, set PIA KB/KBcr status
	 * called between opcodes, when KBcr is accessed
	 * and when KB is accessed
	 */
	void checkKeyboard(bool reading) override;
};
