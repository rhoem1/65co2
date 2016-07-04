#ifndef __msbasic_h__
#define __msbasic_h__


#define MSBASIC 0x400

struct memoryInterceptMSBasic : romIntercept {

#include "msbasic_code.h"


	virtual unsigned char read(unsigned short address) {
		return tmp_applesoft_bin[address - MSBASIC];
	};
};

#endif
