#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "65co2.h"

// addressing mode strings
const char sMODES[15][6] = {
	"NONE ",
	"IMPL ", "IMM  ",
	"ABS  ", "ABS,X", "ABS,Y",
	"IND  ", "X,IND", "IND,Y",
	"ZPG  ", "ZPG,X", "ZPG,Y",
	"A    ",
	"REL  "};

// name
const char nmem[65][4] = {
	"BAD",
	"ADC", "AND", "ASL",
	"BCC", "BCS", "BEQ", "BIT", "BMI", "BNE", "BPL", "BRA", "BRK", "BVC", "BVS",
	"CLC", "CLD", "CLI", "CLV", "CMP", "CPX", "CPY",
	"DEC", "DEX", "DEY",
	"EOR",
	"INC", "INX", "INY",
	"JMP", "JSR",
	"LDA", "LDX", "LDY", "LSR",
	"NOP",
	"ORA",
	"PHA", "PHP", "PLA", "PLP", "PHX", "PHY", "PLX", "PLY",
	"ROL", "ROR", "RTI", "RTS",
	"SBC", "SEC", "SED", "SEI", "STA", "STX", "STY", "STZ",
	"TAX", "TAY", "TSX", "TXA", "TXS", "TYA", "TRB", "TSB"};

void SixtyFiveCeeOhTwo::addIntercept(uint16_t address, memoryIntercept *i)
{
	intercepts[address] = i;
}

void SixtyFiveCeeOhTwo::addInterceptRange(uint16_t start, uint16_t len, memoryIntercept *i)
{
	for (uint32_t a = start; a < start + len; a++)
		if (a < 65536)
			intercepts[a] = i;
}

SixtyFiveCeeOhTwo::SixtyFiveCeeOhTwo()
{
	memset(memory, 0, 65536);
	memset(intercepts, 0, sizeof(memoryIntercept *) * 65536);
}

/**
 * reset the cpu
 * sets A, X, Y and SR to 0, sets SP to 0xFD and PC to whever the reset vetor points to
 * cause reset is actually a BRK with the stack pushes read only, therefor it starts
 * SP at 0, then over the course of attemptying to fake-push PC and SR to the stack
 * it decrements SP 3 times to 0xFD
 */
void SixtyFiveCeeOhTwo::reset_cpu()
{
	r.SRsetByte(0);
	r.SR_INTERRUPT = true;
	r.SP = 0xFD;
	r.PC = readInWord(RESET_VECTOR);
	r.A = 0;
	r.X = 0;
	r.Y = 0;
	r.cycles = 7; // starting number of cycles -- reset always takes 7
	r.intb = false;
}

void SixtyFiveCeeOhTwo::setPC(uint16_t adr)
{
	r.PC = adr;
}

void SixtyFiveCeeOhTwo::push_cpu_interrupt(uint16_t PC, bool fromInt)
{
	push_stack((uint8_t)((PC & 0xFF00) >> 8));
	push_stack((uint8_t)(PC & 0xFF));
	push_stack(r.SRgetByte(fromInt));
}

void SixtyFiveCeeOhTwo::maskable_interrupt(void *source)
{
	interruptSources[source] = true;
	r.intb = true;
}

void SixtyFiveCeeOhTwo::clear_maskable_interrupt(void *source)
{
	if (interruptSources[source])
		interruptSources.erase(source);

	if (interruptSources.empty())
		r.intb = false;
}
/**
	 * non maskable interrupt
	 * do it now!
	 */
void SixtyFiveCeeOhTwo::non_maskable_interrupt()
{
	push_cpu_interrupt(r.PC, false);
	setPC(readInWord(NMI_VECTOR));
}

/**
	 * push a byte to the stack
	 */
void SixtyFiveCeeOhTwo::push_stack(uint8_t b)
{
	writeIn(0x100 + r.SP, b);
	r.SP--;
	// it should loop back to 255 if poping the stack at 0
}

/**
	 * pop a byte from the stack
	 */
uint8_t SixtyFiveCeeOhTwo::pop_stack()
{
	r.SP++;
	uint8_t v = readInByte(0x100 + r.SP);
	// it's supposed to overflow to 0 and stay on the stack page
	return v;
}

/**
	 * do next operation
	 * @return uint8_t number of cycles operation took
	 */
uint8_t SixtyFiveCeeOhTwo::do_cycle()
{

	// save program counter
	r.old_PC = r.PC;

	// get opcode at PC
	opcode = readInByte(r.PC);
	// point to first data byte
	r.PC += 1;

	// starting point for cycle count for this opcode. might increase
	r.cycles = OPCODES[opcode].cycles;

	// reset ALU to 0
	alu = 0;
	zpage = 0;
	address = 0;

	eADDRESSING addressingMode = OPCODES[opcode].mode;

	writeOK = OPCODES[opcode].rw;

	r.cycles += (this->*OPCODES[opcode].fetch)();

	old_alu = alu;

	// move PC now
	r.PC += OPCODES[opcode].step;

	r.cycles += (this->*OPCODES[opcode].op)();

	// store alu using addressing mode
	if (writeOK)
	{
		switch (addressingMode)
		{

		case ADR_ACC:
			r.A = alu & 0xFF;
			break;

		case ADR_ABS:
		case ADR_ABX:
		case ADR_ABY:
		case ADR_AIX:
		case ADR_ZPG:
		case ADR_ZPX:
		case ADR_ZPY:
		case ADR_ZPI:
		case ADR_INX:
		case ADR_INY:
			writeIn(address, alu & 0xFF);
			break;

		default:
			break;
		}
	}

	// check intb state if high and SR_INTERRUPT is low, trigger interrupt
	if (!r.SR_INTERRUPT && r.intb)
	{
		push_cpu_interrupt(r.PC, false);
		setPC(readInWord(IRQBRK_VECTOR));
		r.SR_DECIMAL = false;
		r.SR_INTERRUPT = true;
		r.cycles += 8;
	}

	// all done
	return r.cycles;
}

/**
	 * dumpCpuState
	 * printf the cpu state
	 */
void SixtyFiveCeeOhTwo::dumpCpuState()
{

	printf("%04X: %02X %02X %02X %02X %04X %04X %04X (%02X)%c%c%c%c%c%c: ",
		   r.old_PC,
		   r.A,
		   r.X,
		   r.Y,
		   r.SP,
		   old_alu & 0xFFFF,
		   alu & 0xFFFF,
		   address & 0xFFFF,
		   r.SRgetByte(false),
		   (r.SR_NEGATIVE) ? 'N' : ' ',
		   (r.SR_OVERFLOW) ? 'O' : ' ',
		   (r.SR_DECIMAL) ? 'D' : ' ',
		   (r.SR_INTERRUPT) ? 'I' : ' ',
		   (r.SR_ZERO) ? 'Z' : ' ',
		   (r.SR_CARRY) ? 'C' : ' ');
	uint8_t op = OPCODES[opcode].opcode;
	switch (OPCODES[opcode].step)
	{
	case 0:
		printf("%02X       %s ", opcode, nmem[op]);
		break;
	case 1:
		printf("%02X %02X    %s ", opcode, zpage, nmem[op]);
		break;
	case 2:
		printf("%02X %02X %02X %s ", opcode, address & 0xFF, (address & 0xFF00) >> 8, nmem[op]);
		break;
	}
	switch (OPCODES[opcode].mode)
	{
	case ADR_NON:
	case ADR_IMP:
		break;

	case ADR_IMM:
		printf("#$%02X", zpage);
		break;

	case ADR_ABS:
		printf("$%04X", address);
		break;
	case ADR_ABX:
		printf("$%04X,x", address);
		break;
	case ADR_ABY:
		printf("$%04X,y", address);
		break;

	case ADR_IND:
		printf("($%04X)", address);
		break;
	case ADR_AIX:
		printf("($%04X,x)", address);

	case ADR_ZPG:
		printf("$%02X", zpage);
		break;
	case ADR_ZPX:
		printf("$%02X,x", zpage);
		break;
	case ADR_ZPY:
		printf("$%02X,y", zpage);
		break;

	case ADR_ZPI:
		printf("($%02X)", zpage);
		break;
	case ADR_INX:
		printf("($%02X,x)", zpage);
		break;
	case ADR_INY:
		printf("($%02X),y", zpage);
		break;

	case ADR_ACC:
		printf("A");
	case ADR_REL:
		printf("$%04X", r.PC + (char)zpage);
		break;
	}
	printf("\r\n");
}

/**
	 * printCpuStateHeader
	 * printf the column names for dumpCpuState
	 */
void SixtyFiveCeeOhTwo::printCpuStateHeader()
{
	printf("PC     A  X  Y SP  mem  alu addr flags      + OP data CODE\r\n");
}

// ADR_NON
uint8_t SixtyFiveCeeOhTwo::A_NON()
{
	return 0;
}
// ADR_IMP
uint8_t SixtyFiveCeeOhTwo::A_IMP()
{
	address = 0;
	return 0;
}
// ADR_IMM
uint8_t SixtyFiveCeeOhTwo::A_IMM()
{
	address = 0;
	if (!OPCODES[opcode].rw)
		alu = readInByte(r.PC);
	return 0;
}
// ADR_ABS
uint8_t SixtyFiveCeeOhTwo::A_ABS()
{
	address = readInWord(r.PC);
	if (!OPCODES[opcode].rw)
		alu = readInByte(address);
	return 0;
}
// ADR_ABX
uint8_t SixtyFiveCeeOhTwo::A_ABX()
{
	// look for page crossings
	address = readInWord(r.PC);
	uint16_t page = address & 0xFF00;
	// apparently we need to read the address before
	// adding X
	readInWord(address);
	address += r.X;
	if (!OPCODES[opcode].rw)
		alu = readInByte(address);
	if ((address & 0xFF00) > page)
		return 1;
	return 0;
}
// ADR_ABY
uint8_t SixtyFiveCeeOhTwo::A_ABY()
{
	// look for page crossings
	address = readInWord(r.PC);
	uint16_t page = address & 0xFF00;
	// apparently we need to read the address before
	// adding X
	readInWord(address);
	address += r.Y;
	if (!OPCODES[opcode].rw)
		alu = readInByte(address);
	if ((address & 0xFF00) > page)
		return 1;
	return 0;
}
// ADR_AB_IX
uint8_t SixtyFiveCeeOhTwo::A_AIX()
{
	int32_t o1 = readInByte(r.PC);
	int32_t o2 = readInByte(r.PC + 1) << 8;

	o1 += r.X; // add X to the address specified

	address = readInByte(o2 + o1);
	o1++;
	address += readInByte(o2 + o1) << 8;
	if (!OPCODES[opcode].rw)
		alu = readInByte(address);
	return 0;
}
// ADR_IND
uint8_t SixtyFiveCeeOhTwo::A_IND()
{
	// nmos bug
	// if indirect addressing points to last byte in page
	// then next byte for address is first byte in same page
	// and not first byte from next page
	int32_t o1 = readInByte(r.PC);
	int32_t o2 = readInByte(r.PC + 1) << 8;
	address = readInByte(o2 + o1);
	//the c doesn't have this bug
	//o1 = (o1 + 1) & 0xFF;
	o1 += 1;
	address += readInByte(o2 + o1) << 8;
	if (!OPCODES[opcode].rw)
		alu = readInByte(address);
	return 0;
}
// ADR_INX
uint8_t SixtyFiveCeeOhTwo::A_INX()
{
	zpage = readInByte(r.PC);
	// address to look up is ZP + X without carry
	int32_t o1 = (zpage + r.X) & 0xFF; // is this right?
	address = readInByte(o1);
	address += (readInByte((o1 + 1) & 0xFF)) << 8;
	if (!OPCODES[opcode].rw)
		alu = readInByte(address);
	return 0;
}
// ADR_INY
uint8_t SixtyFiveCeeOhTwo::A_INY()
{
	zpage = readInByte(r.PC);
	// IAH wraps if zpage = 0xFF
	int32_t o1 = readInByte(zpage) + r.Y;
	int32_t o2 = readInByte((zpage + 1) & 0xFF) << 8;
	// now add Y, look for page boundries
	address = o2 + o1;
	if (!OPCODES[opcode].rw)
		alu = readInByte(address);
	if (o1 > 0x0100)
		return 1;
	return 0;
}
// ADR_ZPG
uint8_t SixtyFiveCeeOhTwo::A_ZPG()
{
	zpage = readInByte(r.PC);
	address = zpage;
	if (!OPCODES[opcode].rw)
		alu = readInByte(address);
	return 0;
}
// ADR_ZPX
uint8_t SixtyFiveCeeOhTwo::A_ZPX()
{
	zpage = readInByte(r.PC);
	// address is zpage + X without carry
	address = (zpage + r.X) & 0xFF;
	if (!OPCODES[opcode].rw)
		alu = readInByte(address);
	return 0;
}
// ADR_ZPY
uint8_t SixtyFiveCeeOhTwo::A_ZPY()
{
	zpage = readInByte(r.PC);
	// address is zpage + Y without carry
	address = (zpage + r.Y) & 0xFF;
	if (!OPCODES[opcode].rw)
		alu = readInByte(address);
	return 0;
}
// ADR_ZPI
uint8_t SixtyFiveCeeOhTwo::A_ZPI()
{
	zpage = readInByte(r.PC);
	int32_t o1 = readInByte(zpage);
	int32_t o2 = readInByte(zpage + 1) << 8;
	address = (o1 + o2) & 0XFFFF;
	if (!OPCODES[opcode].rw)
		alu = readInByte(address);
	return 0;
}
// ADR_ACC
uint8_t SixtyFiveCeeOhTwo::A_ACC()
{
	address = 0;
	if (!OPCODES[opcode].rw)
		alu = r.A;
	return 0;
}
// ADR_REL
uint8_t SixtyFiveCeeOhTwo::A_REL()
{
	address = r.PC;
	uint16_t page = address >> 8;
	alu = readInByte(r.PC);
	zpage = alu;
	if (alu >= 0x80)
	{
		// 2's compliment
		alu = -(256 - alu);
	}
	address += (alu + 1) & 0xFFFF;
	// reusing alu as the cycle bump if we branch
	alu = 1;
	if (address >> 8 != page)
		alu = 2;
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::BAD()
{
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::NOP()
{ // no operation
	return 0;
}

// add and subtract
uint8_t SixtyFiveCeeOhTwo::ADC()
{ // add with carry
	int32_t o1 = r.A;
	int32_t o2 = alu;
	if (r.SR_DECIMAL)
	{

		// decimal mode behavior following Marko Makela's explanations
		r.SR_ZERO = ((o1 + o2 + (r.SR_CARRY ? 1 : 0)) & 0xFF) == 0;
		alu = (o1 & 0x0F) + (o2 & 0x0F) + (r.SR_CARRY ? 1 : 0);
		r.A = alu < 0x0A ? alu : alu + 6;
		alu = (o1 & 0xF0) + (o2 & 0xF0) + (alu & 0xF0);
		r.SR_NEGATIVE = ((char)alu) < 0;
		r.SR_OVERFLOW = ((o1 ^ alu) & ~(o1 ^ o2) & 0x80) != 0;
		alu = (r.A & 0x0F) | (alu < 0xA0 ? alu : alu + 0x60);
		r.SRcarry(alu);
		r.A = alu & 0xFF;
	}
	else
	{
		// A + M + C -> A
		alu = o1 + o2 + (r.SR_CARRY ? 1 : 0);

		// clear flags
		// set them
		r.SRcarry(alu);

		//r.SR_OVERFLOW = ((r.A & 0x80) == (old_alu & 0x80)) && ((r.A & 0x80) != (alu & 0x80));
		r.SR_OVERFLOW = ((o1 ^ r.A) & ~(o1 ^ o2) & 0x80) != 0;

		// update A
		r.A = alu & 0xFF;

		r.SRsetNZ(r.A);
	}
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::SBC()
{ // subtract with carry
	int32_t o1 = r.A;
	int32_t o2 = alu;
	if (r.SR_DECIMAL)
	{

		// decimal mode behavior following Marko Makela's explanations
		alu = (o1 & 0x0F) - (o2 & 0x0F) - (r.SR_CARRY ? 0 : 1);
		r.A = (alu & 0x10) == 0 ? alu : alu - 6;
		alu = (o1 & 0xF0) - (o2 & 0xF0) - (r.A & 0x10);
		r.A = (r.A & 0x0F) | ((alu & 0x100) == 0 ? alu : alu - 0x60);
		alu = o1 - o2 - (r.SR_CARRY ? 0 : 1);

		r.SRsetNZ(r.A);
		r.SRborrow(alu);
	}
	else
	{
		// A + M + C -> A
		alu = o1 - o2 - (r.SR_CARRY ? 0 : 1);

		// store A
		r.A = alu & 0xFF;

		// set carry, set others
		r.SR_OVERFLOW = ((o1 ^ o2) & (o1 ^ r.A) & 0x80) != 0;
		r.SRsetNZ(r.A);
		r.SRborrow(alu);
	}
	return 0;
}

// compare memory with...
uint8_t SixtyFiveCeeOhTwo::CMP()
{ // compare with A
	alu = (r.A - alu);
	r.SRborrow(alu);
	r.SRsetNZ(alu);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::CPX()
{ // compare with X
	alu = (r.X - alu);
	r.SRborrow(alu);
	r.SRsetNZ(alu);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::CPY()
{ // compare with Y
	alu = (r.Y - alu);
	r.SRborrow(alu);
	r.SRsetNZ(alu);
	return 0;
}

// logical operators
uint8_t SixtyFiveCeeOhTwo::AND()
{ // and (with accumulator)
	// apply alu to A
	r.A &= alu;
	r.SRsetNZ(r.A);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::ORA()
{ // or with accumulator
	// apply alu to A
	r.A |= alu;
	r.SRsetNZ(r.A);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::EOR()
{ // exclusive or (with accumulator)
	// apply alu to A
	r.A ^= alu;
	r.SRsetNZ(r.A);
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::ASL()
{ // arithmetic shift left
	// catch carry
	r.SR_CARRY = (alu & 0x80) != 0;
	// a jump, to the left!
	alu <<= 1;
	r.SRsetNZ(alu);
	writeOK = true;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::LSR()
{ // logical shift right
	r.SR_CARRY = (alu & 0x01) != 0;
	// and a step, to the right!
	alu >>= 1;
	// set flags
	r.SRsetNZ(alu);
	writeOK = true;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::ROL()
{ // rotate left
	// rotate left and mix in carry
	int32_t o1 = alu & 0x80;
	alu <<= 1;
	if (r.SR_CARRY)
		alu |= 0x01;
	r.SR_CARRY = o1 != 0;
	// set flags
	r.SRsetNZ(alu);
	writeOK = true;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::ROR()
{ // rotate right
	int32_t o1 = alu & 0x01;
	alu >>= 1;
	if (r.SR_CARRY)
		alu |= 0x80;
	// set flags
	r.SR_CARRY = o1 == 0x01;
	r.SRsetNZ(alu);
	writeOK = true;
	return 0;
}

// Increment, decrement A, X, Y
uint8_t SixtyFiveCeeOhTwo::INC()
{ // increment
	alu = (alu + 1) & 0xFF;
	r.SRsetNZ(alu);
	writeOK = true;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::DEC()
{ // decrement
	alu = (alu - 1) & 0xFF;
	r.SRsetNZ(alu);
	writeOK = true;
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::INX()
{ // increment X
	r.X = (r.X + 1) & 0xFF;
	r.SRsetNZ(r.X);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::INY()
{ // increment Y
	r.Y = (r.Y + 1) & 0xFF;
	r.SRsetNZ(r.Y);
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::DEX()
{ // decrement X
	r.X = (r.X - 1) & 0xFF;
	r.SRsetNZ(r.X);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::DEY()
{ // decrement Y
	r.Y = (r.Y - 1) & 0xFF;
	r.SRsetNZ(r.Y);
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::BIT()
{ // bit test
	// does a non-destructive AND vs A and memory
	// sets Z if there's no matches
	// sets N to bit 7
	// sets V to bit 6
	r.SR_OVERFLOW = (alu & SR_OVERFLOW_B); // bit 6
	r.SR_NEGATIVE = (alu & SR_NEGATIVE_B); // bit 7
	r.SR_ZERO = (alu & r.A) == 0;
	return 0;
}

// flag clear and set
uint8_t SixtyFiveCeeOhTwo::CLC()
{ // clear carry
	r.SR_CARRY = false;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::SEC()
{ // set carry
	r.SR_CARRY = true;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::CLD()
{ // clear decimal
	//r.SR_DECIMAL = false;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::SED()
{ // set decimal
	//r.SR_DECIMAL = true;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::CLI()
{ // clear interrupt disable
	r.SR_INTERRUPT = false;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::SEI()
{ // set interrupt disable
	r.SR_INTERRUPT = true;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::CLV()
{ // clear overflow
	r.SR_OVERFLOW = false;
	return 0;
}

// branching
// alu holds bonus cycle count if page is crossed
uint8_t SixtyFiveCeeOhTwo::BCS()
{ // branch on carry set
	if (r.SR_CARRY)
	{
		r.PC = address;
		return alu;
	}
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::BCC()
{ // branch on carry clear
	if (!(r.SR_CARRY))
	{
		r.PC = address;
		return alu;
	}
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::BEQ()
{ // branch on equal (zero set)
	if (r.SR_ZERO)
	{
		r.PC = address;
		return alu;
	}
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::BNE()
{ // branch on not equal (zero clear)
	if (!(r.SR_ZERO))
	{
		r.PC = address;
		return alu;
	}
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::BMI()
{ // branch on minus (negative set)
	if (r.SR_NEGATIVE)
	{
		r.PC = address;
		return alu;
	}
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::BPL()
{ // branch on plus (negative clear)
	if (!(r.SR_NEGATIVE))
	{
		r.PC = address;
		return alu;
	}
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::BVS()
{ // branch on overflow set
	if (r.SR_OVERFLOW)
	{
		r.PC = address;
		return alu;
	}
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::BVC()
{ // branch on overflow clear
	if (!(r.SR_OVERFLOW))
	{
		r.PC = address;
		return alu;
	}
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::BRA()
{
	r.PC = address;
	r.cycles += alu;
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::JMP()
{ // jump
	r.PC = address;
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::JSR()
{ // jump subroutine
	// push PC onto stack
	alu = r.PC - 1;
	push_stack((uint8_t)((alu & 0xFF00) >> 8));
	push_stack((uint8_t)(alu & 0xFF));
	// update to new address in alu
	r.PC = address;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::RTS()
{ // return from subroutine
	// address should be -1 from next opcode
	alu = (uint16_t)pop_stack() | ((uint16_t)pop_stack() << 8);
	alu++;
	alu &= 0xFFFF;
	address = alu;
	r.PC = address;
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::BRK()
{ // interrupt
	push_cpu_interrupt(r.PC, true);
	r.PC = readInWord(IRQBRK_VECTOR);
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::RTI()
{ // return from interrupt
	r.SRsetByte(pop_stack());
	alu = (uint16_t)pop_stack() | ((uint16_t)pop_stack() << 8);
	alu &= 0xFFFF;
	address = alu;
	r.PC = address;
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::LDA()
{ // load accumulator
	r.SRsetNZ(alu);
	r.A = alu & 0xFF;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::LDX()
{ // load X
	r.SRsetNZ(alu);
	r.X = alu & 0xFF;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::LDY()
{ // load Y
	r.SRsetNZ(alu);
	r.Y = alu & 0xFF;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::STA()
{ // store accumulator
	alu = r.A;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::STX()
{ // store X
	alu = r.X;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::STY()
{ // store Y
	alu = r.Y;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::STZ()
{ // store 0
	alu = 0;
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::PHX() // push accumulator
{
	push_stack(r.X);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::PLX() // pull accumulator, uses writeOK
{
	alu = pop_stack();
	r.SRsetNZ(alu);
	r.X = alu & 0xFF;
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::PHY() // push accumulator
{
	push_stack(r.Y);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::PLY() // pull accumulator, uses writeOK
{
	alu = pop_stack();
	r.SRsetNZ(alu);
	r.Y = alu & 0xFF;
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::PHA()
{ // push accumulator
	push_stack(r.A);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::PLA()
{ // pull accumulator, uses writeOK
	alu = pop_stack();
	r.SRsetNZ(alu);
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::PHP()
{ // push processor status (SR)
	push_stack(r.SRgetByte(true));
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::PLP()
{ // pull processor status (SR)
	r.SRsetByte(pop_stack());
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::TSX()
{ // transfer stack pointer to X
	r.X = r.SP;
	r.SRsetNZ(r.X);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::TXS()
{ // transfer X to stack pointer
	r.SP = r.X;
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::TAX()
{ // transfer accumulator to X
	r.X = r.A;
	r.SRsetNZ(r.X);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::TAY()
{ // transfer accumulator to Y
	r.Y = r.A;
	r.SRsetNZ(r.Y);
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::TXA()
{ // transfer X to accumulator
	r.A = r.X;
	r.SRsetNZ(r.A);
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::TYA()
{ // transfer Y to accumulator
	r.A = r.Y;
	r.SRsetNZ(r.A);
	return 0;
}

uint8_t SixtyFiveCeeOhTwo::TRB()
{
	r.SR_ZERO = (alu & r.A) == 0;
	alu &= ~r.A;
	writeOK = 1;
	return 0;
}
uint8_t SixtyFiveCeeOhTwo::TSB()
{
	r.SR_ZERO = (alu & r.A) == 0;
	alu |= r.A;
	writeOK = 1;
	return 0;
}