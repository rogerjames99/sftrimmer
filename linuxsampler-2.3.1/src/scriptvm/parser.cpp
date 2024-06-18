/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         InstrScript_parse
#define yylex           InstrScript_lex
#define yyerror         InstrScript_error
#define yydebug         InstrScript_debug
#define yynerrs         InstrScript_nerrs

/* First part of user prologue.  */
#line 12 "parser.y"

    #define YYERROR_VERBOSE 1
    #include "parser_shared.h"
    #include <string>
    #include <map>
    using namespace LinuxSampler;

    void InstrScript_error(YYLTYPE* locp, LinuxSampler::ParserContext* context, const char* err);
    void InstrScript_warning(YYLTYPE* locp, LinuxSampler::ParserContext* context, const char* txt);
    int InstrScript_tnamerr(char* yyres, const char* yystr);
    int InstrScript_lex(YYSTYPE* lvalp, YYLTYPE* llocp, void* scanner);
    #define scanner context->scanner
    #define PARSE_ERR(loc,txt)  yyerror(&loc, context, txt)
    #define PARSE_WRN(loc,txt)  InstrScript_warning(&loc, context, txt)
    #define PARSE_DROP(loc) \
        context->addPreprocessorComment( \
            loc.first_line, loc.last_line, loc.first_column+1, \
            loc.last_column+1, loc.first_byte, loc.length_bytes \
        );
    #define CODE_BLOCK(loc) { \
        .firstLine = loc.first_line, .lastLine = loc.last_line, \
        .firstColumn = loc.first_column+1, .lastColumn = loc.last_column+1, \
        .firstByte = loc.first_byte, .lengthBytes = loc.length_bytes \
    }
    #define ASSIGNED_EXPR_BLOCK(loc) { \
        .firstLine = loc.first_line, .lastLine = loc.last_line, \
        .firstColumn = loc.first_column+3, .lastColumn = loc.last_column+1, \
        .firstByte = loc.first_byte+2, .lengthBytes = loc.length_bytes-2 \
    }
    #define yytnamerr(res,str)  InstrScript_tnamerr(res, str)

#line 108 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INTEGER = 3,                    /* "integer literal"  */
  YYSYMBOL_REAL = 4,                       /* "real number literal"  */
  YYSYMBOL_INTEGER_UNIT = 5,               /* "integer literal with unit"  */
  YYSYMBOL_REAL_UNIT = 6,                  /* "real number literal with unit"  */
  YYSYMBOL_STRING = 7,                     /* "string literal"  */
  YYSYMBOL_IDENTIFIER = 8,                 /* "function name"  */
  YYSYMBOL_VARIABLE = 9,                   /* "variable name"  */
  YYSYMBOL_ON = 10,                        /* "keyword 'on'"  */
  YYSYMBOL_END = 11,                       /* "keyword 'end'"  */
  YYSYMBOL_INIT = 12,                      /* "keyword 'init'"  */
  YYSYMBOL_NOTE = 13,                      /* "keyword 'note'"  */
  YYSYMBOL_RELEASE = 14,                   /* "keyword 'release'"  */
  YYSYMBOL_CONTROLLER = 15,                /* "keyword 'controller'"  */
  YYSYMBOL_RPN = 16,                       /* "keyword 'rpn'"  */
  YYSYMBOL_NRPN = 17,                      /* "keyword 'nrpn'"  */
  YYSYMBOL_DECLARE = 18,                   /* "keyword 'declare'"  */
  YYSYMBOL_ASSIGNMENT = 19,                /* "operator ':='"  */
  YYSYMBOL_CONST_ = 20,                    /* "keyword 'const'"  */
  YYSYMBOL_POLYPHONIC = 21,                /* "keyword 'polyphonic'"  */
  YYSYMBOL_PATCH = 22,                     /* "keyword 'patch'"  */
  YYSYMBOL_WHILE = 23,                     /* "keyword 'while'"  */
  YYSYMBOL_SYNCHRONIZED = 24,              /* "keyword 'synchronized'"  */
  YYSYMBOL_IF = 25,                        /* "keyword 'if'"  */
  YYSYMBOL_ELSE = 26,                      /* "keyword 'else'"  */
  YYSYMBOL_SELECT = 27,                    /* "keyword 'select'"  */
  YYSYMBOL_CASE = 28,                      /* "keyword 'case'"  */
  YYSYMBOL_TO = 29,                        /* "keyword 'to'"  */
  YYSYMBOL_OR = 30,                        /* "operator 'or'"  */
  YYSYMBOL_AND = 31,                       /* "operator 'and'"  */
  YYSYMBOL_NOT = 32,                       /* "operator 'not'"  */
  YYSYMBOL_BITWISE_OR = 33,                /* "bitwise operator '.or.'"  */
  YYSYMBOL_BITWISE_AND = 34,               /* "bitwise operator '.and.'"  */
  YYSYMBOL_BITWISE_NOT = 35,               /* "bitwise operator '.not.'"  */
  YYSYMBOL_FUNCTION = 36,                  /* "keyword 'function'"  */
  YYSYMBOL_CALL = 37,                      /* "keyword 'call'"  */
  YYSYMBOL_MOD = 38,                       /* "operator 'mod'"  */
  YYSYMBOL_LE = 39,                        /* "operator '<='"  */
  YYSYMBOL_GE = 40,                        /* "operator '>='"  */
  YYSYMBOL_UNKNOWN_CHAR = 41,              /* "unknown character"  */
  YYSYMBOL_42_ = 42,                       /* '['  */
  YYSYMBOL_43_ = 43,                       /* ']'  */
  YYSYMBOL_44_ = 44,                       /* '('  */
  YYSYMBOL_45_ = 45,                       /* ')'  */
  YYSYMBOL_46_ = 46,                       /* ','  */
  YYSYMBOL_47_ = 47,                       /* '+'  */
  YYSYMBOL_48_ = 48,                       /* '-'  */
  YYSYMBOL_49_ = 49,                       /* '!'  */
  YYSYMBOL_50_ = 50,                       /* '&'  */
  YYSYMBOL_51_ = 51,                       /* '<'  */
  YYSYMBOL_52_ = 52,                       /* '>'  */
  YYSYMBOL_53_ = 53,                       /* '='  */
  YYSYMBOL_54_ = 54,                       /* '#'  */
  YYSYMBOL_55_ = 55,                       /* '*'  */
  YYSYMBOL_56_ = 56,                       /* '/'  */
  YYSYMBOL_YYACCEPT = 57,                  /* $accept  */
  YYSYMBOL_script = 58,                    /* script  */
  YYSYMBOL_sections = 59,                  /* sections  */
  YYSYMBOL_section = 60,                   /* section  */
  YYSYMBOL_eventhandler = 61,              /* eventhandler  */
  YYSYMBOL_function_declaration = 62,      /* function_declaration  */
  YYSYMBOL_opt_statements = 63,            /* opt_statements  */
  YYSYMBOL_statements = 64,                /* statements  */
  YYSYMBOL_statement = 65,                 /* statement  */
  YYSYMBOL_caseclauses = 66,               /* caseclauses  */
  YYSYMBOL_caseclause = 67,                /* caseclause  */
  YYSYMBOL_userfunctioncall = 68,          /* userfunctioncall  */
  YYSYMBOL_functioncall = 69,              /* functioncall  */
  YYSYMBOL_args = 70,                      /* args  */
  YYSYMBOL_arg = 71,                       /* arg  */
  YYSYMBOL_opt_qualifiers = 72,            /* opt_qualifiers  */
  YYSYMBOL_qualifiers = 73,                /* qualifiers  */
  YYSYMBOL_qualifier = 74,                 /* qualifier  */
  YYSYMBOL_opt_assignment = 75,            /* opt_assignment  */
  YYSYMBOL_opt_arr_assignment = 76,        /* opt_arr_assignment  */
  YYSYMBOL_assignment = 77,                /* assignment  */
  YYSYMBOL_unary_expr = 78,                /* unary_expr  */
  YYSYMBOL_opt_expr = 79,                  /* opt_expr  */
  YYSYMBOL_expr = 80,                      /* expr  */
  YYSYMBOL_concat_expr = 81,               /* concat_expr  */
  YYSYMBOL_logical_or_expr = 82,           /* logical_or_expr  */
  YYSYMBOL_logical_and_expr = 83,          /* logical_and_expr  */
  YYSYMBOL_bitwise_or_expr = 84,           /* bitwise_or_expr  */
  YYSYMBOL_bitwise_and_expr = 85,          /* bitwise_and_expr  */
  YYSYMBOL_rel_expr = 86,                  /* rel_expr  */
  YYSYMBOL_add_expr = 87,                  /* add_expr  */
  YYSYMBOL_mul_expr = 88                   /* mul_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   191

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  92
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  183

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   296


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    49,     2,    54,     2,     2,    50,     2,
      44,    45,    55,    47,    46,    48,     2,    56,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      51,    53,    52,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    42,     2,    43,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   112,   112,   117,   121,   127,   130,   135,   141,   147,
     153,   159,   165,   173,   185,   188,   193,   200,   209,   212,
     215,   378,   503,   506,   520,   523,   537,   551,   569,   573,
     579,   584,   592,   607,   670,   691,   714,   718,   724,   727,
     730,   735,   738,   745,   748,   751,   756,   759,   764,   767,
     772,   798,   839,   842,   845,   853,   861,   864,   874,   904,
     907,   910,   918,   926,   937,   948,   958,   961,   966,   969,
     970,   983,   984,  1009,  1012,  1037,  1038,  1063,  1066,  1091,
    1092,  1114,  1136,  1158,  1180,  1202,  1226,  1227,  1253,  1281,
    1282,  1308,  1340
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "\"integer literal\"",
  "\"real number literal\"", "\"integer literal with unit\"",
  "\"real number literal with unit\"", "\"string literal\"",
  "\"function name\"", "\"variable name\"", "\"keyword 'on'\"",
  "\"keyword 'end'\"", "\"keyword 'init'\"", "\"keyword 'note'\"",
  "\"keyword 'release'\"", "\"keyword 'controller'\"", "\"keyword 'rpn'\"",
  "\"keyword 'nrpn'\"", "\"keyword 'declare'\"", "\"operator ':='\"",
  "\"keyword 'const'\"", "\"keyword 'polyphonic'\"", "\"keyword 'patch'\"",
  "\"keyword 'while'\"", "\"keyword 'synchronized'\"", "\"keyword 'if'\"",
  "\"keyword 'else'\"", "\"keyword 'select'\"", "\"keyword 'case'\"",
  "\"keyword 'to'\"", "\"operator 'or'\"", "\"operator 'and'\"",
  "\"operator 'not'\"", "\"bitwise operator '.or.'\"",
  "\"bitwise operator '.and.'\"", "\"bitwise operator '.not.'\"",
  "\"keyword 'function'\"", "\"keyword 'call'\"", "\"operator 'mod'\"",
  "\"operator '<='\"", "\"operator '>='\"", "\"unknown character\"", "'['",
  "']'", "'('", "')'", "','", "'+'", "'-'", "'!'", "'&'", "'<'", "'>'",
  "'='", "'#'", "'*'", "'/'", "$accept", "script", "sections", "section",
  "eventhandler", "function_declaration", "opt_statements", "statements",
  "statement", "caseclauses", "caseclause", "userfunctioncall",
  "functioncall", "args", "arg", "opt_qualifiers", "qualifiers",
  "qualifier", "opt_assignment", "opt_arr_assignment", "assignment",
  "unary_expr", "opt_expr", "expr", "concat_expr", "logical_or_expr",
  "logical_and_expr", "bitwise_or_expr", "bitwise_and_expr", "rel_expr",
  "add_expr", "mul_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,    91,    93,    40,    41,    44,    43,    45,    33,
      38,    60,    62,    61,    35,    42,    47
};
#endif

#define YYPACT_NINF (-36)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       5,    60,    14,    36,     5,   -36,   -36,   -36,     8,     8,
       8,     8,     8,     8,     8,   -36,   -36,    -6,     9,    68,
      -4,     8,     3,    59,    42,    41,     8,   -36,   -36,   -36,
     -36,    43,    84,    91,    93,    94,   104,    52,    59,    59,
     -36,   -36,   -36,    89,    68,   -36,    59,   105,    59,   -36,
     -36,   -36,   -36,   -36,    75,    59,    59,    59,    59,    59,
      59,   -36,   -36,    90,    71,    92,    99,   100,    98,    87,
      -5,    15,   -36,   114,   -36,   132,   133,   134,   136,   138,
     101,   -36,    33,   -36,   -36,   -36,   106,    27,   -36,   108,
     131,   112,    59,   -36,   -36,   113,   -36,   -36,   -36,   156,
      16,   -36,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,   -36,   -36,
     -36,   -36,   -36,   -36,   -36,   -36,    59,   141,    59,    59,
     -36,     8,   -36,     8,   118,   -36,   127,   139,   -36,    92,
      99,   100,    98,    87,    -5,    -5,    -5,    -5,    -5,    -5,
      15,    15,   -36,   -36,   -36,   -36,    59,   -36,   122,   -36,
     159,    23,   -36,   164,   -36,   -36,   -36,   152,   149,   148,
       8,     8,   130,   -36,   -36,   -36,   165,   -36,    59,   150,
      40,   -36,   -36
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     2,     3,     6,     5,    14,    14,
      14,    14,    14,    14,    14,     1,     4,    35,     0,    39,
       0,    14,     0,     0,     0,     0,    15,    16,    19,    18,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,    44,    45,     0,    40,    41,     0,     0,     0,    52,
      53,    54,    55,    56,    57,     0,     0,     0,     0,     0,
       0,    60,    89,     0,    68,    69,    71,    73,    75,    77,
      79,    86,    32,     0,    17,     0,     0,     0,     0,     0,
       0,    34,     0,    36,    38,    50,     0,    46,    42,     0,
       0,     0,     0,    64,    63,     0,    61,    62,    65,     0,
       0,    28,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     8,     7,
       9,    10,    11,    12,    13,    33,     0,     0,     0,    66,
      20,    14,    24,    14,     0,    59,    14,     0,    29,    70,
      72,    74,    76,    78,    82,    83,    80,    81,    84,    85,
      87,    88,    92,    90,    91,    37,     0,    47,     0,    67,
       0,     0,    58,     0,    30,    27,    51,    48,     0,     0,
      14,    14,     0,    21,    23,    26,     0,    31,     0,     0,
       0,    25,    49
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -36,   -36,   -36,   173,   -36,   -36,    -2,   -36,   153,   -36,
      78,   -36,    -8,     4,    54,   -36,   -36,   137,   -36,   -36,
     -36,   -35,   -36,    -9,   -36,    81,    82,    80,    83,    85,
       2,   -21
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,     4,     5,     6,     7,    25,    26,    27,   100,
     101,    28,    61,    82,    83,    43,    44,    45,   130,   173,
      30,    62,   158,    84,    64,    65,    66,    67,    68,    69,
      70,    71
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      29,    29,    29,    29,    29,    29,    29,    31,    32,    33,
      34,    35,    36,    29,    63,     1,    17,    18,    29,    47,
      93,    94,    14,    96,    97,    98,    19,   137,    38,    85,
      86,    20,    21,    22,   169,    23,    15,    89,    37,    91,
      46,     2,   113,   114,    99,    24,   128,    48,    95,   170,
      72,    39,    73,   115,    75,    49,    50,    51,    52,    53,
      17,    54,    49,    50,    51,    52,    53,    17,    54,   129,
     116,   117,     8,     9,    10,    11,    12,    13,   125,   126,
     152,   153,   154,   134,    55,   182,   126,    56,    40,    41,
      42,    55,   150,   151,    56,    76,    57,    81,    87,    58,
      59,    60,    77,    57,    78,    79,    58,    59,    60,   144,
     145,   146,   147,   148,   149,    80,    90,    92,    99,   157,
     159,   102,   103,    29,   118,    29,   107,   108,    29,   160,
     104,   161,   106,   105,   164,    17,    18,   124,   109,   110,
     111,   112,   119,   120,   121,    19,   122,   166,   123,   127,
      20,    21,    22,   131,    23,   132,   163,   133,   135,   136,
     156,   162,    29,    29,    24,   167,   165,   171,   176,   177,
     168,   172,   174,   175,   178,   181,   179,    16,   138,    74,
     155,    88,   180,   139,   141,   140,     0,     0,   142,     0,
       0,   143
};

static const yytype_int16 yycheck[] =
{
       8,     9,    10,    11,    12,    13,    14,     9,    10,    11,
      12,    13,    14,    21,    23,    10,     8,     9,    26,    21,
      55,    56,     8,    58,    59,    60,    18,    11,    19,    38,
      39,    23,    24,    25,    11,    27,     0,    46,    44,    48,
      44,    36,    47,    48,    28,    37,    19,    44,    57,    26,
       8,    42,    11,    38,    11,     3,     4,     5,     6,     7,
       8,     9,     3,     4,     5,     6,     7,     8,     9,    42,
      55,    56,    12,    13,    14,    15,    16,    17,    45,    46,
     115,   116,   117,    92,    32,    45,    46,    35,    20,    21,
      22,    32,   113,   114,    35,    11,    44,    45,     9,    47,
      48,    49,    11,    44,    11,    11,    47,    48,    49,   107,
     108,   109,   110,   111,   112,    11,    11,    42,    28,   128,
     129,    50,    30,   131,    10,   133,    39,    40,   136,   131,
      31,   133,    34,    33,   136,     8,     9,    36,    51,    52,
      53,    54,    10,    10,    10,    18,    10,   156,    10,    43,
      23,    24,    25,    45,    27,    24,    29,    45,    45,     3,
      19,    43,   170,   171,    37,    43,    27,     3,   170,   171,
      11,    19,    23,    25,    44,    25,    11,     4,   100,    26,
     126,    44,   178,   102,   104,   103,    -1,    -1,   105,    -1,
      -1,   106
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    10,    36,    58,    59,    60,    61,    62,    12,    13,
      14,    15,    16,    17,     8,     0,    60,     8,     9,    18,
      23,    24,    25,    27,    37,    63,    64,    65,    68,    69,
      77,    63,    63,    63,    63,    63,    63,    44,    19,    42,
      20,    21,    22,    72,    73,    74,    44,    63,    44,     3,
       4,     5,     6,     7,     9,    32,    35,    44,    47,    48,
      49,    69,    78,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     8,    11,    65,    11,    11,    11,    11,    11,
      11,    45,    70,    71,    80,    80,    80,     9,    74,    80,
      11,    80,    42,    78,    78,    80,    78,    78,    78,    28,
      66,    67,    50,    30,    31,    33,    34,    39,    40,    51,
      52,    53,    54,    47,    48,    38,    55,    56,    10,    10,
      10,    10,    10,    10,    36,    45,    46,    43,    19,    42,
      75,    45,    24,    45,    80,    45,     3,    11,    67,    82,
      83,    84,    85,    86,    87,    87,    87,    87,    87,    87,
      88,    88,    78,    78,    78,    71,    19,    80,    79,    80,
      63,    63,    43,    29,    63,    27,    80,    43,    11,    11,
      26,     3,    19,    76,    23,    25,    63,    63,    44,    11,
      70,    25,    45
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    59,    59,    60,    60,    61,    61,    61,
      61,    61,    61,    62,    63,    63,    64,    64,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    66,    66,
      67,    67,    68,    69,    69,    69,    70,    70,    71,    72,
      72,    73,    73,    74,    74,    74,    75,    75,    76,    76,
      77,    77,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    79,    79,    80,    81,
      81,    82,    82,    83,    83,    84,    84,    85,    85,    86,
      86,    86,    86,    86,    86,    86,    87,    87,    87,    88,
      88,    88,    88
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     5,     5,     5,
       5,     5,     5,     5,     0,     1,     1,     2,     1,     1,
       4,     7,     1,     7,     4,     9,     7,     5,     1,     2,
       3,     5,     2,     4,     3,     1,     1,     3,     1,     0,
       1,     1,     2,     1,     1,     1,     0,     2,     0,     4,
       3,     6,     1,     1,     1,     1,     1,     1,     4,     3,
       1,     2,     2,     2,     2,     2,     0,     1,     1,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, context, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, context); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, LinuxSampler::ParserContext* context)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (context);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, LinuxSampler::ParserContext* context)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, context);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, LinuxSampler::ParserContext* context)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), context);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, context); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, LinuxSampler::ParserContext* context)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (context);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (LinuxSampler::ParserContext* context)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= END_OF_FILE)
    {
      yychar = END_OF_FILE;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* script: sections  */
#line 112 "parser.y"
              {
        (yyval.nEventHandlers) = context->handlers = (yyvsp[0].nEventHandlers);
    }
#line 1841 "y.tab.c"
    break;

  case 3: /* sections: section  */
#line 117 "parser.y"
             {
        (yyval.nEventHandlers) = new EventHandlers();
        if ((yyvsp[0].nEventHandler)) (yyval.nEventHandlers)->add((yyvsp[0].nEventHandler));
    }
#line 1850 "y.tab.c"
    break;

  case 4: /* sections: sections section  */
#line 121 "parser.y"
                        {
        (yyval.nEventHandlers) = (yyvsp[-1].nEventHandlers);
        if ((yyvsp[0].nEventHandler)) (yyval.nEventHandlers)->add((yyvsp[0].nEventHandler));
    }
#line 1859 "y.tab.c"
    break;

  case 5: /* section: function_declaration  */
#line 127 "parser.y"
                          {
        (yyval.nEventHandler) = EventHandlerRef();
    }
#line 1867 "y.tab.c"
    break;

  case 6: /* section: eventhandler  */
#line 130 "parser.y"
                    {
        (yyval.nEventHandler) = (yyvsp[0].nEventHandler);
    }
#line 1875 "y.tab.c"
    break;

  case 7: /* eventhandler: "keyword 'on'" "keyword 'note'" opt_statements "keyword 'end'" "keyword 'on'"  */
#line 135 "parser.y"
                                   {
        if (context->onNote)
            PARSE_ERR((yylsp[-3]), "Redeclaration of 'note' event handler.");
        context->onNote = new OnNote((yyvsp[-2].nStatements));
        (yyval.nEventHandler) = context->onNote;
    }
#line 1886 "y.tab.c"
    break;

  case 8: /* eventhandler: "keyword 'on'" "keyword 'init'" opt_statements "keyword 'end'" "keyword 'on'"  */
#line 141 "parser.y"
                                     {
        if (context->onInit)
            PARSE_ERR((yylsp[-3]), "Redeclaration of 'init' event handler.");
        context->onInit = new OnInit((yyvsp[-2].nStatements));
        (yyval.nEventHandler) = context->onInit;
    }
#line 1897 "y.tab.c"
    break;

  case 9: /* eventhandler: "keyword 'on'" "keyword 'release'" opt_statements "keyword 'end'" "keyword 'on'"  */
#line 147 "parser.y"
                                        {
        if (context->onRelease)
            PARSE_ERR((yylsp[-3]), "Redeclaration of 'release' event handler.");
        context->onRelease = new OnRelease((yyvsp[-2].nStatements));
        (yyval.nEventHandler) = context->onRelease;
    }
#line 1908 "y.tab.c"
    break;

  case 10: /* eventhandler: "keyword 'on'" "keyword 'controller'" opt_statements "keyword 'end'" "keyword 'on'"  */
#line 153 "parser.y"
                                           {
        if (context->onController)
            PARSE_ERR((yylsp[-3]), "Redeclaration of 'controller' event handler.");
        context->onController = new OnController((yyvsp[-2].nStatements));
        (yyval.nEventHandler) = context->onController;
    }
#line 1919 "y.tab.c"
    break;

  case 11: /* eventhandler: "keyword 'on'" "keyword 'rpn'" opt_statements "keyword 'end'" "keyword 'on'"  */
#line 159 "parser.y"
                                    {
        if (context->onRpn)
            PARSE_ERR((yylsp[-3]), "Redeclaration of 'rpn' event handler.");
        context->onRpn = new OnRpn((yyvsp[-2].nStatements));
        (yyval.nEventHandler) = context->onRpn;
    }
#line 1930 "y.tab.c"
    break;

  case 12: /* eventhandler: "keyword 'on'" "keyword 'nrpn'" opt_statements "keyword 'end'" "keyword 'on'"  */
#line 165 "parser.y"
                                     {
        if (context->onNrpn)
            PARSE_ERR((yylsp[-3]), "Redeclaration of 'nrpn' event handler.");
        context->onNrpn = new OnNrpn((yyvsp[-2].nStatements));
        (yyval.nEventHandler) = context->onNrpn;
    }
#line 1941 "y.tab.c"
    break;

  case 13: /* function_declaration: "keyword 'function'" "function name" opt_statements "keyword 'end'" "keyword 'function'"  */
#line 173 "parser.y"
                                                     {
        const char* name = (yyvsp[-3].sValue);
        if (context->functionProvider->functionByName(name)) {
            PARSE_ERR((yylsp[-3]), (String("There is already a built-in function with name '") + name + "'.").c_str());
        } else if (context->userFunctionByName(name)) {
            PARSE_ERR((yylsp[-3]), (String("There is already a user defined function with name '") + name + "'.").c_str());
        } else {
            context->userFnTable[name] = new UserFunction((yyvsp[-2].nStatements));
        }
    }
#line 1956 "y.tab.c"
    break;

  case 14: /* opt_statements: %empty  */
#line 185 "parser.y"
                                    {
        (yyval.nStatements) = new Statements();
    }
#line 1964 "y.tab.c"
    break;

  case 15: /* opt_statements: statements  */
#line 188 "parser.y"
                  {
        (yyval.nStatements) = (yyvsp[0].nStatements);
    }
#line 1972 "y.tab.c"
    break;

  case 16: /* statements: statement  */
#line 193 "parser.y"
               {
        (yyval.nStatements) = new Statements();
        if ((yyvsp[0].nStatement)) {
            if (!isNoOperation((yyvsp[0].nStatement))) (yyval.nStatements)->add((yyvsp[0].nStatement)); // filter out NoOperation statements
        } else 
            PARSE_WRN((yylsp[0]), "Not a statement.");
    }
#line 1984 "y.tab.c"
    break;

  case 17: /* statements: statements statement  */
#line 200 "parser.y"
                            {
        (yyval.nStatements) = (yyvsp[-1].nStatements);
        if ((yyvsp[0].nStatement)) {
            if (!isNoOperation((yyvsp[0].nStatement))) (yyval.nStatements)->add((yyvsp[0].nStatement)); // filter out NoOperation statements
        } else
            PARSE_WRN((yylsp[0]), "Not a statement.");
    }
#line 1996 "y.tab.c"
    break;

  case 18: /* statement: functioncall  */
#line 209 "parser.y"
                  {
        (yyval.nStatement) = (yyvsp[0].nFunctionCall);
    }
#line 2004 "y.tab.c"
    break;

  case 19: /* statement: userfunctioncall  */
#line 212 "parser.y"
                        {
        (yyval.nStatement) = (yyvsp[0].nStatements);
    }
#line 2012 "y.tab.c"
    break;

  case 20: /* statement: "keyword 'declare'" opt_qualifiers "variable name" opt_assignment  */
#line 215 "parser.y"
                                                      {
        (yyval.nStatement) = new NoOperation; // just as default result value
        const bool qConst      = (yyvsp[-2].varQualifier) & QUALIFIER_CONST;
        const bool qPolyphonic = (yyvsp[-2].varQualifier) & QUALIFIER_POLYPHONIC;
        const bool qPatch      = (yyvsp[-2].varQualifier) & QUALIFIER_PATCH;
        const char* name = (yyvsp[-1].sValue);
        ExpressionRef expr = (yyvsp[0].nExpression);
        //printf("declared var '%s'\n", name);
        const ExprType_t declType = exprTypeOfVarName(name);
        if (qPatch)
            context->patchVars[name].nameBlock = CODE_BLOCK((yylsp[-1]));
        if (context->variableByName(name)) {
            PARSE_ERR((yylsp[-1]), (String("Redeclaration of variable '") + name + "'.").c_str());
        } else if (qConst && !expr) {
            PARSE_ERR((yylsp[-2]), (String("Variable '") + name + "' declared const without value assignment.").c_str());
        } else if (qConst && qPolyphonic) {
            PARSE_ERR((yylsp[-2]), (String("Variable '") + name + "' must not be declared both const and polyphonic.").c_str());
        } else {
            if (!expr) {
                if (qPolyphonic) {
                    if (name[0] != '$' && name[0] != '~') {
                        PARSE_ERR((yylsp[-1]), "Polyphonic variables must only be declared either as integer or real number type.");
                    } else if (name[0] == '~') {
                        context->vartable[name] = new PolyphonicRealVariable({
                            .ctx = context
                        });
                    } else {
                        context->vartable[name] = new PolyphonicIntVariable({
                            .ctx = context
                        });
                    }
                } else {
                    if (name[0] == '@') {
                        context->vartable[name] = new StringVariable(context);
                    } else if (name[0] == '~') {
                        context->vartable[name] = new RealVariable({
                            .ctx = context
                        });
                    } else if (name[0] == '$') {
                        context->vartable[name] = new IntVariable({
                            .ctx = context
                        });
                    } else if (name[0] == '?') {
                        PARSE_ERR((yylsp[-1]), (String("Real number array variable '") + name + "' declaration requires array size.").c_str());
                    } else if (name[0] == '%') {
                        PARSE_ERR((yylsp[-1]), (String("Integer array variable '") + name + "' declaration requires array size.").c_str());
                    } else {
                        PARSE_ERR((yylsp[-1]), (String("Variable '") + name + "' declared with unknown type.").c_str());
                    }
                }
            } else {
                if (qPatch)
                    context->patchVars[name].exprBlock = ASSIGNED_EXPR_BLOCK((yylsp[0]));
                if (qPolyphonic && !isNumber(expr->exprType())) {
                    PARSE_ERR((yylsp[-1]), "Polyphonic variables must only be declared either as integer or real number type.");
                } else if (expr->exprType() == STRING_EXPR) {
                    if (name[0] != '@')
                        PARSE_WRN((yylsp[-1]), (String("Variable '") + name + "' declared as " + typeStr(declType) + ", string expression assigned though.").c_str());
                    StringExprRef strExpr = expr;
                    String s;
                    if (qConst) {
                        if (strExpr->isConstExpr())
                            s = strExpr->evalStr();
                        else
                            PARSE_ERR((yylsp[0]), (String("Assignment to const string variable '") + name + "' requires const expression.").c_str());
                        ConstStringVariableRef var = new ConstStringVariable(context, s);
                        context->vartable[name] = var;
                    } else {
                        if (strExpr->isConstExpr()) {
                            s = strExpr->evalStr();
                            StringVariableRef var = new StringVariable(context);
                            context->vartable[name] = var;
                            (yyval.nStatement) = new Assignment(var, new StringLiteral(s));
                        } else {
                            StringVariableRef var = new StringVariable(context);
                            context->vartable[name] = var;
                            (yyval.nStatement) = new Assignment(var, strExpr);
                        }
                    }
                } else if (expr->exprType() == REAL_EXPR) {
                    if (name[0] != '~')
                        PARSE_WRN((yylsp[-1]), (String("Variable '") + name + "' declared as " + typeStr(declType) + ", real number expression assigned though.").c_str());
                    RealExprRef realExpr = expr;
                    if (qConst) {
                        if (!realExpr->isConstExpr()) {
                            PARSE_ERR((yylsp[0]), (String("Assignment to const real number variable '") + name + "' requires const expression.").c_str());
                        }
                        ConstRealVariableRef var = new ConstRealVariable(
                            #if defined(__GNUC__) && !defined(__clang__)
                            (const RealVarDef&) // GCC 8.x requires this cast here (looks like a GCC bug to me); cast would cause an error with clang though
                            #endif
                        {
                            .value = (realExpr->isConstExpr()) ? realExpr->evalReal() : vmfloat(0),
                            .unitFactor = (realExpr->isConstExpr()) ? realExpr->unitFactor() : VM_NO_FACTOR,
                            .unitType = realExpr->unitType(),
                            .isFinal = realExpr->isFinal()
                        });
                        context->vartable[name] = var;
                    } else {
                        RealVariableRef var = new RealVariable({
                            .ctx = context,
                            .unitType = realExpr->unitType(),
                            .isFinal = realExpr->isFinal()
                        });
                        if (realExpr->isConstExpr()) {
                            (yyval.nStatement) = new Assignment(var, new RealLiteral({
                                .value = realExpr->evalReal(),
                                .unitFactor = realExpr->unitFactor(),
                                .unitType = realExpr->unitType(),
                                .isFinal = realExpr->isFinal()
                            }));
                        } else {
                            (yyval.nStatement) = new Assignment(var, realExpr);
                        }
                        context->vartable[name] = var;
                    }
                } else if (expr->exprType() == INT_EXPR) {
                    if (name[0] != '$')
                        PARSE_WRN((yylsp[-1]), (String("Variable '") + name + "' declared as " + typeStr(declType) + ", integer expression assigned though.").c_str());
                    IntExprRef intExpr = expr;
                    if (qConst) {
                        if (!intExpr->isConstExpr()) {
                            PARSE_ERR((yylsp[0]), (String("Assignment to const integer variable '") + name + "' requires const expression.").c_str());
                        }
                        ConstIntVariableRef var = new ConstIntVariable(
                            #if defined(__GNUC__) && !defined(__clang__)
                            (const IntVarDef&) // GCC 8.x requires this cast here (looks like a GCC bug to me); cast would cause an error with clang though
                            #endif
                        {
                            .value = (intExpr->isConstExpr()) ? intExpr->evalInt() : 0,
                            .unitFactor = (intExpr->isConstExpr()) ? intExpr->unitFactor() : VM_NO_FACTOR,
                            .unitType = intExpr->unitType(),
                            .isFinal = intExpr->isFinal()
                        });
                        context->vartable[name] = var;
                    } else {
                        IntVariableRef var = new IntVariable({
                            .ctx = context,
                            .unitType = intExpr->unitType(),
                            .isFinal = intExpr->isFinal()
                        });
                        if (intExpr->isConstExpr()) {
                            (yyval.nStatement) = new Assignment(var, new IntLiteral({
                                .value = intExpr->evalInt(),
                                .unitFactor = intExpr->unitFactor(),
                                .unitType = intExpr->unitType(),
                                .isFinal = intExpr->isFinal()
                            }));
                        } else {
                            (yyval.nStatement) = new Assignment(var, intExpr);
                        }
                        context->vartable[name] = var;
                    }
                } else if (expr->exprType() == EMPTY_EXPR) {
                    PARSE_ERR((yylsp[0]), "Expression does not result in a value.");
                    (yyval.nStatement) = new NoOperation;
                } else if (isArray(expr->exprType())) {
                    PARSE_ERR((yylsp[-1]), (String("Variable '") + name + "' declared as scalar type, array expression assigned though.").c_str());
                    (yyval.nStatement) = new NoOperation;
                }
            }
        }
    }
#line 2180 "y.tab.c"
    break;

  case 21: /* statement: "keyword 'declare'" opt_qualifiers "variable name" '[' opt_expr ']' opt_arr_assignment  */
#line 378 "parser.y"
                                                                           {
        (yyval.nStatement) = new NoOperation; // just as default result value
        const bool qConst      = (yyvsp[-5].varQualifier) & QUALIFIER_CONST;
        const bool qPolyphonic = (yyvsp[-5].varQualifier) & QUALIFIER_POLYPHONIC;
        const bool qPatch      = (yyvsp[-5].varQualifier) & QUALIFIER_PATCH;
        const char* name = (yyvsp[-4].sValue);
        if (qPatch)
            context->patchVars[name].nameBlock = CODE_BLOCK((yylsp[-4]));
        if ((yyvsp[-2].nExpression) && !(yyvsp[-2].nExpression)->isConstExpr()) {
            PARSE_ERR((yylsp[-2]), (String("Array variable '") + name + "' must be declared with constant array size.").c_str());
        } else if ((yyvsp[-2].nExpression) && (yyvsp[-2].nExpression)->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[-2]), (String("Size of array variable '") + name + "' declared with non integer expression.").c_str());
        } else if (context->variableByName(name)) {
            PARSE_ERR((yylsp[-4]), (String("Redeclaration of variable '") + name + "'.").c_str());
        } else if (qConst && !(yyvsp[0].nArgs)) {
            PARSE_ERR((yylsp[-5]), (String("Array variable '") + name + "' declared const without value assignment.").c_str());
        } else if (qPolyphonic) {
            PARSE_ERR((yylsp[-5]), (String("Array variable '") + name + "' must not be declared polyphonic.").c_str());
        } else {
            IntExprRef sizeExpr = (yyvsp[-2].nExpression);
            ArgsRef args = (yyvsp[0].nArgs);
            vmint size = (sizeExpr) ? sizeExpr->evalInt() : (args) ? args->argsCount() : 0;
            if (size == 0)
                PARSE_WRN((yylsp[-2]), (String("Array variable '") + name + "' declared with zero array size.").c_str());
            if (size < 0) {
                PARSE_ERR((yylsp[-2]), (String("Array variable '") + name + "' must not be declared with negative array size.").c_str());
            } else if (sizeExpr && (sizeExpr->unitType() || sizeExpr->hasUnitFactorNow())) {
                PARSE_ERR((yylsp[-2]), "Units are not allowed as array size.");
            } else {
                if (sizeExpr && sizeExpr->isFinal())
                    PARSE_WRN((yylsp[-2]), "Final operator '!' is meaningless here.");
                if (!args) {
                    if (name[0] == '?') {
                        context->vartable[name] = new RealArrayVariable(context, size);
                    } else if (name[0] == '%') {
                        context->vartable[name] = new IntArrayVariable(context, size);
                    } else {
                        PARSE_ERR((yylsp[-4]), (String("Variable '") + name + "' declared as unknown array type: use either '%' or '?' instead of '" + String(name).substr(0,1) + "'.").c_str());
                    }
                } else {
                    if (qPatch)
                        context->patchVars[name].exprBlock = ASSIGNED_EXPR_BLOCK((yylsp[0]));
                    if (size == 0)
                        PARSE_WRN((yylsp[-2]), (String("Array variable '") + name + "' declared with zero array size.").c_str());
                    if (size < 0) {
                        PARSE_ERR((yylsp[-2]), (String("Array variable '") + name + "' must not be declared with negative array size.").c_str());
                    } else if (args->argsCount() > size) {
                        PARSE_ERR((yylsp[0]), (String("Array variable '") + name +
                                  "' was declared with size " + ToString(size) +
                                  " but " + ToString(args->argsCount()) +
                                  " values were assigned." ).c_str());
                    } else {
                        if (args->argsCount() < size) {
                            PARSE_WRN((yylsp[-2]), (String("Array variable '") + name +
                                      "' was declared with size " + ToString(size) +
                                      " but only " + ToString(args->argsCount()) +
                                      " values were assigned." ).c_str());
                        }
                        ExprType_t declType = EMPTY_EXPR;
                        if (name[0] == '%') {
                            declType = INT_EXPR;
                        } else if (name[0] == '?') {
                            declType = REAL_EXPR;
                        } else if (name[0] == '$') {
                            PARSE_ERR((yylsp[-4]), (String("Variable '") + name + "' declaration ambiguous: Use '%' as name prefix for integer arrays instead of '$'.").c_str());
                        } else if (name[0] == '~') {
                            PARSE_ERR((yylsp[-4]), (String("Variable '") + name + "' declaration ambiguous: Use '?' as name prefix for real number arrays instead of '~'.").c_str());
                        } else {
                            PARSE_ERR((yylsp[-4]), (String("Variable '") + name + "' declared as unknown array type: use either '%' or '?' instead of '" + String(name).substr(0,1) + "'.").c_str());
                        }
                        bool argsOK = true;
                        if (declType == EMPTY_EXPR) {
                            argsOK = false;
                        } else {
                            for (vmint i = 0; i < args->argsCount(); ++i) {
                                if (args->arg(i)->exprType() != declType) {
                                    PARSE_ERR(
                                        (yylsp[0]),
                                        (String("Array variable '") + name +
                                        "' declared with invalid assignment values. Assigned element " +
                                        ToString(i+1) + " is not an " + typeStr(declType) + " expression.").c_str()
                                    );
                                    argsOK = false;
                                    break;
                                } else if (qConst && !args->arg(i)->isConstExpr()) {
                                    PARSE_ERR(
                                        (yylsp[0]),
                                        (String("const array variable '") + name +
                                        "' must be defined with const values. Assigned element " +
                                        ToString(i+1) + " is not a const expression though.").c_str()
                                    );
                                    argsOK = false;
                                    break;
                                } else if (args->arg(i)->asNumber()->unitType()) {
                                    PARSE_ERR(
                                        (yylsp[0]),
                                        (String("Array variable '") + name +
                                        "' declared with invalid assignment values. Assigned element " +
                                        ToString(i+1) + " contains a unit type, only metric prefixes are allowed for arrays.").c_str()
                                    );
                                    argsOK = false;
                                    break;
                                } else if (args->arg(i)->asNumber()->isFinal()) {
                                    PARSE_ERR(
                                        (yylsp[0]),
                                        (String("Array variable '") + name +
                                        "' declared with invalid assignment values. Assigned element " +
                                        ToString(i+1) + " declared as 'final' value.").c_str()
                                    );
                                    argsOK = false;
                                    break;
                                }
                            }
                        }
                        if (argsOK) {
                            if (declType == REAL_EXPR)
                                context->vartable[name] = new RealArrayVariable(context, size, args, qConst);
                            else
                                context->vartable[name] = new IntArrayVariable(context, size, args, qConst);
                        }
                    }
                }
            }
        }
    }
#line 2310 "y.tab.c"
    break;

  case 22: /* statement: assignment  */
#line 503 "parser.y"
                  {
        (yyval.nStatement) = (yyvsp[0].nStatement);
    }
#line 2318 "y.tab.c"
    break;

  case 23: /* statement: "keyword 'while'" '(' expr ')' opt_statements "keyword 'end'" "keyword 'while'"  */
#line 506 "parser.y"
                                                   {
        if ((yyvsp[-4].nExpression)->exprType() == INT_EXPR) {
            IntExprRef expr = (yyvsp[-4].nExpression);
            if (expr->asNumber()->unitType() ||
                expr->asNumber()->hasUnitFactorEver())
                PARSE_WRN((yylsp[-4]), "Condition for 'while' loops contains a unit.");
            else if (expr->isFinal() && expr->isConstExpr())
                PARSE_WRN((yylsp[-4]), "Final operator '!' is meaningless here.");
            (yyval.nStatement) = new While(expr, (yyvsp[-2].nStatements));
        } else {
            PARSE_ERR((yylsp[-4]), "Condition for 'while' loops must be integer expression.");
            (yyval.nStatement) = new While(new IntLiteral({ .value = 0 }), (yyvsp[-2].nStatements));
        }
    }
#line 2337 "y.tab.c"
    break;

  case 24: /* statement: "keyword 'synchronized'" opt_statements "keyword 'end'" "keyword 'synchronized'"  */
#line 520 "parser.y"
                                                    {
        (yyval.nStatement) = new SyncBlock((yyvsp[-2].nStatements));
    }
#line 2345 "y.tab.c"
    break;

  case 25: /* statement: "keyword 'if'" '(' expr ')' opt_statements "keyword 'else'" opt_statements "keyword 'end'" "keyword 'if'"  */
#line 523 "parser.y"
                                                                 {
        if ((yyvsp[-6].nExpression)->exprType() == INT_EXPR) {
            IntExprRef expr = (yyvsp[-6].nExpression);
            if (expr->asNumber()->unitType() ||
                expr->asNumber()->hasUnitFactorEver())
                PARSE_WRN((yylsp[-6]), "Condition for 'if' contains a unit.");
            else if (expr->isFinal() && expr->isConstExpr())
                PARSE_WRN((yylsp[-6]), "Final operator '!' is meaningless here.");
            (yyval.nStatement) = new If((yyvsp[-6].nExpression), (yyvsp[-4].nStatements), (yyvsp[-2].nStatements));
        } else {
            PARSE_ERR((yylsp[-6]), "Condition for 'if' must be integer expression.");
            (yyval.nStatement) = new If(new IntLiteral({ .value = 0 }), (yyvsp[-4].nStatements), (yyvsp[-2].nStatements));
        }
    }
#line 2364 "y.tab.c"
    break;

  case 26: /* statement: "keyword 'if'" '(' expr ')' opt_statements "keyword 'end'" "keyword 'if'"  */
#line 537 "parser.y"
                                             {
        if ((yyvsp[-4].nExpression)->exprType() == INT_EXPR) {
            IntExprRef expr = (yyvsp[-4].nExpression);
            if (expr->asNumber()->unitType() ||
                expr->asNumber()->hasUnitFactorEver())
                PARSE_WRN((yylsp[-4]), "Condition for 'if' contains a unit.");
            else if (expr->isFinal() && expr->isConstExpr())
                PARSE_WRN((yylsp[-4]), "Final operator '!' is meaningless here.");
            (yyval.nStatement) = new If((yyvsp[-4].nExpression), (yyvsp[-2].nStatements));
        } else {
            PARSE_ERR((yylsp[-4]), "Condition for 'if' must be integer expression.");
            (yyval.nStatement) = new If(new IntLiteral({ .value = 0 }), (yyvsp[-2].nStatements));
        }
    }
#line 2383 "y.tab.c"
    break;

  case 27: /* statement: "keyword 'select'" expr caseclauses "keyword 'end'" "keyword 'select'"  */
#line 551 "parser.y"
                                          {
        if ((yyvsp[-3].nExpression)->exprType() == INT_EXPR) {
            IntExprRef expr = (yyvsp[-3].nExpression);
            if (expr->unitType() || expr->hasUnitFactorEver()) {
                PARSE_ERR((yylsp[-3]), "Units are not allowed here.");
                (yyval.nStatement) = new SelectCase(new IntLiteral({ .value = 0 }), (yyvsp[-2].nCaseBranches));
            } else {
                if (expr->isFinal() && expr->isConstExpr())
                    PARSE_WRN((yylsp[-3]), "Final operator '!' is meaningless here.");
                (yyval.nStatement) = new SelectCase(expr, (yyvsp[-2].nCaseBranches));
            }
        } else {
            PARSE_ERR((yylsp[-3]), "Statement 'select' can only by applied to integer expressions.");
            (yyval.nStatement) = new SelectCase(new IntLiteral({ .value = 0 }), (yyvsp[-2].nCaseBranches));
        }
    }
#line 2404 "y.tab.c"
    break;

  case 28: /* caseclauses: caseclause  */
#line 569 "parser.y"
                {
        (yyval.nCaseBranches) = CaseBranches();
        (yyval.nCaseBranches).push_back((yyvsp[0].nCaseBranch));
    }
#line 2413 "y.tab.c"
    break;

  case 29: /* caseclauses: caseclauses caseclause  */
#line 573 "parser.y"
                              {
        (yyval.nCaseBranches) = (yyvsp[-1].nCaseBranches);
        (yyval.nCaseBranches).push_back((yyvsp[0].nCaseBranch));
    }
#line 2422 "y.tab.c"
    break;

  case 30: /* caseclause: "keyword 'case'" "integer literal" opt_statements  */
#line 579 "parser.y"
                                 {
        (yyval.nCaseBranch) = CaseBranch();
        (yyval.nCaseBranch).from = new IntLiteral({ .value = (yyvsp[-1].iValue) });
        (yyval.nCaseBranch).statements = (yyvsp[0].nStatements);
    }
#line 2432 "y.tab.c"
    break;

  case 31: /* caseclause: "keyword 'case'" "integer literal" "keyword 'to'" "integer literal" opt_statements  */
#line 584 "parser.y"
                                              {
        (yyval.nCaseBranch) = CaseBranch();
        (yyval.nCaseBranch).from = new IntLiteral({ .value = (yyvsp[-3].iValue) });
        (yyval.nCaseBranch).to   = new IntLiteral({ .value = (yyvsp[-1].iValue) });
        (yyval.nCaseBranch).statements = (yyvsp[0].nStatements);
    }
#line 2443 "y.tab.c"
    break;

  case 32: /* userfunctioncall: "keyword 'call'" "function name"  */
#line 592 "parser.y"
                     {
        const char* name = (yyvsp[0].sValue);
        UserFunctionRef fn = context->userFunctionByName(name);
        if (context->functionProvider->functionByName(name)) {
            PARSE_ERR((yylsp[-1]), (String("Keyword 'call' must only be used for user defined functions, not for any built-in function like '") + name + "'.").c_str());
            (yyval.nStatements) = StatementsRef();
        } else if (!fn) {
            PARSE_ERR((yylsp[0]), (String("No user defined function with name '") + name + "'.").c_str());
            (yyval.nStatements) = StatementsRef();
        } else {
            (yyval.nStatements) = fn;
        }
    }
#line 2461 "y.tab.c"
    break;

  case 33: /* functioncall: "function name" '(' args ')'  */
#line 607 "parser.y"
                             {
        const char* name = (yyvsp[-3].sValue);
        //printf("function call of '%s' with args\n", name);
        ArgsRef args = (yyvsp[-1].nArgs);
        VMFunction* fn = context->functionProvider->functionByName(name);
        if (context->userFunctionByName(name)) {
            PARSE_ERR((yylsp[-3]), (String("Missing 'call' keyword before user defined function name '") + name + "'.").c_str());
            (yyval.nFunctionCall) = new FunctionCall(name, args, NULL);
        } else if (!fn) {
            PARSE_ERR((yylsp[-3]), (String("No built-in function with name '") + name + "'.").c_str());
            (yyval.nFunctionCall) = new FunctionCall(name, args, NULL);
        } else if (context->functionProvider->isFunctionDisabled(fn,context)) {
            PARSE_DROP((yyloc));
            (yyval.nFunctionCall) = new NoFunctionCall;
        } else if (args->argsCount() < fn->minRequiredArgs()) {
            PARSE_ERR((yylsp[-1]), (String("Built-in function '") + name + "' requires at least " + ToString(fn->minRequiredArgs()) + " arguments.").c_str());
            (yyval.nFunctionCall) = new FunctionCall(name, args, NULL);
        } else if (args->argsCount() > fn->maxAllowedArgs()) {
            PARSE_ERR((yylsp[-1]), (String("Built-in function '") + name + "' accepts max. " + ToString(fn->maxAllowedArgs()) + " arguments.").c_str());
            (yyval.nFunctionCall) = new FunctionCall(name, args, NULL);
        } else {
            bool argsOK = true;
            for (vmint i = 0; i < args->argsCount(); ++i) {
                if (!fn->acceptsArgType(i, args->arg(i)->exprType())) {
                    PARSE_ERR((yylsp[-1]), (String("Argument ") + ToString(i+1) + " of built-in function '" + name + "' expects " + acceptedArgTypesStr(fn, i) + " type, but type " + typeStr(args->arg(i)->exprType()) + " was given instead.").c_str());
                    argsOK = false;
                    break;
                } else if (fn->modifiesArg(i) && !args->arg(i)->isModifyable()) {
                    PARSE_ERR((yylsp[-1]), (String("Argument ") + ToString(i+1) + " of built-in function '" + name + "' expects an assignable variable.").c_str());
                    argsOK = false;
                    break;
                } else if (isNumber(args->arg(i)->exprType()) && !fn->acceptsArgUnitType(i, args->arg(i)->asNumber()->unitType())) {
                    if (args->arg(i)->asNumber()->unitType())
                        PARSE_ERR((yylsp[-1]), (String("Argument ") + ToString(i+1) + " of built-in function '" + name + "' does not expect unit " + unitTypeStr(args->arg(i)->asNumber()->unitType()) +  ".").c_str());
                    else
                        PARSE_ERR((yylsp[-1]), (String("Argument ") + ToString(i+1) + " of built-in function '" + name + "' expects a unit.").c_str());
                    argsOK = false;
                    break;
                } else if (isNumber(args->arg(i)->exprType()) && args->arg(i)->asNumber()->hasUnitFactorEver() && !fn->acceptsArgUnitPrefix(i, args->arg(i)->asNumber()->unitType())) {
                    if (args->arg(i)->asNumber()->unitType())
                        PARSE_ERR((yylsp[-1]), (String("Argument ") + ToString(i+1) + " of built-in function '" + name + "' does not expect a unit prefix for unit" + unitTypeStr(args->arg(i)->asNumber()->unitType()) + ".").c_str());
                    else
                        PARSE_ERR((yylsp[-1]), (String("Argument ") + ToString(i+1) + " of built-in function '" + name + "' does not expect a unit prefix.").c_str());
                    argsOK = false;
                    break;
                } else if (!fn->acceptsArgFinal(i) && isNumber(args->arg(i)->exprType()) && args->arg(i)->asNumber()->isFinal()) {
                    PARSE_ERR((yylsp[-1]), (String("Argument ") + ToString(i+1) + " of built-in function '" + name + "' does not expect a \"final\" value.").c_str());
                    argsOK = false;
                    break;
                }
            }
            if (argsOK) {
                // perform built-in function's own, custom arguments checks (if any)
                fn->checkArgs(&*args, [&](String err) {
                    PARSE_ERR((yylsp[-1]), (String("Built-in function '") + name + "()': " + err).c_str());
                    argsOK = false;
                }, [&](String wrn) {
                    PARSE_WRN((yylsp[-1]), (String("Built-in function '") + name + "()': " + wrn).c_str());
                });
            }
            (yyval.nFunctionCall) = new FunctionCall(name, args, argsOK ? fn : NULL);
        }
    }
#line 2529 "y.tab.c"
    break;

  case 34: /* functioncall: "function name" '(' ')'  */
#line 670 "parser.y"
                         {
        const char* name = (yyvsp[-2].sValue);
        //printf("function call of '%s' (with empty args)\n", name);
        ArgsRef args = new Args;
        VMFunction* fn = context->functionProvider->functionByName(name);
        if (context->userFunctionByName(name)) {
            PARSE_ERR((yylsp[-2]), (String("Missing 'call' keyword before user defined function name '") + name + "'.").c_str());
            (yyval.nFunctionCall) = new FunctionCall(name, args, NULL);
        } else if (!fn) {
            PARSE_ERR((yylsp[-2]), (String("No built-in function with name '") + name + "'.").c_str());
            (yyval.nFunctionCall) = new FunctionCall(name, args, NULL);
        } else if (context->functionProvider->isFunctionDisabled(fn,context)) {
            PARSE_DROP((yyloc));
            (yyval.nFunctionCall) = new NoFunctionCall;
        } else if (fn->minRequiredArgs() > 0) {
            PARSE_ERR((yylsp[0]), (String("Built-in function '") + name + "' requires at least " + ToString(fn->minRequiredArgs()) + " arguments.").c_str());
            (yyval.nFunctionCall) = new FunctionCall(name, args, NULL);
        } else {
            (yyval.nFunctionCall) = new FunctionCall(name, args, fn);
        }
    }
#line 2555 "y.tab.c"
    break;

  case 35: /* functioncall: "function name"  */
#line 691 "parser.y"
                  {
        const char* name = (yyvsp[0].sValue);
        //printf("function call of '%s' (without args)\n", name);
        ArgsRef args = new Args;
        VMFunction* fn = context->functionProvider->functionByName(name);
        if (context->userFunctionByName(name)) {
            PARSE_ERR((yylsp[0]), (String("Missing 'call' keyword before user defined function name '") + name + "'.").c_str());
            (yyval.nFunctionCall) = new FunctionCall(name, args, NULL);
        } else if (!fn) {
            PARSE_ERR((yylsp[0]), (String("No built-in function with name '") + name + "'.").c_str());
            (yyval.nFunctionCall) = new FunctionCall(name, args, NULL);
        } else if (context->functionProvider->isFunctionDisabled(fn,context)) {
            PARSE_DROP((yyloc));
            (yyval.nFunctionCall) = new NoFunctionCall;
        } else if (fn->minRequiredArgs() > 0) {
            PARSE_ERR((yylsp[0]), (String("Built-in function '") + name + "' requires at least " + ToString(fn->minRequiredArgs()) + " arguments.").c_str());
            (yyval.nFunctionCall) = new FunctionCall(name, args, NULL);
        } else {
            (yyval.nFunctionCall) = new FunctionCall(name, args, fn);
        }
    }
#line 2581 "y.tab.c"
    break;

  case 36: /* args: arg  */
#line 714 "parser.y"
         {
        (yyval.nArgs) = new Args();
        (yyval.nArgs)->add((yyvsp[0].nExpression));
    }
#line 2590 "y.tab.c"
    break;

  case 37: /* args: args ',' arg  */
#line 718 "parser.y"
                    {
        (yyval.nArgs) = (yyvsp[-2].nArgs);
        (yyval.nArgs)->add((yyvsp[0].nExpression));
    }
#line 2599 "y.tab.c"
    break;

  case 39: /* opt_qualifiers: %empty  */
#line 727 "parser.y"
                                    {
        (yyval.varQualifier) = QUALIFIER_NONE;
    }
#line 2607 "y.tab.c"
    break;

  case 40: /* opt_qualifiers: qualifiers  */
#line 730 "parser.y"
                  {
        (yyval.varQualifier) = (yyvsp[0].varQualifier);
    }
#line 2615 "y.tab.c"
    break;

  case 41: /* qualifiers: qualifier  */
#line 735 "parser.y"
               {
        (yyval.varQualifier) = (yyvsp[0].varQualifier);
    }
#line 2623 "y.tab.c"
    break;

  case 42: /* qualifiers: qualifiers qualifier  */
#line 738 "parser.y"
                            {
        if ((yyvsp[-1].varQualifier) & (yyvsp[0].varQualifier))
            PARSE_ERR((yylsp[0]), ("Qualifier '" + qualifierStr((yyvsp[0].varQualifier)) + "' must only be listed once.").c_str());
        (yyval.varQualifier) = (Qualifier_t) ((yyvsp[-1].varQualifier) | (yyvsp[0].varQualifier));
    }
#line 2633 "y.tab.c"
    break;

  case 43: /* qualifier: "keyword 'const'"  */
#line 745 "parser.y"
            {
        (yyval.varQualifier) = QUALIFIER_CONST;
    }
#line 2641 "y.tab.c"
    break;

  case 44: /* qualifier: "keyword 'polyphonic'"  */
#line 748 "parser.y"
                  {
        (yyval.varQualifier) = QUALIFIER_POLYPHONIC;
    }
#line 2649 "y.tab.c"
    break;

  case 45: /* qualifier: "keyword 'patch'"  */
#line 751 "parser.y"
             {
        (yyval.varQualifier) = QUALIFIER_PATCH;
    }
#line 2657 "y.tab.c"
    break;

  case 46: /* opt_assignment: %empty  */
#line 756 "parser.y"
                                    {
        (yyval.nExpression) = ExpressionRef();
    }
#line 2665 "y.tab.c"
    break;

  case 47: /* opt_assignment: "operator ':='" expr  */
#line 759 "parser.y"
                       {
        (yyval.nExpression) = (yyvsp[0].nExpression);
    }
#line 2673 "y.tab.c"
    break;

  case 48: /* opt_arr_assignment: %empty  */
#line 764 "parser.y"
                                    {
        (yyval.nArgs) = ArgsRef();
    }
#line 2681 "y.tab.c"
    break;

  case 49: /* opt_arr_assignment: "operator ':='" '(' args ')'  */
#line 767 "parser.y"
                               {
        (yyval.nArgs) = (yyvsp[-1].nArgs);
    }
#line 2689 "y.tab.c"
    break;

  case 50: /* assignment: "variable name" "operator ':='" expr  */
#line 772 "parser.y"
                              {
        //printf("variable lookup with name '%s' as assignment expr\n", $1);
        const char* name = (yyvsp[-2].sValue);
        VariableRef var = context->variableByName(name);
        if (!var)
            PARSE_ERR((yylsp[-2]), (String("Variable assignment: No variable declared with name '") + name + "'.").c_str());
        else if (var->isConstExpr())
            PARSE_ERR((yylsp[-1]), (String("Variable assignment: Cannot modify const variable '") + name + "'.").c_str());
        else if (!var->isAssignable())
            PARSE_ERR((yylsp[-1]), (String("Variable assignment: Variable '") + name + "' is not assignable.").c_str());
        else if (var->exprType() != (yyvsp[0].nExpression)->exprType())
            PARSE_ERR((yylsp[0]), (String("Variable assignment: Variable '") + name + "' is of type " + typeStr(var->exprType()) + ", assignment is of type " + typeStr((yyvsp[0].nExpression)->exprType()) + " though.").c_str());
        else if (isNumber(var->exprType())) {
            NumberVariableRef numberVar = var;
            NumberExprRef expr = (yyvsp[0].nExpression);
            if (numberVar->unitType() != expr->unitType())
                PARSE_ERR((yylsp[0]), (String("Variable assignment: Variable '") + name + "' has unit type " + unitTypeStr(numberVar->unitType()) + ", assignment has unit type " + unitTypeStr(expr->unitType()) + " though.").c_str());
            else if (numberVar->isFinal() != expr->isFinal())
                PARSE_ERR((yylsp[0]), (String("Variable assignment: Variable '") + name + "' was declared as " + String(numberVar->isFinal() ? "final" : "not final") + ", assignment is " + String(expr->isFinal() ? "final" : "not final") + " though.").c_str());
        }

        if (var)
            (yyval.nStatement) = new Assignment(var, (yyvsp[0].nExpression));
        else
            (yyval.nStatement) = new NoOperation;
    }
#line 2720 "y.tab.c"
    break;

  case 51: /* assignment: "variable name" '[' expr ']' "operator ':='" expr  */
#line 798 "parser.y"
                                             {
        const char* name = (yyvsp[-5].sValue);
        VariableRef var = context->variableByName(name);
        if (!var)
            PARSE_ERR((yylsp[-5]), (String("No variable declared with name '") + name + "'.").c_str());
        else if (!isArray(var->exprType()))
            PARSE_ERR((yylsp[-4]), (String("Variable '") + name + "' is not an array variable.").c_str());
        else if (var->isConstExpr())
            PARSE_ERR((yylsp[-1]), (String("Variable assignment: Cannot modify const array variable '") + name + "'.").c_str());
        else if (!var->isAssignable())
            PARSE_ERR((yylsp[-1]), (String("Variable assignment: Array variable '") + name + "' is not assignable.").c_str());
        else if ((yyvsp[-3].nExpression)->exprType() != INT_EXPR)
            PARSE_ERR((yylsp[-3]), (String("Array variable '") + name + "' accessed with non integer expression.").c_str());
        else if ((yyvsp[-3].nExpression)->asInt()->unitType())
            PARSE_ERR((yylsp[-3]), "Unit types are not allowed as array index.");
        else if ((yyvsp[0].nExpression)->exprType() != scalarTypeOfArray(var->exprType()))
            PARSE_ERR((yylsp[-1]), (String("Variable '") + name + "' was declared as " + typeStr(var->exprType()) + ", assigned expression is " + typeStr((yyvsp[0].nExpression)->exprType()) + " though.").c_str());
        else if ((yyvsp[0].nExpression)->asNumber()->unitType())
            PARSE_ERR((yylsp[0]), "Unit types are not allowed for array variables.");
        else if ((yyvsp[0].nExpression)->asNumber()->isFinal())
            PARSE_ERR((yylsp[0]), "Final operator '!' not allowed for array variables.");
        else if ((yyvsp[-3].nExpression)->isConstExpr() && (yyvsp[-3].nExpression)->asInt()->evalInt() >= ((ArrayExprRef)var)->arraySize())
            PARSE_WRN((yylsp[-3]), (String("Index ") + ToString((yyvsp[-3].nExpression)->asInt()->evalInt()) +
                          " exceeds size of array variable '" + name +
                          "' which was declared with size " +
                          ToString(((ArrayExprRef)var)->arraySize()) + ".").c_str());
        else if ((yyvsp[-3].nExpression)->asInt()->isFinal())
            PARSE_WRN((yylsp[-3]), "Final operator '!' is meaningless here.");

        if (!var) {
            (yyval.nStatement) = new NoOperation;
        } else if (var->exprType() == INT_ARR_EXPR) {
            IntArrayElementRef element = new IntArrayElement(var, (yyvsp[-3].nExpression));
            (yyval.nStatement) = new Assignment(element, (yyvsp[0].nExpression));
        } else if (var->exprType() == REAL_ARR_EXPR) {
            RealArrayElementRef element = new RealArrayElement(var, (yyvsp[-3].nExpression));
            (yyval.nStatement) = new Assignment(element, (yyvsp[0].nExpression));
        }
    }
#line 2764 "y.tab.c"
    break;

  case 52: /* unary_expr: "integer literal"  */
#line 839 "parser.y"
             {
        (yyval.nExpression) = new IntLiteral({ .value = (yyvsp[0].iValue) });
    }
#line 2772 "y.tab.c"
    break;

  case 53: /* unary_expr: "real number literal"  */
#line 842 "parser.y"
            {
        (yyval.nExpression) = new RealLiteral({ .value = (yyvsp[0].fValue) });
    }
#line 2780 "y.tab.c"
    break;

  case 54: /* unary_expr: "integer literal with unit"  */
#line 845 "parser.y"
                    {
        IntLiteralRef literal = new IntLiteral({
            .value = (yyvsp[0].iUnitValue).iValue,
            .unitFactor = VMUnit::unitFactor((yyvsp[0].iUnitValue).prefix),
            .unitType = (yyvsp[0].iUnitValue).unit
        });
        (yyval.nExpression) = literal;
    }
#line 2793 "y.tab.c"
    break;

  case 55: /* unary_expr: "real number literal with unit"  */
#line 853 "parser.y"
                 {
        RealLiteralRef literal = new RealLiteral({
            .value = (yyvsp[0].fUnitValue).fValue,
            .unitFactor = VMUnit::unitFactor((yyvsp[0].fUnitValue).prefix),
            .unitType = (yyvsp[0].fUnitValue).unit
        });
        (yyval.nExpression) = literal;
    }
#line 2806 "y.tab.c"
    break;

  case 56: /* unary_expr: "string literal"  */
#line 861 "parser.y"
                {
        (yyval.nExpression) = new StringLiteral((yyvsp[0].sValue));
    }
#line 2814 "y.tab.c"
    break;

  case 57: /* unary_expr: "variable name"  */
#line 864 "parser.y"
                {
        //printf("variable lookup with name '%s' as unary expr\n", $1);
        VariableRef var = context->variableByName((yyvsp[0].sValue));
        if (var)
            (yyval.nExpression) = var;
        else {
            PARSE_ERR((yylsp[0]), (String("No variable declared with name '") + (yyvsp[0].sValue) + "'.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        }
    }
#line 2829 "y.tab.c"
    break;

  case 58: /* unary_expr: "variable name" '[' expr ']'  */
#line 874 "parser.y"
                             {
        const char* name = (yyvsp[-3].sValue);
        VariableRef var = context->variableByName(name);
        if (!var) {
            PARSE_ERR((yylsp[-3]), (String("No variable declared with name '") + name + "'.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!isArray(var->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Variable '") + name + "' is not an array variable.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if ((yyvsp[-1].nExpression)->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[-1]), (String("Array variable '") + name + "' accessed with non integer expression.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if ((yyvsp[-1].nExpression)->asInt()->unitType() || (yyvsp[-1].nExpression)->asInt()->hasUnitFactorEver()) {
            PARSE_ERR((yylsp[-1]), "Units are not allowed as array index.");
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if ((yyvsp[-1].nExpression)->isConstExpr() && (yyvsp[-1].nExpression)->asInt()->evalInt() >= ((ArrayExprRef)var)->arraySize())
                PARSE_WRN((yylsp[-1]), (String("Index ") + ToString((yyvsp[-1].nExpression)->asInt()->evalInt()) +
                               " exceeds size of array variable '" + name +
                               "' which was declared with size " +
                               ToString(((ArrayExprRef)var)->arraySize()) + ".").c_str());
            else if ((yyvsp[-1].nExpression)->asInt()->isFinal())
                PARSE_WRN((yylsp[-1]), "Final operator '!' is meaningless here.");
            if (var->exprType() == REAL_ARR_EXPR) {
                (yyval.nExpression) = new RealArrayElement(var, (yyvsp[-1].nExpression));
            } else {
                (yyval.nExpression) = new IntArrayElement(var, (yyvsp[-1].nExpression));
            }
        }
    }
#line 2864 "y.tab.c"
    break;

  case 59: /* unary_expr: '(' expr ')'  */
#line 904 "parser.y"
                    {
        (yyval.nExpression) = (yyvsp[-1].nExpression);
    }
#line 2872 "y.tab.c"
    break;

  case 60: /* unary_expr: functioncall  */
#line 907 "parser.y"
                    {
        (yyval.nExpression) = (yyvsp[0].nFunctionCall);
    }
#line 2880 "y.tab.c"
    break;

  case 61: /* unary_expr: '+' unary_expr  */
#line 910 "parser.y"
                      {
        if (isNumber((yyvsp[0].nExpression)->exprType())) {
            (yyval.nExpression) = (yyvsp[0].nExpression);
        } else {
            PARSE_ERR((yylsp[0]), (String("Unary '+' operator requires number, is ") + typeStr((yyvsp[0].nExpression)->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        }
    }
#line 2893 "y.tab.c"
    break;

  case 62: /* unary_expr: '-' unary_expr  */
#line 918 "parser.y"
                      {
        if (isNumber((yyvsp[0].nExpression)->exprType())) {
            (yyval.nExpression) = new Neg((yyvsp[0].nExpression));
        } else {
            PARSE_ERR((yylsp[0]), (String("Unary '-' operator requires number, is ") + typeStr((yyvsp[0].nExpression)->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        }
    }
#line 2906 "y.tab.c"
    break;

  case 63: /* unary_expr: "bitwise operator '.not.'" unary_expr  */
#line 926 "parser.y"
                              {
        if ((yyvsp[0].nExpression)->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[0]), (String("Right operand of bitwise operator '.not.' must be an integer expression, is ") + typeStr((yyvsp[0].nExpression)->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if ((yyvsp[0].nExpression)->asInt()->unitType() || (yyvsp[0].nExpression)->asInt()->hasUnitFactorEver()) {
            PARSE_ERR((yylsp[0]), "Units are not allowed for operands of bitwise operations.");
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            (yyval.nExpression) = new BitwiseNot((yyvsp[0].nExpression));
        }
    }
#line 2922 "y.tab.c"
    break;

  case 64: /* unary_expr: "operator 'not'" unary_expr  */
#line 937 "parser.y"
                      {
        if ((yyvsp[0].nExpression)->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[0]), (String("Right operand of operator 'not' must be an integer expression, is ") + typeStr((yyvsp[0].nExpression)->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if ((yyvsp[0].nExpression)->asInt()->unitType() || (yyvsp[0].nExpression)->asInt()->hasUnitFactorEver()) {
            PARSE_ERR((yylsp[0]), "Units are not allowed for operands of logical operations.");
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            (yyval.nExpression) = new Not((yyvsp[0].nExpression));
        }
    }
#line 2938 "y.tab.c"
    break;

  case 65: /* unary_expr: '!' unary_expr  */
#line 948 "parser.y"
                      {
        if (!isNumber((yyvsp[0].nExpression)->exprType())) {
            PARSE_ERR((yylsp[0]), (String("Right operand of \"final\" operator '!' must be a scalar number expression, is ") + typeStr((yyvsp[0].nExpression)->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            (yyval.nExpression) = new Final((yyvsp[0].nExpression));
        }
    }
#line 2951 "y.tab.c"
    break;

  case 66: /* opt_expr: %empty  */
#line 958 "parser.y"
                                    {
        (yyval.nExpression) = NULL;
    }
#line 2959 "y.tab.c"
    break;

  case 67: /* opt_expr: expr  */
#line 961 "parser.y"
            {
        (yyval.nExpression) = (yyvsp[0].nExpression);
    }
#line 2967 "y.tab.c"
    break;

  case 70: /* concat_expr: concat_expr '&' logical_or_expr  */
#line 970 "parser.y"
                                       {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (lhs->isConstExpr() && rhs->isConstExpr()) {
            (yyval.nExpression) = new StringLiteral(
                lhs->evalCastToStr() + rhs->evalCastToStr()
            );
        } else {
            (yyval.nExpression) = new ConcatString(lhs, rhs);
        }
    }
#line 2983 "y.tab.c"
    break;

  case 72: /* logical_or_expr: logical_or_expr "operator 'or'" logical_and_expr  */
#line 984 "parser.y"
                                           {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (lhs->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator 'or' must be an integer expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (rhs->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[0]), (String("Right operand of operator 'or' must be an integer expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asInt()->unitType() || lhs->asInt()->hasUnitFactorEver()) {
            PARSE_ERR((yylsp[-2]), "Units are not allowed for operands of logical operations.");
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (rhs->asInt()->unitType() || rhs->asInt()->hasUnitFactorEver()) {
            PARSE_ERR((yylsp[0]), "Units are not allowed for operands of logical operations.");
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asInt()->isFinal() && !rhs->asInt()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of 'or' operation is not 'final', result will be 'final' though since left operand is 'final'.");
            else if (!lhs->asInt()->isFinal() && rhs->asInt()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of 'or' operation is not 'final', result will be 'final' though since right operand is 'final'.");
            (yyval.nExpression) = new Or(lhs, rhs);
        }
    }
#line 3011 "y.tab.c"
    break;

  case 73: /* logical_and_expr: bitwise_or_expr  */
#line 1009 "parser.y"
                     {
        (yyval.nExpression) = (yyvsp[0].nExpression);
    }
#line 3019 "y.tab.c"
    break;

  case 74: /* logical_and_expr: logical_and_expr "operator 'and'" bitwise_or_expr  */
#line 1012 "parser.y"
                                            {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (lhs->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator 'and' must be an integer expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (rhs->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[0]), (String("Right operand of operator 'and' must be an integer expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asInt()->unitType() || lhs->asInt()->hasUnitFactorEver()) {
            PARSE_ERR((yylsp[-2]), "Units are not allowed for operands of logical operations.");
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (rhs->asInt()->unitType() || rhs->asInt()->hasUnitFactorEver()) {
            PARSE_ERR((yylsp[0]), "Units are not allowed for operands of logical operations.");
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asInt()->isFinal() && !rhs->asInt()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of 'and' operation is not 'final', result will be 'final' though since left operand is 'final'.");
            else if (!lhs->asInt()->isFinal() && rhs->asInt()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of 'and' operation is not 'final', result will be 'final' though since right operand is 'final'.");
            (yyval.nExpression) = new And(lhs, rhs);
        }
    }
#line 3047 "y.tab.c"
    break;

  case 76: /* bitwise_or_expr: bitwise_or_expr "bitwise operator '.or.'" bitwise_and_expr  */
#line 1038 "parser.y"
                                                   {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (lhs->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of bitwise operator '.or.' must be an integer expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (rhs->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[0]), (String("Right operand of bitwise operator '.or.' must be an integer expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asInt()->unitType() || lhs->asInt()->hasUnitFactorEver()) {
            PARSE_ERR((yylsp[-2]), "Units are not allowed for operands of bitwise operations.");
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (rhs->asInt()->unitType() || rhs->asInt()->hasUnitFactorEver()) {
            PARSE_ERR((yylsp[0]), "Units are not allowed for operands of bitwise operations.");
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asInt()->isFinal() && !rhs->asInt()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '.or.' operation is not 'final', result will be 'final' though since left operand is 'final'.");
            else if (!lhs->asInt()->isFinal() && rhs->asInt()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '.or.' operation is not 'final', result will be 'final' though since right operand is 'final'.");
            (yyval.nExpression) = new BitwiseOr(lhs, rhs);
        }
    }
#line 3075 "y.tab.c"
    break;

  case 77: /* bitwise_and_expr: rel_expr  */
#line 1063 "parser.y"
              {
        (yyval.nExpression) = (yyvsp[0].nExpression);
    }
#line 3083 "y.tab.c"
    break;

  case 78: /* bitwise_and_expr: bitwise_and_expr "bitwise operator '.and.'" rel_expr  */
#line 1066 "parser.y"
                                             {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (lhs->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of bitwise operator '.and.' must be an integer expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (rhs->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[0]), (String("Right operand of bitwise operator '.and.' must be an integer expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asInt()->unitType() || lhs->asInt()->hasUnitFactorEver()) {
            PARSE_ERR((yylsp[-2]), "Units are not allowed for operands of bitwise operations.");
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (rhs->asInt()->unitType() || rhs->asInt()->hasUnitFactorEver()) {
            PARSE_ERR((yylsp[0]), "Units are not allowed for operands of bitwise operations.");
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asInt()->isFinal() && !rhs->asInt()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '.and.' operation is not 'final', result will be 'final' though since left operand is 'final'.");
            else if (!lhs->asInt()->isFinal() && rhs->asInt()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '.and.' operation is not 'final', result will be 'final' though since right operand is 'final'.");
            (yyval.nExpression) = new BitwiseAnd(lhs, rhs);
        }
    }
#line 3111 "y.tab.c"
    break;

  case 80: /* rel_expr: rel_expr '<' add_expr  */
#line 1092 "parser.y"
                             {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (!isNumber(lhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator '<' must be a scalar number expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!isNumber(rhs->exprType())) {
            PARSE_ERR((yylsp[0]), (String("Right operand of operator '<' must be a scalar number expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asNumber()->unitType() != rhs->asNumber()->unitType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of relative operations must have same unit, left operand is ") +
                unitTypeStr(lhs->asNumber()->unitType()) + " and right operand is " +
                unitTypeStr(rhs->asNumber()->unitType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asNumber()->isFinal() && !rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '<' comparison is not 'final', left operand is 'final' though.");
            else if (!lhs->asNumber()->isFinal() && rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '<' comparison is not 'final', right operand is 'final' though.");
            (yyval.nExpression) = new Relation(lhs, Relation::LESS_THAN, rhs);
        }
    }
#line 3138 "y.tab.c"
    break;

  case 81: /* rel_expr: rel_expr '>' add_expr  */
#line 1114 "parser.y"
                             {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (!isNumber(lhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator '>' must be a scalar number expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!isNumber(rhs->exprType())) {
            PARSE_ERR((yylsp[0]), (String("Right operand of operator '>' must be a scalar number expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asNumber()->unitType() != rhs->asNumber()->unitType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of relative operations must have same unit, left operand is ") +
                unitTypeStr(lhs->asNumber()->unitType()) + " and right operand is " +
                unitTypeStr(rhs->asNumber()->unitType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asNumber()->isFinal() && !rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '>' comparison is not 'final', left operand is 'final' though.");
            else if (!lhs->asNumber()->isFinal() && rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '>' comparison is not 'final', right operand is 'final' though.");
            (yyval.nExpression) = new Relation(lhs, Relation::GREATER_THAN, rhs);
        }
    }
#line 3165 "y.tab.c"
    break;

  case 82: /* rel_expr: rel_expr "operator '<='" add_expr  */
#line 1136 "parser.y"
                            {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (!isNumber(lhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator '<=' must be a scalar number expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!isNumber(rhs->exprType())) {
            PARSE_ERR((yylsp[0]), (String("Right operand of operator '<=' must be a scalar number expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asNumber()->unitType() != rhs->asNumber()->unitType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of relative operations must have same unit, left operand is ") +
                unitTypeStr(lhs->asNumber()->unitType()) + " and right operand is " +
                unitTypeStr(rhs->asNumber()->unitType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asNumber()->isFinal() && !rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '<=' comparison is not 'final', left operand is 'final' though.");
            else if (!lhs->asNumber()->isFinal() && rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '<=' comparison is not 'final', right operand is 'final' though.");
            (yyval.nExpression) = new Relation(lhs, Relation::LESS_OR_EQUAL, rhs);
        }
    }
#line 3192 "y.tab.c"
    break;

  case 83: /* rel_expr: rel_expr "operator '>='" add_expr  */
#line 1158 "parser.y"
                            {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (!isNumber(lhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator '>=' must be a scalar number expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!isNumber(rhs->exprType())) {
            PARSE_ERR((yylsp[0]), (String("Right operand of operator '>=' must be a scalar number expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asNumber()->unitType() != rhs->asNumber()->unitType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of relative operations must have same unit, left operand is ") +
                unitTypeStr(lhs->asNumber()->unitType()) + " and right operand is " +
                unitTypeStr(rhs->asNumber()->unitType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asNumber()->isFinal() && !rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '>=' comparison is not 'final', left operand is 'final' though.");
            else if (!lhs->asNumber()->isFinal() && rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '>=' comparison is not 'final', right operand is 'final' though.");
            (yyval.nExpression) = new Relation(lhs, Relation::GREATER_OR_EQUAL, rhs);
        }
    }
#line 3219 "y.tab.c"
    break;

  case 84: /* rel_expr: rel_expr '=' add_expr  */
#line 1180 "parser.y"
                             {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (!isNumber(lhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator '=' must be a scalar number expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!isNumber(rhs->exprType())) {
            PARSE_ERR((yylsp[0]), (String("Right operand of operator '=' must be a scalar number expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asNumber()->unitType() != rhs->asNumber()->unitType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of relative operations must have same unit, left operand is ") +
                unitTypeStr(lhs->asNumber()->unitType()) + " and right operand is " +
                unitTypeStr(rhs->asNumber()->unitType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asNumber()->isFinal() && !rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '=' comparison is not 'final', left operand is 'final' though.");
            else if (!lhs->asNumber()->isFinal() && rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '=' comparison is not 'final', right operand is 'final' though.");
            (yyval.nExpression) = new Relation(lhs, Relation::EQUAL, rhs);
        }
    }
#line 3246 "y.tab.c"
    break;

  case 85: /* rel_expr: rel_expr '#' add_expr  */
#line 1202 "parser.y"
                             {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (!isNumber(lhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator '#' must be a scalar number expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!isNumber(rhs->exprType())) {
            PARSE_ERR((yylsp[0]), (String("Right operand of operator '#' must be a scalar number expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asNumber()->unitType() != rhs->asNumber()->unitType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of relative operations must have same unit, left operand is ") +
                unitTypeStr(lhs->asNumber()->unitType()) + " and right operand is " +
                unitTypeStr(rhs->asNumber()->unitType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asNumber()->isFinal() && !rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '#' comparison is not 'final', left operand is 'final' though.");
            else if (!lhs->asNumber()->isFinal() && rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '#' comparison is not 'final', right operand is 'final' though.");
            (yyval.nExpression) = new Relation(lhs, Relation::NOT_EQUAL, rhs);
        }
    }
#line 3273 "y.tab.c"
    break;

  case 87: /* add_expr: add_expr '+' mul_expr  */
#line 1227 "parser.y"
                             {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (!isNumber(lhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator '+' must be a scalar number expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!isNumber(rhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Right operand of operator '+' must be a scalar number expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->exprType() != rhs->exprType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of operator '+' must have same type; left operand is ") +
                      typeStr(lhs->exprType()) + " and right operand is " + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asNumber()->unitType() != rhs->asNumber()->unitType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of '+' operations must have same unit, left operand is ") +
                unitTypeStr(lhs->asNumber()->unitType()) + " and right operand is " +
                unitTypeStr(rhs->asNumber()->unitType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asNumber()->isFinal() && !rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '+' operation is not 'final', result will be 'final' though since left operand is 'final'.");
            else if (!lhs->asNumber()->isFinal() && rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '+' operation is not 'final', result will be 'final' though since right operand is 'final'.");
            (yyval.nExpression) = new Add(lhs,rhs);
        }
    }
#line 3304 "y.tab.c"
    break;

  case 88: /* add_expr: add_expr '-' mul_expr  */
#line 1253 "parser.y"
                             {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (!isNumber(lhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator '-' must be a scalar number expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!isNumber(rhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Right operand of operator '-' must be a scalar number expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->exprType() != rhs->exprType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of operator '-' must have same type; left operand is ") +
                      typeStr(lhs->exprType()) + " and right operand is " + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asNumber()->unitType() != rhs->asNumber()->unitType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of '-' operations must have same unit, left operand is ") +
                unitTypeStr(lhs->asNumber()->unitType()) + " and right operand is " +
                unitTypeStr(rhs->asNumber()->unitType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asNumber()->isFinal() && !rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '-' operation is not 'final', result will be 'final' though since left operand is 'final'.");
            else if (!lhs->asNumber()->isFinal() && rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '-' operation is not 'final', result will be 'final' though since right operand is 'final'.");
            (yyval.nExpression) = new Sub(lhs,rhs);
        }
    }
#line 3335 "y.tab.c"
    break;

  case 90: /* mul_expr: mul_expr '*' unary_expr  */
#line 1282 "parser.y"
                               {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (!isNumber(lhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator '*' must be a scalar number expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!isNumber(rhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Right operand of operator '*' must be a scalar number expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asNumber()->unitType() && rhs->asNumber()->unitType()) {
            PARSE_ERR((yylsp[-1]), (String("Only one operand of operator '*' may have a unit type, left operand is ") +
                unitTypeStr(lhs->asNumber()->unitType()) + " and right operand is " +
                unitTypeStr(rhs->asNumber()->unitType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->exprType() != rhs->exprType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of operator '*' must have same type; left operand is ") +
                      typeStr(lhs->exprType()) + " and right operand is " + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asNumber()->isFinal() && !rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '*' operation is not 'final', result will be 'final' though since left operand is 'final'.");
            else if (!lhs->asNumber()->isFinal() && rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '*' operation is not 'final', result will be 'final' though since right operand is 'final'.");
            (yyval.nExpression) = new Mul(lhs,rhs);
        }
    }
#line 3366 "y.tab.c"
    break;

  case 91: /* mul_expr: mul_expr '/' unary_expr  */
#line 1308 "parser.y"
                               {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (!isNumber(lhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of operator '/' must be a scalar number expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!isNumber(rhs->exprType())) {
            PARSE_ERR((yylsp[-2]), (String("Right operand of operator '/' must be a scalar number expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->asNumber()->unitType() && rhs->asNumber()->unitType() &&
                   lhs->asNumber()->unitType() != rhs->asNumber()->unitType())
        {
            PARSE_ERR((yylsp[-1]), (String("Operands of operator '/' with two different unit types, left operand is ") +
                unitTypeStr(lhs->asNumber()->unitType()) + " and right operand is " +
                unitTypeStr(rhs->asNumber()->unitType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (!lhs->asNumber()->unitType() && rhs->asNumber()->unitType()) {
            PARSE_ERR((yylsp[0]), ("Dividing left operand without any unit type by right operand with unit type (" +
                unitTypeStr(rhs->asNumber()->unitType()) + ") is not possible.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (lhs->exprType() != rhs->exprType()) {
            PARSE_ERR((yylsp[-1]), (String("Operands of operator '/' must have same type; left operand is ") +
                      typeStr(lhs->exprType()) + " and right operand is " + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asNumber()->isFinal() && !rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of '/' operation is not 'final', result will be 'final' though since left operand is 'final'.");
            else if (!lhs->asNumber()->isFinal() && rhs->asNumber()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of '/' operation is not 'final', result will be 'final' though since right operand is 'final'.");
            (yyval.nExpression) = new Div(lhs,rhs);
        }
    }
#line 3403 "y.tab.c"
    break;

  case 92: /* mul_expr: mul_expr "operator 'mod'" unary_expr  */
#line 1340 "parser.y"
                               {
        ExpressionRef lhs = (yyvsp[-2].nExpression);
        ExpressionRef rhs = (yyvsp[0].nExpression);
        if (lhs->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[-2]), (String("Left operand of modulo operator must be an integer expression, is ") + typeStr(lhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else if (rhs->exprType() != INT_EXPR) {
            PARSE_ERR((yylsp[0]), (String("Right operand of modulo operator must be an integer expression, is ") + typeStr(rhs->exprType()) + " though.").c_str());
            (yyval.nExpression) = new IntLiteral({ .value = 0 });
        } else {
            if (lhs->asInt()->unitType() || lhs->asInt()->hasUnitFactorEver())
                PARSE_ERR((yylsp[-2]), "Operands of modulo operator must not use any unit.");
            if (rhs->asInt()->unitType() || rhs->asInt()->hasUnitFactorEver())
                PARSE_ERR((yylsp[0]), "Operands of modulo operator must not use any unit.");
            if (lhs->asInt()->isFinal() && !rhs->asInt()->isFinal())
                PARSE_WRN((yylsp[0]), "Right operand of 'mod' operation is not 'final', result will be 'final' though since left operand is 'final'.");
            else if (!lhs->asInt()->isFinal() && rhs->asInt()->isFinal())
                PARSE_WRN((yylsp[-2]), "Left operand of 'mod' operation is not 'final', result will be 'final' though since right operand is 'final'.");
            (yyval.nExpression) = new Mod(lhs,rhs);
        }
    }
#line 3429 "y.tab.c"
    break;


#line 3433 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, context, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= END_OF_FILE)
        {
          /* Return failure if at end of input.  */
          if (yychar == END_OF_FILE)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, context);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, context);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, context, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, context);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, context);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1362 "parser.y"


void InstrScript_error(YYLTYPE* locp, LinuxSampler::ParserContext* context, const char* err) {
    //fprintf(stderr, "%d: %s\n", locp->first_line, err);
    context->addErr(locp->first_line, locp->last_line, locp->first_column+1,
                    locp->last_column+1, locp->first_byte, locp->length_bytes,
                    err);
}

void InstrScript_warning(YYLTYPE* locp, LinuxSampler::ParserContext* context, const char* txt) {
    //fprintf(stderr, "WRN %d: %s\n", locp->first_line, txt);
    context->addWrn(locp->first_line, locp->last_line, locp->first_column+1,
                    locp->last_column+1, locp->first_byte, locp->length_bytes,
                    txt);
}

/// Custom implementation of yytnamerr() to ensure quotation is always stripped from token names before printing them to error messages.
int InstrScript_tnamerr(char* yyres, const char* yystr) {
  if (*yystr == '"') {
      int yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
/*
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
*/
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
/*
    do_not_strip_quotes: ;
*/
    }

  if (! yyres)
    return (int) yystrlen (yystr);

  return int( yystpcpy (yyres, yystr) - yyres );
}
