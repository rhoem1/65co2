
// emulation
#include <memory>
#include <string>
#include "apple1.h"


struct AppleOneStdio : public AppleOne
{
	bool debugging = false;
	bool stepping = false;

  bool eightBitOutput = false;
	bool width40 = true;
  bool alwaysUppercase = true;
	int cursorX = 0;

  // output during file load allowed
  bool verbose = false;
  // output allowed
  bool quiet = false;

  // keyboard check counter
  const int MAX_KEYBOARD_CHECKS = 100;
  int counter = 0;

  struct memoryInterceptQuietOutput : memoryIntercept
  {
    AppleOneStdio *apple;
    memoryInterceptQuietOutput(AppleOneStdio *_apple); 
    virtual unsigned char read(unsigned short address);
    virtual void write(unsigned char value, unsigned short address);
  };
  std::unique_ptr<memoryInterceptQuietOutput> quietOutput;

  AppleOneStdio();

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
  
  void flush();
  
};
