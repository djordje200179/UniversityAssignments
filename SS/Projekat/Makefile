IDIR = h
OBJDIR = obj
SRCDIR = .
CC = gcc
CFLAGS = -Werror -I$(IDIR)

clean:
	rm -f $(OBJDIR)/*.o
	rm -f *.d
	rm -f $(PROGRAM)
	rm -f *~

assembler:
	bison --defines="h/assembler/parser.h" -o src/assembler/parser.c misc/assembler/parser.y
	sed -i 's/parser\.h/assembler\/parser\.h/g' src/assembler/parser.c
	flex -o src/assembler/lexer.c misc/assembler/lexer.l 
	gcc -Ih/ -g src/common/*.c src/assembler/*.c -lfl -o test
	./test tests/test.s

.PHONY: clean