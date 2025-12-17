#include "Computer.h"
#include <iostream>

Computer::Computer(const std::array<uint8_t, ROM_SIZE>& ROM)
	: m_ROM{ ROM }
{

}

/// <summary>
/// Runs a .s file
/// </summary>
void Computer::Run()
{
	bool running{ true };
	while (running)
	{
		if (!HandleInstruction(m_ROM[m_ProgramCounter]))
		{
			running = false;
		}
	}
}

/// <summary>
/// Performs the instruction specified by opCode using argument
/// </summary>
/// <returns>false if exit program</returns>
bool Computer::HandleInstruction(uint8_t opCode)
{
	switch (opCode)
	{
	case 0x00:
		//-------------------
		// BRK
		//-------------------
		return false;
	case 0x01:
		//-------------------
		// CMP #val
		//-------------------
		++m_ProgramCounter;
		if (m_RegA == m_ROM[m_ProgramCounter]) {
			m_Flags[0] = true;
			m_Flags[1] = false;
		}
		else if(m_RegA < m_ROM[m_ProgramCounter]) {
			m_Flags[0] = false;
			m_Flags[1] = true;
		}
		else {
			m_Flags[0] = false;
			m_Flags[1] = false;
		}
		++m_ProgramCounter;
		break;
	case 0x02:
		//-------------------
		// CMP addr
		//-------------------
		++m_ProgramCounter;
		if (m_RegA == m_RAM[m_ROM[m_ProgramCounter]]) {
			m_Flags[0] = true;
			m_Flags[1] = false;
		}
		else if (m_RegA < m_RAM[m_ROM[m_ProgramCounter]]) {
			m_Flags[0] = false;
			m_Flags[1] = true;
		}
		else {
			m_Flags[0] = false;
			m_Flags[1] = false;
		}
		++m_ProgramCounter;
		break;
	case 0x03:
		//-------------------
		// BEQ #val
		//-------------------
		++m_ProgramCounter;
		if (m_Flags[0] == true) {
			m_ProgramCounter = m_ROM[m_ProgramCounter];
		}
		else {
			++m_ProgramCounter;
		}
		break;
	case 0x04:
		//-------------------
		// BMI #val
		//-------------------
		++m_ProgramCounter;
		if (m_Flags[1] == true) {
			m_ProgramCounter = m_ROM[m_ProgramCounter];
		}
		else {
			++m_ProgramCounter;
		}
		break;
	case 0x05:
		//-------------------
		// JMP #val
		//-------------------
		++m_ProgramCounter;

		m_ProgramCounter = m_ROM[m_ProgramCounter];
		break;
	case 0x06:
		//-------------------
		// JSR #val
		//-------------------
		++m_ProgramCounter;

		m_Stack[m_StackPointer] = m_ProgramCounter;
		++m_StackPointer;
		m_ProgramCounter = m_ROM[m_ProgramCounter];
		break;
	case 0x08:
		//-------------------
		// RTS
		//-------------------
		if (m_StackPointer == 0)
		{
			return false;
		}
		--m_StackPointer;
		m_ProgramCounter = m_Stack[m_StackPointer];
		++m_ProgramCounter;
		break;
	case 0x09:
		//-------------------
		// LDA #val
		//-------------------
		++m_ProgramCounter;

		m_RegA = m_ROM[m_ProgramCounter];
		++m_ProgramCounter;
		break;
	case 0x0a:
		//-------------------
		// LDA addr
		//-------------------
		++m_ProgramCounter;

		m_RegA = m_RAM[m_ROM[m_ProgramCounter]];
		++m_ProgramCounter;
		break;
	case 0x0b:
		//-------------------
		// LDX #val
		//-------------------
		++m_ProgramCounter;

		m_RegX = m_ROM[m_ProgramCounter];
		++m_ProgramCounter;
		break;
	case 0x0c:
		//-------------------
		// LDX addr
		//-------------------
		++m_ProgramCounter;

		m_RegX = m_RAM[m_ROM[m_ProgramCounter]];
		++m_ProgramCounter;
		break;
	case 0x0d:
		//-------------------
		// TAX
		//-------------------
		m_RegX = m_RegA;
		++m_ProgramCounter;
		break;
	case 0x0e:
		//-------------------
		// TXA
		//-------------------
		m_RegA = m_RegX;
		++m_ProgramCounter;
		break;
	case 0x0f:
		//-------------------
		// STA addr
		//-------------------
		++m_ProgramCounter;

		WriteRAM(m_ROM[m_ProgramCounter], m_RegA);
		++m_ProgramCounter;
		break;
	case 0x10:
		//-------------------
		// STX addr
		//-------------------
		++m_ProgramCounter;

		WriteRAM(m_ROM[m_ProgramCounter], m_RegX);
		++m_ProgramCounter;
		break;
	case 0x11:
		//-------------------
		// PHA
		//-------------------
		m_Stack[m_StackPointer] = m_RegA;
		++m_StackPointer;
		++m_ProgramCounter;
		break;
	case 0x12:
		//-------------------
		// PLA
		//-------------------
		if (m_StackPointer == 0)
		{
			return false;
		}
		--m_StackPointer;
		m_RegA = m_Stack[m_StackPointer];
		++m_ProgramCounter;
		break;
	case 0x13:
		//-------------------
		// PHX
		//-------------------
		m_Stack[m_StackPointer] = m_RegX;
		++m_StackPointer;
		++m_ProgramCounter;
		break;
	case 0x14:
		//-------------------
		// PLX
		//-------------------
		if (m_StackPointer == 0)
		{
			return false;
		}
		--m_StackPointer;
		m_RegX = m_Stack[m_StackPointer];
		++m_ProgramCounter;
		break;
	case 0x15:
		//-------------------
		// AND #val
		//-------------------
		++m_ProgramCounter;

		m_RegA = m_RegA & m_ROM[m_ProgramCounter];
		++m_ProgramCounter;
		break;
	case 0x16:
		//-------------------
		// AND addr
		//-------------------
		++m_ProgramCounter;

		m_RegA = m_RegA & m_RAM[m_ROM[m_ProgramCounter]];
		++m_ProgramCounter;
		break;
	case 0x17:
		//-------------------
		// ORA #val
		//-------------------
		++m_ProgramCounter;

		m_RegA = m_RegA | m_ROM[m_ProgramCounter];
		++m_ProgramCounter;
		break;
	case 0x18:
		//-------------------
		// ORA addr
		//-------------------
		++m_ProgramCounter;

		m_RegA = m_RegA | m_RAM[m_ROM[m_ProgramCounter]];
		++m_ProgramCounter;
		break;
	case 0x19:
		//-------------------
		// XOR #val
		//-------------------
		++m_ProgramCounter;

		m_RegA = m_RegA ^ m_ROM[m_ProgramCounter];
		++m_ProgramCounter;
		break;
	case 0x1a:
		//-------------------
		// XOR addr
		//-------------------
		++m_ProgramCounter;

		m_RegA = m_RegA ^ m_RAM[m_ROM[m_ProgramCounter]];
		++m_ProgramCounter;
		break;
	case 0x1b:
		//-------------------
		// ADD #val
		//-------------------
		++m_ProgramCounter;

		m_RegA += m_ROM[m_ProgramCounter];
		++m_ProgramCounter;
		break;
	case 0x1c:
		//-------------------
		// ADD addr
		//-------------------
		++m_ProgramCounter;

		m_RegA += m_RAM[m_ROM[m_ProgramCounter]];
		++m_ProgramCounter;
		break;
	case 0x1d:
		//-------------------
		// INX
		//-------------------
		++m_RegX;
		++m_ProgramCounter;
		break;
	case 0x1e:
		//-------------------
		// SHL
		//-------------------
		m_RegA = m_RegA << 1;
		++m_ProgramCounter;
		break;
	case 0x1f:
		//-------------------
		// SHR
		//-------------------
		m_RegA = m_RegA >> 1;
		++m_ProgramCounter;
		break;
	default:
		//-------------------
		// NOP
		//-------------------
		++m_ProgramCounter;
		break;
	}

	return true;
}

void Computer::WriteRAM(uint8_t address, uint8_t data)
{
	if (address == 0xfe)
	{
		m_OutputBuffer += data;
	}
	else if (address == 0xff)
	{
		switch (data)
		{
		case 0x00:
			// reset buffer
			m_OutputBuffer = "";
			break;
		case 0x10:
			// write buffer to output
			std::cout << m_OutputBuffer;
			break;
		case 0x11:
			// write buffer to output with newline
			std::cout << m_OutputBuffer << "\n";
			break;
		case 0x12:
			// write buffer to output and reset
			std::cout << m_OutputBuffer;
			m_OutputBuffer = "";
			break;
		case 0x13:
			// write buffer to output with newline and reset
			std::cout << m_OutputBuffer << "\n";
			m_OutputBuffer = "";
			break;
		default:
			break;
		}
	}
	else
	{
		m_RAM[address] = data;
	}
}