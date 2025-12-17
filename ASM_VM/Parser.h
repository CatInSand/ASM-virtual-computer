#pragma once

#include <string>
#include "Computer.h"

class Parser final
{
public:
	Parser();
	void ParseTXTtoROM(const std::string& path, uint8_t ROM[Computer::ROM_SIZE]);

private:

};

