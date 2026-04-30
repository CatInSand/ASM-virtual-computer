#pragma once

#include <string>
#include "Computer.h"
#include <unordered_map>
#include <unordered_set>

class Parser final
{
public:
	Parser() = default;
	static void ParseTextToTokens(const std::string& inputPath, const std::string& outputPath);
	static std::array<uint8_t, Computer::ROM_SIZE> ParseTokensToROM(const std::string& path);

private:
	enum class ReadingType {
		none = -1,
			uLabel,
			chars,
				nLabel,
				opcode,
					unaryOpcode,		
					binaryOpcode,
						normalBinaryOpcode,
						nLabeledOpcode,
						uLabeledOpcode,
	};
	struct LocalLabel {
		std::string name{};
		int line{};
	};
	static bool GetChar(std::ifstream& inputStream, char& currentChar, bool allowThrow = true);
	static unsigned int FindUnnamedLabel(bool positive, unsigned int count, unsigned int currentline, std::vector<unsigned int>& unnamedLabels);
	static ReadingType DetermineReadingType(const std::string& charStream);
	static std::string IntToHexString(uint8_t integer);
	static void ResetRead(std::string& charStream, ReadingType& readingType, std::ifstream& inputStream);
	static bool IsValid(char character);
	static bool IsOpcode(const std::string& stringStream);

	static const std::unordered_set<std::string> m_OpcodeSet;
	static const std::unordered_map<std::string, uint8_t> m_OpcodeHashMap;
	static const std::unordered_map<std::string, uint8_t> m_UnaryOpcodeHashMap;
};