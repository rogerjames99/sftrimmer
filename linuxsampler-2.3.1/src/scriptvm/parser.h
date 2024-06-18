/* A Bison parser, made by GNU Bison 3.7.5.  */

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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_INSTRSCRIPT_Y_TAB_H_INCLUDED
# define YY_INSTRSCRIPT_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int InstrScript_debug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    END_OF_FILE = 0,               /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INTEGER = 258,                 /* "integer literal"  */
    REAL = 259,                    /* "real number literal"  */
    INTEGER_UNIT = 260,            /* "integer literal with unit"  */
    REAL_UNIT = 261,               /* "real number literal with unit"  */
    STRING = 262,                  /* "string literal"  */
    IDENTIFIER = 263,              /* "function name"  */
    VARIABLE = 264,                /* "variable name"  */
    ON = 265,                      /* "keyword 'on'"  */
    END = 266,                     /* "keyword 'end'"  */
    INIT = 267,                    /* "keyword 'init'"  */
    NOTE = 268,                    /* "keyword 'note'"  */
    RELEASE = 269,                 /* "keyword 'release'"  */
    CONTROLLER = 270,              /* "keyword 'controller'"  */
    RPN = 271,                     /* "keyword 'rpn'"  */
    NRPN = 272,                    /* "keyword 'nrpn'"  */
    DECLARE = 273,                 /* "keyword 'declare'"  */
    ASSIGNMENT = 274,              /* "operator ':='"  */
    CONST_ = 275,                  /* "keyword 'const'"  */
    POLYPHONIC = 276,              /* "keyword 'polyphonic'"  */
    PATCH = 277,                   /* "keyword 'patch'"  */
    WHILE = 278,                   /* "keyword 'while'"  */
    SYNCHRONIZED = 279,            /* "keyword 'synchronized'"  */
    IF = 280,                      /* "keyword 'if'"  */
    ELSE = 281,                    /* "keyword 'else'"  */
    SELECT = 282,                  /* "keyword 'select'"  */
    CASE = 283,                    /* "keyword 'case'"  */
    TO = 284,                      /* "keyword 'to'"  */
    OR = 285,                      /* "operator 'or'"  */
    AND = 286,                     /* "operator 'and'"  */
    NOT = 287,                     /* "operator 'not'"  */
    BITWISE_OR = 288,              /* "bitwise operator '.or.'"  */
    BITWISE_AND = 289,             /* "bitwise operator '.and.'"  */
    BITWISE_NOT = 290,             /* "bitwise operator '.not.'"  */
    FUNCTION = 291,                /* "keyword 'function'"  */
    CALL = 292,                    /* "keyword 'call'"  */
    MOD = 293,                     /* "operator 'mod'"  */
    LE = 294,                      /* "operator '<='"  */
    GE = 295,                      /* "operator '>='"  */
    UNKNOWN_CHAR = 296             /* "unknown character"  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define END_OF_FILE 0
#define YYerror 256
#define YYUNDEF 257
#define INTEGER 258
#define REAL 259
#define INTEGER_UNIT 260
#define REAL_UNIT 261
#define STRING 262
#define IDENTIFIER 263
#define VARIABLE 264
#define ON 265
#define END 266
#define INIT 267
#define NOTE 268
#define RELEASE 269
#define CONTROLLER 270
#define RPN 271
#define NRPN 272
#define DECLARE 273
#define ASSIGNMENT 274
#define CONST_ 275
#define POLYPHONIC 276
#define PATCH 277
#define WHILE 278
#define SYNCHRONIZED 279
#define IF 280
#define ELSE 281
#define SELECT 282
#define CASE 283
#define TO 284
#define OR 285
#define AND 286
#define NOT 287
#define BITWISE_OR 288
#define BITWISE_AND 289
#define BITWISE_NOT 290
#define FUNCTION 291
#define CALL 292
#define MOD 293
#define LE 294
#define GE 295
#define UNKNOWN_CHAR 296

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int InstrScript_parse (LinuxSampler::ParserContext* context);

#endif /* !YY_INSTRSCRIPT_Y_TAB_H_INCLUDED  */
