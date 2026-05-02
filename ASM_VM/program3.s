LDA# 5
CMP# 4
BMI# :+1
BEQ# :+1
JSR print_a_twenty_times

:
	BRK

print_a_twenty_times:
	PHA
	PHX

	LDX 0

	loop:
		LDA 41
		STA FE
		INX
		PHA
		TXA
		CMP 14
		PLA
		BEQ :+1
		JMP loop

	:
	LDA 11
	STA FF
	PLX
	PLA
	RTS