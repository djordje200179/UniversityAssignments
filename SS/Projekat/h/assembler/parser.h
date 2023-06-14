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
#line 13 "parser.y"

	#include "assembler/syntax.h"

	// #define YYDEBUG 1

#line 55 "../../h/assembler/parser.h"

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
    INST_BEQ = 266,                /* INST_BEQ  */
    INST_BNE = 267,                /* INST_BNE  */
    INST_BGT = 268,                /* INST_BGT  */
    INST_COND_JUMP = 269,          /* INST_COND_JUMP  */
    INST_CALL = 270,               /* INST_CALL  */
    INST_JMP = 271,                /* INST_JMP  */
    INST_UNCOND_JUMP = 272,        /* INST_UNCOND_JUMP  */
    INST_HALT = 273,               /* INST_HALT  */
    INST_INT = 274,                /* INST_INT  */
    INST_IRET = 275,               /* INST_IRET  */
    INST_RET = 276,                /* INST_RET  */
    INST_PARAMLESS = 277,          /* INST_PARAMLESS  */
    INST_PUSH = 278,               /* INST_PUSH  */
    INST_POP = 279,                /* INST_POP  */
    INST_NOT = 280,                /* INST_NOT  */
    INST_UNIPARAM = 281,           /* INST_UNIPARAM  */
    INST_XCHG = 282,               /* INST_XCHG  */
    INST_ADD = 283,                /* INST_ADD  */
    INST_SUB = 284,                /* INST_SUB  */
    INST_MUL = 285,                /* INST_MUL  */
    INST_DIV = 286,                /* INST_DIV  */
    INST_AND = 287,                /* INST_AND  */
    INST_OR = 288,                 /* INST_OR  */
    INST_XOR = 289,                /* INST_XOR  */
    INST_SHL = 290,                /* INST_SHL  */
    INST_SHR = 291,                /* INST_SHR  */
    INST_BIPARAM = 292,            /* INST_BIPARAM  */
    INST_LD = 293,                 /* INST_LD  */
    INST_ST = 294,                 /* INST_ST  */
    INST_CSRRD = 295,              /* INST_CSRRD  */
    INST_CSRWR = 296,              /* INST_CSRWR  */
    REG = 297,                     /* REG  */
    CREG = 298,                    /* CREG  */
    INT_LITERAL = 299,             /* INT_LITERAL  */
    STR_LITERAL = 300,             /* STR_LITERAL  */
    SYMBOL = 301                   /* SYMBOL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 19 "parser.y"

	int int_literal;
	char* str_literal;

	char* symbol;

	int reg;
	int inst_type;

	struct const_operand const_operand;
	struct const_operands const_operands;
	struct operand operand;
	struct inst_params inst_params;

	struct dir dir;
	struct inst inst;
	struct line line;
	struct lines lines;

#line 138 "../../h/assembler/parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (struct lines* ret_lines);


#endif /* !YY_YY_H_ASSEMBLER_PARSER_H_INCLUDED  */
