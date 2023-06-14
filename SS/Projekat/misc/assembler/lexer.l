%{
	#include <stdio.h>
	#include "assembler/parser.h"

	int fileno(FILE *stream);

	int line_num = 1;
%}

%x COMMENT
%x END

%%
<END>.|\n ;

<INITIAL>\n	{ ++line_num; return ENDL; }
<COMMENT>\n	{ ++line_num; BEGIN(INITIAL); return ENDL; }
#			{ BEGIN(COMMENT); }
<COMMENT>.	;

[ \t]	;
[,:]	return yytext[0];

\.global	return DIR_GLOBAL;
\.extern	return DIR_EXTERN;
\.section	return DIR_SECTION;
\.word		return DIR_WORD;
\.skip		return DIR_SKIP;
\.ascii		return DIR_ASCII;
\.equ		return DIR_EQU;
\.end		BEGIN(END);

halt	{ yylval.inst_type = INST_HALT;	return INST_PARAMLESS; }
int		{ yylval.inst_type = INST_INT;	return INST_PARAMLESS; }
iret	{ yylval.inst_type = INST_IRET;	return INST_PARAMLESS; }
call	{ yylval.inst_type = INST_CALL;	return INST_UNCOND_JUMP; }
ret		{ yylval.inst_type = INST_RET;	return INST_PARAMLESS; }
jmp		{ yylval.inst_type = INST_JMP;	return INST_UNCOND_JUMP; }
beq		{ yylval.inst_type = INST_BEQ;	return INST_COND_JUMP; }
bne		{ yylval.inst_type = INST_BNE;	return INST_COND_JUMP; }
bgt		{ yylval.inst_type = INST_BGT;	return INST_COND_JUMP; }
push	{ yylval.inst_type = INST_PUSH;	return INST_UNIPARAM; }
pop		{ yylval.inst_type = INST_POP;	return INST_UNIPARAM; }
add		{ yylval.inst_type = INST_ADD;	return INST_BIPARAM; }
sub		{ yylval.inst_type = INST_SUB;	return INST_BIPARAM; }
mul		{ yylval.inst_type = INST_MUL;	return INST_BIPARAM; }
div		{ yylval.inst_type = INST_DIV;	return INST_BIPARAM; }
not		{ yylval.inst_type = INST_NOT;	return INST_UNIPARAM; }
and		{ yylval.inst_type = INST_AND;	return INST_BIPARAM; }
or		{ yylval.inst_type = INST_OR;	return INST_BIPARAM; }
xor		{ yylval.inst_type = INST_XOR;	return INST_BIPARAM; }
shl		{ yylval.inst_type = INST_SHL;	return INST_BIPARAM; }
shr		{ yylval.inst_type = INST_SHR;	return INST_BIPARAM; }
ld		return INST_LD;
st		return INST_ST;
xchg	{ yylval.inst_type = INST_XCHG; return INST_BIPARAM; }
csrrd	return INST_CSRRD;
csrwr	return INST_CSRWR;

%r([0-9]|1[0-5])	{ yylval.reg = atoi(yytext+2); return REG; }
%sp					{ yylval.reg = 14; return REG; }
%pc					{ yylval.reg = 15; return REG; }
%status				{ yylval.reg = 0; return CREG; }
%handler			{ yylval.reg = 1; return CREG; }
%cause				{ yylval.reg = 2; return CREG; }

[\+\-\[\]\$]	return yytext[0];

\".*\"					{ yylval.str_literal = strdup(yytext+1); yylval.str_literal[strlen(yylval.str_literal)-1] = '\0'; return STR_LITERAL; }
[0-9]+					{ yylval.int_literal = atoi(yytext); return INT_LITERAL; }
0(x|X)[0-9a-fA-F]+		{ yylval.int_literal = strtol(yytext, NULL, 16); return INT_LITERAL; }
[a-zA-Z][a-zA-Z0-9\-_]*	{ yylval.symbol = strdup(yytext); return SYMBOL; }

.	;
%%