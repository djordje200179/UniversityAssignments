/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_H_ASSEMBLER_PARSER_H_INCLUDED
# define YY_YY_H_ASSEMBLER_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 14 "misc/assembler/parser.y"

	#include "syntax.h"

#line 53 "h/assembler/parser.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ENDL = 258,                    /* ENDL  */
    DIR_GLOBAL = 259,              /* DIR_GLOBAL  */
    DIR_EXTERN = 260,              /* DIR_EXTERN  */
    DIR_SECTION = 261,             /* DIR_SECTION  */
    DIR_WORD = 262,                /* DIR_WORD  */
    DIR_SKIP = 263,                /* DIR_SKIP  */
    DIR_ASCII = 264,               /* DIR_ASCII  */
    DIR_EQU = 265,                 /* DIR_EQU  */
    INST_HALT = 266,               /* INST_HALT  */
    INST_INT = 267,                /* INST_INT  */
    INST_IRET = 268,               /* INST_IRET  */
    INST_CALL = 269,               /* INST_CALL  */
    INST_RET = 270,                /* INST_RET  */
    INST_JMP = 271,                /* INST_JMP  */
    INST_BEQ = 272,                /* INST_BEQ  */
    INST_BNE = 273,                /* INST_BNE  */
    INST_BGT = 274,                /* INST_BGT  */
    INST_PUSH = 275,               /* INST_PUSH  */
    INST_POP = 276,                /* INST_POP  */
    INST_ADD = 277,                /* INST_ADD  */
    INST_SUB = 278,                /* INST_SUB  */
    INST_MUL = 279,                /* INST_MUL  */
    INST_DIV = 280,                /* INST_DIV  */
    INST_NOT = 281,                /* INST_NOT  */
    INST_AND = 282,                /* INST_AND  */
    INST_OR = 283,                 /* INST_OR  */
    INST_XOR = 284,                /* INST_XOR  */
    INST_SHL = 285,                /* INST_SHL  */
    INST_SHR = 286,                /* INST_SHR  */
    INST_LD = 287,                 /* INST_LD  */
    INST_ST = 288,                 /* INST_ST  */
    INST_XCHG = 289,               /* INST_XCHG  */
    INST_CSRRD = 290,              /* INST_CSRRD  */
    INST_CSRWR = 291,              /* INST_CSRWR  */
    REG = 292,                     /* REG  */
    CREG = 293,                    /* CREG  */
    INT_LITERAL = 294,             /* INT_LITERAL  */
    STR_LITERAL = 295,             /* STR_LITERAL  */
    SYMBOL = 296                   /* SYMBOL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "misc/assembler/parser.y"

	int ival;	
	float fval;
	char* sval;

	struct symbol_list symbols;
	struct word_args word_args;
	struct operand operand;

	struct dir dir;
	struct inst inst;
	struct line line;
	struct line_list lines;

#line 126 "h/assembler/parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (struct line_list* ret_lines);


#endif /* !YY_YY_H_ASSEMBLER_PARSER_H_INCLUDED  */
