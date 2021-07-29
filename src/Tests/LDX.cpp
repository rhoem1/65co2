#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, LDXImmediate)
{
  uint8_t val = randomByte();
  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDX_IM);
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
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, val);
	EXPECT_EQ(cpu.r.Y, 0);
	EXPECT_EQ(cpu.r.PC, 0x0202);
  
}

TEST_F(CpuTest, LDXZeroPage)
{
  uint8_t val = randomByte();
  uint8_t zp = randomByte();
  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDX_ZP);
  cpu.write(0x0201, zp);
  cpu.write(zp, val);
  
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
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, val);
	EXPECT_EQ(cpu.r.Y, 0);
	EXPECT_EQ(cpu.r.PC, 0x0202);
  
}

TEST_F(CpuTest, LDXZeroPageY)
{
  uint8_t val = randomByte();
  uint8_t zp = randomByte();
  uint8_t y = randomByte();
  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDX_ZPY);
  cpu.write(0x0201, zp);
  cpu.write((zp + y) & 0xFF, val);
  
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
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, val);
	EXPECT_EQ(cpu.r.Y, y);
	EXPECT_EQ(cpu.r.PC, 0x0202);
  
}

TEST_F(CpuTest, LDXAbsolute)
{
  uint8_t val = randomByte();
  uint8_t zp = randomByte();
  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDX_ABS);
  cpu.write(0x0201, zp);
  cpu.write(0x0202, 0x80);
  cpu.write(0x8000 + zp, val);
  
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
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, val);
	EXPECT_EQ(cpu.r.Y, 0);
	EXPECT_EQ(cpu.r.PC, 0x0203);
  
}

TEST_F(CpuTest, LDXAbsoluteY)
{
  uint8_t val = randomByte();
  uint8_t y = randomByte();
  cpu.write(0x0200, SixtyFiveCeeOhTwo::INS_LDX_ABSY);
  cpu.write(0x0201, 0x00);
  cpu.write(0x0202, 0x80);
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
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, val);
	EXPECT_EQ(cpu.r.Y, y);
	EXPECT_EQ(cpu.r.PC, 0x0203);
  
}