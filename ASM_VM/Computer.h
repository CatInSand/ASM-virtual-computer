#pragma once
#include <vector>
#include <string>

// Computer holds all data and performs parsed ASM instructions
//
// Writing to address 0xfe streams bytes into output buffer (interpreted as ASCII)
// Writing to address 0xff handles console output
//			writing 0x00 resets the output buffer
//			writing 0x10 outputs the output buffer
//			writing 0x11 outputs the output buffer with newline
//			writing 0x12 outputs the output buffer and resets
//			writing 0x13 outputs the output buffer with newline and resets

class Computer final
{
public:
	static constexpr unsigned int ROM_SIZE{ 256 };
	Computer(const uint8_t ROM[ROM_SIZE]);
	void Run();
private:
	bool HandleInstruction(uint8_t opCode);
	void WriteRAM(uint8_t address, uint8_t data);

	static constexpr unsigned int STACK_SIZE{ 64 };
	static constexpr unsigned int RAM_SIZE{ 256 };
	static constexpr unsigned int FLAG_COUNT{ 8 };

	uint8_t m_Stack[STACK_SIZE];
	uint8_t m_RAM[RAM_SIZE];
	uint8_t m_ROM[ROM_SIZE];

	uint8_t m_ProgramCounter{};
	uint8_t m_StackPointer{};

	uint8_t m_RegA{};
	uint8_t m_RegX{};
	bool m_Flags[FLAG_COUNT]{};
	// 7 6 5 4   3 2 1 0
	// | | | |   | | | +-- zero
	// | | | |   | | +---- negative
	// +-+-+-+---+-+------ unused

	std::string m_OutputBuffer{};
};

