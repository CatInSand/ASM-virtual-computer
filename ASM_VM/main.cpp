#include "Computer.h"
#include "Parser.h"

int main()
{
	const std::array<uint8_t, Computer::ROM_SIZE> ROM{
		Parser::ParseTokensToROM("program.s")
	};

	Computer computer{ ROM };
	computer.Run();
}