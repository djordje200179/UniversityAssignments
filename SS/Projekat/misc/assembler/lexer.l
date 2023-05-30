%{
	#include <stdio.h>
	#include "../../h/assembler/parser.h"

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

[ \t]			;
,				return ',';

\.global	return DIR_GLOBAL;
\.extern	return DIR_EXTERN;
\.section	return DIR_SECTION;
\.word		return DIR_WORD;
\.skip		return DIR_SKIP;
\.ascii		return DIR_ASCII;
\.equ		return DIR_EQU;
\.end		BEGIN(END);

halt	return INST_HALT;
int		return INST_INT;
iret	return INST_IRET;
call	return INST_CALL;
ret		return INST_RET;
jmp		return INST_JMP;
beq		return INST_BEQ;
bne		return INST_BNE;
bgt		return INST_BGT;
push	return INST_PUSH;
pop		return INST_POP;
add		return INST_ADD;
sub		return INST_SUB;
mul		return INST_MUL;
div		return INST_DIV;
not		return INST_NOT;
and		return INST_AND;
or		return INST_OR;
xor		return INST_XOR;
shl		return INST_SHL;
shr		return INST_SHR;
ld		return INST_LD;
st		return INST_ST;
xchg	return INST_XCHG;
csrrd	return INST_CSRRD;
csrwr	return INST_CSRWR;

%r([0-9]|1[0-5])	{ yylval.ival = atoi(yytext+2); return REG; }
%sp				 	{ yylval.ival = 14; return REG; }
%pc				 	{ yylval.ival = 15; return REG; }
%status				{ yylval.ival = 0; return CREG; }
%handler			{ yylval.ival = 1; return CREG; }
%cause				{ yylval.ival = 2; return CREG; }

[\+\-\[\]]	return yytext[0];

\".*\"					{ yylval.sval = strdup(yytext+1); yylval.sval[strlen(yylval.sval)-1] = '\0'; return STR_LITERAL; }
[0-9]+					{ yylval.ival = atoi(yytext); return INT_LITERAL; }
[a-zA-Z][a-zA-Z0-9]*	{ yylval.sval = strdup(yytext); return SYMBOL; }

.	;
%%