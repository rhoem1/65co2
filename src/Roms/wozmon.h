#ifndef __wozmon_h__
#define __wozmon_h__

// woz monitor
#define WOZMON 0xFF00

/**
 * wozmonitor rom
 * WOZMON for Apple I
 */
struct memoryInterceptWozMon : romIntercept {

#include "wozmon_code.h"

	virtual unsigned char read(unsigned short address) {
		return monitor[address - WOZMON];
	};
};

#endif
