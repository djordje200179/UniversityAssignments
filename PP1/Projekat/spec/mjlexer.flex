package rs.ac.bg.etf.pp1;

import java_cup.runtime.Symbol;

%%

%{
	private Symbol new_symbol(int type) {
		return new Symbol(type, yyline+1, yycolumn);
	}
	
	private Symbol new_symbol(int type, Object value) {
		return new Symbol(type, yyline+1, yycolumn, value);
	}
%}

%cup
%line
%column

%xstate COMMENT

%eofval{
	return new_symbol(sym.EOF);
%eofval}

%%

" " 	{ }
"\b" 	{ }
"\t" 	{ }
"\r\n" 	{ }
"\f" 	{ }

"program"   { return new_symbol(sym.PROGRAM, yytext()); }
"namespace"	{ return new_symbol(sym.NAMESPACE, yytext()); }
"read" 	{ return new_symbol(sym.READ, yytext()); }
"print" 	{ return new_symbol(sym.PRINT, yytext()); }
"return" 	{ return new_symbol(sym.RETURN, yytext()); }
"void" 		{ return new_symbol(sym.VOID, yytext()); }
"const" 	{ return new_symbol(sym.CONST, yytext()); }
"new" 		{ return new_symbol(sym.NEW, yytext()); }
"break" 	{ return new_symbol(sym.BREAK, yytext()); }
"continue" 	{ return new_symbol(sym.CONTINUE, yytext()); }

"+" { return new_symbol(sym.PLUS, yytext()); }
"-" { return new_symbol(sym.MINUS, yytext()); }
"*" { return new_symbol(sym.ASTERISK, yytext()); }
"/" { return new_symbol(sym.SLASH, yytext()); }
"%" { return new_symbol(sym.PERCENT, yytext()); }
"=" { return new_symbol(sym.EQUAL, yytext()); }
";" { return new_symbol(sym.SEMI, yytext()); }
"," { return new_symbol(sym.COMMA, yytext()); }
"::" { return new_symbol(sym.DBLCOLON, yytext()); }
"(" { return new_symbol(sym.LPAREN, yytext()); }
")" { return new_symbol(sym.RPAREN, yytext()); }
"{" { return new_symbol(sym.LBRACE, yytext()); }
"}"	{ return new_symbol(sym.RBRACE, yytext()); }
"[" { return new_symbol(sym.LBRACKET, yytext()); }
"]" { return new_symbol(sym.RBRACKET, yytext()); }

<YYINITIAL> "//" { yybegin(COMMENT); }
<COMMENT> .      { yybegin(COMMENT); }
<COMMENT> "\r\n" { yybegin(YYINITIAL); }

[0-9]+ 					{ return new_symbol(sym.NUM, Integer.valueOf(yytext())); }
true|false  			{ return new_symbol(sym.BOOL, Boolean.valueOf(yytext())); }
'.'		  				{ return new_symbol(sym.CHAR, yytext().charAt(0)); }
[a-zA-Z][a-zA-Z0-9_]*	{ return new_symbol (sym.IDENT, yytext()); }

. { System.err.println("Found illegal character <" + yytext() + ">" + " at line " + yyline + ", column " + yycolumn); }






