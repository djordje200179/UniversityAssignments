.global a, b, c
.section PrvaSekcija
.extern printf, scanf
.section DrugaSekcija # bla
halt
iret
.skip 74
.word a, 8, b
jmp 0x500
jmp label
#komentar
#.section TrecaSekcija
#pocetak: .skip 8
#kraj: 
.ascii "Tartalja je dobar covjek"
.end
.ascii "Tartalja je govno"
halt
