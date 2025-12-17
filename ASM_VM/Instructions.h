#pragma once

// Set of all instructions
//
// Custom ASM based on 6502
//		Focus on minimal instruction set
//
//
// Instructions
//		Evergreen
//			BRK			::	0x00	::		stop the program
//			
//		Branch
//			CMP #val	::	0x01	::		subtract val from register A and set flags accordingly
//			CMP addr	::	0x02	::		subtract RAM[addr] from register A and set flags accordingly
//			BEQ	#val	::	0x03	::		branch if zero flag is set
//			BMI	#val	::	0x04	::		branch if negative flag is set
//			JMP	#val	::	0x05	::		set the program counter to 'addr'
//			JSR	#val	::	0x06	::		push program counter onto the stack and jump to val
//	---		Removed		::	0x07	::		
//			RTS			::	0x08	::		pop stack into program counter
// 
//		Load
//			LDA #val	::	0x09	::		load val into register A
//			LDA addr	::	0x0a	::		load RAM[addr] into register A
// 			LDX #val	::	0x0b	::		load val into register X
//			LDX addr	::	0x0c	::		load RAM[addr] into register X
//			TAX			::	0x0d	::		transfer register A to register X
//			TXA			::	0x0e	::		transfer register X to register A
// 
//		Store
//			STA addr	::	0x0f	::		load register A into RAM[addr]
//			STX	addr	::	0x10	::		load register X into RAM[addr]
// 
//		Stack
//			PHA			::	0x11	::		push register A onto the stack
//			PLA			::	0x12	::		pop stack into register A
//			PHX			::	0x13	::		push register X onto the stack
//			PLX			::	0x14	::		pop stack into register X
// 
//		Boolean operations
//			AND #val	::	0x15	::		perform bitwise and between val and register A, then store result in register A
//			AND addr	::	0x16	::		perform bitwise and between RAM[addr] and register A, then store result in register A
//			ORA	#val	::	0x17	::		perform bitwise or between val and register A, then store result in register A
//			ORA addr	::	0x18	::		perform bitwise or between RAM[addr] and register A, then store result in register A
//			XOR	#val	::	0x19	::		perform bitwise xor between val and register A, then store result in register A
//			XOR addr	::	0x1a	::		perform bitwise xor between RAM[addr] and register A, then store result in register A
// 
//		Arithmetic operations
//			ADD	#val	::	0x1b	::		add val to register A
//			ADD	addr	::	0x1c	::		add RAM[addr] to register A
//			INX			::	0x1d	::		increment register X
//			SHL			::	0x1e	::		shift register A left 1 bit
//			SHR			::	0x1f	::		shift register A right 1 bit
//