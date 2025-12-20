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
			ReadingType readingType{ ReadingType::none };
			char currentChar{};
			std::string charStream{};
			std::string outputString{};
			std::string garbageString{};
			bool reading{ true };

			uint8_t currentULabel{ 0 };

			while (reading)
			{
				switch (readingType)
				{
				case Parser::ReadingType::none:
					//chars or unnamed label
					if (GetChar(inputStream, currentChar, false))
					{
						//found end of file
						reading = false;
						continue;
					}
					switch (currentChar)
					{
					case ' ':
					case '\t':
					case '\n':
						//ignore whitespace
						break;
					case ';':
						//skip commented line
						ResetRead(charStream, readingType, inputStream);
						break;
					case ':':
						//unnamed label
						readingType = ReadingType::uLabel;
						break;
					default:
						//found character
						charStream += currentChar;
						readingType = ReadingType::chars;
						break;
					}
					break;
				case Parser::ReadingType::uLabel:
					outputString = "l :" + IntToHexString(currentULabel) + "\n";
					++currentULabel;
					outputStream.write(outputString.c_str(), outputString.size());
					ResetRead(charStream, readingType, inputStream);
					break;
				case Parser::ReadingType::chars:
					//named label or opcode
					while (true)
					{
						GetChar(inputStream, currentChar);
						if (!IsValid(currentChar))
						{
							throw std::exception("Invalid syntax: expected letter, instead found \'" + currentChar + '\'');
						}
						else if (currentChar == ':')
						{
							//named label
							readingType = ReadingType::nLabel;
							break;
						}
						else if (IsOpcode(charStream))
						{
							readingType = ReadingType::opcode;
							break;
						}
						else
						{
							charStream += currentChar;
						}
					}
					break;
				case Parser::ReadingType::nLabel:
					outputString = "l " + charStream + "\n";
					outputStream.write(outputString.c_str(), outputString.size());
					ResetRead(charStream, readingType, inputStream);
					break;
				case Parser::ReadingType::opcode:
					//unary or binary opcode
					if (m_UnaryOpcodeHashMap.contains(charStream))
					{
						readingType = ReadingType::unaryOpcode;
						break;
					}
					else
					{
						readingType = ReadingType::binaryOpcode;
						break;
					}
					break;
				case Parser::ReadingType::unaryOpcode:
					outputString = "o " + charStream + "-\n";
					outputStream.write(outputString.c_str(), outputString.size());
					ResetRead(charStream, readingType, inputStream);
					break;
				case Parser::ReadingType::binaryOpcode:
					//normal binary, named-labeled or unnamed-labeled opcode
					GetChar(inputStream, currentChar); //skip leading space
					if (currentChar != ' ') throw std::exception("Invalid syntax: expected \' \', instead found \'" + currentChar + '\'');

					while (true)
					{
						GetChar(inputStream, currentChar);
						switch (currentChar)
						{
						case '#':

						default:
							break;
						}
					}
					break;
				case Parser::ReadingType::normalBinaryOpcode:
					break;
				case Parser::ReadingType::nLabeledOpcode:
					break;
				case Parser::ReadingType::uLabeledOpcode:
					break;
				default:
					break;
				}
			}
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
		std::vector<unsigned int> unnamedLabels{};
		std::vector<LocalLabel> localLabels{};
		bool reading{ true };
		unsigned int currentLine{ 0 };
		std::string token{};
		std::string labelName{};
		std::string opcode{};
		std::string argument{};

		//parse labels
		while (reading)
		{
			if (std::getline(inputStream, token, '.'))
			{
				if(token == "nlb")
				{
					//named label
					std::getline(inputStream, labelName, '.');
					labelHashMap[labelName] = static_cast<uint8_t>(currentLine);
				}
				else if(token == "ulb")
				{
					//unnamed label
					unnamedLabels.push_back(currentLine);
				}
				else if (token == "llb")
				{
					//local label
					std::getline(inputStream, labelName, '.');
					localLabels.emplace_back(labelName, currentLine);
				}
				else if (token == "uop")
				{
					++currentLine;
				}
				else if (token == "bop")
				{
					++currentLine += 2;
				}
				else
				{
					throw std::exception("Unknown leading token");
				}
				std::getline(inputStream, labelName);	//skip rest of line
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
			if (std::getline(inputStream, token, '.'))
			{
				if (token == "nlb" || token == "ulb" || token == "llb")
				{
					//ignore labels
				}
				else if (token == "uop")
				{
					std::getline(inputStream, opcode, '.');
					result[currentLine] = m_OpcodeHashMap.at(opcode);
					++currentLine;
				}
				else if (token == "bop")
				{
					std::getline(inputStream, opcode, '.');		//get opcode
					std::getline(inputStream, argument, '.');	//get addressing type
					if (argument == "#")
					{
						opcode += '#';
					}
					result[currentLine] = m_OpcodeHashMap.at(opcode);
					++currentLine;
					std::getline(inputStream, argument, '.');	//get type
					if (argument == "val")
					{
						std::getline(inputStream, argument, '.');	//skip type (assume hex)
						std::getline(inputStream, argument, '.');	//get value
						result[currentLine] = static_cast<uint8_t>(std::stoi(argument, 0, 16));
					}
					else if (argument == "nlb")
					{
						std::getline(inputStream, labelName, '.');	//get label
						result[currentLine] = labelHashMap.at(labelName);
					}
					else if (argument == "ulb")
					{
						std::getline(inputStream, labelName, '.');	//get sign
						if (labelName == "+")
						{
							std::getline(inputStream, labelName, '.');	//get count
							//
						}
					}
					++currentLine;
				}
				else
				{
					throw std::exception("Unknown leading token");
				}
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

bool Parser::GetChar(std::ifstream& inputStream, char& currentChar, bool allowThrow)
{
	if (inputStream.get(currentChar))
	{
		return true;
	}
	else
	{
		if (allowThrow)
		{
			throw std::exception("Unexpected end of file");
		}
		else
		{
			return false;
		}
	}
}

Parser::ReadingType Parser::DetermineReadingType(const std::string& charStream)
{
	
}

std::string Parser::IntToHexString(uint8_t integer)
{
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(sizeof(uint8_t) * 2) << std::hex << integer;
	return stream.str();
}

void Parser::ResetRead(std::string& charStream, Parser::ReadingType& readingType, std::ifstream& inputStream)
{
	readingType = ReadingType::none;
	std::getline(inputStream, charStream);
	charStream = "";
}

bool Parser::IsValid(char character)
{
	switch (character)
	{
	case ' ':
	case '\t':
	case '\n':
	case ';':
		return false;
	default:
		return true;
	}
}
bool Parser::IsOpcode(const std::string& stringStream)
{
	//check size for fast return
	if (stringStream.size() == 3 && m_OpcodeSet.contains(stringStream))
	{
		return true;
	}
	return false;
}

const std::unordered_set<std::string> Parser::m_OpcodeSet{
	"BRK",
	"CMP",
	"BEQ",
	"BMI",
	"JMP",
	"JSR",
	"RTS",
	"LDA",
	"LDX",
	"TAX",
	"TXA",
	"STA",
	"STX",
	"PHA",
	"PLA",
	"PHX",
	"PLX",
	"AND",
	"ORA",
	"XOR",
	"ADD",
	"INX",
	"SHL",
	"SHR"
};
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