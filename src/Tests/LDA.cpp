#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

/**
 * LDA tests
 */


TEST_F(CpuTest, LDAImmediate)
{
  uint16_t addr = 0x200;
  uint8_t val = randomByte();
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  
  uint64_t cycles = cpu.do_cycle();
  
  EXPECT_EQ(cycles, 2);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}


TEST_F(CpuTest, LDAZeroPage)
{
  uint16_t addr = 0x200;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();

  cpu.write(zp, val);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_ZP);
  addr++;
  cpu.write(addr, zp);
  addr++;
  
  uint64_t cycles = cpu.do_cycle();
  
  EXPECT_EQ(cycles, 3);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}


TEST_F(CpuTest, LDAZeroPageX)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t x = randomByte();
  uint8_t zp = randomByte();

  cpu.write((zp + x) & 0xFF, val);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_IM);
  addr++;
  cpu.write(addr, x);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_ZPX);
  addr++;
  cpu.write(addr, zp);
  addr++;
  
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 4);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, x);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}


TEST_F(CpuTest, LDAAbsolute)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();

  cpu.write(0x8000, val);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_ABS);
  addr++;
  cpu.write(addr, 0x00);
  addr++;
  cpu.write(addr, 0x80);
  addr++;
  
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 4);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}

TEST_F(CpuTest, LDAAbsoluteX)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t x = randomByte();

  cpu.write(0x8080 + x, val);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_IM);
  addr++;
  cpu.write(addr, x);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_ABSX);
  addr++;
  cpu.write(addr, 0x80);
  addr++;
  cpu.write(addr, 0x80);
  addr++;

  cycles = cpu.do_cycle();
  
  if(x > 0x7F)
  EXPECT_EQ(cycles, 5);
  else
  EXPECT_EQ(cycles, 4);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, x);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}

TEST_F(CpuTest, LDAAbsoluteY)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t y = randomByte();

  cpu.write(0x8080 + y, val);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_IM);
  addr++;
  cpu.write(addr, y);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_ABSY);
  addr++;
  cpu.write(addr, 0x80);
  addr++;
  cpu.write(addr, 0x80);
  addr++;

  cycles = cpu.do_cycle();
  if(y > 0x7F)
  EXPECT_EQ(cycles, 5);
  else
  EXPECT_EQ(cycles, 4);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, y);
  EXPECT_EQ(cpu.r.PC, addr);

}

TEST_F(CpuTest, LDAIndirectX)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t x = randomByte();

  cpu.write(0x300, val);
  cpu.write(x, 0x00);
  cpu.write((x + 1) & 0xFF, 0x03);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_IM);
  addr++;
  cpu.write(addr, x);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_INDX);
  addr++;
  cpu.write(addr, 0x00);
  addr++;
  
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 6);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, x);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}


TEST_F(CpuTest, LDAIndirectY)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t y = randomByte();
  uint8_t zp = randomByte();

  cpu.write(0x380 + y, val);

  cpu.write(zp, 0x80);
  cpu.write((zp + 1) & 0xFF, 0x03);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_IM);
  addr++;
  cpu.write(addr, y);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_INDY);
  addr++;
  cpu.write(addr, zp);
  addr++;
  
  cycles = cpu.do_cycle();
  if(y > 127)
  EXPECT_EQ(cycles, 6);
  else
  EXPECT_EQ(cycles, 5);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, y);
  EXPECT_EQ(cpu.r.PC, addr);

}


TEST_F(CpuTest, LDAZeroPageI)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();

  cpu.write(0x300, val);

  cpu.write(zp, 0x00);
  cpu.write((zp + 1) & 0xFF, 0x03);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_ZPI);
  addr++;
  cpu.write(addr, zp);
  addr++;

  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 5);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}