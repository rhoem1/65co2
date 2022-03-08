65co2 cpu emulation with googletest unit tests suite

This project uses the emulation in an apple1 emulator, which includes several different BASIC interperters and the Krusader v1.3 assembler.

Requirements
Posix, c++ that supports c++14 or newer, cmake

apple1 [-abkmqdsw] [loadfile]
  -a install ApplesoftLite @ E000
  -b start Woz Basic
  -k install Krusader13 @ F000
  -m install MSBasic @ 400
	-v verbose output
  -q quiet mode on
  -d turn on opcode trace
  -s turn on opcode trace in step mode
                                        
  -w turn off 40 column width limitation

 ctrl-c to quit, f2 to reset, f3 to nmi

The version of msbasic is derived from the msbasic project 
with modifications to allow it to workon an apple1.  It is mostly
based on the applesoft 2 build.  The fork of the source tree is in
the msbasic folder.
https://github.com/mist64/msbasic

applesoft lite is the latest version at
https://cowgod.org/replica1/applesoft/

krusader v1.3 comes from
http://school.anhb.uwa.edu.au/personalpages/kwessen/apple1/Krusader.htm

wozbasic and wozmonitor are from the pom1 apple1 emulator
http://pom1.sourceforge.net/

Differences from the Apple 1:

$C000 block is ram
$CFFF returns a random number 0-255
I/O PIA is at $D010 to $D013 only and is not replicated through all of the 
$D000 block
$D01E toggle output
$D01F quit emulation

Unit tests via GoogleTest
