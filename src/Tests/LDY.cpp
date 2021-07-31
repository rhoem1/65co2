#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, LDYImmediate)
{
  uint16_t addr = 0x200;
  uint8_t val = randomByte();
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_IM);
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
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, val);
  EXPECT_EQ(cpu.r.PC, addr);
  
}

TEST_F(CpuTest, LDYZeroPage)
{
  uint16_t addr = 0x200;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_ZP);
  addr++;
  cpu.write(addr, zp);
  addr++;
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
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, val);
  EXPECT_EQ(cpu.r.PC, addr);
  
}

TEST_F(CpuTest, LDYZeroPageX)
{
  uint16_t addr = 0x200;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();
  uint8_t x = randomByte();
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_ZPX);
  addr++;
  cpu.write(addr, zp);
  addr++;
  cpu.write((zp + x) & 0xFF, val);
  
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
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, x);
	EXPECT_EQ(cpu.r.Y, val);
  EXPECT_EQ(cpu.r.PC, addr);
  
}

TEST_F(CpuTest, LDYAbsolute)
{
  uint16_t addr = 0x200;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_ABS);
  addr++;
  cpu.write(addr, zp);
  addr++;
  cpu.write(addr, 0x80);
  addr++;
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
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, val);
  EXPECT_EQ(cpu.r.PC, addr);
  
}

TEST_F(CpuTest, LDYAbsoluteX)
{
  uint16_t addr = 0x200;
  uint8_t val = randomByte();
  uint8_t x = randomByte();
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_ABSX);
  addr++;
  cpu.write(addr, 0x00);
  addr++;
  cpu.write(addr, 0x80);
  addr++;
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
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, x);
	EXPECT_EQ(cpu.r.Y, val);
  EXPECT_EQ(cpu.r.PC, addr);
  
}