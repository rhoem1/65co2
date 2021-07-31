#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, LDXImmediate)
{
  uint8_t val = randomByte();
  uint16_t addr = 0x200;
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_IM);
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
	EXPECT_EQ(cpu.r.X, val);
	EXPECT_EQ(cpu.r.Y, 0);
	EXPECT_EQ(cpu.r.PC, addr);
  
}

TEST_F(CpuTest, LDXZeroPage)
{
  uint16_t addr = 0x200;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_ZP);
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
	EXPECT_EQ(cpu.r.X, val);
	EXPECT_EQ(cpu.r.Y, 0);
	EXPECT_EQ(cpu.r.PC, addr);
  
}

TEST_F(CpuTest, LDXZeroPageY)
{
  uint16_t addr = 0x200;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();
  uint8_t y = randomByte();
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_ZPY);
  addr++;
  cpu.write(addr, zp);
  addr++;
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
	EXPECT_EQ(cpu.r.PC, addr);
  
}

TEST_F(CpuTest, LDXAbsolute)
{
  uint16_t addr = 0x200;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_ABS);
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
	EXPECT_EQ(cpu.r.X, val);
	EXPECT_EQ(cpu.r.Y, 0);
	EXPECT_EQ(cpu.r.PC, addr);
  
}

TEST_F(CpuTest, LDXAbsoluteY)
{
  uint16_t addr = 0x200;
  uint8_t val = randomByte();
  uint8_t y = 0xFF; //randomByte();
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_ABSY);
  addr++;
  cpu.write(addr, 0x80);
  addr++;
  cpu.write(addr, 0x80);
  addr++;
  cpu.write(0x8080 + y, val);
  
  cpu.r.Y = y;

  uint64_t cycles = cpu.do_cycle();
  
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
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, val);
	EXPECT_EQ(cpu.r.Y, y);
	EXPECT_EQ(cpu.r.PC, addr);
  
}
