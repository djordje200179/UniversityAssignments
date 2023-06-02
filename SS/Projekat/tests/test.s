.extern print
.extern a

.global struktura

.section data
struktura:
	.word 0x100, a, 15
neki_niz:
	.skip 0x20
kraj_nekog_niza:

.section text
# Funkcija koja sabira 6 brojeva
fun:
	add %r1, %r0
	add %r2, %r0
	add %r3, %r0
	add %r4, %r0
	add %r5, %r0

	ret
main:
	ld $5, %r0
	ld $6, %r1
	ld $7, %r2
	ld $8, %r3
	ld $9, %r4
	ld $0x10, %r5

	call fun

# Ispis na konzolu
	ld $0x41, %r0
	int

	halt
