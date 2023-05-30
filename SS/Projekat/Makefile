run:
	bison --defines="h/assembler/parser.h" -o src/assembler/parser.c misc/assembler/parser.y
	sed -i 's/\"parser\.h\"/\"\.\.\/\.\.\/h\/assembler\/parser\.h\"/' src/assembler/parser.c

	flex -o src/assembler/lexer.c misc/assembler/lexer.l 
	gcc src/assembler/main.c src/assembler/syntax.c src/assembler/parser.c src/assembler/lexer.c -lfl -o test
	./test test.s