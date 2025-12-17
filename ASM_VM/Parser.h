#pragma once

#include <string>
#include "Computer.h"
#include <unordered_map>

class Parser final
{
public:
	Parser() = default;
	static std::array<uint8_t, Computer::ROM_SIZE> ParseTokensToROM(const std::string& path);

	static const std::unordered_map<std::string, uint8_t> m_OpcodeHashMap;
	static const std::unordered_map<std::string, uint8_t> m_UnaryOpcodeHashMap;

private:

};