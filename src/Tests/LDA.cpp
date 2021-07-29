#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"


TEST_F(CpuTest, LDAImmediate)
{
  uint8_t val = randomByte();
  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDA_IM);
  cpu.write(0x0201, val);
  
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
	EXPECT_EQ(cpu.r.PC, 0x0202);

}


TEST_F(CpuTest, LDAZeroPage)
{
  uint8_t val = randomByte();
  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDA_ZP);
  cpu.write(0x201, 0x00);
  cpu.write(0x0000, val);
  
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
	EXPECT_EQ(cpu.r.PC, 0x0202);

}


TEST_F(CpuTest, LDAZeroPageX)
{
  uint8_t val = randomByte();
  uint8_t x = randomByte();
  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDA_ZPX);
  cpu.write(0x201, 0x00);
  cpu.write(0x0000 + x, val);
  
  cpu.r.X = x;
  
  uint64_t cycles = cpu.do_cycle();
  
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
	EXPECT_EQ(cpu.r.PC, 0x0202);

}


TEST_F(CpuTest, LDAAbsolute)
{
  uint8_t val = randomByte();
  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDA_ABS);
  cpu.write(0x201, 0x00);
  cpu.write(0x202, 0x80);
  cpu.write(0x8000, val);
  
  uint64_t cycles = cpu.do_cycle();
  
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
	EXPECT_EQ(cpu.r.PC, 0x0203);

}

TEST_F(CpuTest, LDAAbsoluteX)
{
  uint8_t val = randomByte();
  uint8_t x = randomByte();

  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDA_ABSX);
  cpu.write(0x201, 0x00);
  cpu.write(0x202, 0x80);
  cpu.write(0x8000 + x, val);
  
  cpu.r.X = x;

  uint64_t cycles = cpu.do_cycle();
  
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
	EXPECT_EQ(cpu.r.PC, 0x0203);

}

TEST_F(CpuTest, LDAAbsoluteY)
{
  uint8_t val = randomByte();
  uint8_t y = randomByte();

  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDA_ABSY);
  cpu.write(0x201, 0x00);
  cpu.write(0x202, 0x80);
  cpu.write(0x8000 + y, val);
  
  cpu.r.Y = y;

  uint64_t cycles = cpu.do_cycle();
  
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
	EXPECT_EQ(cpu.r.PC, 0x0203);

}

TEST_F(CpuTest, LDAIndirectX)
{
  uint8_t val = randomByte();
  uint8_t x = randomByte();

  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDA_INDX);
  cpu.write(0x201, 0x00);
  cpu.write(0x300, val);
  cpu.write(x, 0x00);
  cpu.write((x + 1) & 0xFF, 0x03);
  
  cpu.r.X = x;

  uint64_t cycles = cpu.do_cycle();
  
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
	EXPECT_EQ(cpu.r.PC, 0x0202);

}


TEST_F(CpuTest, LDAIndirectY)
{
  uint8_t val = randomByte();
  uint8_t y = randomByte();
  uint8_t zp = randomByte();

  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDA_INDY);
  cpu.write(0x201, zp);

  cpu.write(0x300 + y, val);

  cpu.write(zp, 0x00);
  cpu.write((zp + 1) & 0xFF, 0x03);
  
  cpu.r.Y = y;

  uint64_t cycles = cpu.do_cycle();
  
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
	EXPECT_EQ(cpu.r.PC, 0x0202);

}


TEST_F(CpuTest, LDAZeroPageI)
{
  uint8_t val = randomByte();
  uint8_t zp = randomByte();

  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDA_ZPI);
  cpu.write(0x201, zp);

  cpu.write(0x300, val);

  cpu.write(zp, 0x00);
  cpu.write((zp + 1) & 0xFF, 0x03);
  

  uint64_t cycles = cpu.do_cycle();
  
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
	EXPECT_EQ(cpu.r.PC, 0x0202);

}