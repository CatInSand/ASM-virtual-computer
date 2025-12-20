o LDA#-0x05-
o CMP#-0x04-
o BMI#-lbl-:0x00-
o BEQ#-lbl-:0x00-
o JSR#-lbl-print_a_twenty_times-
l :0x00
o BRK-
l print_a_twenty_times
o PHA-
o PHX-
o LDX#-0x00-
l loop
o LDA#-0x41-
o STA-0xFE-
o INX-
o PHA-
o TXA-
o CMP#-0x14-
o PLA-
o BEQ#-lbl-:0x01-
o JMP#-lbl-loop-
l :0x01
o LDA#-0x11-
o STA-0xFF-
o PLX-
o PLA-
o RTS-


bop.LDA.#.val.hex.5.
bop.CMP.#.val.hex.4.
bop.BMI.#.ulb.+.1.
bop.BEQ.#.ulb.+.1.
bop.JSR.#.nlb.print_a_twenty_times.
ulb.
uop.BRK.
nlb.print_a_twenty_times.
uop.PHA.
uop.PHX.
bop.LDX.#.val.hex.0.
llb.loop.
bop.LDA.#.val.hex.41.
bop.LDA.0.val.hex.FE.
uop.INX.
uop.PHA.
uop.TXA.
bop.CMP.#.val.hex.14.
uop.PLA.
bop.BEQ.#.ulb.+.1.
bop.JMP.#.llb.loop.
ulb.
bop.LDA.#.val.hex.11.
bop.STA.0.val.hex.FF.
uop.PLX.
uop.PLA.
uop.RTS.