#pragma once

// a simple breakpoint on address intercept
struct miBreakpoint : memoryIntercept {
	list<unsigned short> addresses;
	AppleOne *cpu;
	unsigned char v;
	unsigned char read(unsigned short address) {
		if(address == cpu->r.PC) {
			cpu->debugging = true;
			cpu->stepping = true;
		}
		return v;
	};
	void write(unsigned char value, unsigned short address) {
		v = value;
	};
	void add(unsigned short address) {

	};
	void remove(unsigned short address) {
	};
};
