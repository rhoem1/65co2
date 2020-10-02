#ifndef __65CO2_H__
#define __65CO2_H__

#include <stdint.h>
#include <map>

#define ZEROPAGE 0x0000
#define STACK 0x0100

#define IRQBRK_VECTOR 0xFFFE
#define RESET_VECTOR 0xFFFC
#define NMI_VECTOR 0xFFFA

// status constants
#define SR_NEGATIVE_B 0x80
#define SR_OVERFLOW_B 0x40
#define SR_UNUSED_B 0x20
#define SR_BREAK_B 0x10
#define SR_DECIMAL_B 0x08
#define SR_INTERRUPT_B 0x04
#define SR_ZERO_B 0x02
#define SR_CARRY_B 0x01

// addressing modes
enum eADDRESSING
{
	ADR_NON = 0,
	ADR_IMP,
	ADR_IMM,
	ADR_ABS,
	ADR_ABX,
	ADR_ABY,
	ADR_IND,
	ADR_AIX,
	ADR_INX,
	ADR_INY,
	ADR_ZPG,
	ADR_ZPX,
	ADR_ZPY,
	ADR_ZPI,
	ADR_ACC,
	ADR_REL
};

enum eOPCODES
{
	OP_BAD = 0,
	OP_ADC, // add with carry
	OP_AND, // and (with accumulator)
	OP_ASL, // arithmetic shift left
	OP_BCC, // branch on carry clear
	OP_BCS, // branch on carry set
	OP_BEQ, // branch on equal (zero set)
	OP_BIT, // bit test
	OP_BMI, // branch on minus (negative set)
	OP_BNE, // branch on not equal (zero clear)
	OP_BPL, // branch on plus (negative clear)
	OP_BRA, // branch always
	OP_BRK, // interrupt
	OP_BVC, // branch on overflow clear
	OP_BVS, // branch on overflow set
	OP_CLC, // clear carry
	OP_CLD, // clear decimal
	OP_CLI, // clear interrupt disable
	OP_CLV, // clear overflow
	OP_CMP, // compare (with accumulator)
	OP_CPX, // compare with X
	OP_CPY, // compare with Y
	OP_DEC, // decrement
	OP_DEX, // decrement X
	OP_DEY, // decrement Y
	OP_EOR, // exclusive or (with accumulator)
	OP_INC, // increment
	OP_INX, // increment X
	OP_INY, // increment Y
	OP_JMP, // jump
	OP_JSR, // jump subroutine
	OP_LDA, // load accumulator
	OP_LDX, // load X
	OP_LDY, // load Y
	OP_LSR, // logical shift right
	OP_NOP, // no operation
	OP_ORA, // or with accumulator
	OP_PHA, // push accumulator
	OP_PHP, // push processor status (SR)
	OP_PLA, // pull accumulator
	OP_PLP, // pull processor status (SR)
	OP_PHX, // push X
	OP_PHY, // push Y
	OP_PLX, // pull X
	OP_PLY, // pull Y
	OP_ROL, // rotate left
	OP_ROR, // rotate right
	OP_RTI, // return from interrupt
	OP_RTS, // return from subroutine
	OP_SBC, // subtract with carry
	OP_SEC, // set carry
	OP_SED, // set decimal
	OP_SEI, // set interrupt disable
	OP_STA, // store accumulator
	OP_STX, // store X
	OP_STY, // store Y
	OP_STZ, // store a 0
	OP_TAX, // transfer accumulator to X
	OP_TAY, // transfer accumulator to Y
	OP_TSX, // transfer stack pointer to X
	OP_TXA, // transfer X to accumulator
	OP_TXS, // transfer X to stack pointer
	OP_TYA, // transfer Y to accumulator
	OP_TRB, // test and reset bits
	OP_TSB	// test and set bits
};


struct memoryIntercept
{
	memoryIntercept() {}
	virtual ~memoryIntercept() {}
	virtual uint8_t read(uint16_t address) { return 0; }
	virtual void write(uint8_t value, uint16_t address) {}
};


class SixtyFiveCeeOhTwo
{
public:

	SixtyFiveCeeOhTwo();

	// read a byte from memory ignoring intercepts
	inline uint8_t readByte(uint16_t address)
	{
		return memory[address];
	}

	// read a word from memory ignoring intercepts
	inline uint16_t readWord(uint16_t address)
	{
		return ((uint16_t)readByte(address + 1) << 8) | readByte(address);
	}

	// write to memory ignoring intercepts
	inline void write(uint16_t address, int value)
	{
		memory[address] = value & 0xFF;
	}

	// increment a byte
	inline void incByte(uint16_t address)
	{
		memory[address]++;
	}

	// decrement a byte
	inline void decByte(uint16_t address)
	{
		memory[address]--;
	}

	// copy a byte from one address to another
	inline void copyByte(uint16_t fromAdr, uint16_t toAdr)
	{
		memory[toAdr] = memory[fromAdr];
	}

	/**
	 * add intercept to specific address
	 */
	void addIntercept(uint16_t address, memoryIntercept *i);

	/**
	 * add intercept to a range of addresses
	 */
	void addInterceptRange(uint16_t start, uint16_t len, memoryIntercept *i);

	/**
	 * reset the cpu
	 * sets A, X, Y and SR to 0, sets SP to 0xFD and PC to whever the reset vetor points to
	 * cause reset is actually a BRK with the stack pushes read only, therefor it starts
	 * SP at 0, then over the course of attemptying to fake-push PC and SR to the stack
	 * it decrements SP 3 times to 0xFD
	 */
	void reset_cpu();

	/**
	 * set PC to address
	 */
	void setPC(uint16_t adr);

	/**
	 * Interrupt the pc, maskable
	 * do it maybe!
	 */
	void maskable_interrupt(void *source);

	void clear_maskable_interrupt(void *source);

	/**
	 * non maskable interrupt
	 * do it now!
	 */
	void non_maskable_interrupt();

	/**
	 * do next operation
	 * @return uint8_t number of cycles operation took
	 */
	uint8_t do_cycle();

	/**
	 * dumpCpuState
	 * printf the cpu state
	 */
	void dumpCpuState();

	/**
	 * printCpuStateHeader
	 * printf the column names for dumpCpuState
	 */
	void printCpuStateHeader();

private:

typedef struct __registers
{
	uint8_t cycles;
	uint16_t PC;	   // program counter
	uint16_t old_PC; // program counter
	int32_t A;				   // Accumulator
	int32_t X;				   // X index
	int32_t Y;				   // Y index
	uint8_t SP;	   // stack pointer

	bool SR_NEGATIVE;
	bool SR_OVERFLOW;
	bool SR_DECIMAL;
	bool SR_INTERRUPT;
	bool SR_ZERO;
	bool SR_CARRY;

	bool intb;

	inline void SRsetByte(uint8_t b)
	{
		SR_NEGATIVE = (b & SR_NEGATIVE_B);
		SR_OVERFLOW = (b & SR_OVERFLOW_B);
		SR_DECIMAL = (b & SR_DECIMAL_B);
		SR_INTERRUPT = (b & SR_INTERRUPT_B);
		SR_ZERO = (b & SR_ZERO_B);
		SR_CARRY = (b & SR_CARRY_B);
	}
	inline uint8_t SRgetByte(bool SR_BREAK)
	{
		return (SR_NEGATIVE ? SR_NEGATIVE_B : 0) |
				(SR_OVERFLOW ? SR_OVERFLOW_B : 0) |
				SR_UNUSED_B |
				(SR_BREAK ? SR_BREAK_B : 0) |
				(SR_DECIMAL ? SR_DECIMAL_B : 0) |
				(SR_INTERRUPT ? SR_INTERRUPT_B : 0) |
				(SR_ZERO ? SR_ZERO_B : 0) |
				(SR_CARRY ? SR_CARRY_B : 0);
	}

	inline void SRsetNZ(uint8_t v)
	{
		SR_NEGATIVE = (v & SR_NEGATIVE_B) == SR_NEGATIVE_B;
		SR_ZERO = (v == 0x00);
	}

	inline void SRborrow(uint16_t alu)
	{
		SR_CARRY = (alu & 0x100) == 0;
	}

	inline void SRcarry(uint16_t alu)
	{
		SR_CARRY = (alu & 0x100) != 0;
	}

} REGISTERS;

	// opcode attributes
	struct OPCODE
	{
		OPCODE(eOPCODES _opcode, uint8_t (SixtyFiveCeeOhTwo::*_op)(void), eADDRESSING _mode, uint8_t (SixtyFiveCeeOhTwo::*_fetch)(), uint8_t _step, uint8_t _cycles, uint8_t _rw) {
			opcode = _opcode; 
			op = _op; 
			mode = _mode; 
			fetch = _fetch; 
			step = _step; 
			cycles = _cycles;
			rw = _rw;
		};

		eOPCODES opcode; // the opcode enum
		uint8_t (SixtyFiveCeeOhTwo::*op)(void) = nullptr;

		eADDRESSING mode; // addressing mode
		uint8_t (SixtyFiveCeeOhTwo::*fetch)() = nullptr;

		uint8_t step;	  // PC steps
		uint8_t cycles; // minimum cycles
		uint8_t rw;	  // write mode
	};

	// opcode attribute table

	OPCODE OPCODES[256] = {
		OPCODE(OP_BRK, &SixtyFiveCeeOhTwo::BRK, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 1, 7, 0), // 0x00
		OPCODE(OP_ORA, &SixtyFiveCeeOhTwo::ORA, ADR_INX, &SixtyFiveCeeOhTwo::A_INX, 1, 6, 0), // 0x01
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x02
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x03
		OPCODE(OP_TSB, &SixtyFiveCeeOhTwo::TSB, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 5, 0), // 0x04
		OPCODE(OP_ORA, &SixtyFiveCeeOhTwo::ORA, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0x05
		OPCODE(OP_ASL, &SixtyFiveCeeOhTwo::ASL, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 5, 0), // 0x06
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x07
		OPCODE(OP_PHP, &SixtyFiveCeeOhTwo::PHP, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 3, 0), // 0x08
		OPCODE(OP_ORA, &SixtyFiveCeeOhTwo::ORA, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0x09
		OPCODE(OP_ASL, &SixtyFiveCeeOhTwo::ASL, ADR_ACC, &SixtyFiveCeeOhTwo::A_ACC, 0, 2, 0), // 0x0A
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x0B
		OPCODE(OP_TSB, &SixtyFiveCeeOhTwo::TSB, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 6, 0), // 0x0C
		OPCODE(OP_ORA, &SixtyFiveCeeOhTwo::ORA, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0x0D
		OPCODE(OP_ASL, &SixtyFiveCeeOhTwo::ASL, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 6, 0), // 0x0E
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x0F
		OPCODE(OP_BPL, &SixtyFiveCeeOhTwo::BPL, ADR_REL, &SixtyFiveCeeOhTwo::A_REL, 1, 2, 0), // 0x10
		OPCODE(OP_ORA, &SixtyFiveCeeOhTwo::ORA, ADR_INY, &SixtyFiveCeeOhTwo::A_INY, 1, 5, 0), // 0x11
		OPCODE(OP_ORA, &SixtyFiveCeeOhTwo::ORA, ADR_ZPI, &SixtyFiveCeeOhTwo::A_ZPI, 1, 5, 0), // 0x12
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x13
		OPCODE(OP_TRB, &SixtyFiveCeeOhTwo::TRB, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 5, 0), // 0x14
		OPCODE(OP_ORA, &SixtyFiveCeeOhTwo::ORA, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 0), // 0x15
		OPCODE(OP_ASL, &SixtyFiveCeeOhTwo::ASL, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 6, 0), // 0x16
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x17
		OPCODE(OP_CLC, &SixtyFiveCeeOhTwo::CLC, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0x18
		OPCODE(OP_ORA, &SixtyFiveCeeOhTwo::ORA, ADR_ABY, &SixtyFiveCeeOhTwo::A_ABY, 2, 4, 0), // 0x19
		OPCODE(OP_DEC, &SixtyFiveCeeOhTwo::DEC, ADR_ACC, &SixtyFiveCeeOhTwo::A_ACC, 0, 2, 0), // 0x1A
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x1B
		OPCODE(OP_TRB, &SixtyFiveCeeOhTwo::TRB, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 6, 0), // 0x1C
		OPCODE(OP_ORA, &SixtyFiveCeeOhTwo::ORA, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 4, 0), // 0x1D
		OPCODE(OP_ASL, &SixtyFiveCeeOhTwo::ASL, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 6, 0), // 0x1E
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x1F
		OPCODE(OP_JSR, &SixtyFiveCeeOhTwo::JSR, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 6, 0), // 0x20
		OPCODE(OP_AND, &SixtyFiveCeeOhTwo::AND, ADR_INX, &SixtyFiveCeeOhTwo::A_INX, 1, 6, 0), // 0x21
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x22
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x23
		OPCODE(OP_BIT, &SixtyFiveCeeOhTwo::BIT, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0x24
		OPCODE(OP_AND, &SixtyFiveCeeOhTwo::AND, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0x25
		OPCODE(OP_ROL, &SixtyFiveCeeOhTwo::ROL, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 5, 0), // 0x26
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x27
		OPCODE(OP_PLP, &SixtyFiveCeeOhTwo::PLP, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 4, 0), // 0x28
		OPCODE(OP_AND, &SixtyFiveCeeOhTwo::AND, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0x29
		OPCODE(OP_ROL, &SixtyFiveCeeOhTwo::ROL, ADR_ACC, &SixtyFiveCeeOhTwo::A_ACC, 0, 2, 0), // 0x2A
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x2B
		OPCODE(OP_BIT, &SixtyFiveCeeOhTwo::BIT, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0x2C
		OPCODE(OP_AND, &SixtyFiveCeeOhTwo::AND, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0x2D
		OPCODE(OP_ROL, &SixtyFiveCeeOhTwo::ROL, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 6, 0), // 0x2E
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x2F
		OPCODE(OP_BMI, &SixtyFiveCeeOhTwo::BMI, ADR_REL, &SixtyFiveCeeOhTwo::A_REL, 1, 2, 0), // 0x30
		OPCODE(OP_AND, &SixtyFiveCeeOhTwo::AND, ADR_INY, &SixtyFiveCeeOhTwo::A_INY, 1, 5, 0), // 0x31
		OPCODE(OP_AND, &SixtyFiveCeeOhTwo::AND, ADR_ZPI, &SixtyFiveCeeOhTwo::A_ZPI, 1, 5, 0), // 0x32
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x33
		OPCODE(OP_BIT, &SixtyFiveCeeOhTwo::BIT, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 0), // 0x34
		OPCODE(OP_AND, &SixtyFiveCeeOhTwo::AND, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 0), // 0x35
		OPCODE(OP_ROL, &SixtyFiveCeeOhTwo::ROL, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 6, 0), // 0x36
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x37
		OPCODE(OP_SEC, &SixtyFiveCeeOhTwo::SEC, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0x38
		OPCODE(OP_AND, &SixtyFiveCeeOhTwo::AND, ADR_ABY, &SixtyFiveCeeOhTwo::A_ABY, 2, 4, 0), // 0x39
		OPCODE(OP_INC, &SixtyFiveCeeOhTwo::INC, ADR_ACC, &SixtyFiveCeeOhTwo::A_ACC, 0, 2, 0), // 0x3A
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x3B
		OPCODE(OP_BIT, &SixtyFiveCeeOhTwo::BIT, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 4, 0), // 0x3C
		OPCODE(OP_AND, &SixtyFiveCeeOhTwo::AND, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 4, 0), // 0x3D
		OPCODE(OP_ROL, &SixtyFiveCeeOhTwo::ROL, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 6, 0), // 0x3E
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x3F
		OPCODE(OP_RTI, &SixtyFiveCeeOhTwo::RTI, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 6, 0), // 0x40
		OPCODE(OP_EOR, &SixtyFiveCeeOhTwo::EOR, ADR_INX, &SixtyFiveCeeOhTwo::A_INX, 1, 6, 0), // 0x41
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x42
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x43
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x44
		OPCODE(OP_EOR, &SixtyFiveCeeOhTwo::EOR, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0x45
		OPCODE(OP_LSR, &SixtyFiveCeeOhTwo::LSR, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 5, 0), // 0x46
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x47
		OPCODE(OP_PHA, &SixtyFiveCeeOhTwo::PHA, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 3, 0), // 0x48
		OPCODE(OP_EOR, &SixtyFiveCeeOhTwo::EOR, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0x49
		OPCODE(OP_LSR, &SixtyFiveCeeOhTwo::LSR, ADR_ACC, &SixtyFiveCeeOhTwo::A_ACC, 0, 2, 0), // 0x4A
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x4B
		OPCODE(OP_JMP, &SixtyFiveCeeOhTwo::JMP, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 3, 0), // 0x4C
		OPCODE(OP_EOR, &SixtyFiveCeeOhTwo::EOR, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0x4D
		OPCODE(OP_LSR, &SixtyFiveCeeOhTwo::LSR, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 6, 0), // 0x4E
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x4F
		OPCODE(OP_BVC, &SixtyFiveCeeOhTwo::BVC, ADR_REL, &SixtyFiveCeeOhTwo::A_REL, 1, 2, 0), // 0x50
		OPCODE(OP_EOR, &SixtyFiveCeeOhTwo::EOR, ADR_INY, &SixtyFiveCeeOhTwo::A_INY, 1, 5, 0), // 0x51
		OPCODE(OP_EOR, &SixtyFiveCeeOhTwo::EOR, ADR_ZPI, &SixtyFiveCeeOhTwo::A_ZPI, 1, 5, 0), // 0x52
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x53
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x54
		OPCODE(OP_EOR, &SixtyFiveCeeOhTwo::EOR, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 0), // 0x55
		OPCODE(OP_LSR, &SixtyFiveCeeOhTwo::LSR, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 6, 0), // 0x56
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x57
		OPCODE(OP_CLI, &SixtyFiveCeeOhTwo::CLI, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0x58
		OPCODE(OP_EOR, &SixtyFiveCeeOhTwo::EOR, ADR_ABY, &SixtyFiveCeeOhTwo::A_ABY, 2, 4, 0), // 0x59
		OPCODE(OP_PHY, &SixtyFiveCeeOhTwo::PHY, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 3, 0), // 0x5A
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x5B
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x5C
		OPCODE(OP_EOR, &SixtyFiveCeeOhTwo::EOR, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 4, 0), // 0x5D
		OPCODE(OP_LSR, &SixtyFiveCeeOhTwo::LSR, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 6, 0), // 0x5E
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x5F
		OPCODE(OP_RTS, &SixtyFiveCeeOhTwo::RTS, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 6, 0), // 0x60
		OPCODE(OP_ADC, &SixtyFiveCeeOhTwo::ADC, ADR_INX, &SixtyFiveCeeOhTwo::A_INX, 1, 6, 0), // 0x61
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x62
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x63
		OPCODE(OP_STZ, &SixtyFiveCeeOhTwo::STZ, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0x64
		OPCODE(OP_ADC, &SixtyFiveCeeOhTwo::ADC, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0x65
		OPCODE(OP_ROR, &SixtyFiveCeeOhTwo::ROR, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 5, 0), // 0x66
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x67
		OPCODE(OP_PLA, &SixtyFiveCeeOhTwo::PLA, ADR_ACC, &SixtyFiveCeeOhTwo::A_ACC, 0, 4, 1), // 0x68
		OPCODE(OP_ADC, &SixtyFiveCeeOhTwo::ADC, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0x69
		OPCODE(OP_ROR, &SixtyFiveCeeOhTwo::ROR, ADR_ACC, &SixtyFiveCeeOhTwo::A_ACC, 0, 2, 0), // 0x6A
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x6B
		OPCODE(OP_JMP, &SixtyFiveCeeOhTwo::JMP, ADR_IND, &SixtyFiveCeeOhTwo::A_IND, 2, 5, 0), // 0x6C
		OPCODE(OP_ADC, &SixtyFiveCeeOhTwo::ADC, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0x6D
		OPCODE(OP_ROR, &SixtyFiveCeeOhTwo::ROR, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 6, 0), // 0x6E
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x6F
		OPCODE(OP_BVS, &SixtyFiveCeeOhTwo::BVS, ADR_REL, &SixtyFiveCeeOhTwo::A_REL, 1, 2, 0), // 0x70
		OPCODE(OP_ADC, &SixtyFiveCeeOhTwo::ADC, ADR_INY, &SixtyFiveCeeOhTwo::A_INY, 1, 5, 0), // 0x71
		OPCODE(OP_ADC, &SixtyFiveCeeOhTwo::ADC, ADR_ZPI, &SixtyFiveCeeOhTwo::A_ZPI, 1, 5, 0), // 0x72
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x73
		OPCODE(OP_STZ, &SixtyFiveCeeOhTwo::STZ, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 0), // 0x74
		OPCODE(OP_ADC, &SixtyFiveCeeOhTwo::ADC, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 0), // 0x75
		OPCODE(OP_ROR, &SixtyFiveCeeOhTwo::ROR, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 6, 0), // 0x76
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x77
		OPCODE(OP_SEI, &SixtyFiveCeeOhTwo::SEI, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0x78
		OPCODE(OP_ADC, &SixtyFiveCeeOhTwo::ADC, ADR_ABY, &SixtyFiveCeeOhTwo::A_ABY, 2, 4, 0), // 0x79
		OPCODE(OP_PLY, &SixtyFiveCeeOhTwo::PLY, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 4, 0), // 0x7A
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x7B
		OPCODE(OP_JMP, &SixtyFiveCeeOhTwo::JMP, ADR_AIX, &SixtyFiveCeeOhTwo::A_AIX, 2, 6, 0), // 0x7C
		OPCODE(OP_ADC, &SixtyFiveCeeOhTwo::ADC, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 4, 0), // 0x7D
		OPCODE(OP_ROR, &SixtyFiveCeeOhTwo::ROR, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 6, 0), // 0x7E
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x7F
		OPCODE(OP_BRA, &SixtyFiveCeeOhTwo::BRA, ADR_REL, &SixtyFiveCeeOhTwo::A_REL, 1, 3, 0), // 0x80
		OPCODE(OP_STA, &SixtyFiveCeeOhTwo::STA, ADR_INX, &SixtyFiveCeeOhTwo::A_INX, 1, 6, 1), // 0x81
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x82
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x83
		OPCODE(OP_STY, &SixtyFiveCeeOhTwo::STY, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 1), // 0x84
		OPCODE(OP_STA, &SixtyFiveCeeOhTwo::STA, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 1), // 0x85
		OPCODE(OP_STX, &SixtyFiveCeeOhTwo::STX, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 1), // 0x86
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x87
		OPCODE(OP_DEY, &SixtyFiveCeeOhTwo::DEY, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0x88
		OPCODE(OP_BIT, &SixtyFiveCeeOhTwo::BIT, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0x89
		OPCODE(OP_TXA, &SixtyFiveCeeOhTwo::TXA, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0x8A
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x8B
		OPCODE(OP_STY, &SixtyFiveCeeOhTwo::STY, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 1), // 0x8C
		OPCODE(OP_STA, &SixtyFiveCeeOhTwo::STA, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 1), // 0x8D
		OPCODE(OP_STX, &SixtyFiveCeeOhTwo::STX, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 1), // 0x8E
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x8F
		OPCODE(OP_BCC, &SixtyFiveCeeOhTwo::BCC, ADR_REL, &SixtyFiveCeeOhTwo::A_REL, 1, 2, 0), // 0x90
		OPCODE(OP_STA, &SixtyFiveCeeOhTwo::STA, ADR_INY, &SixtyFiveCeeOhTwo::A_INY, 1, 6, 1), // 0x91
		OPCODE(OP_STA, &SixtyFiveCeeOhTwo::STA, ADR_ZPI, &SixtyFiveCeeOhTwo::A_ZPI, 1, 5, 1), // 0x92
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x93
		OPCODE(OP_STY, &SixtyFiveCeeOhTwo::STY, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 1), // 0x94
		OPCODE(OP_STA, &SixtyFiveCeeOhTwo::STA, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 1), // 0x95
		OPCODE(OP_STX, &SixtyFiveCeeOhTwo::STX, ADR_ZPY, &SixtyFiveCeeOhTwo::A_ZPY, 1, 4, 1), // 0x96
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x97
		OPCODE(OP_TYA, &SixtyFiveCeeOhTwo::TYA, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0x98
		OPCODE(OP_STA, &SixtyFiveCeeOhTwo::STA, ADR_ABY, &SixtyFiveCeeOhTwo::A_ABY, 2, 5, 1), // 0x99
		OPCODE(OP_TXS, &SixtyFiveCeeOhTwo::TXS, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0x9A
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x9B
		OPCODE(OP_STZ, &SixtyFiveCeeOhTwo::STZ, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0x9C
		OPCODE(OP_STA, &SixtyFiveCeeOhTwo::STA, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 5, 1), // 0x9D
		OPCODE(OP_STZ, &SixtyFiveCeeOhTwo::STZ, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 5, 0), // 0x9E
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0x9F
		OPCODE(OP_LDY, &SixtyFiveCeeOhTwo::LDY, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0xA0
		OPCODE(OP_LDA, &SixtyFiveCeeOhTwo::LDA, ADR_INX, &SixtyFiveCeeOhTwo::A_INX, 1, 6, 0), // 0xA1
		OPCODE(OP_LDX, &SixtyFiveCeeOhTwo::LDX, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0xA2
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xA3
		OPCODE(OP_LDY, &SixtyFiveCeeOhTwo::LDY, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0xA4
		OPCODE(OP_LDA, &SixtyFiveCeeOhTwo::LDA, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0xA5
		OPCODE(OP_LDX, &SixtyFiveCeeOhTwo::LDX, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0xA6
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xA7
		OPCODE(OP_TAY, &SixtyFiveCeeOhTwo::TAY, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0xA8
		OPCODE(OP_LDA, &SixtyFiveCeeOhTwo::LDA, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0xA9
		OPCODE(OP_TAX, &SixtyFiveCeeOhTwo::TAX, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0xAA
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xAB
		OPCODE(OP_LDY, &SixtyFiveCeeOhTwo::LDY, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0xAC
		OPCODE(OP_LDA, &SixtyFiveCeeOhTwo::LDA, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0xAD
		OPCODE(OP_LDX, &SixtyFiveCeeOhTwo::LDX, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0xAE
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xAF
		OPCODE(OP_BCS, &SixtyFiveCeeOhTwo::BCS, ADR_REL, &SixtyFiveCeeOhTwo::A_REL, 1, 2, 0), // 0xB0
		OPCODE(OP_LDA, &SixtyFiveCeeOhTwo::LDA, ADR_INY, &SixtyFiveCeeOhTwo::A_INY, 1, 5, 0), // 0xB1
		OPCODE(OP_LDA, &SixtyFiveCeeOhTwo::LDA, ADR_ZPI, &SixtyFiveCeeOhTwo::A_ZPI, 1, 5, 0), // 0xB2
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xB3
		OPCODE(OP_LDY, &SixtyFiveCeeOhTwo::LDY, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 0), // 0xB4
		OPCODE(OP_LDA, &SixtyFiveCeeOhTwo::LDA, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 0), // 0xB5
		OPCODE(OP_LDX, &SixtyFiveCeeOhTwo::LDX, ADR_ZPY, &SixtyFiveCeeOhTwo::A_ZPY, 1, 4, 0), // 0xB6
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xB7
		OPCODE(OP_CLV, &SixtyFiveCeeOhTwo::CLV, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0xB8
		OPCODE(OP_LDA, &SixtyFiveCeeOhTwo::LDA, ADR_ABY, &SixtyFiveCeeOhTwo::A_ABY, 2, 4, 0), // 0xB9
		OPCODE(OP_TSX, &SixtyFiveCeeOhTwo::TSX, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0xBA
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xBB
		OPCODE(OP_LDY, &SixtyFiveCeeOhTwo::LDY, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 4, 0), // 0xBC
		OPCODE(OP_LDA, &SixtyFiveCeeOhTwo::LDA, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 4, 0), // 0xBD
		OPCODE(OP_LDX, &SixtyFiveCeeOhTwo::LDX, ADR_ABY, &SixtyFiveCeeOhTwo::A_ABY, 2, 4, 0), // 0xBE
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xBF
		OPCODE(OP_CPY, &SixtyFiveCeeOhTwo::CPY, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0xC0
		OPCODE(OP_CMP, &SixtyFiveCeeOhTwo::CMP, ADR_INX, &SixtyFiveCeeOhTwo::A_INX, 1, 6, 0), // 0xC1
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xC2
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xC3
		OPCODE(OP_CPY, &SixtyFiveCeeOhTwo::CPY, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0xC4
		OPCODE(OP_CMP, &SixtyFiveCeeOhTwo::CMP, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0xC5
		OPCODE(OP_DEC, &SixtyFiveCeeOhTwo::DEC, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 5, 0), // 0xC6
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xC7
		OPCODE(OP_INY, &SixtyFiveCeeOhTwo::INY, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0xC8
		OPCODE(OP_CMP, &SixtyFiveCeeOhTwo::CMP, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0xC9
		OPCODE(OP_DEX, &SixtyFiveCeeOhTwo::DEX, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0xCA
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xCB
		OPCODE(OP_CPY, &SixtyFiveCeeOhTwo::CPY, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0xCC
		OPCODE(OP_CMP, &SixtyFiveCeeOhTwo::CMP, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0xCD
		OPCODE(OP_DEC, &SixtyFiveCeeOhTwo::DEC, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 3, 0), // 0xCE
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xCF
		OPCODE(OP_BNE, &SixtyFiveCeeOhTwo::BNE, ADR_REL, &SixtyFiveCeeOhTwo::A_REL, 1, 2, 0), // 0xD0
		OPCODE(OP_CMP, &SixtyFiveCeeOhTwo::CMP, ADR_INY, &SixtyFiveCeeOhTwo::A_INY, 1, 5, 0), // 0xD1
		OPCODE(OP_CMP, &SixtyFiveCeeOhTwo::CMP, ADR_ZPI, &SixtyFiveCeeOhTwo::A_ZPI, 1, 5, 0), // 0xD2
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xD3
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xD4
		OPCODE(OP_CMP, &SixtyFiveCeeOhTwo::CMP, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 0), // 0xD5
		OPCODE(OP_DEC, &SixtyFiveCeeOhTwo::DEC, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 6, 0), // 0xD6
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xD7
		OPCODE(OP_CLD, &SixtyFiveCeeOhTwo::CLD, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0xD8
		OPCODE(OP_CMP, &SixtyFiveCeeOhTwo::CMP, ADR_ABY, &SixtyFiveCeeOhTwo::A_ABY, 2, 4, 0), // 0xD9
		OPCODE(OP_PHX, &SixtyFiveCeeOhTwo::PHX, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 3, 0), // 0xDA
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xDB
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xDC
		OPCODE(OP_CMP, &SixtyFiveCeeOhTwo::CMP, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 4, 0), // 0xDD
		OPCODE(OP_DEC, &SixtyFiveCeeOhTwo::DEC, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 7, 0), // 0xDE
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xDF
		OPCODE(OP_CPX, &SixtyFiveCeeOhTwo::CPX, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0xE0
		OPCODE(OP_SBC, &SixtyFiveCeeOhTwo::SBC, ADR_INX, &SixtyFiveCeeOhTwo::A_INX, 1, 6, 0), // 0xE1
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xE2
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xE3
		OPCODE(OP_CPX, &SixtyFiveCeeOhTwo::CPX, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0xE4
		OPCODE(OP_SBC, &SixtyFiveCeeOhTwo::SBC, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 3, 0), // 0xE5
		OPCODE(OP_INC, &SixtyFiveCeeOhTwo::INC, ADR_ZPG, &SixtyFiveCeeOhTwo::A_ZPG, 1, 5, 0), // 0xE6
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xE7
		OPCODE(OP_INX, &SixtyFiveCeeOhTwo::INX, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0xE8
		OPCODE(OP_SBC, &SixtyFiveCeeOhTwo::SBC, ADR_IMM, &SixtyFiveCeeOhTwo::A_IMM, 1, 2, 0), // 0xE9
		OPCODE(OP_NOP, &SixtyFiveCeeOhTwo::NOP, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0xEA
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xEB
		OPCODE(OP_CPX, &SixtyFiveCeeOhTwo::CPX, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0xEC
		OPCODE(OP_SBC, &SixtyFiveCeeOhTwo::SBC, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 4, 0), // 0xED
		OPCODE(OP_INC, &SixtyFiveCeeOhTwo::INC, ADR_ABS, &SixtyFiveCeeOhTwo::A_ABS, 2, 6, 0), // 0xEE
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xEF
		OPCODE(OP_BEQ, &SixtyFiveCeeOhTwo::BEQ, ADR_REL, &SixtyFiveCeeOhTwo::A_REL, 1, 2, 0), // 0xF0
		OPCODE(OP_SBC, &SixtyFiveCeeOhTwo::SBC, ADR_INY, &SixtyFiveCeeOhTwo::A_INY, 1, 5, 0), // 0xF1
		OPCODE(OP_SBC, &SixtyFiveCeeOhTwo::SBC, ADR_ZPI, &SixtyFiveCeeOhTwo::A_ZPI, 1, 5, 0), // 0xF2
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xF3
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xF4
		OPCODE(OP_SBC, &SixtyFiveCeeOhTwo::SBC, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 4, 0), // 0xF5
		OPCODE(OP_INC, &SixtyFiveCeeOhTwo::INC, ADR_ZPX, &SixtyFiveCeeOhTwo::A_ZPX, 1, 6, 0), // 0xF6
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xF7
		OPCODE(OP_SED, &SixtyFiveCeeOhTwo::SED, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 2, 0), // 0xF8
		OPCODE(OP_SBC, &SixtyFiveCeeOhTwo::SBC, ADR_ABY, &SixtyFiveCeeOhTwo::A_ABY, 2, 4, 0), // 0xF9
		OPCODE(OP_PLX, &SixtyFiveCeeOhTwo::PLX, ADR_IMP, &SixtyFiveCeeOhTwo::A_IMP, 0, 4, 0), // 0xFA
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xFB
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xFC
		OPCODE(OP_SBC, &SixtyFiveCeeOhTwo::SBC, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 4, 0), // 0xFD
		OPCODE(OP_INC, &SixtyFiveCeeOhTwo::INC, ADR_ABX, &SixtyFiveCeeOhTwo::A_ABX, 2, 7, 0), // 0xFE
		OPCODE(OP_BAD, &SixtyFiveCeeOhTwo::BAD, ADR_NON, &SixtyFiveCeeOhTwo::A_NON, 0, 0, 0), // 0xFF
	};

	// state variables

	REGISTERS r;
	memoryIntercept *intercepts[65536];
	uint8_t memory[65536];

	uint8_t opcode;

	// arithmatic logic unit
	int32_t alu;
	int32_t old_alu;

	// byte just after opcode
	uint8_t zpage;

	// two bytes just after opcode as a 16 bit word
	uint16_t address;

	// will we write?
	bool writeOK;

	std::map<void *, bool> interruptSources;


	/**
	 * read a byte from an address through the intercepts
	 */
	inline uint8_t readInByte(uint16_t address)
	{
		if (intercepts[address])
			return intercepts[address]->read(address);
		return memory[address];
	}

	/**
	 * read a word (2 bytes in LSB) from an address via readInByte
	 */
	inline uint16_t readInWord(uint16_t address)
	{
		return ((uint16_t)readInByte(address + 1) << 8) | readInByte(address);
	}

	/**
	 * write a byte to an address through the intercepts
	 */
	inline void writeIn(uint16_t address, int value)
	{
		if (intercepts[address])
		{
			intercepts[address]->write(value & 0xFF, address);
		}
		else
			memory[address] = value & 0xFF;
	}




	/**
	 * push cpu state on interrupt
	 */
	void push_cpu_interrupt(uint16_t PC, bool fromInt);

	/**
	 * push a byte to the stack
	 */
	void push_stack(uint8_t b);

	/**
	 * pop a byte from the stack
	 */
	uint8_t pop_stack();


	/**
	 * Addressing modes
	 */
	uint8_t A_NON(); // ADR_NONE
	uint8_t A_IMP(); // ADR_IMPL
	uint8_t A_IMM(); // ADR_IMM
	uint8_t A_ABS(); // ADR_ABS
	uint8_t A_ABX(); // ADR_ABS_X
	uint8_t A_ABY(); // ADR_ABS_Y
	uint8_t A_AIX(); // ADR_AB_IX
	uint8_t A_IND(); // ADR_IND
	uint8_t A_INX(); // ADR_IND_X
	uint8_t A_INY(); // ADR_IND_Y
	uint8_t A_ZPG(); // ADR_ZPG
	uint8_t A_ZPX(); // ADR_ZPG_X
	uint8_t A_ZPY(); // ADR_ZPG_Y
	uint8_t A_ZPI(); // ADR_ZPG_I
	uint8_t A_ACC(); // ADR_A
	uint8_t A_REL(); // ADR_REL


	/**
	 * Opcodes
	 */
	uint8_t BAD();
	uint8_t ADC(); uint8_t AND(); uint8_t ASL();
	uint8_t BCC(); uint8_t BCS(); uint8_t BEQ(); uint8_t BIT(); uint8_t BMI(); uint8_t BNE();
	uint8_t BPL(); uint8_t BRA(); uint8_t BRK(); uint8_t BVC(); uint8_t BVS();
	uint8_t CLC(); uint8_t CLD(); uint8_t CLI(); uint8_t CLV(); uint8_t CMP(); uint8_t CPX();
	uint8_t CPY();
	uint8_t DEC(); uint8_t DEX(); uint8_t DEY();
	uint8_t EOR();
	uint8_t INC(); uint8_t INX(); uint8_t INY();
	uint8_t JMP(); uint8_t JSR();
	uint8_t LDA(); uint8_t LDX(); uint8_t LDY(); uint8_t LSR();
	uint8_t NOP();
	uint8_t ORA();
	uint8_t PHA(); uint8_t PHP(); uint8_t PLA(); uint8_t PLP();
	uint8_t PHX(); uint8_t PLX();
	uint8_t PHY(); uint8_t PLY();
	uint8_t ROL(); uint8_t ROR(); uint8_t RTI(); uint8_t RTS();
	uint8_t SBC(); uint8_t SEC(); uint8_t SED(); uint8_t SEI(); uint8_t STA(); uint8_t STX();
	uint8_t STY(); uint8_t STZ();
	uint8_t TAX(); uint8_t TAY();
	uint8_t TXA(); uint8_t TYA();
	uint8_t TSX(); uint8_t TXS(); 
	uint8_t TRB(); uint8_t TSB();

};

/**
 * romIntercept
 * used to turn a section of memory into "ROM"
 */
struct romIntercept : public memoryIntercept
{
	SixtyFiveCeeOhTwo *cpu;
	romIntercept(){};
	romIntercept(SixtyFiveCeeOhTwo *_cpu) { cpu = _cpu; }
	virtual uint8_t read(uint16_t address)
	{
		return cpu->readByte(address);
	}
	virtual void write(uint8_t value, uint16_t address)
	{
	}
};

#endif
