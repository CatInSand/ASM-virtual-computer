#include "Parser.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

// text file -> token file -> ROM
// 
// text file contains assembly, labels and comments as text with free formatting
// token file contains assembly and labels as tokenized text with strict formatting
// ROM contains raw opcodes as an array of uint8_t

void Parser::ParseTextToTokens(const std::string& inputPath, const std::string& outputPath)
{
	if (std::ifstream inputStream{ inputPath }; inputStream.is_open())
	{
		if (std::ofstream outputStream{ outputPath }; outputStream.is_open())
		{
			//read char per char
			//	if(charstream.isopcode)
			//		if(charstream.isunaryopcode)
			//			add("o " + charstream + '-')
			//		else
			//			skipchar
			//	else
			// 
			//	case ';':
			//		skipline
			//		empty charstream
			//	case ':':
			//		add("l " + charstream)
			//		empty charstream
			//  case ws:
			//		if(!charstream.isempty)
			//			throw invalid syntax exception
			//	case default:
			//		charstream.append(char)
			//

		}
		else
		{
			std::cout << "Couldn't open input stream." << std::endl;
		}
	}
	else
	{
		std::cout << "Couldn't open input stream." << std::endl;
	}
}

std::array<uint8_t, Computer::ROM_SIZE> Parser::ParseTokensToROM(const std::string& path)
{
	if (std::ifstream inputStream{ path }; inputStream.is_open())
	{
		std::array<uint8_t, Computer::ROM_SIZE> result{};
		std::unordered_map<std::string, uint8_t> labelHashMap{};
		bool reading{ true };
		int currentLine{ 0 };
		std::string token{};
		std::string labelName{};
		std::string opcode{};
		std::string argument{};

		//parse labels
		while (reading)
		{
			if (std::getline(inputStream, token, ' '))
			{
				if (token == "l")
				{
					std::getline(inputStream, labelName);
					labelHashMap[labelName] = static_cast<uint8_t>(currentLine);
				}
				else
				{
					//increment currentLine correct count
					std::getline(inputStream, opcode, '-');

					if (!m_UnaryOpcodeHashMap.contains(opcode))
					{
						++currentLine;

						std::getline(inputStream, argument, '-');
						if (argument == "lbl")
						{
							std::getline(inputStream, argument, '-');
						}

					}

					++currentLine;
					inputStream.get();
				}
			}
			else
			{
				reading = false;
				continue;
			}
		}

		inputStream.clear();	//reset flags
		inputStream.seekg(0, std::ios::beg);	//return to start of file
		reading = true;
		currentLine = 0;

		//parse opcodes
		while (reading)
		{
			if (std::getline(inputStream, token, ' '))
			{
				if (token == "o")
				{
					std::getline(inputStream, opcode, '-');
					result[currentLine] = m_OpcodeHashMap.at(opcode);

					if (!m_UnaryOpcodeHashMap.contains(opcode))
					{
						++currentLine;

						std::getline(inputStream, argument, '-');
						if (argument == "lbl")
						{
							std::getline(inputStream, argument, '-');
							result[currentLine] = labelHashMap.at(argument);
						}
						else
						{
							result[currentLine] = static_cast<uint8_t>(std::stoi(argument, 0, 16));	//turn hex string into uint8_t
						}

					}

					++currentLine;
					inputStream.get();
				}
				else
				{
					//skip label
					std::getline(inputStream, opcode);
				}
			}
			else
			{
				//reached end of file
				reading = false;
				continue;
			}
		}

		return result;
	}
	else
	{
		std::cout << "Couldn't open input stream." << std::endl;
	}
	return std::array<uint8_t, Computer::ROM_SIZE>{};
}

const std::unordered_map<std::string, uint8_t> Parser::m_OpcodeHashMap{
		{"BRK", 0x00},
		{"CMP#", 0x01},
		{"CMP", 0x02},
		{"BEQ#", 0x03},
		{"BMI#", 0x04},
		{"JMP#", 0x05},
		{"JSR#", 0x06},
		{"RTS", 0x08},
		{"LDA#", 0x09},
		{"LDA", 0x0a},
		{"LDX#", 0x0b},
		{"LDX", 0x0c},
		{"TAX", 0x0d},
		{"TXA", 0x0e},
		{"STA", 0x0f},
		{"STX", 0x10},
		{"PHA", 0x11},
		{"PLA", 0x12},
		{"PHX", 0x13},
		{"PLX", 0x14},
		{"AND#", 0x15},
		{"AND", 0x16},
		{"ORA#", 0x17},
		{"ORA", 0x18},
		{"XOR#", 0x19},
		{"XOR", 0x1a},
		{"ADD#", 0x1b},
		{"ADD", 0x1c},
		{"INX", 0x1d},
		{"SHL", 0x1e},
		{"SHR", 0x1f},
};
const std::unordered_map<std::string, uint8_t> Parser::m_UnaryOpcodeHashMap{
		{"BRK", 0x00},
		{"RTS", 0x08},
		{"TAX", 0x0d},
		{"TXA", 0x0e},
		{"PHA", 0x11},
		{"PLA", 0x12},
		{"PHX", 0x13},
		{"PLX", 0x14},
		{"INX", 0x1d},
		{"SHL", 0x1e},
		{"SHR", 0x1f},
};