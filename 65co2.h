#ifndef __65CO2_H__
#define __65CO2_H__



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



struct memoryIntercept {
	memoryIntercept() {}
	virtual unsigned char read(unsigned short address) {return 0;}
	virtual void write(unsigned char value, unsigned short address) {}
};



struct SixtyFiveCeeOhTwo {


	typedef struct __registers {

		unsigned short PC; // program counter
		unsigned short old_PC; // program counter
		int A; // Accumulator
		int X; // X index
		int Y; // Y index
		unsigned char SP; // stack pointer

		bool SR_NEGATIVE;
		bool SR_OVERFLOW;
		bool SR_UNUSED;
		bool SR_BREAK;
		bool SR_DECIMAL;
		bool SR_INTERRUPT;
		bool SR_ZERO;
		bool SR_CARRY;

		inline void SRsetByte(unsigned char b) {
			SR_NEGATIVE = (b & SR_NEGATIVE_B);
			SR_OVERFLOW = (b & SR_OVERFLOW_B);
			SR_UNUSED = true; //(b & SR_UNUSED_B);
			SR_BREAK = (b & SR_BREAK_B);
			SR_DECIMAL = (b & SR_DECIMAL_B);
			SR_INTERRUPT = (b & SR_INTERRUPT_B);
			SR_ZERO = (b & SR_ZERO_B);
			SR_CARRY = (b & SR_CARRY_B);
		}
		inline unsigned char SRgetByte() {
			return
				(SR_NEGATIVE ? SR_NEGATIVE_B : 0) |
				(SR_OVERFLOW ? SR_OVERFLOW_B : 0) |
				SR_UNUSED_B |
				(SR_BREAK ? SR_BREAK_B : 0) |
				(SR_DECIMAL ? SR_DECIMAL_B : 0) |
				(SR_INTERRUPT ? SR_INTERRUPT_B : 0) |
				(SR_ZERO ? SR_ZERO_B : 0) |
				(SR_CARRY ? SR_CARRY_B : 0); 
		}

		inline void SRsetNZ(unsigned char v) {
			SR_NEGATIVE = (v & SR_NEGATIVE_B) == SR_NEGATIVE_B;
			SR_ZERO = (v == 0x00);
		}

		inline void SRborrow(unsigned short alu) {
			SR_CARRY = (alu & 0x100) == 0;

		}

		inline void SRcarry(unsigned short alu) {
			SR_CARRY = (alu & 0x100) != 0;
		}

	} REGISTERS;

	REGISTERS r;

	// addressing modes
	enum eADDRESSING{
		ADR_NONE = 0,
		ADR_IMPL,
		ADR_IMM,
		ADR_ABS,
		ADR_ABS_X,
		ADR_ABS_Y,
		ADR_IND,
		ADR_AB_IX,
		ADR_IND_X,
		ADR_IND_Y,
		ADR_ZPG,
		ADR_ZPG_X,
		ADR_ZPG_Y,
		ADR_ZPG_I,
		ADR_A,
		ADR_REL
	};

	// addressing mode strings
	char sMODES[15][6] = {
		"NONE ",
		"IMPL ",
		"IMM  ",
		"ABS  ",
		"ABS,X",
		"ABS,Y",
		"IND  ",
		"X,IND",
		"IND,Y",
		"ZPG  ",
		"ZPG,X",
		"ZPG,Y",
		"A    ",
		"REL  "
	};

	enum eOPCODES{
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
		OP_TSB  // test and set bits
	} ;

	// name
	char nmem[65][4] = {
		"BAD",
		"ADC",
		"AND",
		"ASL",
		"BCC",
		"BCS",
		"BEQ",
		"BIT",
		"BMI",
		"BNE",
		"BPL",
		"BRA",
		"BRK",
		"BVC",
		"BVS",
		"CLC",
		"CLD",
		"CLI",
		"CLV",
		"CMP",
		"CPX",
		"CPY",
		"DEC",
		"DEX",
		"DEY",
		"EOR",
		"INC",
		"INX",
		"INY",
		"JMP",
		"JSR",
		"LDA",
		"LDX",
		"LDY",
		"LSR",
		"NOP",
		"ORA",
		"PHA",
		"PHP",
		"PLA",
		"PLP",
		"PHX",
		"PHY",
		"PLX",
		"PLY",
		"ROL",
		"ROR",
		"RTI",
		"RTS",
		"SBC",
		"SEC",
		"SED",
		"SEI",
		"STA",
		"STX",
		"STY",
		"STZ",
		"TAX",
		"TAY",
		"TSX",
		"TXA",
		"TXS",
		"TYA",
		"TRB",
		"TSB"
	};

	// opcode attributes
	typedef struct __OPCODES {
		eOPCODES op; // the op enum
		eADDRESSING mode; // addressing mode
		unsigned char step; // PC steps
		unsigned char cycles; // minimum cycles
		unsigned char rw; // write mode
	} OPCODE;

	// opcode attribute table

	OPCODE OPCODES[256] = {
		{ OP_BRK,  ADR_IMPL,  0, 7, 0},  // 0x00
		{ OP_ORA,  ADR_IND_X, 1, 6, 0},  // 0x01
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x02
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x03
		{ OP_TSB,  ADR_ZPG,   1, 5, 0},  // 0x04
		{ OP_ORA,  ADR_ZPG,   1, 3, 0},  // 0x05
		{ OP_ASL,  ADR_ZPG,   1, 5, 0},  // 0x06
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x07
		{ OP_PHP,  ADR_IMPL,  0, 3, 0},  // 0x08
		{ OP_ORA,  ADR_IMM,   1, 2, 0},  // 0x09
		{ OP_ASL,  ADR_A,     0, 2, 0},  // 0x0A
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x0B
		{ OP_TSB,  ADR_ABS,   2, 6, 0},  // 0x0C
		{ OP_ORA,  ADR_ABS,   2, 4, 0},  // 0x0D
		{ OP_ASL,  ADR_ABS,   2, 6, 0},  // 0x0E
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x0F
		{ OP_BPL,  ADR_REL,   1, 2, 0},  // 0x10
		{ OP_ORA,  ADR_IND_Y, 1, 5, 0},  // 0x11
		{ OP_ORA,  ADR_ZPG_I, 1, 5, 0},  // 0x12
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x13
		{ OP_TRB,  ADR_ZPG,   1, 5, 0},  // 0x14
		{ OP_ORA,  ADR_ZPG_X, 1, 4, 0},  // 0x15
		{ OP_ASL,  ADR_ZPG_X, 1, 6, 0},  // 0x16
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x17
		{ OP_CLC,  ADR_IMPL,  0, 2, 0},  // 0x18
		{ OP_ORA,  ADR_ABS_Y, 2, 4, 0},  // 0x19
		{ OP_DEC,  ADR_A,     0, 2, 0},  // 0x1A
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x1B
		{ OP_TRB,  ADR_ABS,   2, 6, 0},  // 0x1C
		{ OP_ORA,  ADR_ABS_X, 2, 4, 0},  // 0x1D
		{ OP_ASL,  ADR_ABS_X, 2, 6, 0},  // 0x1E
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x1F
		{ OP_JSR,  ADR_ABS,   2, 6, 0},  // 0x20
		{ OP_AND,  ADR_IND_X, 1, 6, 0},  // 0x21
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x22
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x23
		{ OP_BIT,  ADR_ZPG,   1, 3, 0},  // 0x24
		{ OP_AND,  ADR_ZPG,   1, 3, 0},  // 0x25
		{ OP_ROL,  ADR_ZPG,   1, 5, 0},  // 0x26
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x27
		{ OP_PLP,  ADR_IMPL,  0, 4, 0},  // 0x28
		{ OP_AND,  ADR_IMM,   1, 2, 0},  // 0x29
		{ OP_ROL,  ADR_A,     0, 2, 0},  // 0x2A
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x2B
		{ OP_BIT,  ADR_ABS,   2, 4, 0},  // 0x2C
		{ OP_AND,  ADR_ABS,   2, 4, 0},  // 0x2D
		{ OP_ROL,  ADR_ABS,   2, 6, 0},  // 0x2E
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x2F
		{ OP_BMI,  ADR_REL,   1, 2, 0},  // 0x30
		{ OP_AND,  ADR_IND_Y, 1, 5, 0},  // 0x31
		{ OP_AND,  ADR_ZPG_I, 1, 5, 0},  // 0x32
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x33
		{ OP_BIT,  ADR_ZPG_X, 1, 4, 0},  // 0x34
		{ OP_AND,  ADR_ZPG_X, 1, 4, 0},  // 0x35
		{ OP_ROL,  ADR_ZPG_X, 1, 6, 0},  // 0x36
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x37
		{ OP_SEC,  ADR_IMPL,  0, 2, 0},  // 0x38
		{ OP_AND,  ADR_ABS_Y, 2, 4, 0},  // 0x39
		{ OP_INC,  ADR_A,     0, 2, 0},  // 0x3A
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x3B
		{ OP_BIT,  ADR_ABS_X, 2, 4, 0},  // 0x3C
		{ OP_AND,  ADR_ABS_X, 2, 4, 0},  // 0x3D
		{ OP_ROL,  ADR_ABS_X, 2, 6, 0},  // 0x3E
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x3F
		{ OP_RTI,  ADR_IMPL,  0, 6, 0},  // 0x40
		{ OP_EOR,  ADR_IND_X, 1, 6, 0},  // 0x41
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x42
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x43
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x44
		{ OP_EOR,  ADR_ZPG,   1, 3, 0},  // 0x45
		{ OP_LSR,  ADR_ZPG,   1, 5, 0},  // 0x46
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x47
		{ OP_PHA,  ADR_IMPL,  0, 3, 0},  // 0x48
		{ OP_EOR,  ADR_IMM,   1, 2, 0},  // 0x49
		{ OP_LSR,  ADR_A,     0, 2, 0},  // 0x4A
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x4B
		{ OP_JMP,  ADR_ABS,   2, 3, 0},  // 0x4C
		{ OP_EOR,  ADR_ABS,   2, 4, 0},  // 0x4D
		{ OP_LSR,  ADR_ABS,   2, 6, 0},  // 0x4E
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x4F
		{ OP_BVC,  ADR_REL,   1, 2, 0},  // 0x50
		{ OP_EOR,  ADR_IND_Y, 1, 5, 0},  // 0x51
		{ OP_EOR,  ADR_ZPG_I, 1, 5, 0},  // 0x52
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x53
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x54
		{ OP_EOR,  ADR_ZPG_X, 1, 4, 0},  // 0x55
		{ OP_LSR,  ADR_ZPG_X, 1, 6, 0},  // 0x56
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x57
		{ OP_CLI,  ADR_IMPL,  0, 2, 0},  // 0x58
		{ OP_EOR,  ADR_ABS_Y, 2, 4, 0},  // 0x59
		{ OP_PHY,  ADR_IMPL,  0, 3, 0},  // 0x5A
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x5B
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x5C
		{ OP_EOR,  ADR_ABS_X, 2, 4, 0},  // 0x5D
		{ OP_LSR,  ADR_ABS_X, 2, 6, 0},  // 0x5E
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x5F
		{ OP_RTS,  ADR_IMPL,  0, 6, 0},  // 0x60
		{ OP_ADC,  ADR_IND_X, 1, 6, 0},  // 0x61
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x62
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x63
		{ OP_STZ,  ADR_ZPG,   1, 3, 0},  // 0x64
		{ OP_ADC,  ADR_ZPG,   1, 3, 0},  // 0x65
		{ OP_ROR,  ADR_ZPG,   1, 5, 0},  // 0x66
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x67
		{ OP_PLA,  ADR_A,     0, 4, 1},  // 0x68
		{ OP_ADC,  ADR_IMM,   1, 2, 0},  // 0x69
		{ OP_ROR,  ADR_A,     0, 2, 0},  // 0x6A
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x6B
		{ OP_JMP,  ADR_IND,   2, 5, 0},  // 0x6C
		{ OP_ADC,  ADR_ABS,   2, 4, 0},  // 0x6D
		{ OP_ROR,  ADR_ABS,   2, 6, 0},  // 0x6E
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x6F
		{ OP_BVS,  ADR_REL,   1, 2, 0},  // 0x70
		{ OP_ADC,  ADR_IND_Y, 1, 5, 0},  // 0x71
		{ OP_ADC,  ADR_ZPG_I, 1, 5, 0},  // 0x72
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x73
		{ OP_STZ,  ADR_ZPG_X, 1, 4, 0},  // 0x74
		{ OP_ADC,  ADR_ZPG_X, 1, 4, 0},  // 0x75
		{ OP_ROR,  ADR_ZPG_X, 1, 6, 0},  // 0x76
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x77
		{ OP_SEI,  ADR_IMPL,  0, 2, 0},  // 0x78
		{ OP_ADC,  ADR_ABS_Y, 2, 4, 0},  // 0x79
		{ OP_PLY,  ADR_IMPL,  0, 4, 0},  // 0x7A
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x7B
		{ OP_JMP,  ADR_AB_IX, 2, 6, 0},  // 0x7C
		{ OP_ADC,  ADR_ABS_X, 2, 4, 0},  // 0x7D
		{ OP_ROR,  ADR_ABS_X, 2, 6, 0},  // 0x7E
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x7F
		{ OP_BRA,  ADR_REL,   1, 3, 0},  // 0x80
		{ OP_STA,  ADR_IND_X, 1, 6, 1},  // 0x81
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x82
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x83
		{ OP_STY,  ADR_ZPG,   1, 3, 1},  // 0x84
		{ OP_STA,  ADR_ZPG,   1, 3, 1},  // 0x85
		{ OP_STX,  ADR_ZPG,   1, 3, 1},  // 0x86
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x87
		{ OP_DEY,  ADR_IMPL,  0, 2, 0},  // 0x88
		{ OP_BIT,  ADR_IMM,   1, 2, 0},  // 0x89
		{ OP_TXA,  ADR_IMPL,  0, 2, 0},  // 0x8A
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x8B
		{ OP_STY,  ADR_ABS,   2, 4, 1},  // 0x8C
		{ OP_STA,  ADR_ABS,   2, 4, 1},  // 0x8D
		{ OP_STX,  ADR_ABS,   2, 4, 1},  // 0x8E
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x8F
		{ OP_BCC,  ADR_REL,   1, 2, 0},  // 0x90
		{ OP_STA,  ADR_IND_Y, 1, 6, 1},  // 0x91
		{ OP_STA,  ADR_ZPG_I, 1, 5, 1},  // 0x92
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x93
		{ OP_STY,  ADR_ZPG_X, 1, 4, 1},  // 0x94
		{ OP_STA,  ADR_ZPG_X, 1, 4, 1},  // 0x95
		{ OP_STX,  ADR_ZPG_Y, 1, 4, 1},  // 0x96
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x97
		{ OP_TYA,  ADR_IMPL,  0, 2, 0},  // 0x98
		{ OP_STA,  ADR_ABS_Y, 2, 5, 1},  // 0x99
		{ OP_TXS,  ADR_IMPL,  0, 2, 0},  // 0x9A
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x9B
		{ OP_STZ,  ADR_ABS,   2, 4, 0},  // 0x9C
		{ OP_STA,  ADR_ABS_X, 2, 5, 1},  // 0x9D
		{ OP_STZ,  ADR_ABS_X, 2, 5, 0},  // 0x9E
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0x9F
		{ OP_LDY,  ADR_IMM,   1, 2, 0},  // 0xA0
		{ OP_LDA,  ADR_IND_X, 1, 6, 0},  // 0xA1
		{ OP_LDX,  ADR_IMM,   1, 2, 0},  // 0xA2
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xA3
		{ OP_LDY,  ADR_ZPG,   1, 3, 0},  // 0xA4
		{ OP_LDA,  ADR_ZPG,   1, 3, 0},  // 0xA5
		{ OP_LDX,  ADR_ZPG,   1, 3, 0},  // 0xA6
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xA7
		{ OP_TAY,  ADR_IMPL,  0, 2, 0},  // 0xA8
		{ OP_LDA,  ADR_IMM,   1, 2, 0},  // 0xA9
		{ OP_TAX,  ADR_IMPL,  0, 2, 0},  // 0xAA
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xAB
		{ OP_LDY,  ADR_ABS,   2, 4, 0},  // 0xAC
		{ OP_LDA,  ADR_ABS,   2, 4, 0},  // 0xAD
		{ OP_LDX,  ADR_ABS,   2, 4, 0},  // 0xAE
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xAF
		{ OP_BCS,  ADR_REL,   1, 2, 0},  // 0xB0
		{ OP_LDA,  ADR_IND_Y, 1, 5, 0},  // 0xB1
		{ OP_LDA,  ADR_ZPG_I, 1, 5, 0},  // 0xB2
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xB3
		{ OP_LDY,  ADR_ZPG_X, 1, 4, 0},  // 0xB4
		{ OP_LDA,  ADR_ZPG_X, 1, 4, 0},  // 0xB5
		{ OP_LDX,  ADR_ZPG_Y, 1, 4, 0},  // 0xB6
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xB7
		{ OP_CLV,  ADR_IMPL,  0, 2, 0},  // 0xB8
		{ OP_LDA,  ADR_ABS_Y, 2, 4, 0},  // 0xB9
		{ OP_TSX,  ADR_IMPL,  0, 2, 0},  // 0xBA
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xBB
		{ OP_LDY,  ADR_ABS_X, 2, 4, 0},  // 0xBC
		{ OP_LDA,  ADR_ABS_X, 2, 4, 0},  // 0xBD
		{ OP_LDX,  ADR_ABS_Y, 2, 4, 0},  // 0xBE
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xBF
		{ OP_CPY,  ADR_IMM,   1, 2, 0},  // 0xC0
		{ OP_CMP,  ADR_IND_X, 1, 6, 0},  // 0xC1
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xC2
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xC3
		{ OP_CPY,  ADR_ZPG,   1, 3, 0},  // 0xC4
		{ OP_CMP,  ADR_ZPG,   1, 3, 0},  // 0xC5
		{ OP_DEC,  ADR_ZPG,   1, 5, 0},  // 0xC6
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xC7
		{ OP_INY,  ADR_IMPL,  0, 2, 0},  // 0xC8
		{ OP_CMP,  ADR_IMM,   1, 2, 0},  // 0xC9
		{ OP_DEX,  ADR_IMPL,  0, 2, 0},  // 0xCA
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xCB
		{ OP_CPY,  ADR_ABS,   2, 4, 0},  // 0xCC
		{ OP_CMP,  ADR_ABS,   2, 4, 0},  // 0xCD
		{ OP_DEC,  ADR_ABS,   2, 3, 0},  // 0xCE
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xCF
		{ OP_BNE,  ADR_REL,   1, 2, 0},  // 0xD0
		{ OP_CMP,  ADR_IND_Y, 1, 5, 0},  // 0xD1
		{ OP_CMP,  ADR_ZPG_I, 1, 5, 0},  // 0xD2
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xD3
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xD4
		{ OP_CMP,  ADR_ZPG_X, 1, 4, 0},  // 0xD5
		{ OP_DEC,  ADR_ZPG_X, 1, 6, 0},  // 0xD6
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xD7
		{ OP_CLD,  ADR_IMPL,  0, 2, 0},  // 0xD8
		{ OP_CMP,  ADR_ABS_Y, 2, 4, 0},  // 0xD9
		{ OP_PHX,  ADR_IMPL,  0, 3, 0},  // 0xDA
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xDB
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xDC
		{ OP_CMP,  ADR_ABS_X, 2, 4, 0},  // 0xDD
		{ OP_DEC,  ADR_ABS_X, 2, 7, 0},  // 0xDE
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xDF
		{ OP_CPX,  ADR_IMM,   1, 2, 0},  // 0xE0
		{ OP_SBC,  ADR_IND_X, 1, 6, 0},  // 0xE1
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xE2
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xE3
		{ OP_CPX,  ADR_ZPG,   1, 3, 0},  // 0xE4
		{ OP_SBC,  ADR_ZPG,   1, 3, 0},  // 0xE5
		{ OP_INC,  ADR_ZPG,   1, 5, 0},  // 0xE6
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xE7
		{ OP_INX,  ADR_IMPL,  0, 2, 0},  // 0xE8
		{ OP_SBC,  ADR_IMM,   1, 2, 0},  // 0xE9
		{ OP_NOP,  ADR_IMPL,  0, 2, 0},  // 0xEA
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xEB
		{ OP_CPX,  ADR_ABS,   2, 4, 0},  // 0xEC
		{ OP_SBC,  ADR_ABS,   2, 4, 0},  // 0xED
		{ OP_INC,  ADR_ABS,   2, 6, 0},  // 0xEE
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xEF
		{ OP_BEQ,  ADR_REL,   1, 2, 0},  // 0xF0
		{ OP_SBC,  ADR_IND_Y, 1, 5, 0},  // 0xF1
		{ OP_SBC,  ADR_ZPG_I, 1, 5, 0},  // 0xF2
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xF3
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xF4
		{ OP_SBC,  ADR_ZPG_X, 1, 4, 0},  // 0xF5
		{ OP_INC,  ADR_ZPG_X, 1, 6, 0},  // 0xF6
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xF7
		{ OP_SED,  ADR_IMPL,  0, 2, 0},  // 0xF8
		{ OP_SBC,  ADR_ABS_Y, 2, 4, 0},  // 0xF9
		{ OP_PLX,  ADR_IMPL,  0, 4, 0},  // 0xFA
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xFB
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xFC
		{ OP_SBC,  ADR_ABS_X, 2, 4, 0},  // 0xFD
		{ OP_INC,  ADR_ABS_X, 2, 7, 0},  // 0xFE
		{ OP_BAD,  ADR_NONE,  0, 0, 0},  // 0xFF
	};



	// state variables


	unsigned char memory[65536];

	memoryIntercept *intercepts[65536];

	unsigned char opcode;

	// SR before opcode
	unsigned char SR_before;

	// arithmatic logic unit
	int alu;
	int old_alu;

	// byte just after opcode
	unsigned char zpage;

	// two bytes just after opcode as a 16 bit word
	unsigned short address;

	// will we write?
	bool write;

	// used for debugging
	bool paused;


	// are maskable interrupts active
	bool interrupts_active;
	bool interrupted;





	void addIntercept(unsigned short address, memoryIntercept *i) {
		intercepts[address] = i;
	}

	void addInterceptRange(unsigned short start, unsigned short len, memoryIntercept *i) {
		for(int a = start; a < start + len; a++)
			if(a < 65536)
				intercepts[a] = i;
	}

	SixtyFiveCeeOhTwo() {
		paused = false;

		for(int i = 0; i < 65536; i++)
			intercepts[i] = NULL;
	};


	/**
	 * read a byte from an address through the intercepts
	 */
	unsigned char readMemoryByte(unsigned short address) {
		if(intercepts[address] != NULL) 
			return intercepts[address]->read(address);
		return memory[address];
	}

	/**
	 * read a word (2 bytes in LSB) from an address via readMemoryByte
	 */
	unsigned short readMemoryWord(unsigned short address) { 
		return ((unsigned short)readMemoryByte(address + 1) << 8) | readMemoryByte(address);
	}

	/**
	 * write a byte to an address through the intercepts
	 */
	void writeMemory(unsigned short address, int value) {
		if(intercepts[address] != NULL) {
			intercepts[address]->write(value & 0xFF, address);
		} else
			memory[address] = value;
	}

	/**
	 * reset the cpu
	 * sets A, X, Y and SR to 0, sets SP to 0xFD and PC to whever the reset vetor points to
	 * cause reset is actually a BRK with the stack pushes read only, therefor it starts
	 * SP at 0, then over the course of attemptying to fake-push PC and SR to the stack
	 * it decrements SP 3 times to 0xFD
	 */
	void reset_cpu() {
		r.SRsetByte(0);
		r.SR_INTERRUPT = true;
		r.SR_BREAK = true;
		r.SP = 0xFD; 
		r.PC = readMemoryWord(RESET_VECTOR);
		r.A = 0;
		r.X = 0;
		r.Y = 0;
	}

	/**
	 * Interrupt the pc, maskable
	 */
	void interrupt_cpu() {
		if(interrupts_active) {
			push_stack((unsigned char)((r.PC & 0xFF00) >> 8));
			push_stack((unsigned char)(r.PC & 0xFF));
			push_stack(r.SRgetByte());
			r.PC = readMemoryWord(IRQBRK_VECTOR);
			interrupted = true;
		}
	}

	/**
	 * non maskable interrupt
	 */
	void nmi_interrupt_cpu() {
		push_stack((unsigned char)((r.PC & 0xFF00) >> 8));
		push_stack((unsigned char)(r.PC & 0xFF));
		push_stack(r.SRgetByte());
		r.PC = readMemoryWord(NMI_VECTOR);
		interrupted = true;
	}

	/**
	 * push a byte to the stack
	 */
	void push_stack(unsigned char b) {
		writeMemory(0x100 + r.SP,b);
		r.SP--;
		// it should loop back to 255 if poping the stack at 0
	}

	/**
	 * pop a byte from the stack
	 */
	unsigned char pop_stack() {
		r.SP++;
		unsigned char v = readMemoryByte(0x100 + r.SP);
		// it's supposed to overflow to 0 and stay on the stack page
		return v;
	}


	/**
	 * do next operation
	 * @return unsigned char number of cycles operation took
	 */
	unsigned char do_op() {	


		// save program counter
		r.old_PC = r.PC;
		// save SR;
		SR_before = r.SRgetByte();

		// get opcode at PC
		opcode = readMemoryByte(r.PC);
		// point to first data byte
		r.PC += 1;

		// starting point for cycle count for this opcode. might increase
		unsigned char cycles = OPCODES[opcode].cycles;
		int o1,o2;
		unsigned short page;



		// reset ALU to 0
		alu = 0;
		zpage = 0;
		address = 0;

		eADDRESSING addressingMode = OPCODES[opcode].mode;


		write = OPCODES[opcode].rw;

		switch (addressingMode) {

			case ADR_NONE:
			case ADR_IMPL:
			case ADR_A:
			case ADR_IMM:
				address = 0;
				break;

			case ADR_ABS:
				address = readMemoryWord(r.PC);
				break;

			case ADR_ABS_X:
				// look for page crossings
				address = readMemoryWord(r.PC);
				page = address & 0xFF00;
				// apparently we need to read the address before
				// adding X
				readMemoryWord(address);
				address += r.X;
				if((address & 0xFF00) > page)
					cycles++;
				// fetch
				break;
			case ADR_ABS_Y:
				// look for page crossings
				address = readMemoryWord(r.PC);
				page = address & 0xFF00;
				// apparently we need to read the address before
				// adding X
				readMemoryWord(address);
				address += r.Y;
				if((address & 0xFF00) > page)
					cycles++;
				break;

				// used by jmp, so doesn't set alu, since absolute mode for jmp is address
			case ADR_IND:
				// nmos bug
				// if indirect addressing points to last byte in page
				// then next byte for address is first byte in same page
				// and not first byte from next page
				o1 = readMemoryByte(r.PC);
				o2 = readMemoryByte(r.PC + 1) << 8;
				address = readMemoryByte(o2 + o1);
				//o1 = (o1 + 1) & 0xFF;
				//
				//the c doesn't have this bug
				o1++;
				address += readMemoryByte(o2 + o1) << 8;
				break;

			case ADR_AB_IX:
				o1 = readMemoryByte(r.PC);
				o2 = readMemoryByte(r.PC + 1) << 8;

				o1 += r.X; // add X to the address specified

				address = readMemoryByte(o2 + o1);
				o1++;
				address += readMemoryByte(o2 + o1) << 8;


				break;

			case ADR_ZPG:
				zpage = readMemoryByte(r.PC);
				address = zpage;
				break;
			case ADR_ZPG_X:
				zpage = readMemoryByte(r.PC);
				address = (zpage + r.X) & 0xFF;
				// address is zpage + X without carry
				break;
			case ADR_ZPG_Y:
				zpage = readMemoryByte(r.PC);
				// address is zpage + Y without carry
				address = (zpage + r.Y) & 0xFF;
				break;
			case ADR_ZPG_I:
				zpage = readMemoryByte(r.PC);
				o1 = readMemoryByte(zpage);
				o2 = readMemoryByte(zpage + 1) << 8;
				address = o1 + o2;
				break;

			case ADR_IND_X:
				zpage = readMemoryByte(r.PC);
				// address to look up is ZP + X without carry
				o1 = (zpage + r.X) & 0xFF; // is this right?
				address = readMemoryByte(o1);
				address += (readMemoryByte((o1 + 1) & 0xFF)) << 8;
				break;
			case ADR_IND_Y:
				zpage = readMemoryByte(r.PC);
				// IAH wraps if zpage = 0xFF
				o1 = readMemoryByte(zpage) + r.Y;
				o2 = readMemoryByte((zpage + 1) & 0xFF) << 8;
				// now add Y, look for page boundries
				if(o1 > 0x0100)
					cycles++;
				address = o2 + o1;
				break;

			case ADR_REL:
				address = r.PC;
				page = address >> 8;
				alu = readMemoryByte(r.PC);
				zpage = alu;
				if(alu >= 0x80) {
					// 2's compliment
					alu = -(256 - alu);
				}
				address += (OPCODES[opcode].step + alu) & 0xFFFF;
				// reusing alu as the cycle bump if we branch
				alu = 1;
				if(address >> 8 != page)
					alu = 2;
				break;

		}
		


		// fetch alu from where we need to fetch it
		if(!write) {
			switch (addressingMode) {

				case ADR_NONE:
				case ADR_IND:
				case ADR_REL:
				case ADR_IMPL:
					break;

				case ADR_A:
					alu = r.A;
					break;

				case ADR_IMM:
					zpage = readMemoryByte(r.PC);
					alu = zpage;
					break;

				case ADR_ABS:
				case ADR_ABS_X:
				case ADR_ABS_Y:
				case ADR_AB_IX:
				case ADR_ZPG_I:
				case ADR_ZPG:
				case ADR_ZPG_X:
				case ADR_ZPG_Y:
				case ADR_IND_X:
				case ADR_IND_Y:
					alu = readMemoryByte(address);
					break;


			}
		}

		old_alu = alu;

		// move PC now
		r.PC += OPCODES[opcode].step;


		// opcode contemplation
		switch (OPCODES[opcode].op) {
			// illegal opcodes are skipped, even the dangerous ones
			case OP_BAD:
				break;

			case OP_NOP:  // no operation
				break;


				// add and subtract
			case OP_ADC:  // add with carry
				o1 = r.A;
				o2 = alu;
				if(r.SR_DECIMAL) {


					// decimal mode behavior following Marko Makela's explanations
					r.SR_ZERO = ((o1 + o2 + (r.SR_CARRY ? 1 : 0)) & 0xFF) == 0;

					// add low nybs of A and alu with carry
					alu = (o1 & 0x0F) + (o2 & 0x0F) + (r.SR_CARRY ? 1 : 0);

					// if alu > 10 then alu += 6
					r.A = alu < 0x0A ? alu : alu + 6;

					// add high nybs of A, input alu and overflow from low nyb
					alu = (o1 & 0xF0) + (o2 & 0xF0) + (r.A & 0xF0);

					r.SR_NEGATIVE = ((char) alu) < 0;

					r.SR_OVERFLOW = ((o1 & 0x80) == (o2 & 0x80)) && ((o1 & 0x80) != (alu & 0x80));

					// redo alu for output, merge low nyb from r.A
					// and high nyb from alu, overflow if alu > 160 (100)
					alu = (r.A & 0x0F) | (alu < 0xA0 ? alu : alu + 0x60);

					r.SRcarry(alu);

					r.A = alu & 0xFF;

					cycles++;

				} else {
					// A + M + C -> A
					alu = o1 + o2 + (r.SR_CARRY?1:0);  

					// clear flags
					// set them
					r.SRcarry(alu);

					// the test wants this?
					r.SR_OVERFLOW = ((o1 & 0x80) == (o2 & 0x80)) && ((o1 & 0x80) != (alu & 0x80));


					// update A
					r.A = alu & 0xFF;

					r.SRsetNZ(r.A);


				}
				break;
			case OP_SBC:  // subtract with carry
				o1 = r.A;
				o2 = alu;
				if(r.SR_DECIMAL) {

					// decimal mode behavior following Marko Makela's explanations
					alu = (o1 & 0x0F) - (o2 & 0x0F) - (r.SR_CARRY ? 0 : 1);
					r.A = (alu & 0x10) == 0 ? alu : alu - 6;
					alu = (o1 & 0xF0) - (o2 & 0xF0) - (r.A & 0x10);
					r.A = (r.A & 0x0F) | ((alu & 0x100) == 0 ? alu : alu - 0x60);
					alu = o1 - o2 - (r.SR_CARRY ? 0 : 1);

					r.SRsetNZ(r.A);
					r.SRborrow(alu);

					cycles++;

				} else {
					// A + M + C -> A
					alu = o1 - o2 - (r.SR_CARRY?0:1);  

					// store A
					r.A = alu & 0xFF;

					// set carry, set others
					// oddly, this is what's needed here, even tho it's different than ADC's overflow
					r.SR_OVERFLOW = ((o1 ^ o2) & (o1 ^ r.A) & 0x80) != 0;
					r.SRsetNZ(r.A);
					r.SRborrow(alu);

				}
				break;


				// compare memory with...
			case OP_CMP:  // compare with A
				alu = (r.A - alu);
				r.SRborrow(alu);
				r.SRsetNZ(alu);
				break;
			case OP_CPX:  // compare with X
				alu = (r.X - alu);
				r.SRborrow(alu);
				r.SRsetNZ(alu);
				break;
			case OP_CPY:  // compare with Y
				alu = (r.Y - alu);
				r.SRborrow(alu);
				r.SRsetNZ(alu);
				break;

				// logical operators
			case OP_AND:  // and (with accumulator)
				// apply alu to A
				r.A &= alu;
				r.SRsetNZ(r.A);
				break;
			case OP_ORA:  // or with accumulator
				// apply alu to A
				r.A |= alu;
				r.SRsetNZ(r.A);
				break;
			case OP_EOR:  // exclusive or (with accumulator)
				// apply alu to A
				r.A ^= alu;
				r.SRsetNZ(r.A);
				break;


			case OP_ASL:  // arithmetic shift left
				// catch carry
				r.SR_CARRY = (alu & 0x80) != 0;
				// a jump, to the left!
				alu <<= 1;
				r.SRsetNZ(alu);
				write = 1;
				break;
			case OP_LSR:  // logical shift right
				r.SR_CARRY = (alu & 0x01) != 0;
				// and a step, to the right!
				alu >>= 1;
				// set flags
				r.SRsetNZ(alu);
				write = 1;
				break;
			case OP_ROL:  // rotate left
				// rotate left and mix in carry
				o1 = alu & 0x80;
				alu <<= 1;
			    if(r.SR_CARRY)
					alu |= 0x01;
				r.SR_CARRY = o1 != 0;
				// set flags
				r.SRsetNZ(alu);
				write = 1;
				break;
			case OP_ROR:  // rotate right
				o1 = alu & 0x01;
				alu >>= 1;
				if(r.SR_CARRY)
					alu |= 0x80;
				// set flags
				r.SR_CARRY = o1 == 0x01;
				r.SRsetNZ(alu);
				write = 1;
				break;


				// Increment, decrement A, X, Y
			case OP_INC:  // increment
				alu = (alu + 1) & 0xFF;
				r.SRsetNZ(alu);
				write = 1;
				break;
			case OP_DEC:  // decrement
				alu = (alu - 1) & 0xFF;
				r.SRsetNZ(alu);
				write = 1;
				break;

			case OP_INX:  // increment X
				r.X = (r.X + 1) & 0xFF;
				r.SRsetNZ(r.X);
				break;
			case OP_INY:  // increment Y
				r.Y = (r.Y + 1) & 0xFF;
				r.SRsetNZ(r.Y);
				break;

			case OP_DEX:  // decrement X
				r.X = (r.X - 1) & 0xFF;
				r.SRsetNZ(r.X);
				break;
			case OP_DEY:  // decrement Y
				r.Y = (r.Y - 1) & 0xFF;
				r.SRsetNZ(r.Y);
				break;

			case OP_BIT:  // bit test
				// does a non-destructive AND vs A and memory
				// sets Z if there's no matches
				// sets N to bit 7
				// sets V to bit 6
				if(addressingMode != ADR_IMM)
					r.SR_OVERFLOW = (alu & SR_OVERFLOW_B); // bit 6
				r.SR_NEGATIVE = (alu & SR_NEGATIVE_B); // bit 7
				r.SR_ZERO = (alu & r.A) == 0;
				break;




				// flag clear and set
			case OP_CLC:  // clear carry
				r.SR_CARRY = false;
				break;
			case OP_SEC:  // set carry
				r.SR_CARRY = true;
				break;
			case OP_CLD:  // clear decimal
				r.SR_DECIMAL = false;
				break;
			case OP_SED:  // set decimal
				r.SR_DECIMAL = true;
				break;
			case OP_CLI:  // clear interrupt disable
				r.SR_INTERRUPT = false;
				interrupts_active = true;
				break;
			case OP_SEI:  // set interrupt disable
				r.SR_INTERRUPT = true;
				interrupts_active = false;
				break;
			case OP_CLV:  // clear overflow
				r.SR_OVERFLOW = false;
				break;


				// branching
				// alu holds bonus cycle count if page is crossed
			case OP_BCS:  // branch on carry set
				if(r.SR_CARRY) {
					r.PC = address;
					cycles += alu;
				}
				break;
			case OP_BCC:  // branch on carry clear
				if(!(r.SR_CARRY)) {
					r.PC = address;
					cycles += alu;
				}
				break;
			case OP_BEQ:  // branch on equal (zero set)
				if(r.SR_ZERO) {
					r.PC = address;
					cycles += alu;
				}
				break;
			case OP_BNE:  // branch on not equal (zero clear)
				if(!(r.SR_ZERO)) {
					r.PC = address;
					cycles += alu;
				}
				break;
			case OP_BMI:  // branch on minus (negative set)
				if(r.SR_NEGATIVE) {
					r.PC = address;
					cycles += alu;
				}
				break;
			case OP_BPL:  // branch on plus (negative clear)
				if(!(r.SR_NEGATIVE)) {
					r.PC = address;
					cycles += alu;
				}
				break;
			case OP_BVS:  // branch on overflow set
				if(r.SR_OVERFLOW) {
					r.PC = address;
					cycles += alu;
				}
				break;
			case OP_BVC:  // branch on overflow clear
				if(!(r.SR_OVERFLOW)) {
					r.PC = address;
					cycles += alu;
				}
				break;
			case OP_BRA:
				r.PC = address;
				cycles += alu;
				break;


	
			case OP_JMP:  // jump
				r.PC = address;
				break;

			case OP_JSR:  // jump subroutine
				// push PC onto stack
				alu = r.PC - 1;
				push_stack((unsigned char)((alu & 0xFF00) >> 8));
				push_stack((unsigned char)(alu & 0xFF));
				// update to new address in alu
				r.PC = address;
				break;
			case OP_RTS:  // return from subroutine
				// address should be -1 from next opcode
				alu = (unsigned short)pop_stack() | ((unsigned short)pop_stack() << 8);
				alu++;
				alu &= 0xFFFF;
				address = alu;
				r.PC = address;
				break;

			case OP_BRK:  // interrupt
				r.SR_BREAK = true;
				push_stack((unsigned char)(((r.PC + 1) & 0xFF00) >> 8));
				push_stack((unsigned char)((r.PC + 1) & 0xFF));
				push_stack(r.SRgetByte());
				r.PC = readMemoryWord(IRQBRK_VECTOR);
				r.SR_DECIMAL = false;
				r.SR_INTERRUPT = true;
				interrupted = true;
				break;

			case OP_RTI:  // return from interrupt
				r.SRsetByte(pop_stack());
				alu = (unsigned short)pop_stack() | ((unsigned short)pop_stack() << 8);
				alu &= 0xFFFF;
				address = alu;
				r.PC = address;
				break;



			case OP_LDA:  // load accumulator
				r.SRsetNZ(alu);
				r.A = alu & 0xFF;
				break;
			case OP_LDX:  // load X
				r.SRsetNZ(alu);
				r.X = alu & 0xFF;
				break;
			case OP_LDY:  // load Y
				r.SRsetNZ(alu);
				r.Y = alu & 0xFF;
				break;
			case OP_STA:  // store accumulator
				alu = r.A;
				break;
			case OP_STX:  // store X
				alu = r.X;
				break;
			case OP_STY:  // store Y
				alu = r.Y;
				break;
			case OP_STZ:
				alu = 0;
				break;



			case OP_PHA:  // push accumulator
				push_stack(r.A);
				break;
			case OP_PLA:  // pull accumulator, uses write
				alu = pop_stack();
				r.SRsetNZ(alu);
				break;

			case OP_PHX:  // push accumulator
				push_stack(r.X);
				break;
			case OP_PLX:  // pull accumulator, uses write
				alu = pop_stack();
				r.SRsetNZ(alu);
				r.X = alu & 0xFF;
				break;

			case OP_PHY:  // push accumulator
				push_stack(r.Y);
				break;
			case OP_PLY:  // pull accumulator, uses write
				alu = pop_stack();
				r.SRsetNZ(alu);
				r.Y = alu & 0xFF;
				break;

			case OP_PHP:  // push processor status (SR)
				push_stack(r.SRgetByte());
				break;
			case OP_PLP:  // pull processor status (SR)
				r.SRsetByte(pop_stack());
				r.SR_BREAK = true;
				break;


			case OP_TSX:  // transfer stack pointer to X
				r.X = r.SP;
				r.SRsetNZ(r.X);
				break;
			case OP_TXS:  // transfer X to stack pointer
				r.SP = r.X;
				break;

			case OP_TAX:  // transfer accumulator to X
				r.X = r.A;
				r.SRsetNZ(r.X);
				break;
			case OP_TAY:  // transfer accumulator to Y
				r.Y = r.A;
				r.SRsetNZ(r.Y);
				break;

			case OP_TXA:  // transfer X to accumulator
				r.A = r.X;
				r.SRsetNZ(r.A);
				break;
			case OP_TYA:  // transfer Y to accumulator
				r.A = r.Y;
				r.SRsetNZ(r.A);
				break;

			case OP_TRB: // test and reset bits
				r.SR_ZERO = (alu & r.A) == 0;
				alu &= ~r.A;
				write = 1;
				break;
			case OP_TSB: // test and set bits
				r.SR_ZERO = (alu & r.A) == 0;
				alu |= r.A;
				write = 1;
				break;
		}
		// phew

		// store alu using addressing mode
		if( write ) {
			switch (addressingMode) {
				case ADR_NONE:
					// wah?
				case ADR_IMM:
					// right away, sir!
				case ADR_IMPL:
					// implied, so we've done something already
				case ADR_IND:
					// only used by jsr
				case ADR_REL:
					// used by branches
					break;

				case ADR_A:
					r.A = alu & 0xFF;
					break;

				case ADR_ABS:
				case ADR_ABS_X:
				case ADR_ABS_Y:
				case ADR_AB_IX:
				case ADR_ZPG:
				case ADR_ZPG_X:
				case ADR_ZPG_Y:
				case ADR_ZPG_I:
				case ADR_IND_X:
				case ADR_IND_Y:
					writeMemory(address,alu & 0xFF);
					break;


			}
		}

		// all done

		return cycles;
	}


	/**
	 * dumpCpuState
	 * printf the cpu state
	 */
	void dumpCpuState() {

		printf("%04X: %02X %02X %02X %02X %04X %04X %04X (%02X)%c%cU%c%c%c%c%c: ",
				r.old_PC,
				r.A, 
				r.X,
				r.Y, 
				r.SP,
				old_alu & 0xFFFF, 
				alu & 0xFFFF, 
				address & 0xFFFF,
				r.SRgetByte(),
				(r.SR_NEGATIVE)?'N':' ',
				(r.SR_OVERFLOW)?'O':' ',
				(r.SR_BREAK)?'B':' ',
				(r.SR_DECIMAL)?'D':' ',
				(r.SR_INTERRUPT)?'I':' ',
				(r.SR_ZERO)?'Z':' ',
				(r.SR_CARRY)?'C':' '
			   );
		switch(OPCODES[opcode].step) {
			case 0:
				printf("%02X       ", opcode);
				break;
			case 1:
				printf("%02X %02X    ", opcode, zpage);
				break;
			case 2:
				printf("%02X %02X %02X ", opcode, address & 0xFF, (address & 0xFF00) >> 8);
				break;
		}
		printf("%s %s\r\n", nmem[OPCODES[opcode].op], sMODES[OPCODES[opcode].mode]);
	}

	/**
	 * printCpuStateHeader
	 * printf the column names for dumpCpuState
	 */
	void printCpuStateHeader() {
		printf("PC     A  X  Y SP  mem  alu addr flags        + OP data OPR MODE\r\n");
	}
};


/**
 * romIntercept
 * used to turn a section of memory into "ROM"
 */
struct romIntercept : public memoryIntercept {
	SixtyFiveCeeOhTwo *cpu;
	romIntercept() {}
	virtual unsigned char read(unsigned short address) {
		return cpu->memory[address];
	}
	virtual void write(unsigned char value, unsigned short address) {
	}
};

#endif
