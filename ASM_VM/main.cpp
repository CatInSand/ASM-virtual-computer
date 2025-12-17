#include "Computer.h"

int main()
{
	const std::array<uint8_t, Computer::ROM_SIZE> ROM{
		0x09, 0x05, //	LDA #$05
		0x01, 0x04, //	CMP #$04
		0x04, 0x0a, //	BMI :+
		0x03, 0x0a, //	BEQ :+
		0x06, 0x0b, //		JSR print_a_twenty_times
					//	:
		0x00,		//	BRK
					// 
					//	print_a_twenty_times:
		0x11,		//		PHA
		0x13,		//		PHX
					//		
		0x0b, 0x00,	//		LDX #$00
					//	@loop:
		0x09, 0x41, //		LDA #'A'
		0x0f, 0xfe, //		STA $FE
					// 
		0x1d,		//		INX
		0x11,		//		PHA
		0x0e,		//		TXA
		0x01, 0x14,	//		CMP #$14
		0x12,		//		PLA
		0x03, 0x1d,	//		BEQ :+
		0x05, 0x0f,	//			JMP @loop
					//		:
					// 
		0x09, 0x11,	//		LDA #$11
		0x0f, 0xff,	//		STA $FF
					//		
		0x14,		//		PLX
		0x12,		//		PLA
		0x08,		//		RTS
					//
	};

	Computer computer{ ROM };
	computer.Run();
}