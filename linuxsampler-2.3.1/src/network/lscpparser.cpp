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




/* First part of user prologue.  */
#line 32 "lscp.y"


#include "lscpparser.h"
#include "lscpserver.h"
#include "lscpevent.h"

#if AC_APPLE_UNIVERSAL_BUILD
# include "lscp.tab.h"
#else
# include "lscpsymbols.h"
#endif

#include <algorithm>
#include "lscp.h"

namespace LinuxSampler {

// to save us typing work in the rules action definitions
#define LSCPSERVER ((yyparse_param_t*) yyparse_param)->pServer
#define SESSION_PARAM ((yyparse_param_t*) yyparse_param)
#define INCREMENT_LINE { SESSION_PARAM->onNextLine(); sParsed.clear(); }

// clears input buffer
void restart(yyparse_param_t* pparam, int& yychar);
#define RESTART restart((yyparse_param_t*) YYPARSE_PARAM, yychar)

static char buf[1024]; // input buffer to feed the parser with new characters
static int bytes = 0;  // current number of characters in the input buffer
static int ptr   = 0;  // current position in the input buffer
static String sLastError; // error message of the last error occured
static String sParsed; ///< Characters of current line which have already been shifted (consumed/parsed) by the parser.

// external reference to the function which actually reads from the socket
extern int GetLSCPCommand( void *buf, int max_size);

// external reference to the function in lscpserver.cpp which returns the
// current session (only works because the server runs as singleton)
extern yyparse_param_t* GetCurrentYaccSession();

// returns true if supplied characters has an ASCII code of 128 or higher
inline bool isExtendedAsciiChar(const char c) {
    return (c < 0);
}

// returns true if the given character is between between a to z.
inline bool isLowerCaseAlphaChar(const char c) {
    return c >= 'a' && c <= 'z';
}

// converts the given (expected) lower case character to upper case
inline char alphaCharToUpperCase(const char c) {
    return (c - 'a') + 'A';
}

// custom scanner function which reads from the socket
// (bison expects it to return the numerical ID of the next
// "recognized token" from the input stream)
int yylex(YYSTYPE* yylval) {
    // check if we have to read new characters
    if (ptr >= bytes) {
        bytes = GetLSCPCommand(buf, 1023);
        ptr   = 0;
        if (bytes < 0) {
            bytes = 0;
            return 0;
        }
    }
    // this is the next character in the input stream
    const char c = buf[ptr++];
    // increment current reading position (just for verbosity / messages)
    GetCurrentYaccSession()->iColumn++;
    sParsed += c;
    // we have to handle "normal" and "extended" ASCII characters separately
    if (isExtendedAsciiChar(c)) {
        // workaround for characters with ASCII code higher than 127
        yylval->Char = c;
        return EXT_ASCII_CHAR;
    } else {
        // simply return the ASCII code as terminal symbol ID
        return (int) c;
    }
}

// parser helper functions

int octalsToNumber(char oct_digit0, char oct_digit1 = '0', char oct_digit2 = '0') {
    const char d0[] = { oct_digit0, '\0' };
    const char d1[] = { oct_digit1, '\0' };
    const char d2[] = { oct_digit2, '\0' };
    return atoi(d2)*8*8 + atoi(d1)*8 + atoi(d0);
}

}

using namespace LinuxSampler;

static std::set<String> yyExpectedSymbols();

/**
 * Will be called when an error occured (usually syntax error).
 *
 * We provide our own version of yyerror() so we a) don't have to link against
 * the yacc library and b) can render more helpful syntax error messages.
 */
void yyerror(void* x, const char* s) {
    yyparse_param_t* param = GetCurrentYaccSession();

    // get the text part already parsed (of current line)
    const bool bContainsLineFeed =
        sParsed.find('\r') != std::string::npos ||
        sParsed.find('\n') != std::string::npos;
    // remove potential line feed characters
    if (bContainsLineFeed) {
        for (size_t p = sParsed.find('\r'); p != std::string::npos;
             p = sParsed.find('\r')) sParsed.erase(p);
        for (size_t p = sParsed.find('\n'); p != std::string::npos;
             p = sParsed.find('\n')) sParsed.erase(p);
    }

    // start assembling the error message with Bison's own message
    String txt = s;

    // append exact position info of syntax error
    txt += (" (line:"  + ToString(param->iLine+1)) +
           (",column:" + ToString(param->iColumn)) + ")";

    // append the part of the lined that has already been parsed
    txt += ". Context: \"" + sParsed;
    if (txt.empty() || bContainsLineFeed)
        txt += "^";
    else
        txt.insert(txt.size() - 1, "^");
    txt += "...\"";

    // append the non-terminal symbols expected now/next
    std::set<String> expectedSymbols = yyExpectedSymbols();
    for (std::set<String>::const_iterator it = expectedSymbols.begin();
         it != expectedSymbols.end(); ++it)
    {
        if (it == expectedSymbols.begin())
            txt += " -> Should be: " + *it;
        else
            txt += " | " + *it;
    }

    dmsg(2,("LSCPParser: %s\n", txt.c_str()));
    sLastError = txt;
}


#line 222 "y.tab.c"

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


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    EXT_ASCII_CHAR = 258           /* EXT_ASCII_CHAR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define EXT_ASCII_CHAR 258

/* Value type.  */



int yyparse (void* yyparse_param);


/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_EXT_ASCII_CHAR = 3,             /* EXT_ASCII_CHAR  */
  YYSYMBOL_4_ = 4,                         /* '#'  */
  YYSYMBOL_5_ = 5,                         /* '='  */
  YYSYMBOL_6_ = 6,                         /* ','  */
  YYSYMBOL_7_ = 7,                         /* '.'  */
  YYSYMBOL_8_ = 8,                         /* '+'  */
  YYSYMBOL_9_ = 9,                         /* '-'  */
  YYSYMBOL_10_0_ = 10,                     /* '0'  */
  YYSYMBOL_11_1_ = 11,                     /* '1'  */
  YYSYMBOL_12_2_ = 12,                     /* '2'  */
  YYSYMBOL_13_3_ = 13,                     /* '3'  */
  YYSYMBOL_14_4_ = 14,                     /* '4'  */
  YYSYMBOL_15_5_ = 15,                     /* '5'  */
  YYSYMBOL_16_6_ = 16,                     /* '6'  */
  YYSYMBOL_17_7_ = 17,                     /* '7'  */
  YYSYMBOL_18_8_ = 18,                     /* '8'  */
  YYSYMBOL_19_9_ = 19,                     /* '9'  */
  YYSYMBOL_20_a_ = 20,                     /* 'a'  */
  YYSYMBOL_21_b_ = 21,                     /* 'b'  */
  YYSYMBOL_22_c_ = 22,                     /* 'c'  */
  YYSYMBOL_23_d_ = 23,                     /* 'd'  */
  YYSYMBOL_24_e_ = 24,                     /* 'e'  */
  YYSYMBOL_25_f_ = 25,                     /* 'f'  */
  YYSYMBOL_26_A_ = 26,                     /* 'A'  */
  YYSYMBOL_27_B_ = 27,                     /* 'B'  */
  YYSYMBOL_28_C_ = 28,                     /* 'C'  */
  YYSYMBOL_29_D_ = 29,                     /* 'D'  */
  YYSYMBOL_30_E_ = 30,                     /* 'E'  */
  YYSYMBOL_31_F_ = 31,                     /* 'F'  */
  YYSYMBOL_32_ = 32,                       /* '\''  */
  YYSYMBOL_33_ = 33,                       /* '"'  */
  YYSYMBOL_34_ = 34,                       /* '/'  */
  YYSYMBOL_35_ = 35,                       /* ':'  */
  YYSYMBOL_36_ = 36,                       /* '\\'  */
  YYSYMBOL_37_G_ = 37,                     /* 'G'  */
  YYSYMBOL_38_H_ = 38,                     /* 'H'  */
  YYSYMBOL_39_I_ = 39,                     /* 'I'  */
  YYSYMBOL_40_J_ = 40,                     /* 'J'  */
  YYSYMBOL_41_K_ = 41,                     /* 'K'  */
  YYSYMBOL_42_L_ = 42,                     /* 'L'  */
  YYSYMBOL_43_M_ = 43,                     /* 'M'  */
  YYSYMBOL_44_N_ = 44,                     /* 'N'  */
  YYSYMBOL_45_O_ = 45,                     /* 'O'  */
  YYSYMBOL_46_P_ = 46,                     /* 'P'  */
  YYSYMBOL_47_Q_ = 47,                     /* 'Q'  */
  YYSYMBOL_48_R_ = 48,                     /* 'R'  */
  YYSYMBOL_49_S_ = 49,                     /* 'S'  */
  YYSYMBOL_50_T_ = 50,                     /* 'T'  */
  YYSYMBOL_51_U_ = 51,                     /* 'U'  */
  YYSYMBOL_52_V_ = 52,                     /* 'V'  */
  YYSYMBOL_53_W_ = 53,                     /* 'W'  */
  YYSYMBOL_54_X_ = 54,                     /* 'X'  */
  YYSYMBOL_55_Y_ = 55,                     /* 'Y'  */
  YYSYMBOL_56_Z_ = 56,                     /* 'Z'  */
  YYSYMBOL_57_g_ = 57,                     /* 'g'  */
  YYSYMBOL_58_h_ = 58,                     /* 'h'  */
  YYSYMBOL_59_i_ = 59,                     /* 'i'  */
  YYSYMBOL_60_j_ = 60,                     /* 'j'  */
  YYSYMBOL_61_k_ = 61,                     /* 'k'  */
  YYSYMBOL_62_l_ = 62,                     /* 'l'  */
  YYSYMBOL_63_m_ = 63,                     /* 'm'  */
  YYSYMBOL_64_n_ = 64,                     /* 'n'  */
  YYSYMBOL_65_o_ = 65,                     /* 'o'  */
  YYSYMBOL_66_p_ = 66,                     /* 'p'  */
  YYSYMBOL_67_q_ = 67,                     /* 'q'  */
  YYSYMBOL_68_r_ = 68,                     /* 'r'  */
  YYSYMBOL_69_s_ = 69,                     /* 's'  */
  YYSYMBOL_70_t_ = 70,                     /* 't'  */
  YYSYMBOL_71_u_ = 71,                     /* 'u'  */
  YYSYMBOL_72_v_ = 72,                     /* 'v'  */
  YYSYMBOL_73_w_ = 73,                     /* 'w'  */
  YYSYMBOL_74_x_ = 74,                     /* 'x'  */
  YYSYMBOL_75_y_ = 75,                     /* 'y'  */
  YYSYMBOL_76_z_ = 76,                     /* 'z'  */
  YYSYMBOL_77_ = 77,                       /* '!'  */
  YYSYMBOL_78_ = 78,                       /* '$'  */
  YYSYMBOL_79_ = 79,                       /* '%'  */
  YYSYMBOL_80_ = 80,                       /* '&'  */
  YYSYMBOL_81_ = 81,                       /* '('  */
  YYSYMBOL_82_ = 82,                       /* ')'  */
  YYSYMBOL_83_ = 83,                       /* '*'  */
  YYSYMBOL_84_ = 84,                       /* ';'  */
  YYSYMBOL_85_ = 85,                       /* '<'  */
  YYSYMBOL_86_ = 86,                       /* '>'  */
  YYSYMBOL_87_ = 87,                       /* '?'  */
  YYSYMBOL_88_ = 88,                       /* '@'  */
  YYSYMBOL_89_ = 89,                       /* '['  */
  YYSYMBOL_90_ = 90,                       /* ']'  */
  YYSYMBOL_91_ = 91,                       /* '^'  */
  YYSYMBOL_92___ = 92,                     /* '_'  */
  YYSYMBOL_93_ = 93,                       /* '{'  */
  YYSYMBOL_94_ = 94,                       /* '|'  */
  YYSYMBOL_95_ = 95,                       /* '}'  */
  YYSYMBOL_96_ = 96,                       /* '~'  */
  YYSYMBOL_97_ = 97,                       /* ' '  */
  YYSYMBOL_98_n_ = 98,                     /* '\n'  */
  YYSYMBOL_99_r_ = 99,                     /* '\r'  */
  YYSYMBOL_YYACCEPT = 100,                 /* $accept  */
  YYSYMBOL_input = 101,                    /* input  */
  YYSYMBOL_line = 102,                     /* line  */
  YYSYMBOL_statement = 103,                /* statement  */
  YYSYMBOL_comment = 104,                  /* comment  */
  YYSYMBOL_command = 105,                  /* command  */
  YYSYMBOL_add_instruction = 106,          /* add_instruction  */
  YYSYMBOL_subscribe_event = 107,          /* subscribe_event  */
  YYSYMBOL_unsubscribe_event = 108,        /* unsubscribe_event  */
  YYSYMBOL_map_instruction = 109,          /* map_instruction  */
  YYSYMBOL_unmap_instruction = 110,        /* unmap_instruction  */
  YYSYMBOL_remove_instruction = 111,       /* remove_instruction  */
  YYSYMBOL_get_instruction = 112,          /* get_instruction  */
  YYSYMBOL_set_instruction = 113,          /* set_instruction  */
  YYSYMBOL_create_instruction = 114,       /* create_instruction  */
  YYSYMBOL_reset_instruction = 115,        /* reset_instruction  */
  YYSYMBOL_clear_instruction = 116,        /* clear_instruction  */
  YYSYMBOL_find_instruction = 117,         /* find_instruction  */
  YYSYMBOL_move_instruction = 118,         /* move_instruction  */
  YYSYMBOL_copy_instruction = 119,         /* copy_instruction  */
  YYSYMBOL_destroy_instruction = 120,      /* destroy_instruction  */
  YYSYMBOL_load_instruction = 121,         /* load_instruction  */
  YYSYMBOL_append_instruction = 122,       /* append_instruction  */
  YYSYMBOL_insert_instruction = 123,       /* insert_instruction  */
  YYSYMBOL_set_chan_instruction = 124,     /* set_chan_instruction  */
  YYSYMBOL_edit_instruction = 125,         /* edit_instruction  */
  YYSYMBOL_format_instruction = 126,       /* format_instruction  */
  YYSYMBOL_modal_arg = 127,                /* modal_arg  */
  YYSYMBOL_key_val_list = 128,             /* key_val_list  */
  YYSYMBOL_buffer_size_type = 129,         /* buffer_size_type  */
  YYSYMBOL_list_instruction = 130,         /* list_instruction  */
  YYSYMBOL_send_instruction = 131,         /* send_instruction  */
  YYSYMBOL_load_instr_args = 132,          /* load_instr_args  */
  YYSYMBOL_load_engine_args = 133,         /* load_engine_args  */
  YYSYMBOL_instr_load_mode = 134,          /* instr_load_mode  */
  YYSYMBOL_effect_instance = 135,          /* effect_instance  */
  YYSYMBOL_device_index = 136,             /* device_index  */
  YYSYMBOL_audio_channel_index = 137,      /* audio_channel_index  */
  YYSYMBOL_audio_output_type_name = 138,   /* audio_output_type_name  */
  YYSYMBOL_midi_input_port_index = 139,    /* midi_input_port_index  */
  YYSYMBOL_midi_input_channel_index = 140, /* midi_input_channel_index  */
  YYSYMBOL_midi_input_type_name = 141,     /* midi_input_type_name  */
  YYSYMBOL_midi_map = 142,                 /* midi_map  */
  YYSYMBOL_midi_bank = 143,                /* midi_bank  */
  YYSYMBOL_midi_prog = 144,                /* midi_prog  */
  YYSYMBOL_midi_ctrl = 145,                /* midi_ctrl  */
  YYSYMBOL_volume_value = 146,             /* volume_value  */
  YYSYMBOL_control_value = 147,            /* control_value  */
  YYSYMBOL_sampler_channel = 148,          /* sampler_channel  */
  YYSYMBOL_instrument_index = 149,         /* instrument_index  */
  YYSYMBOL_fx_send_id = 150,               /* fx_send_id  */
  YYSYMBOL_engine_name = 151,              /* engine_name  */
  YYSYMBOL_filename = 152,                 /* filename  */
  YYSYMBOL_db_path = 153,                  /* db_path  */
  YYSYMBOL_map_name = 154,                 /* map_name  */
  YYSYMBOL_entry_name = 155,               /* entry_name  */
  YYSYMBOL_fx_send_name = 156,             /* fx_send_name  */
  YYSYMBOL_effect_name = 157,              /* effect_name  */
  YYSYMBOL_effect_index = 158,             /* effect_index  */
  YYSYMBOL_effect_chain = 159,             /* effect_chain  */
  YYSYMBOL_chain_pos = 160,                /* chain_pos  */
  YYSYMBOL_input_control = 161,            /* input_control  */
  YYSYMBOL_param_val_list = 162,           /* param_val_list  */
  YYSYMBOL_param_val = 163,                /* param_val  */
  YYSYMBOL_query_val_list = 164,           /* query_val_list  */
  YYSYMBOL_query_val = 165,                /* query_val  */
  YYSYMBOL_scan_mode = 166,                /* scan_mode  */
  YYSYMBOL_effect_system = 167,            /* effect_system  */
  YYSYMBOL_module = 168,                   /* module  */
  YYSYMBOL_boolean = 169,                  /* boolean  */
  YYSYMBOL_dotnum = 170,                   /* dotnum  */
  YYSYMBOL_real = 171,                     /* real  */
  YYSYMBOL_digits = 172,                   /* digits  */
  YYSYMBOL_digit = 173,                    /* digit  */
  YYSYMBOL_digit_oct = 174,                /* digit_oct  */
  YYSYMBOL_digit_hex = 175,                /* digit_hex  */
  YYSYMBOL_number = 176,                   /* number  */
  YYSYMBOL_path = 177,                     /* path  */
  YYSYMBOL_path_base = 178,                /* path_base  */
  YYSYMBOL_path_prefix = 179,              /* path_prefix  */
  YYSYMBOL_path_body = 180,                /* path_body  */
  YYSYMBOL_stringval = 181,                /* stringval  */
  YYSYMBOL_stringval_escaped = 182,        /* stringval_escaped  */
  YYSYMBOL_text = 183,                     /* text  */
  YYSYMBOL_text_escaped_base = 184,        /* text_escaped_base  */
  YYSYMBOL_text_escaped = 185,             /* text_escaped  */
  YYSYMBOL_string = 186,                   /* string  */
  YYSYMBOL_string_escaped = 187,           /* string_escaped  */
  YYSYMBOL_char = 188,                     /* char  */
  YYSYMBOL_alpha_char = 189,               /* alpha_char  */
  YYSYMBOL_char_base = 190,                /* char_base  */
  YYSYMBOL_escape_seq = 191,               /* escape_seq  */
  YYSYMBOL_escape_seq_octal = 192,         /* escape_seq_octal  */
  YYSYMBOL_escape_seq_hex = 193,           /* escape_seq_hex  */
  YYSYMBOL_SP = 194,                       /* SP  */
  YYSYMBOL_LF = 195,                       /* LF  */
  YYSYMBOL_CR = 196,                       /* CR  */
  YYSYMBOL_ADD = 197,                      /* ADD  */
  YYSYMBOL_GET = 198,                      /* GET  */
  YYSYMBOL_MAP = 199,                      /* MAP  */
  YYSYMBOL_UNMAP = 200,                    /* UNMAP  */
  YYSYMBOL_CLEAR = 201,                    /* CLEAR  */
  YYSYMBOL_FIND = 202,                     /* FIND  */
  YYSYMBOL_FILE_AS_DIR = 203,              /* FILE_AS_DIR  */
  YYSYMBOL_MOVE = 204,                     /* MOVE  */
  YYSYMBOL_COPY = 205,                     /* COPY  */
  YYSYMBOL_CREATE = 206,                   /* CREATE  */
  YYSYMBOL_DESTROY = 207,                  /* DESTROY  */
  YYSYMBOL_LIST = 208,                     /* LIST  */
  YYSYMBOL_LOAD = 209,                     /* LOAD  */
  YYSYMBOL_ALL = 210,                      /* ALL  */
  YYSYMBOL_NONE = 211,                     /* NONE  */
  YYSYMBOL_DEFAULT = 212,                  /* DEFAULT  */
  YYSYMBOL_NON_MODAL = 213,                /* NON_MODAL  */
  YYSYMBOL_REMOVE = 214,                   /* REMOVE  */
  YYSYMBOL_SET = 215,                      /* SET  */
  YYSYMBOL_SHELL = 216,                    /* SHELL  */
  YYSYMBOL_INTERACT = 217,                 /* INTERACT  */
  YYSYMBOL_AUTO_CORRECT = 218,             /* AUTO_CORRECT  */
  YYSYMBOL_APPEND = 219,                   /* APPEND  */
  YYSYMBOL_INSERT = 220,                   /* INSERT  */
  YYSYMBOL_SUBSCRIBE = 221,                /* SUBSCRIBE  */
  YYSYMBOL_UNSUBSCRIBE = 222,              /* UNSUBSCRIBE  */
  YYSYMBOL_CHANNEL = 223,                  /* CHANNEL  */
  YYSYMBOL_AVAILABLE_ENGINES = 224,        /* AVAILABLE_ENGINES  */
  YYSYMBOL_AVAILABLE_AUDIO_OUTPUT_DRIVERS = 225, /* AVAILABLE_AUDIO_OUTPUT_DRIVERS  */
  YYSYMBOL_CHANNELS = 226,                 /* CHANNELS  */
  YYSYMBOL_INFO = 227,                     /* INFO  */
  YYSYMBOL_AUDIO_OUTPUT_DEVICE_COUNT = 228, /* AUDIO_OUTPUT_DEVICE_COUNT  */
  YYSYMBOL_AUDIO_OUTPUT_DEVICE_INFO = 229, /* AUDIO_OUTPUT_DEVICE_INFO  */
  YYSYMBOL_MIDI_INPUT_DEVICE_COUNT = 230,  /* MIDI_INPUT_DEVICE_COUNT  */
  YYSYMBOL_MIDI_INPUT_DEVICE_INFO = 231,   /* MIDI_INPUT_DEVICE_INFO  */
  YYSYMBOL_MIDI_INSTRUMENT_MAP_COUNT = 232, /* MIDI_INSTRUMENT_MAP_COUNT  */
  YYSYMBOL_MIDI_INSTRUMENT_MAP_INFO = 233, /* MIDI_INSTRUMENT_MAP_INFO  */
  YYSYMBOL_MIDI_INSTRUMENT_COUNT = 234,    /* MIDI_INSTRUMENT_COUNT  */
  YYSYMBOL_MIDI_INSTRUMENT_INFO = 235,     /* MIDI_INSTRUMENT_INFO  */
  YYSYMBOL_DB_INSTRUMENT_DIRECTORY_COUNT = 236, /* DB_INSTRUMENT_DIRECTORY_COUNT  */
  YYSYMBOL_DB_INSTRUMENT_DIRECTORY_INFO = 237, /* DB_INSTRUMENT_DIRECTORY_INFO  */
  YYSYMBOL_DB_INSTRUMENT_COUNT = 238,      /* DB_INSTRUMENT_COUNT  */
  YYSYMBOL_DB_INSTRUMENT_INFO = 239,       /* DB_INSTRUMENT_INFO  */
  YYSYMBOL_DB_INSTRUMENT_FILES = 240,      /* DB_INSTRUMENT_FILES  */
  YYSYMBOL_DB_INSTRUMENTS_JOB_INFO = 241,  /* DB_INSTRUMENTS_JOB_INFO  */
  YYSYMBOL_CHANNEL_COUNT = 242,            /* CHANNEL_COUNT  */
  YYSYMBOL_CHANNEL_MIDI = 243,             /* CHANNEL_MIDI  */
  YYSYMBOL_DEVICE_MIDI = 244,              /* DEVICE_MIDI  */
  YYSYMBOL_CHANNEL_INFO = 245,             /* CHANNEL_INFO  */
  YYSYMBOL_FX_SEND_COUNT = 246,            /* FX_SEND_COUNT  */
  YYSYMBOL_FX_SEND_INFO = 247,             /* FX_SEND_INFO  */
  YYSYMBOL_BUFFER_FILL = 248,              /* BUFFER_FILL  */
  YYSYMBOL_STREAM_COUNT = 249,             /* STREAM_COUNT  */
  YYSYMBOL_VOICE_COUNT = 250,              /* VOICE_COUNT  */
  YYSYMBOL_TOTAL_STREAM_COUNT = 251,       /* TOTAL_STREAM_COUNT  */
  YYSYMBOL_TOTAL_VOICE_COUNT = 252,        /* TOTAL_VOICE_COUNT  */
  YYSYMBOL_TOTAL_VOICE_COUNT_MAX = 253,    /* TOTAL_VOICE_COUNT_MAX  */
  YYSYMBOL_GLOBAL_INFO = 254,              /* GLOBAL_INFO  */
  YYSYMBOL_EFFECT_INSTANCE_COUNT = 255,    /* EFFECT_INSTANCE_COUNT  */
  YYSYMBOL_EFFECT_INSTANCE_INFO = 256,     /* EFFECT_INSTANCE_INFO  */
  YYSYMBOL_SEND_EFFECT_CHAIN_COUNT = 257,  /* SEND_EFFECT_CHAIN_COUNT  */
  YYSYMBOL_SEND_EFFECT_CHAIN_INFO = 258,   /* SEND_EFFECT_CHAIN_INFO  */
  YYSYMBOL_INSTRUMENT = 259,               /* INSTRUMENT  */
  YYSYMBOL_INSTRUMENTS = 260,              /* INSTRUMENTS  */
  YYSYMBOL_ENGINE = 261,                   /* ENGINE  */
  YYSYMBOL_ON_DEMAND = 262,                /* ON_DEMAND  */
  YYSYMBOL_ON_DEMAND_HOLD = 263,           /* ON_DEMAND_HOLD  */
  YYSYMBOL_PERSISTENT = 264,               /* PERSISTENT  */
  YYSYMBOL_AUDIO_OUTPUT_DEVICE_PARAMETER = 265, /* AUDIO_OUTPUT_DEVICE_PARAMETER  */
  YYSYMBOL_AUDIO_OUTPUT_DEVICES = 266,     /* AUDIO_OUTPUT_DEVICES  */
  YYSYMBOL_AUDIO_OUTPUT_DEVICE = 267,      /* AUDIO_OUTPUT_DEVICE  */
  YYSYMBOL_AUDIO_OUTPUT_DRIVER_PARAMETER = 268, /* AUDIO_OUTPUT_DRIVER_PARAMETER  */
  YYSYMBOL_AUDIO_OUTPUT_DRIVER = 269,      /* AUDIO_OUTPUT_DRIVER  */
  YYSYMBOL_AUDIO_OUTPUT_CHANNEL_PARAMETER = 270, /* AUDIO_OUTPUT_CHANNEL_PARAMETER  */
  YYSYMBOL_AUDIO_OUTPUT_CHANNEL = 271,     /* AUDIO_OUTPUT_CHANNEL  */
  YYSYMBOL_AUDIO_OUTPUT_TYPE = 272,        /* AUDIO_OUTPUT_TYPE  */
  YYSYMBOL_AVAILABLE_EFFECTS = 273,        /* AVAILABLE_EFFECTS  */
  YYSYMBOL_EFFECT = 274,                   /* EFFECT  */
  YYSYMBOL_EFFECT_INSTANCE = 275,          /* EFFECT_INSTANCE  */
  YYSYMBOL_EFFECT_INSTANCES = 276,         /* EFFECT_INSTANCES  */
  YYSYMBOL_EFFECT_INSTANCE_INPUT_CONTROL = 277, /* EFFECT_INSTANCE_INPUT_CONTROL  */
  YYSYMBOL_SEND_EFFECT_CHAIN = 278,        /* SEND_EFFECT_CHAIN  */
  YYSYMBOL_SEND_EFFECT_CHAINS = 279,       /* SEND_EFFECT_CHAINS  */
  YYSYMBOL_AVAILABLE_MIDI_INPUT_DRIVERS = 280, /* AVAILABLE_MIDI_INPUT_DRIVERS  */
  YYSYMBOL_MIDI_INPUT_DEVICE_PARAMETER = 281, /* MIDI_INPUT_DEVICE_PARAMETER  */
  YYSYMBOL_MIDI_INPUT_PORT_PARAMETER = 282, /* MIDI_INPUT_PORT_PARAMETER  */
  YYSYMBOL_MIDI_INPUT_DEVICES = 283,       /* MIDI_INPUT_DEVICES  */
  YYSYMBOL_MIDI_INPUT_DEVICE = 284,        /* MIDI_INPUT_DEVICE  */
  YYSYMBOL_MIDI_INPUT_DRIVER_PARAMETER = 285, /* MIDI_INPUT_DRIVER_PARAMETER  */
  YYSYMBOL_MIDI_INSTRUMENT = 286,          /* MIDI_INSTRUMENT  */
  YYSYMBOL_MIDI_INSTRUMENTS = 287,         /* MIDI_INSTRUMENTS  */
  YYSYMBOL_MIDI_INSTRUMENT_MAP = 288,      /* MIDI_INSTRUMENT_MAP  */
  YYSYMBOL_MIDI_INSTRUMENT_MAPS = 289,     /* MIDI_INSTRUMENT_MAPS  */
  YYSYMBOL_MIDI_INPUT_DRIVER = 290,        /* MIDI_INPUT_DRIVER  */
  YYSYMBOL_MIDI_INPUT_PORT = 291,          /* MIDI_INPUT_PORT  */
  YYSYMBOL_MIDI_INPUT_CHANNEL = 292,       /* MIDI_INPUT_CHANNEL  */
  YYSYMBOL_MIDI_INPUT_TYPE = 293,          /* MIDI_INPUT_TYPE  */
  YYSYMBOL_MIDI_INPUT = 294,               /* MIDI_INPUT  */
  YYSYMBOL_MIDI_INPUTS = 295,              /* MIDI_INPUTS  */
  YYSYMBOL_MIDI_CONTROLLER = 296,          /* MIDI_CONTROLLER  */
  YYSYMBOL_SEND = 297,                     /* SEND  */
  YYSYMBOL_FX_SEND = 298,                  /* FX_SEND  */
  YYSYMBOL_FX_SENDS = 299,                 /* FX_SENDS  */
  YYSYMBOL_DB_INSTRUMENT_DIRECTORY = 300,  /* DB_INSTRUMENT_DIRECTORY  */
  YYSYMBOL_DB_INSTRUMENT_DIRECTORIES = 301, /* DB_INSTRUMENT_DIRECTORIES  */
  YYSYMBOL_DB_INSTRUMENTS = 302,           /* DB_INSTRUMENTS  */
  YYSYMBOL_DB_INSTRUMENT = 303,            /* DB_INSTRUMENT  */
  YYSYMBOL_DB_INSTRUMENTS_JOB = 304,       /* DB_INSTRUMENTS_JOB  */
  YYSYMBOL_INSTRUMENTS_DB = 305,           /* INSTRUMENTS_DB  */
  YYSYMBOL_DESCRIPTION = 306,              /* DESCRIPTION  */
  YYSYMBOL_FORCE = 307,                    /* FORCE  */
  YYSYMBOL_FLAT = 308,                     /* FLAT  */
  YYSYMBOL_RECURSIVE = 309,                /* RECURSIVE  */
  YYSYMBOL_NON_RECURSIVE = 310,            /* NON_RECURSIVE  */
  YYSYMBOL_LOST = 311,                     /* LOST  */
  YYSYMBOL_FILE_PATH = 312,                /* FILE_PATH  */
  YYSYMBOL_SERVER = 313,                   /* SERVER  */
  YYSYMBOL_VOLUME = 314,                   /* VOLUME  */
  YYSYMBOL_LEVEL = 315,                    /* LEVEL  */
  YYSYMBOL_VALUE = 316,                    /* VALUE  */
  YYSYMBOL_MUTE = 317,                     /* MUTE  */
  YYSYMBOL_SOLO = 318,                     /* SOLO  */
  YYSYMBOL_VOICES = 319,                   /* VOICES  */
  YYSYMBOL_STREAMS = 320,                  /* STREAMS  */
  YYSYMBOL_BYTES = 321,                    /* BYTES  */
  YYSYMBOL_PERCENTAGE = 322,               /* PERCENTAGE  */
  YYSYMBOL_FILE = 323,                     /* FILE  */
  YYSYMBOL_EDIT = 324,                     /* EDIT  */
  YYSYMBOL_FORMAT = 325,                   /* FORMAT  */
  YYSYMBOL_MIDI_DATA = 326,                /* MIDI_DATA  */
  YYSYMBOL_RESET = 327,                    /* RESET  */
  YYSYMBOL_MISCELLANEOUS = 328,            /* MISCELLANEOUS  */
  YYSYMBOL_NAME = 329,                     /* NAME  */
  YYSYMBOL_ECHO = 330,                     /* ECHO  */
  YYSYMBOL_DOC = 331,                      /* DOC  */
  YYSYMBOL_QUIT = 332                      /* QUIT  */
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
typedef yytype_int16 yy_state_t;

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
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  64
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   5021

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  233
/* YYNRULES -- Number of rules.  */
#define YYNRULES  669
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  2662

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   258


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
      98,     2,     2,    99,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    97,    77,    33,     4,    78,    79,    80,    32,
      81,    82,    83,     8,     6,     9,     7,    34,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    35,    84,
      85,     5,    86,    87,    88,    26,    27,    28,    29,    30,
      31,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    89,    36,    90,    91,    92,     2,    20,    21,    22,
      23,    24,    25,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    93,    94,    95,    96,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   227,   227,   228,   231,   232,   235,   236,   237,   240,
     241,   242,   243,   244,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   354,   355,   356,   357,   360,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   459,   460,
     461,   462,   463,   464,   465,   466,   469,   472,   473,   476,
     477,   478,   479,   480,   483,   484,   487,   488,   491,   492,
     493,   494,   497,   498,   501,   504,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   520,
     523,   526,   529,   530,   533,   534,   537,   538,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   562,   565,   566,
     569,   572,   573,   574,   577,   580,   583,   586,   589,   592,
     593,   596,   599,   602,   605,   608,   611,   612,   615,   618,
     621,   624,   627,   630,   640,   643,   646,   649,   652,   655,
     658,   661,   664,   667,   668,   672,   673,   674,   675,   678,
     679,   682,   683,   686,   687,   688,   691,   694,   702,   703,
     706,   707,   708,   711,   712,   713,   714,   715,   716,   720,
     721,   724,   725,   726,   727,   728,   729,   730,   731,   732,
     733,   736,   737,   738,   739,   740,   741,   742,   743,   746,
     747,   748,   749,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,   760,   761,   762,   763,   764,   765,   766,
     767,   770,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   782,   783,   786,   789,   790,   793,   794,   795,   798,
     799,   802,   803,   806,   807,   808,   809,   813,   814,   815,
     816,   819,   820,   821,   822,   825,   826,   829,   830,   831,
     832,   836,   837,   838,   842,   842,   842,   842,   842,   842,
     842,   842,   842,   842,   842,   842,   842,   842,   842,   842,
     842,   842,   842,   842,   842,   842,   842,   842,   842,   842,
     843,   843,   843,   843,   843,   843,   843,   843,   843,   843,
     843,   843,   843,   843,   843,   843,   843,   843,   843,   843,
     843,   843,   843,   843,   843,   843,   847,   848,   848,   848,
     848,   848,   848,   848,   848,   848,   848,   849,   849,   849,
     849,   849,   849,   849,   849,   849,   849,   849,   849,   850,
     850,   850,   850,   850,   850,   850,   851,   851,   851,   851,
     852,   852,   852,   852,   853,   856,   857,   858,   859,   860,
     861,   862,   863,   864,   865,   866,   869,   870,   871,   874,
     875,   880,   883,   886,   889,   892,   895,   898,   901,   904,
     907,   910,   913,   916,   919,   922,   925,   928,   931,   934,
     937,   940,   943,   946,   949,   952,   955,   958,   961,   964,
     967,   970,   973,   976,   979,   982,   985,   988,   991,   994,
     997,  1000,  1003,  1006,  1009,  1012,  1015,  1018,  1021,  1024,
    1027,  1030,  1033,  1036,  1039,  1042,  1045,  1048,  1051,  1054,
    1057,  1060,  1063,  1066,  1069,  1072,  1075,  1078,  1081,  1084,
    1087,  1090,  1093,  1096,  1099,  1102,  1105,  1108,  1111,  1114,
    1117,  1120,  1123,  1126,  1129,  1132,  1135,  1138,  1141,  1144,
    1147,  1150,  1153,  1156,  1159,  1162,  1165,  1168,  1171,  1174,
    1177,  1180,  1183,  1186,  1189,  1192,  1195,  1198,  1201,  1204,
    1207,  1210,  1213,  1216,  1219,  1222,  1225,  1228,  1231,  1234,
    1237,  1240,  1243,  1246,  1249,  1252,  1255,  1258,  1261,  1264,
    1267,  1270,  1273,  1276,  1279,  1282,  1285,  1288,  1291,  1294
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
  "\"end of file\"", "error", "\"invalid token\"", "EXT_ASCII_CHAR",
  "'#'", "'='", "','", "'.'", "'+'", "'-'", "'0'", "'1'", "'2'", "'3'",
  "'4'", "'5'", "'6'", "'7'", "'8'", "'9'", "'a'", "'b'", "'c'", "'d'",
  "'e'", "'f'", "'A'", "'B'", "'C'", "'D'", "'E'", "'F'", "'\\''", "'\"'",
  "'/'", "':'", "'\\\\'", "'G'", "'H'", "'I'", "'J'", "'K'", "'L'", "'M'",
  "'N'", "'O'", "'P'", "'Q'", "'R'", "'S'", "'T'", "'U'", "'V'", "'W'",
  "'X'", "'Y'", "'Z'", "'g'", "'h'", "'i'", "'j'", "'k'", "'l'", "'m'",
  "'n'", "'o'", "'p'", "'q'", "'r'", "'s'", "'t'", "'u'", "'v'", "'w'",
  "'x'", "'y'", "'z'", "'!'", "'$'", "'%'", "'&'", "'('", "')'", "'*'",
  "';'", "'<'", "'>'", "'?'", "'@'", "'['", "']'", "'^'", "'_'", "'{'",
  "'|'", "'}'", "'~'", "' '", "'\\n'", "'\\r'", "$accept", "input", "line",
  "statement", "comment", "command", "add_instruction", "subscribe_event",
  "unsubscribe_event", "map_instruction", "unmap_instruction",
  "remove_instruction", "get_instruction", "set_instruction",
  "create_instruction", "reset_instruction", "clear_instruction",
  "find_instruction", "move_instruction", "copy_instruction",
  "destroy_instruction", "load_instruction", "append_instruction",
  "insert_instruction", "set_chan_instruction", "edit_instruction",
  "format_instruction", "modal_arg", "key_val_list", "buffer_size_type",
  "list_instruction", "send_instruction", "load_instr_args",
  "load_engine_args", "instr_load_mode", "effect_instance", "device_index",
  "audio_channel_index", "audio_output_type_name", "midi_input_port_index",
  "midi_input_channel_index", "midi_input_type_name", "midi_map",
  "midi_bank", "midi_prog", "midi_ctrl", "volume_value", "control_value",
  "sampler_channel", "instrument_index", "fx_send_id", "engine_name",
  "filename", "db_path", "map_name", "entry_name", "fx_send_name",
  "effect_name", "effect_index", "effect_chain", "chain_pos",
  "input_control", "param_val_list", "param_val", "query_val_list",
  "query_val", "scan_mode", "effect_system", "module", "boolean", "dotnum",
  "real", "digits", "digit", "digit_oct", "digit_hex", "number", "path",
  "path_base", "path_prefix", "path_body", "stringval",
  "stringval_escaped", "text", "text_escaped_base", "text_escaped",
  "string", "string_escaped", "char", "alpha_char", "char_base",
  "escape_seq", "escape_seq_octal", "escape_seq_hex", "SP", "LF", "CR",
  "ADD", "GET", "MAP", "UNMAP", "CLEAR", "FIND", "FILE_AS_DIR", "MOVE",
  "COPY", "CREATE", "DESTROY", "LIST", "LOAD", "ALL", "NONE", "DEFAULT",
  "NON_MODAL", "REMOVE", "SET", "SHELL", "INTERACT", "AUTO_CORRECT",
  "APPEND", "INSERT", "SUBSCRIBE", "UNSUBSCRIBE", "CHANNEL",
  "AVAILABLE_ENGINES", "AVAILABLE_AUDIO_OUTPUT_DRIVERS", "CHANNELS",
  "INFO", "AUDIO_OUTPUT_DEVICE_COUNT", "AUDIO_OUTPUT_DEVICE_INFO",
  "MIDI_INPUT_DEVICE_COUNT", "MIDI_INPUT_DEVICE_INFO",
  "MIDI_INSTRUMENT_MAP_COUNT", "MIDI_INSTRUMENT_MAP_INFO",
  "MIDI_INSTRUMENT_COUNT", "MIDI_INSTRUMENT_INFO",
  "DB_INSTRUMENT_DIRECTORY_COUNT", "DB_INSTRUMENT_DIRECTORY_INFO",
  "DB_INSTRUMENT_COUNT", "DB_INSTRUMENT_INFO", "DB_INSTRUMENT_FILES",
  "DB_INSTRUMENTS_JOB_INFO", "CHANNEL_COUNT", "CHANNEL_MIDI",
  "DEVICE_MIDI", "CHANNEL_INFO", "FX_SEND_COUNT", "FX_SEND_INFO",
  "BUFFER_FILL", "STREAM_COUNT", "VOICE_COUNT", "TOTAL_STREAM_COUNT",
  "TOTAL_VOICE_COUNT", "TOTAL_VOICE_COUNT_MAX", "GLOBAL_INFO",
  "EFFECT_INSTANCE_COUNT", "EFFECT_INSTANCE_INFO",
  "SEND_EFFECT_CHAIN_COUNT", "SEND_EFFECT_CHAIN_INFO", "INSTRUMENT",
  "INSTRUMENTS", "ENGINE", "ON_DEMAND", "ON_DEMAND_HOLD", "PERSISTENT",
  "AUDIO_OUTPUT_DEVICE_PARAMETER", "AUDIO_OUTPUT_DEVICES",
  "AUDIO_OUTPUT_DEVICE", "AUDIO_OUTPUT_DRIVER_PARAMETER",
  "AUDIO_OUTPUT_DRIVER", "AUDIO_OUTPUT_CHANNEL_PARAMETER",
  "AUDIO_OUTPUT_CHANNEL", "AUDIO_OUTPUT_TYPE", "AVAILABLE_EFFECTS",
  "EFFECT", "EFFECT_INSTANCE", "EFFECT_INSTANCES",
  "EFFECT_INSTANCE_INPUT_CONTROL", "SEND_EFFECT_CHAIN",
  "SEND_EFFECT_CHAINS", "AVAILABLE_MIDI_INPUT_DRIVERS",
  "MIDI_INPUT_DEVICE_PARAMETER", "MIDI_INPUT_PORT_PARAMETER",
  "MIDI_INPUT_DEVICES", "MIDI_INPUT_DEVICE", "MIDI_INPUT_DRIVER_PARAMETER",
  "MIDI_INSTRUMENT", "MIDI_INSTRUMENTS", "MIDI_INSTRUMENT_MAP",
  "MIDI_INSTRUMENT_MAPS", "MIDI_INPUT_DRIVER", "MIDI_INPUT_PORT",
  "MIDI_INPUT_CHANNEL", "MIDI_INPUT_TYPE", "MIDI_INPUT", "MIDI_INPUTS",
  "MIDI_CONTROLLER", "SEND", "FX_SEND", "FX_SENDS",
  "DB_INSTRUMENT_DIRECTORY", "DB_INSTRUMENT_DIRECTORIES", "DB_INSTRUMENTS",
  "DB_INSTRUMENT", "DB_INSTRUMENTS_JOB", "INSTRUMENTS_DB", "DESCRIPTION",
  "FORCE", "FLAT", "RECURSIVE", "NON_RECURSIVE", "LOST", "FILE_PATH",
  "SERVER", "VOLUME", "LEVEL", "VALUE", "MUTE", "SOLO", "VOICES",
  "STREAMS", "BYTES", "PERCENTAGE", "FILE", "EDIT", "FORMAT", "MIDI_DATA",
  "RESET", "MISCELLANEOUS", "NAME", "ECHO", "DOC", "QUIT", YY_NULLPTR
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
       0,   256,   257,   258,    35,    61,    44,    46,    43,    45,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      97,    98,    99,   100,   101,   102,    65,    66,    67,    68,
      69,    70,    39,    34,    47,    58,    92,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    33,    36,    37,
      38,    40,    41,    42,    59,    60,    62,    63,    64,    91,
      93,    94,    95,   123,   124,   125,   126,    32,    10,    13
};
#endif

#define YYPACT_NINF (-1718)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-382)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     893, -1718, -1718,   116,   128,    47,    75,    95,   145,   246,
     379,   148,   194,   268,    91,   381,   395, -1718,   358,  3095,
   -1718,   411,   411,   411,   411,   411,   411,   411,   411,   411,
     411,   411,   411,   411,   411,   411,   411,   411,   411,   411,
     411,   411,   411, -1718,   492,   482,   510,   503,   592,   594,
     587,   601,   605,   651,   670,   674,   702,   698,   716,   715,
     548,   455,   742,   555, -1718, -1718, -1718, -1718,   673, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718,   892,   892,   892,
     892,   892,   892,   892,   892,   892, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718,  3853, -1718, -1718, -1718, -1718,   201,
     496,   729,   729,   730,   174,   745,   745,   422,   422,   301,
     138,   415,   580,   726,   726,   466,   466,   749,   749,   740,
     749, -1718,   751,   756,   736,   768,   747,   748,   766,   765,
   -1718,   782,   763,   785, -1718,   801,   788,   770,   815,   819,
   -1718,   803,   827,   804, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718,   892, -1718,   892,   892,   892,
     892,   892,   892,   892,   892, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718,   818,   830,   820,
     831, -1718,   411,   411,   411,   411,   411,   161,   822,   835,
     355,   215,   824,   102,   821,   828, -1718,   411, -1718, -1718,
   -1718, -1718, -1718, -1718,   411, -1718,   411,   411,   411,   411,
     411, -1718,   411,   411, -1718,   411,   411,   411, -1718,   411,
   -1718,   411,   411,   411,   411,   411, -1718,   411,   411,   411,
     411,   411,   411,   411,   411,   411,   411, -1718, -1718, -1718,
     411,   833, -1718,   411, -1718,   411,   836, -1718,   411,   840,
     829, -1718,   411,   411,   411,   842, -1718,   411,   411, -1718,
     411,   411,   825,   846,   839,   856, -1718,   411,   411,   411,
     411, -1718,   411,   411,   411,   411,   544,   847,   295,   857,
     868, -1718,   411, -1718, -1718, -1718, -1718, -1718, -1718,   411,
   -1718, -1718,   411, -1718,   411,   411,   411,   411,   869,   871,
   -1718,   411,   411, -1718,   411,   411,   411,   411,   411,   411,
     849,   333,   877,   278, -1718,   411,   411,   411,   411,   411,
     411,   411,   411,   411,   411,   411,   411,   411,   411,   411,
   -1718,   411, -1718,   411,   866,   867,   887,   335,   895,   875,
     885,   894,   107,   886,   889, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,   411, -1718,
     411,   899, -1718, -1718, -1718,   411,   901,   890, -1718,   896,
     891, -1718, -1718,   911,   900, -1718, -1718, -1718, -1718,   897,
     913, -1718,   923,   918,   938, -1718,   941,   876,   942,   931,
     945,  1195,   598,   653,   269,   955,   960,   964,   902,   962,
     959,   956,   905,   971,   167,   953,   952,   433,   114,   965,
     965,   965,   965,   965,   965,   965,   965,   965,   965,  1195,
     965,   965,   965,   965,   873,   965,   965,   965,   965,  1195,
     965,   208,   208,   965,   965,   965,   966,   977,   963,  1195,
     979,   873,   917,   961,   382,   382,   982,   929,   653,   653,
     653,   653,   993,   992,   932,   999,  3853,  3947,  3853,  1195,
    1195,  1195,  1195,  1195,  1000,  1001,   939,  1004,   986,   994,
    1195,   873,  1195,   208,   208,   996,   987,   500,  3853,   394,
     547,   873,  1008,   627,   653,  1011,  1003,  1012,  1013,  1016,
     491,    81,  1195,  1195,   995,  1195,  1195,  1017,   359,   310,
     416,  1507,  1195,  1195,  3947,  1008,  1008,  1027,  1029,  1036,
     972,  1014,  1034,   975,  1023,   130,  1031,  1025,  1026,  1040,
    1042,  1044,  1028,  1195,  1052, -1718,  1056,  1043,  1037,  1039,
    1060, -1718,  1045, -1718,  1046,  1047,  1055,  1057,  1068,  1062,
     411,   892,   892,   892,   892,   892,   892,   892,   892,   892,
   -1718, -1718,  3190,  3190, -1718, -1718,  2355,  2355, -1718, -1718,
    1063,  1054,  1077,   411,   411,   411, -1718, -1718, -1718,  1070,
    1072,  1069,  1075,  1085,  1078,  1086,  1071,  1080,  1092,  1073,
    1096,  1101,  1102,  1081,  1087,  1079,   411,   411,   411,   411,
     411,   411,   411,   411,   411,   411,   411,   411,   411,   411,
   -1718,   411,   411,   411,   411,  1091, -1718, -1718, -1718,   411,
     411,   411,   411, -1718, -1718,   411, -1718,   411, -1718,   411,
     411,   411, -1718,  1090,   411,   411,  1097,  1098,  1195,   411,
     411,  1103, -1718, -1718,  1107,  1094,  1104,   411,   411,   411,
     411,  1114, -1718,  1109,   411,   411,   411,   411,  1111,  1121,
    1106,  1113,  3285, -1718,   411, -1718,  3853,  3285,   411, -1718,
   -1718, -1718,   411,  1115,  1126,  1108,  1119,  1131,  1122,   411,
   -1718, -1718, -1718, -1718, -1718,   411, -1718,   411,  1118,   411,
    1117, -1718,   411, -1718,   411, -1718,   411,  3853, -1718,   411,
    1133,   411,   411, -1718, -1718,   411,  1120, -1718,   411, -1718,
    1132,  1125,  1142,  1137,  1138,  1128,  1136,  1140,   411,   411,
     411,  1134,   349,  1141,  1143, -1718,   411,   411,   411,   411,
     411,   411,   411,   411,   411,   411,   411,   411,   411,   411,
    1161,   411,   411,   411,  1163,   411,  1145,  1162,  1152,   411,
     411,   411,   411,   411,  1164,   411,   411,  1154,   411,   411,
     411,   892,   892, -1718, -1718,   943,    57, -1718, -1718, -1718,
   -1718, -1718,  3853,   411,   411,  1158,  1167,  1155,  1178,  1182,
    1171,  1180,  1200,  1191,  1203,  1204,  1202,  1208,  1207,  1198,
     411,   411,  1189, -1718, -1718, -1718,  1186, -1718, -1718, -1718,
    1217,  1214,  1213,  1215,  1166,  1168,  1232,  1195, -1718,  1038,
    3570,  2710,  4793, -1718, -1718, -1718, -1718, -1718,  2810, -1718,
    1242, -1718,  1241,  1245,  1254,  1238,  1255,   653,   676,   456,
    1239,  1248,  1249,  1253,  1270,  1268, -1718,  1283,  1222,  1223,
    1286,  1293,  1276,  1290,  1278,  1291,  1195,   225,  1195,  1195,
    3853,  1195,  3853,  3853,  1195,  1195,  1195,  1195,  1195,  1195,
    1195,  1195,  3853,  1195,  1294,  1195,  3853,  1195,  1195,   653,
     653,   653,   653,  1195,  1288,   965,   653,  1246,  1295,   411,
   -1718,  1195,  1250,  1296, -1718,  1297,  3853,   653,  3853,   653,
    1251,  1300,   653,   653,   653,   653,  1303,  1321,  1320,  1259,
    3853,   653,  3853,  1195,  1195,  1307,  1326,  1325,  1264,  1265,
    1330,  1195,   653,   653,  1312,   653,  1314,  1195,   653,  1195,
    1195,  1332,  1195,  1195,  1195,  1318,   653,  1319, -1718,  1350,
    1287,  1338,  1331,  1354,  1333,  3947,  3947,  3947,  1355,  1356,
    1336,  1345,  1346,  1195,  1195,  1195,  1195,  1195,  1195,  1195,
    1195,  1195,  1195,  1195,  1195,  3853,  1195,  1348,  1195,  3853,
    1195,  1349,  1195,  1362,  1343,  1378,  1195,  1195,  1195,  1195,
    1195,  1359,   653,   653,  1367,   653,   653,   653,  1236,  1292,
     892,  1195,  1195,  1365,  1381,  1368,  1369,  1391,  1395,  1394,
    1399,  1334,  1397,  1337,  1404,  1389,  1402,  1405,  3853,  1195,
    1385, -1718,  1408,  1388,  1407,  1393,  1406,  1414,  1409,   411,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718,  1980,   790,  4793,
   -1718, -1718, -1718,  3570, -1718, -1718, -1718, -1718,  3380,  1412,
   -1718,  1400,  1357,  1396,   411,  1413,   411,   411,   411,   411,
    1370,  1425,  1424,  1411,  1415,  1427,  1417,  1428,  1433,  1390,
    1421,  1374,  1419,  1439,  1432, -1718,  1429,  1442,   411, -1718,
   -1718, -1718, -1718, -1718, -1718,  3285,  3853,   411,   411, -1718,
   -1718, -1718,   411,   411,   411, -1718,  3285,   411, -1718, -1718,
    3853,   411,   411, -1718, -1718, -1718, -1718, -1718,  1437,   411,
   -1718,  1444,  1398,  1195,   411, -1718,  1449,  1440,  1418,   411,
    4041,   411,   411,   411,  1452,  1443, -1718, -1718, -1718, -1718,
    1435,  1448,  1455,  1461,   411,  4135, -1718,   411,   411,   411,
   -1718, -1718, -1718,  1436,  1451,  1459,  1465,  1475,  1467, -1718,
   -1718, -1718,  1483, -1718,  1458,   411, -1718,   411, -1718,   411,
    1504, -1718, -1718,   411,   411,  1510, -1718,  1447,  1490,  1505,
   -1718,  1498, -1718,  1515, -1718, -1718, -1718,  1508,  1509,  1519,
    1506,   411,   411,   411,   411,   411,   411,   411,   411,   411,
     411,   411,   411,  4229,   411,  1499,   411,  4323,   411,  1524,
     411,  1516,  1526,  1518,   411,   411,   411,   411,   411,  1530,
     411,   411,  1529,   411,   411,   411,   892,   892,   892,   411,
     411,  1468,  1513,  1532,  1520,  1535,  1517,  1527,  1528,  1533,
    1531,  1538,  1534,  1484,  1486,  1536,  3285, -1718,  1539,  1549,
    1541,  1540,  1542,  1547,  1552,  1494,  1195, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,  1980,
     790, -1718,  3570, -1718, -1718,   294,  1545,  1195,  1553,   653,
     653,   653,   676,  1551,  1567,  1555,  1561,  1521, -1718,  1583,
    1570,  1585, -1718,  1568,   501, -1718, -1718, -1718,  1569,  1572,
    1195,  3853,  1195,  1195,  1195,  1195,  1195,  3853,  1195,  1195,
    1195,  1573,   653,  1574,  1580,   411,  1195,  1581,  1576,  1579,
    3853,  2615,  3853,  3853,  1584,  1586,  1589,  1543,  1595,  1593,
    3853,  3665,   598,   598,  1599,  1546,  1611,  1601,  1615,  1556,
    1602,  1604,  1195,  1195,  1195,  1621,  1195,  1195,  1622,  1606,
    1562,  1609,  1563,  1608,  1614,  1571, -1718, -1718,  1195,  1195,
    3853,  1195,   209,  1195,   873,  3853,  1195,  1507,  3947,  3947,
    3665,  3853,  1630,  1195,  3665,  3853, -1718,   598,  1616,  1620,
    1578,  1195,  1195,  1195,  1195,  1195,  1617,   598,   598,  1582,
     598,   653,   598,   892,   892,  1195,  1195,  1619,  1587,  1624,
    1623,  1588,  1594,  1642,  1597,  1628,  1633,  1645,  1598,   518,
    1649,  1600,  1195,  1635, -1718,  1654, -1718,  1637,  1634,  1656,
    1652,   411, -1718, -1718,  1648,  1664,  1650, -1718,  1668, -1718,
     411,   411,   411,   411,  1655,  1658,  1660,  1653,  1671,  1662,
     523,  1672, -1718,  1657,  1667,  1683,  1686, -1718,  3285,   411,
   -1718, -1718, -1718, -1718,   411,  3285,   411, -1718, -1718,  1665,
     411,  1666,  1195, -1718, -1718,  1669,  1673,  4417, -1718, -1718,
    3475,   411,   411,  1670,  1675,  1674,  1678, -1718,  1680,  4511,
     892,   892,  3285,  3285,  1714, -1718, -1718, -1718, -1718,  3853,
   -1718, -1718, -1718, -1718,  1676,  1689,  1662,   632,  1699,  1694,
    1685,  1704, -1718,   411,   411,  1687,   411, -1718, -1718,  1688,
    1703,   633,  1710,  1717,  1659,  1705, -1718,   411, -1718, -1718,
    3853, -1718,  1716,  1702, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718,  3853,   411, -1718, -1718, -1718,  1714,  4605,
   -1718,   411,  1714,  4699, -1718,  1661, -1718,  1721,   411,   411,
     411,   411,   411,  1715, -1718, -1718,  1709, -1718, -1718, -1718,
     411,   411,  1706,  1725,  1681,  1711,  1719,  1729,  1682,  1736,
     634,  1732,  1749,  1755,  1739,  1740,  1757,   411,  1758,  1759,
    1741,  1737,  1761,  1751,  1195,  1767,  1769,  1760,  1708,   653,
    1195,   653,   653,  1748,  1771, -1718,  1752,  1762, -1718,  1753,
    1763,  1772,  1764,  1766,  1768,  1777,  3853,  3853,  3853,  3853,
    1195,  1765,  1195,  1770,   411,  1773,  1775,  2615,  1774,  1776,
    1778,  1785,  1779,  3665,  2905,  3853, -1718,  3000,  3665,  1781,
    1788,  1782,  1786,  1780,  1793,  1796,  1797,  1195,  1195, -1718,
    1195,  1790,  1798,  1792,  1801,  1783,  1804,  1806,  1195,  1791,
    1807,  1195,  3665,  1591,  3759,  1808,  1809,  1195,  1195,  1195,
    1507,   598,  1810,  1826,  1195,  1195,  1811,  1818,   353,  1812,
    1819,  1815,   452,  1816,  1817,  1820,  1822,  1789,  1824,  1823,
    1821,  1841,  1195,  1827, -1718,  1830,  1828,  1846,  1832, -1718,
    1853,  1829,  1831,  1855, -1718, -1718, -1718,   411,  1836,  1794,
    1842,  1835,  1837,  1840,  1861,  1860,  1863, -1718,  1848,   411,
    3853,  3853,   411, -1718,  1865, -1718,  1845,  3853,  1849,  1856,
   -1718,  1852,  1859,  1850,  1854,  1857,  1714, -1718,  3853, -1718,
   -1718, -1718,  1858,  1862,  1864,  1867,  1877,  1866,  1876,  1868,
   -1718, -1718, -1718, -1718,  1870,  1871,  1869,  1873,  1872,  1875,
     683, -1718,  1882,  1879,   411,  1714,   892,   892, -1718, -1718,
    1316,  1702,  1714, -1718,  1878,  1880,   411,   411, -1718, -1718,
   -1718,  1883,  1884, -1718,   411,  1881,  1886,  1885,  1892,  1898,
    1874,  1894,  1889,  1895,  1897,  1908,  1893,  1896,  1912,  1917,
    1899,  1918,  1903,  1901,   411,  1902,  1919,  1904,  1906,  1907,
    1915,  1900,  1929,  1913,   653,  1914,   476,  1933,  1920,  1888,
    1916,  1921,  1942,  1939,  1922,  1930,  1924,   411,  1925,  1943,
    1926,  1948,  1931,  1936,  1891,  1937,  1962,  1923,  1963,  1966,
    1967,  1969, -1718,  1968,  1970,  1932,  1973, -1718,  1975,  1972,
    1976, -1718,   873,  1358,  1387,   892,  1981,  1982,  1195,  1195,
    1978,  1949,  1195,  1979,  1984,  1983,  1950,  2004,  2005,  1997,
    1987,  1988,  2007,  1995,  1951,  1990,  1999,  1992,  2001,  2016,
    1998,  2021,  1195,  2000,  2006,  2008,  1960,  2003, -1718,  2009,
   -1718,  1964, -1718,  2010,  2011,   419,  2018,  2015,  2029,   132,
    2022,  1974,  2024,  1977,  2042,  2020,  2030,   653,  2032,  2028,
    2033,  2034,  2027,  2054,  2052,  2035,  2056,  2055,  2043,  1996,
    2037,  2039,  2040,  2065,   260,  2062,  2047,  2044,  2053, -1718,
     892,   892,   892,  2050,  2049, -1718, -1718,  2057, -1718, -1718,
    2048, -1718,  2063,  2061,  2060,  2064, -1718,  2074,  2066,  2067,
   -1718,  2072,  2070,  2058,  2023,  2068,  2025, -1718, -1718, -1718,
    2031,  2069,  2084,  2088, -1718,  2081,  2092,  2036,  2093,  2094,
    2087,  2098,  2079,  2086,   247,  2091,  2101,  2104,  2041,  2109,
    2103,  2089,  2111,   411,  2112,  2095,  2113,  2096,  2059,  2100,
    2117,  2080,  2105,  2118,  2122,  2125,  2106,  2089,  2083,  2110,
    2141,  2128,  2149,  2127,  2102,  2130,   892,   892,  2135,  2142,
    2144,  2108,  2140, -1718, -1718,  2143,  2151,  2146, -1718,  2167,
    2168,  2152,  2174, -1718,  2175,  2176,    19,  2160,  2171,  2154,
    2173,   686,  2177,  2180,  2178,  2179,    24,  2185,  2163,  2181,
    2182,  2183,  2184,  2191,  2186,  2193, -1718,  2188,  1195,  2189,
      27,  2192,  2129,  2195,  2197,  2187,  2199,  2198,  2190,  2201,
    2203, -1718,   708,  2207,  2194,  2200,  2202,  2204,   264, -1718,
    2205,  2208, -1718,  2214, -1718,    30,  2219, -1718,  2206,  2209,
   -1718,  2211,  2212,  2224, -1718,  2227,  2210,  2233,  2231,  2215,
    2226,   267,  2217,  2237,  2223,  2196,  2213,  2239,  2240,  2230,
    2220,  2221,  2225,  2247,  2229,  2228, -1718,  2232,   411,  2234,
    2248,  2235,  2246,  2250,  2242,  2253,  2256,  2252,  2243,  2261,
    2251,  2262,  2267,  2259,  2249, -1718,  2216,  2263,  2255,  1754,
    2218,  2236,  2272,  2222,    41,  2273,  2265,  2257,  2280,  2258,
   -1718,  2274,  2238,  2276, -1718, -1718,  2285,  2260,  2277,  2241,
    2268,  2287,  2281,  2279,  2282,    46,  2294,  2293,  2244,    66,
    2286,  2275,  2283, -1718,  1507,  2288,  2289,  2254,  2290, -1718,
    2296,  2291,  2292,  2301,    68,  2295,  2305,  2297,  2264,  2304,
    2244,   189,  2309,  2299,  2298,  2310,  2315,  2302,  2300,  2303,
    2311,  2313,  2266,  2325,  2269,  2316,  2318,  2312,  2320,  2321,
    2322,  2328,  2307,  2319,  2338,  2341,  2324,  2329,  2343, -1718,
    2330,  2340,  2384,  2387, -1718,  2391,  2392,  2393,  2385,   411,
    2390,  2308,  2400,  2410,  2402,  2412,  2401,  2399,  2405,  2406,
    2416,  2413,  2395,  2408,  2403,  2424,  2407,  2417,  2414,  2409,
    2415,  2426,   462,  2428,   412,  2418,  2411,  2434,  2439, -1718,
    2422,  2440,  2437,  2419, -1718, -1718,  2423,  2444,  2442,  2429,
    2425,  2388,  2453,  2455,  2433,  2435,  2394,   542,  2454,  2448,
    2460, -1718,  2461,  2425,  2457,  2433,  2446,  2463,  2420,  2466,
    2449,  2450, -1718, -1718, -1718,  2469,  2471,  2456,  2472,  2458,
    2459,  2462,  2421,  2464,  2467,  2473,  2427, -1718,  2479,  2468,
    2478,  2480,  2470,  2451,  2465, -1718,  2487,  2474, -1718,  2475,
    2476,  2477, -1718, -1718,  2482,  2483,  2492,   411, -1718, -1718,
   -1718, -1718, -1718,  2489,  2484,  2498,  2499,  2485,  2500,  2502,
    2488,  2491,  2503,  2506,  2507,  2447,  2490,  2510, -1718,  2493,
    2511,   562,  2494,  2512,  2495,   577,  2496, -1718,  2508,  2504,
    2481,  2505,  2509,  2513,  2497,  2523,  2526,  2515,  2528,  2486,
    2514,   598,  2516,  2525, -1718,  2504,  2515,  2518,  2501,  2530,
    2517, -1718,  2527,  2519,  2529, -1718,  2520,  2521,  2522,  2531,
    2533,  2535,  2536,  2524,  2537,  2539,  2540,  2651, -1718,  2534,
    2538,  2716,  2542,  2541,  2544,  2532, -1718,  2543,  2545,  2546,
   -1718,  2549,  2547,  2651,  2552,  2551,  2554, -1718,   612,  2556,
    2557,  2553, -1718,  2550,  2559,  2558, -1718,   624,  2555,  2573,
    2561,  2564,  2562,  2563,  2578,  2566,  2717,  2586,  2585, -1718,
    2780,  2772,  2764, -1718,  2912,  3006,  3085,  3083,  3178,  3192,
    3269, -1718,  3274,  3367, -1718,  3368,  3463,  3464,  3557,  3548,
    3578,  3837,  3794,  3931,   296,  3952,  4030,  4024,  4124,  4119,
    4222,  4214,  4312, -1718,  4305,  4418,  4405,  4488,  4494, -1718,
    4586,  4607,  4681, -1718, -1718,  4684,  4799,  4797,  4788,  4860,
    4846,  4862,  4863,  4868,  4845,  4870,  4867,  4854,  4855, -1718,
    4807, -1718,  4857,  4858, -1718,  4876,  4861,  4859,  4853,  4864,
    4865,  4856,  4866,  4871,  4878,  4869,  4872,  4873, -1718,   678,
    4874, -1718,  4875,  4880,  4877,  4881, -1718,  4879,  4887, -1718,
    4890,  4882,  4891,  4895, -1718,  4883,  4886, -1718,  4897,  4884,
    4885,  4888,  4892,  4893,  4828,  4889,  4894,  4898,  4896,  4900,
    4899,  4902,  4903,  4901,  4906, -1718,  4904, -1718,  4905, -1718,
    4908,  4909,  4913,  4907,  4910, -1718,  4911,  4912,  4914,  4915,
   -1718,  4916, -1718,  4917, -1718,  4918, -1718, -1718, -1718, -1718,
    4919, -1718
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,     3,     9,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     0,     7,
       8,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    36,    37,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     1,   532,   533,     4,     0,   514,
      10,   502,   498,   497,   495,   496,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   450,   451,   452,   453,
     454,   455,   424,   425,   426,   427,   428,   429,   423,   499,
     422,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   487,   489,   490,   491,   492,   493,   494,   500,   501,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   531,   381,    12,    13,   415,   476,   421,    11,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,     0,     0,     0,     0,     0,     0,     0,     0,
     535,     0,     0,     0,   536,     0,     0,     0,     0,     0,
     552,     0,     0,     0,     5,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   382,   339,   383,   384,   385,
     386,   387,   388,   389,   390,   488,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   486,   416,     0,     0,     0,
       0,    14,    38,     0,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    17,     0,   130,   142,
     154,   161,   162,   163,     0,   146,     0,     0,     0,     0,
       0,   131,     0,     0,   132,     0,     0,     0,   138,     0,
     147,     0,     0,     0,     0,     0,   167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,   179,   180,
       0,     0,    15,     0,    16,     0,     0,    27,     0,     0,
       0,    28,     0,     0,     0,     0,    29,     0,     0,    30,
       0,     0,     0,     0,     0,     0,    18,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    20,     0,   262,   267,   260,   258,   263,   264,     0,
     266,   259,     0,   270,     0,     0,     0,     0,     0,     0,
      21,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,    53,    54,    55,    56,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    57,
      58,    59,    63,    64,    65,    62,    61,    60,    76,    77,
      78,    79,    80,    81,    82,    75,    25,    83,    84,    85,
      86,    96,    97,    98,    99,   100,   101,   102,   103,   104,
      87,    88,    89,    93,    94,    95,    92,    91,    90,   106,
     107,   108,   109,   110,   111,   112,   105,    33,     0,    31,
       0,     0,    32,   251,    26,     0,     0,     0,   542,     0,
       0,   661,   539,     0,     0,   545,   546,   541,   669,     0,
       0,   634,     0,     0,     0,   340,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   538,     0,     0,     0,     0,
       0,   664,     0,   537,     0,     0,     0,     0,     0,     0,
       0,   342,   343,   344,   345,   346,   347,   348,   349,   350,
      52,   285,     0,     0,    51,   305,     0,     0,    41,   304,
       0,     0,     0,     0,     0,     0,   325,   323,   324,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     136,     0,     0,     0,     0,     0,   164,   292,   165,     0,
       0,     0,     0,   169,   299,     0,   172,     0,   175,     0,
       0,     0,   160,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   218,     0,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   209,   214,     0,   309,   326,   211,     0,   228,
     231,   229,     0,     0,     0,     0,     0,     0,     0,     0,
     265,   268,   269,   271,   273,     0,   275,     0,     0,     0,
       0,   232,     0,   303,     0,   233,     0,   302,   118,     0,
       0,     0,     0,   122,   123,     0,     0,   128,     0,   129,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   205,   296,     0,   339,   297,   206,   207,
     201,   328,   329,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   216,   556,   543,     0,   662,   557,   551,
       0,     0,     0,     0,     0,     0,     0,     0,   411,     0,
     412,     0,   408,   417,   418,   524,   525,   407,     0,   394,
       0,   396,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   660,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     253,     0,     0,     0,   648,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   667,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   544,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,   353,   354,   355,   356,   357,   358,   521,   515,
     516,   518,   517,   519,   520,   522,   523,     0,   526,   410,
     409,   401,   413,   414,   419,   420,   402,   391,   393,     0,
     392,     0,     0,     0,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,     0,     0,     0,   256,
     257,   157,   158,   159,   148,     0,   143,     0,     0,   133,
     134,   284,     0,     0,     0,   149,     0,     0,   547,   168,
     139,     0,     0,   173,   171,   174,   176,   177,     0,     0,
     181,     0,     0,     0,     0,   293,     0,     0,     0,   222,
       0,     0,   220,     0,     0,     0,   224,   225,   226,   227,
       0,     0,     0,     0,   208,     0,   327,     0,   210,   212,
     295,   230,   301,     0,     0,     0,     0,     0,     0,   261,
     272,   274,     0,   276,     0,     0,   300,     0,   280,   119,
       0,   124,   310,     0,     0,     0,   127,     0,     0,     0,
     553,     0,   668,     0,   202,   203,   204,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   330,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   529,
     527,   397,   398,   395,   645,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   611,   598,     0,
       0,     0,   650,     0,     0,   656,   651,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   654,   655,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   666,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   331,   332,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   558,     0,   560,     0,     0,     0,
       0,    39,   530,   528,     0,     0,     0,    49,     0,    44,
       0,    46,     0,     0,     0,     0,   560,     0,     0,   635,
       0,     0,   657,     0,     0,     0,     0,   156,   144,     0,
     152,   135,   312,   137,     0,   140,     0,   150,   170,     0,
       0,     0,     0,   117,   294,     0,     0,     0,   319,   322,
     321,   221,   219,     0,     0,     0,     0,   635,     0,     0,
     495,   496,     0,     0,   254,   313,   318,   317,   316,   315,
     215,   308,   213,   307,     0,     0,     0,     0,     0,     0,
       0,     0,   278,     0,   120,     0,     0,   126,   644,     0,
       0,     0,     0,     0,     0,     0,   236,     0,   286,   238,
     287,   240,     0,     0,   247,   248,   249,   241,   288,   242,
     289,   290,   243,   291,     0,   244,   245,   246,   183,     0,
     653,     0,   185,     0,   190,     0,   652,     0,     0,     0,
       0,     0,     0,     0,   197,   196,     0,   199,   200,   198,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   563,     0,     0,   636,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   611,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,     0,     0,    40,
       0,     0,     0,     0,    45,    47,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   658,     0,   145,
     153,   151,   141,   166,     0,   182,     0,     0,     0,     0,
     320,     0,     0,     0,     0,     0,   255,   399,   406,   405,
     400,   314,     0,     0,     0,     0,     0,     0,     0,     0,
     279,   121,   125,   311,     0,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   184,     0,     0,   188,   298,
     336,   437,   187,   186,     0,     0,     0,     0,   193,   194,
     191,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   596,     0,     0,     0,     0,   554,     0,     0,
       0,   548,     0,   337,   338,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   550,     0,
     646,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   239,
       0,     0,   333,     0,     0,   192,   195,     0,   649,   235,
       0,   585,     0,     0,     0,     0,   581,     0,     0,     0,
     591,     0,     0,     0,     0,     0,     0,   587,   663,   277,
       0,     0,     0,     0,   631,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   596,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   631,     0,   334,   335,     0,     0,
       0,     0,     0,   582,   580,     0,     0,     0,   584,     0,
       0,     0,     0,   586,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   640,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   597,     0,     0,     0,
       0,     0,   640,     0,     0,     0,     0,     0,     0,     0,
       0,   632,     0,     0,     0,     0,     0,     0,     0,   549,
       0,     0,   643,     0,   579,     0,     0,   583,     0,     0,
     665,     0,     0,     0,   639,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   639,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   659,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     642,     0,     0,     0,   647,   540,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   612,     0,     0,   628,   623,
       0,     0,     0,   623,     0,     0,     0,     0,     0,   612,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   613,
       0,     0,     0,     0,   624,     0,     0,     0,     0,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   615,
       0,     0,     0,     0,   610,   561,     0,     0,     0,     0,
     621,   627,     0,     0,   615,     0,   589,     0,     0,     0,
       0,   621,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   628,   630,   633,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   589,     0,     0,
       0,     0,     0,     0,     0,   641,     0,     0,   620,     0,
       0,     0,   616,   588,     0,     0,     0,   114,   115,   306,
     281,   282,   283,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,     0,     0,   625,     0,   604,
     606,     0,     0,     0,     0,     0,     0,   625,     0,     0,
       0,     0,     0,     0,   604,     0,     0,     0,     0,     0,
       0,   609,     0,     0,     0,   575,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   608,   603,     0,
       0,     0,     0,     0,     0,     0,   626,     0,     0,     0,
     116,     0,     0,     0,     0,     0,     0,   629,     0,     0,
       0,     0,   593,     0,     0,     0,   572,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   590,
       0,     0,     0,   577,     0,     0,     0,     0,     0,     0,
       0,   592,     0,     0,   571,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   608,     0,     0,     0,     0,     0,   568,
       0,     0,     0,   595,   637,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   578,
       0,   567,     0,     0,   594,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   566,     0,
       0,   570,     0,     0,     0,     0,   638,     0,     0,   619,
       0,     0,     0,     0,   565,     0,     0,   569,     0,     0,
       0,     0,     0,     0,   599,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   622,     0,   601,     0,   618,
       0,     0,     0,     0,     0,   617,     0,     0,     0,     0,
     574,     0,   605,     0,   614,     0,   602,   573,   607,   562,
       0,   600
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718, -1718, -1718, -1718, -1718, -1718,  -982, -1718,
   -1718, -1718, -1718, -1718, -1718,  -925,  -492, -1683, -1718, -1620,
     -32, -1718,  -534, -1177, -1500,   798, -1438, -1718,  1105, -1349,
   -1335,  -959,  -905,  -468,  1095,   120,   797, -1718,  3972, -1368,
   -1717,  3472, -1416,  3228,  -954,  3242,  4022, -1718, -1718,  -986,
    -597, -1718,     8,   351, -1082,  3594,    12,  -544,  4290, -1718,
   -1718, -1718, -1323,  3385,  -886,    70,   769,  -894,   346,    73,
    -501,  -888, -1718, -1718,   -19,  4920, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718,  3587, -1718, -1718, -1718, -1718, -1718, -1718,
    -462,  3216, -1718,  -453, -1718, -1718, -1718, -1718, -1718, -1718,
   -1718, -1718, -1718,   467,  4792,  4794,  4795,  -346,  4786,  4789,
    4790,  4791,  4796,  4798,  4800,  4801,  4803,  4804,  4805,  4806,
   -1718,  4808,  4809,  4810,  4811,  4812,  4813,  4814,  -145,  -141,
    -131,   566,   571, -1718,  4815,  4816,  4817,  4818,  4819,  -490,
    4383,  4826, -1718, -1718, -1718, -1718,  4802,  -138, -1718, -1718,
    4825,  -220, -1718,  4829,  -420,   584,  4830,  4831,   437,  4832,
    4833, -1718,  4834,  4835,  -126, -1718,   593,   471,  -132,  4836,
   -1718,  4368, -1718, -1718,  -113, -1718, -1718, -1718,    65,  4838,
     486,   275,   373,   241, -1718, -1718,  4360, -1718, -1718,  -480,
     202, -1718, -1718, -1718,  -149, -1718, -1718, -1718, -1718,  4837,
    4839, -1718, -1718,  4841, -1718, -1718, -1718, -1718,  4824,   115,
   -1718, -1718, -1718
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    16,    17,    18,    19,    20,   251,   415,   446,   312,
     314,   373,   266,   384,   336,   484,   317,   321,   326,   329,
     341,   370,   400,   402,   835,   479,   482,   748,  1234,  1178,
     351,   477,   801,   805,  2407,  1190,   670,  1617,  1619,  1627,
    1629,  1632,   726,  1214,  1563,  1239,   873,  1858,   733,  1255,
    1241,   806,   802,   678,   674,  2408,  1592,  1590,   773,  1261,
    1842,  1551,  1584,  1585,  1219,  1568,   684,   774,  1237,   880,
    1586,  1859,   875,   162,  1138,  1369,   734,   679,   930,   931,
    1148,  1588,   675,  1724,   920,  1570,  1589,   922,   165,   166,
     167,   924,   925,   926,   927,    67,    68,    21,    22,    23,
      24,    25,    26,  1157,    27,    28,    29,    30,    31,    32,
    1631,  1625,  1626,   685,    33,    34,   385,   828,   829,    35,
      36,    37,    38,   252,   268,   269,   270,   706,   416,   417,
     418,   419,   420,   421,   422,   423,   424,   425,   426,   427,
     762,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   271,   272,   273,   440,   441,   442,   443,   444,   371,
     745,   274,  2410,  2411,  2412,   387,   275,   276,   277,   278,
     279,   280,   838,   281,   282,   283,   284,   285,   253,   287,
     288,   390,   289,   290,   291,   292,   293,   294,   254,   296,
     297,   298,   842,   843,   660,   789,   861,    39,   299,   300,
     255,   302,   256,   304,   305,   483,   865,   818,   686,   687,
     688,   324,   869,   306,   307,   862,   851,   846,   847,   308,
     309,  1179,  1180,   310,    40,    41,   900,    42,   445,   855,
     399,   830,    43
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     168,  1183,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   874,   750,  1139,   752,  1527,  1635,
    1238,   163,  1154,   396,  1145,  1143,  1415,  1553,   295,   337,
     342,   466,  1143,  1192,  1222,   467,   683,   720,  1874,   376,
     392,   339,   344,   803,  1638,   468,  1370,   791,  1642,  1274,
    1275,  1276,  1714,  -381,  1789,  1851,   744,   813,  2174,  2279,
    2280,   737,   739,  2186,  1866,  1558,  2174,    49,  1606,  2223,
    2281,  1210,   728,   736,   738,   225,   227,   228,   229,   230,
     231,   232,   233,   234,  1603,  2299,   757,   759,  1569,   753,
     764,   765,   766,   767,    50,   749,  1236,   831,   811,  1591,
    1593,  2175,  1607,   795,   797,  2304,  2187,  2304,  1841,  2200,
    1253,    61,  2224,  1257,   832,   794,   796,  1660,  1661,   792,
     833,  1854,   524,   834,    51,   817,   819,   636,  2300,   814,
      52,   405,    62,  1296,   804,    44,  1648,  1649,  1650,  1651,
    1652,   901,   525,   704,  -381,  -381,  -381,   637,  2305,   893,
    2316,  2064,    45,   705,  1654,  1655,   414,  1657,   368,  1659,
      46,   923,   923,    47,    57,    53,    48,   369,  2065,   894,
     812,  1314,   815,   710,   711,   712,   713,   714,   715,   716,
     717,   718,   719,    58,   721,   722,   723,   724,   860,   729,
     730,   731,   732,   319,   735,   883,   884,   740,   741,   742,
    1813,   698,   515,   516,   989,   699,   320,  2321,  2202,   215,
     661,   662,   663,   664,   665,   666,   667,   668,   669,   247,
     248,  1556,  2026,   510,   511,   512,   513,   514,  1622,  2322,
     676,   677,   340,   345,   249,    59,   377,   393,   528,  1139,
     250,  1145,  1176,  1623,   521,   529,   682,   530,   531,   532,
     533,   534,  1372,   535,   536,  2025,   537,   538,   539,   522,
     540,  1177,   541,   542,   543,   544,   545,  2128,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,  1523,  2090,
      54,   556,  2217,  2080,   558,  2129,   559,  2237,    60,   561,
     680,   676,   677,   564,   565,   566,  2091,  1826,   568,   569,
    2218,   570,   571,   681,  2219,  2238,   609,   682,   576,   577,
     578,   579,  1869,   580,   581,   582,   583,   346,   525,   258,
     319,   347,   348,   589,   521,  2569,  1855,  1524,  1862,   864,
     590,  1795,  1525,   591,   349,   592,   593,   594,   595,   586,
     350,  2564,   597,   598,   854,   599,   600,   601,   602,   603,
     604,   606,   630,  1815,   607,   631,   610,   611,   612,   613,
     614,   615,   616,   617,   618,   619,   620,   621,   622,   623,
     624,  1877,   625,   707,   626,   856,   519,   708,  1049,   810,
    1867,   837,  1878,   803,  1569,    64,  1879,   709,   859,   520,
    1050,   857,   858,   854,   215,   661,   662,   663,   664,   665,
     666,   667,   668,   669,   676,   677,   328,   331,    55,   923,
     923,  1144,   379,   395,    56,    63,   756,   923,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   659,  1593,  1197,
    2383,  1199,   803,   247,   325,   864,   334,   867,   332,   322,
    2059,  2384,   333,   334,   365,  2385,    65,    66,   249,   640,
     854,   641,   845,  2060,   250,   335,   643,   803,  1571,  1572,
    1156,  1158,   702,   836,  1529,   703,  1531,  1193,  1139,   840,
    1883,   803,  1636,  1637,   803,   839,   809,   680,   676,   677,
    2380,  1884,   404,   405,   406,   407,   408,   409,   844,   209,
     756,  2381,  1984,   410,   682,   210,  1985,  1560,   161,   411,
     246,  1203,  1204,  1205,  1206,   412,   413,   825,   414,  1986,
     826,   191,   257,   671,   258,   259,   260,   261,   192,  1221,
     827,  1223,   676,   677,  1226,  1227,  1228,  1229,  1300,   262,
     193,  1263,   803,   303,   747,   263,   264,   323,   265,   194,
    1543,   671,   366,  1544,  1250,  1251,   727,   215,   661,   662,
     663,   664,   665,   666,   667,   668,   669,  1543,  1266,  1699,
    1674,   727,  1700,   727,   672,   673,   505,   810,   505,   505,
     505,   505,   505,   505,   505,   505,  1658,  2405,  2406,   775,
    2469,   207,   779,   780,   781,   584,   516,   208,   212,  1319,
    1320,  2470,   790,   727,   213,  2474,   380,   286,   247,   325,
     381,   334,   671,   727,  1310,  1311,  2475,  1313,   375,  1315,
     401,   403,   195,   382,   848,   849,   197,   852,   853,   383,
     672,   673,   265,   877,   878,   879,   881,   267,  1144,  1209,
    2527,   917,   923,   196,   318,   198,   352,   923,   374,   386,
     362,  2528,  2535,   199,   478,   480,   301,   485,   816,   676,
     677,   327,   330,  2536,   937,   938,   939,   378,   394,   225,
     227,   228,   229,   230,   231,   232,   233,   234,  1731,  1743,
    1774,  1732,  1681,  1775,  1143,   676,   677,   956,   957,   958,
     959,   960,   961,   962,   963,   964,   965,   966,   967,   968,
     969,   200,   970,   971,   972,   973,  2615,  1155,   676,   677,
     975,   976,   977,   978,  2180,  2181,   979,  2616,   980,   201,
     981,   982,   983,   202,  1809,   985,   986,  1812,   203,  1939,
     990,   991,  1681,   863,   866,   870,  2210,  2211,   996,   997,
     998,   999,   921,   928,   204,  1002,  1003,  1004,  1005,   932,
     932,   438,   469,  1010,   206,  1011,   439,   470,  1012,  1013,
     727,   338,   343,  1014,   313,   315,   758,   760,   205,   211,
    1021,    65,   311,   316,   325,   250,  1022,   247,  1023,   481,
    1025,   486,   487,  1027,  1794,  1028,  1796,  1029,   164,  2198,
    1030,   488,  1032,  1033,   489,   492,  1034,   490,   491,  1036,
    1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,   493,  1045,
    1046,  1047,   494,   495,   496,   499,  2309,  1053,  1054,  1055,
    1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,  1065,
    1066,   497,  1068,  1069,  1070,   803,  1072,   803,   498,  1873,
    1076,  1077,  1078,  1079,  1080,   500,  1082,  1083,   501,  1085,
    1086,  1087,   502,   503,  1521,   504,   506,   507,  1917,   508,
     517,   509,   518,   523,  1091,  1092,   526,   562,   803,   567,
     874,   923,   557,   527,   563,   560,   572,   573,   585,  1088,
    1089,  1108,  1109,   215,   661,   662,   663,   664,   665,   666,
     667,   668,   669,   574,     1,   575,   587,     2,   588,   725,
     605,  1140,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,  1530,   520,  1532,   596,   608,   627,   628,     3,
     923,     4,     5,     6,     7,   629,   632,   634,  1624,   633,
       8,   638,     9,   635,   639,    10,    11,   648,   645,   647,
      12,    13,    14,   642,    15,   644,   646,   803,   649,   650,
    1090,   652,  1604,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   651,   653,   654,  1616,   655,   656,  1621,
    1213,   657,   876,  1184,  1634,   658,  1187,  1188,   775,  1191,
    1191,   671,  1194,  1195,   689,   727,   690,   727,   659,  1201,
     691,    -6,    -6,   693,   692,  1207,   694,   696,   695,  1982,
     697,   700,   701,  1215,   704,   743,   746,   747,   751,   754,
     755,   761,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   763,   768,   769,   770,  1240,  1242,  2029,   771,   783,
     787,   785,   784,   786,  2409,   798,   800,   788,   810,  1256,
     820,   821,   823,   822,  1262,   671,   824,   850,  1120,  1121,
    1122,  1123,  1124,  1125,  1126,  1127,   885,   881,   881,   881,
     886,   854,   887,  1128,   888,   890,   889,   891,   892,   923,
    1129,  1130,  1131,   896,  1132,   895,   897,   902,  1294,   898,
    1191,   904,  1298,   369,   727,   899,   905,   907,   906,   908,
     909,   912,  2073,   910,   913,   911,   914,   915,  1318,   935,
    1346,   916,  1133,   671,   671,   934,  1134,   936,  1135,   940,
    1136,   941,  1137,   942,   943,   944,   946,   945,   246,   948,
     947,   949,   246,   246,  1140,   950,   951,   952,  2409,   637,
     953,   955,   954,   974,   984,  1377,   987,  1379,  1380,  1381,
    1382,  1000,   992,   988,   994,   803,   993,   803,  1001,   995,
    1006,  1007,  1009,   246,  1015,  1008,  1016,  1017,  1018,  1400,
    1019,  1020,  1024,   874,  1031,  1035,  1401,  1026,  1402,  1403,
    1038,  1037,  1039,  1404,  1405,  1406,  1040,  1407,  1408,  1042,
    1041,  1043,  1409,  1410,  1044,  1048,  1051,  1067,  1052,  1071,
    1412,  1075,  1074,  1084,  1081,  1416,  1073,  1093,  1094,  1095,
    1420,  1098,  1422,  1420,  1423,   215,   661,   662,   663,   664,
     665,   666,   667,   668,   669,  1430,   923,  1096,  1432,  1430,
    1433,  1097,   226,   226,  1797,  1215,   505,  1100,   246,  1099,
    1101,  1102,  1104,  1103,  1105,  1106,  1442,  1107,  1443,  1110,
    1444,  1111,  1113,  1316,  1446,  1447,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,  1112,  1114,  1116,  1115,
    1117,  1118,  1458,  1459,  1460,  1461,  1462,  1463,  1464,  1465,
    1466,  1467,  1468,  1469,  1147,  1471,  1149,  1473,  1150,  1475,
    1151,  1477,  1152,  1153,  1160,  1481,  1482,  1483,  1484,  1485,
    1161,  1487,  1488,  1162,  1490,  1491,  1492,  1163,  1164,  1317,
    1495,  1496,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,  1165,  1166,  1167,  1168,  1169,  1512,  1171,  1170,
    1172,  1173,  1174,  1945,  1493,  1494,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,  1198,  1208,  1211,  1212,
    1217,  1218,  1216,  1224,  1225,   772,   776,   777,  1230,  1231,
    1232,  1233,  1243,  1140,  1244,  1245,  1246,  1247,   671,  1248,
     803,  1252,  1254,  1260,  1267,  2020,  1265,   807,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,  1268,  1269,
    1270,  1271,  1272,  1273,  1277,  1278,  1279,  1280,   703,  1256,
    1295,  1301,  1299,   882,  2021,  1302,  1562,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,  1303,  1309,  1312,
    1321,  1322,  1323,  1324,  1549,  1550,  1552,  1262,  1554,  1325,
    1215,  1557,  1242,  1326,  1327,  1328,  1329,  1330,  1564,  1331,
    1332,  1333,  1334,  1338,  1335,  1339,  1340,  1341,  1392,   505,
     505,   226,  1342,  1587,  1344,  1343,  1373,  1376,  1345,  1375,
    1374,  1384,  1378,   803,  1385,  1256,   671,  1388,  1262,  1242,
    1386,  1389,  1383,  1391,  1393,  1387,  1394,  1390,  1395,  1396,
     671,  1618,  1399,   671,   727,  1628,  1630,  1397,   671,   877,
     881,   881,  1587,  1413,  1398,  1552,  1587,  1411,  1417,  1418,
    1414,  1424,  1425,  1242,  1242,  1242,  1242,  1242,  1427,  1428,
    1429,  1435,  1684,  1436,  1437,  1438,  1439,  1262,  1262,  1441,
    1419,  1689,  1690,  1691,  1692,   871,   872,   215,   661,   662,
     663,   664,   665,   666,   667,   668,   669,  1426,  1434,  1706,
    1707,   246,   246,  1440,  1445,  1708,  1709,  1710,  1448,  1449,
    1450,  1712,   246,  1452,  1451,  1453,   246,  1454,  1455,  1456,
    1472,  1457,  1420,  1420,  1476,  1478,  1479,  1480,  1486,  1489,
    1497,  1498,  1499,  1726,  1726,  1501,   246,  1502,  1507,  1500,
    1504,  1503,  1505,  1506,  1564,  1511,  1509,  1508,  1510,  1514,
    1515,   246,  1516,  1519,  1737,  1738,  1520,  1740,  1088,  1089,
    1513,  1518,  1517,  1526,  1535,  1528,  1534,  1536,  1748,  1856,
    1857,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,  1537,  1539,  1538,  1540,  1751,  1541,  1542,  1561,  1545,
    1546,  1559,  1753,  1524,  1577,  1565,  1566,  1525,  1573,  1757,
    1758,  1759,  1760,  1761,  1575,  1576,  1574,  1578,  1595,   246,
    1596,  1764,  1765,   246,  1594,  1597,  1598,  1601,  1599,  1605,
    1600,  1609,  1608,  1611,  1610,  1612,  1613,   874,  1782,  1614,
    1640,  1645,  1646,  1615,  1662,  1653,  1664,   226,   226,   505,
    1647,  1668,  1670,  1665,  1656,  1671,  1672,  1675,  1678,  1663,
    1666,  1679,   246,  1681,   778,  1680,  1667,  1682,   782,  1669,
    1673,  1683,  1676,  1685,  1686,  1817,  1628,   793,  1688,  1687,
    1694,  1696,  1256,  1701,   808,  1829,  1693,  1702,  1829,  1695,
    1697,  1698,  1703,  1704,  1705,  1713,  1711,  1721,  1715,  1718,
    1728,  1716,  1564,  1719,  1256,  1720,  1722,  1729,  1730,   807,
    1733,  1185,  1186,  1734,  1736,  1587,  1735,  1739,  1744,  1741,
    1587,  1196,  1742,  1745,  1747,  1200,  1749,  1750,   903,  1756,
    1628,  1746,  1843,  1755,  1762,  1763,  1767,  1766,  1776,  1769,
    1618,  1860,  1770,  1628,  1587,  1220,  1587,  1220,  1771,  1618,
    1262,  1240,   877,  1768,  1772,  1773,  1191,  1843,  1904,  1235,
    1777,  1235,   876,  1778,  1779,  1780,  1781,  1786,  1783,  1784,
    1430,  1787,  1785,  1430,  1894,  1788,  1790,  1791,  1798,  1792,
    1793,  1799,  1804,  1800,  1802,  1806,  1801,  1808,  1814,  2274,
    1836,  1848,  1805,  1803,   882,   882,   882,  1807,   876,  1889,
    1816,   876,  1852,  1818,  1821,   876,  1819,  1822,  1823,  1824,
    1825,  1832,  1833,  1834,  1293,  1942,  1835,  1837,  1297,  1838,
    1844,  1839,  1845,  1846,   505,   505,  1847,  1948,  1949,  1849,
    1850,  1853,  1872,  1864,  1865,  1952,  1871,  1876,  1881,  1882,
    1885,  1875,  1891,  1880,  1943,  1944,  1888,  1893,  1886,  1890,
    1887,  1895,  1892,  1896,  1898,  1972,  1897,  1336,  1899,  1900,
    1901,  1903,  1905,  1902,  1908,  1907,  1906,  1909,  1910,  1911,
    1912,  1913,  1914,  1916,   246,  1915,  1922,  1918,  1997,  1919,
    1920,   246,  1921,  1923,  1925,  1929,  1931,  1924,  1940,  1941,
    2019,  1926,  1930,   246,  1927,  1928,  1933,  1958,  1932,  1935,
    1934,  1936,  1937,  1959,  1947,   246,  1938,  1953,  1954,  1946,
    1955,   226,   226,  1950,  1951,   246,  1956,  1957,  1960,  1963,
    1961,  1962,  1966,  1964,  1965,  1967,  1969,  1970,  1979,  1974,
    1968,  1971,  1973,  2022,  1630,  1975,  1976,  1978,  1977,  1980,
    1618,  1843,  1981,  1987,  1191,  1983,   246,  1990,  1992,  1993,
    1988,  1991,  1994,  1999,  1995,  1996,  1998,  2000,  2001,   246,
    1989,  2033,  2002,  2004,  2049,   246,  2003,  2028,  2005,   246,
    1347,  1348,  1349,  1350,  1351,  1352,  1353,  1354,  1355,  1356,
    1357,  1358,  1359,  1360,  1361,  1362,  1363,  1364,  1365,  1366,
    1367,  1368,  2006,  2011,  2008,  2007,  2009,  2027,  2010,  2012,
    2013,  2015,  1119,  2017,  2014,  2016,  2031,  2018,  2096,  2097,
    2030,  2023,  2024,  2034,  2032,  2035,  2036,  2037,  2039,  2038,
    2040,  2042,  2044,  2041,  2043,  2045,  2046,  2048,  2047,  2050,
    2051,  2052,  2053,  2054,  2138,  2063,  2056,  2061,  2055,  2062,
    2057,  1175,  2058,  1181,  1182,  2066,  2067,  2068,  2070,  2069,
    2071,   246,  2075,  2072,   876,  2074,  2076,  2078,  2077,   876,
    2079,  2080,  2082,  1202,  2083,  2081,  2084,  2086,  2085,  2087,
    2088,  2089,  2092,  2093,  2094,  2095,  2098,  2099,  2101,  2104,
    2106,  2109,  2100,   876,   226,   876,  2103,  2102,  2105,  2111,
    2107,   876,  2108,  2110,  2117,  2112,  2118,  2114,  2113,  2116,
    2119,  2120,  2122,  2115,  2124,  2123,  1249,  2125,  2121,  2126,
    2127,  2131,  2132,  2133,  1258,  1259,  2130,  2134,  2136,  1264,
    2135,  2137,  2139,  2141,  2144,  2140,  2142,  2145,  2148,  2147,
    1256,  2143,  2149,  2150,  2153,  2151,   246,   246,  1281,  1282,
    1283,  1284,  1285,  1286,  1287,  1288,  1289,  1290,  1291,  1292,
    1548,  2154,  2146,  2155,   246,  2152,  1555,  2156,  2157,  2254,
    2159,  1304,  1305,  1306,  1307,  1308,  2160,  2161,  2162,  1567,
    2164,  1220,  1220,  2165,  2158,  2166,  2167,  2168,  2169,  1579,
    2163,  2170,  2171,  2172,  2176,  2173,  2178,   226,   226,  2177,
    2183,   505,  2179,  2188,  1337,  2189,  2182,  2184,  2185,  2194,
    2190,  2175,  2193,  2196,  2202,  2203,  2206,  2192,  2205,  1620,
    2191,  2195,  2197,  2199,  1633,  2212,  2201,   882,   882,  2204,
    1639,  2209,  2207,  2222,  1643,  2208,  2213,  2225,  2214,  2228,
    2221,  2230,  2215,  2227,  2216,  2220,  2231,  2229,  2226,  2233,
    2232,  2234,  2239,  2235,  2236,  2240,   877,  2241,  2244,  2246,
    2245,  2248,  2247,  2250,  2251,  2249,  2258,  2256,  2276,  2252,
    2259,  2260,  2253,  2261,  2255,  2257,  2262,  2265,  2242,  2266,
    2357,  2263,  2267,  2264,   505,   505,   226,  2268,  2269,  2270,
    2273,  2272,  2277,  2282,  2283,  2243,  2285,  2284,  2271,  2286,
    2275,  2290,  2291,  2287,  2278,  2289,  2292,  2295,  2294,  2297,
    2296,  2298,  2301,  2302,  2313,  2306,  2307,  2308,  2310,  2315,
    2288,  2318,  2320,  2293,  2317,  2323,  2303,  2304,  2321,  2359,
    2328,  2299,  2312,  2314,  2325,  2326,  2311,  2324,  2329,  2319,
    2330,  1725,  1725,  2333,  2327,  2335,  2300,  2331,  2332,  2342,
    2337,  2334,  2336,  2338,  2343,  2339,  2340,  2341,  2346,  2349,
    2350,   226,   226,   505,  2347,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,  2344,  2451,   929,
    2345,  2348,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,  2351,  2352,  2353,  2356,  2354,  2355,  2358,  2360,
    2361,  2362,  2363,  2365,  2364,  2367,  2368,   505,   505,  2366,
    2370,  2369,  2371,  2372,  2373,  2374,  2375,  2379,  2382,  2376,
    2377,  2387,  2386,  2378,  2388,  2389,  2390,  2392,  2391,  2394,
    2393,  2395,  2396,  2397,  2398,  1235,  1810,  1811,  1235,  2400,
    2399,  2401,  2402,  2417,  2413,  2403,  2404,  2414,  2415,  2416,
    2418,  2419,  2421,  1828,  2423,  2422,  1828,  2424,  2425,  2434,
    2426,  2441,  2427,  2428,  2429,  1547,  2430,  2436,  2438,  2432,
    2439,  2433,  2420,  2431,  2437,  2443,  2442,  2452,  2440,  2435,
    2444,  2445,  2450,  2447,  2446,  2448,  2453,  2449,  2454,  2455,
    2457,  2456,  2458,  2461,  2459,  2460,  2462,  2463,  2466,  2464,
    2465,  2473,  2468,  2472,  2467,  2471,  2476,  1602,  2483,  2484,
    2477,  2480,  2485,  2478,  2487,  2492,  2495,  1868,  1870,  2481,
    2493,  2496,  2489,  2482,  2486,  2501,  2491,  2502,  2499,  2497,
    2500,  2490,  1644,  2479,  2520,  2518,  2503,  2504,  2488,  2505,
    2512,  2506,  2508,  2509,  2526,  2510,   807,  2515,  2541,  2513,
    2533,  2516,  2517,  2494,  2522,  2521,  2523,  2519,  2524,  2525,
    2529,  2532,  2530,  2531,  2538,   876,  2537,  2543,  2534,  2539,
    2540,  2498,  2546,  2542,  2544,  2547,  2507,  1677,    69,   235,
      71,    72,    73,    74,    75,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,   672,   673,   918,
      99,   919,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    69,   235,    71,    72,    73,    74,    75,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,  1141,  2511,  1142,    99,   919,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,  2514,  2545,
    2548,  2549,  2550,    69,   235,    71,    72,    73,    74,    75,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,  1840,  1146,  1142,    99,   919,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    69,   235,
      71,    72,    73,    74,    75,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,  1827,  2551,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    69,   235,    71,    72,    73,    74,    75,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,  2552,  1830,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,  2553,  2554,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    69,   235,    71,    72,    73,    74,    75,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,  2555,  2556,   918,    99,   919,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    69,   235,
      71,    72,    73,    74,    75,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,  2557,  2558,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    69,   235,    71,    72,    73,    74,    75,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,  2559,  2560,  1371,    99,   919,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    69,   235,
      71,    72,    73,    74,    75,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,  2561,  2562,  1142,
      99,   919,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    69,   235,    71,    72,    73,    74,    75,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,  2563,  2564,  2565,    99,   919,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    69,   235,
      71,    72,    73,  1580,  1581,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,  1582,  1583,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    69,   235,    71,    72,    73,  1580,  1581,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,  1582,  1583,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,  1861,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    69,   235,    71,    72,
      73,    74,    75,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,  2566,  2567,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      69,   235,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,  2568,
    2570,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    69,   235,  1421,    72,    73,    74,
      75,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,  2571,  2572,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,    69,   235,
    1431,    72,    73,    74,    75,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,  2573,  2574,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    69,   235,  1470,    72,    73,    74,    75,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,  2569,  2575,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    69,   235,  1474,    72,
      73,    74,    75,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,  2576,  2577,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      69,   235,  1717,    72,    73,    74,    75,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,  2578,
    2579,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    69,   235,  1723,    72,    73,    74,
      75,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,  2580,  2581,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,    69,   235,
    1752,    72,    73,    74,    75,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,  2582,  2583,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    69,   235,  1754,    72,    73,    74,    75,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,  2584,  2585,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    69,   235,    71,    72,
      73,    74,    75,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,  2586,  2587,  2588,    99,   919,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
    2589,  2590,  2591,  2592,  2593,  2594,  2595,  2596,  2597,  2599,
    2598,  2600,  2602,  2601,  2603,  2605,  2608,  2604,  2611,  2607,
    2619,  2621,  2612,  2606,  2609,  2610,  2620,  2623,  2618,  2624,
    2636,  2626,  2613,  2614,  2617,  2627,  2625,  2630,  2628,  2622,
    2629,  2641,  2643,  2644,  2631,  2648,  2633,  2632,  1189,  2637,
    2634,  2635,  2647,  2651,  2638,  1641,  2639,  2640,  2661,  2642,
    2645,  2646,  2649,  2654,  2650,  2652,  1831,  2655,  2653,  1820,
    2660,  1159,  2656,  1522,  2658,  2657,  2659,   933,  1727,  1533,
    1863,   353,   447,   354,   355,   448,   449,   450,   799,   841,
     868,   356,   451,     0,   452,     0,   453,   454,   214,   455,
     456,   457,   458,     0,   459,   460,   461,   462,   463,   464,
     465,   471,   472,   473,   474,   475,   372,   388,   357,   358,
     476,   359,   360,   389,   361,   363,   391,   364,     0,   397,
     367,   398
};

static const yytype_int16 yycheck[] =
{
      19,   960,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,   621,   559,   920,   561,  1377,  1467,
    1012,    19,   937,   182,   922,   921,  1213,  1405,   170,   177,
     178,   186,   928,   968,   998,   186,   514,   539,  1765,   181,
     182,   177,   178,   597,  1470,   186,  1138,   591,  1474,  1045,
    1046,  1047,  1562,     6,  1684,  1748,   556,   601,    49,    28,
      29,   551,   552,    49,  1757,  1410,    49,    30,  1446,    49,
      39,   986,   544,   551,   552,    77,    78,    79,    80,    81,
      82,    83,    84,    85,  1443,    49,   564,   565,  1421,   561,
     568,   569,   570,   571,    29,   558,  1011,    26,   600,  1432,
    1433,    92,  1447,   593,   594,    49,    92,    49,  1738,    92,
    1025,    30,    92,  1028,    43,   593,   594,  1495,  1496,   591,
      49,  1751,    30,    52,    39,   603,   604,    30,    92,   601,
      45,    27,    51,  1068,   597,    29,  1481,  1482,  1483,  1484,
    1485,   641,    50,    39,    97,    98,    99,    50,    92,    29,
      92,    29,    46,    49,  1487,  1488,    52,  1490,    30,  1492,
      42,   672,   673,    45,    26,    30,    48,    39,    46,    49,
     600,  1086,   602,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,    45,   540,   541,   542,   543,   618,   545,
     546,   547,   548,    29,   550,   625,   626,   553,   554,   555,
    1710,    44,    51,    52,   748,    48,    42,    28,    29,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    28,
      29,  1408,  1949,   252,   253,   254,   255,   256,    29,    50,
      32,    33,   177,   178,    43,    51,   181,   182,   267,  1143,
      49,  1139,    27,    44,    39,   274,    48,   276,   277,   278,
     279,   280,  1148,   282,   283,  1948,   285,   286,   287,    54,
     289,    46,   291,   292,   293,   294,   295,    30,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,  1370,    29,
      44,   310,    28,    29,   313,    48,   315,    30,    30,   318,
      31,    32,    33,   322,   323,   324,    46,  1723,   327,   328,
      46,   330,   331,    44,    50,    48,    38,    48,   337,   338,
     339,   340,  1760,   342,   343,   344,   345,    26,    50,    28,
      29,    30,    31,   352,    39,    39,  1752,    43,  1754,    29,
     359,  1690,    48,   362,    43,   364,   365,   366,   367,    54,
      49,    55,   371,   372,    44,   374,   375,   376,   377,   378,
     379,    28,    27,  1712,    31,    30,   385,   386,   387,   388,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,    28,   401,   528,   403,    26,    31,   528,    39,    30,
    1758,   611,    39,   937,  1717,     0,    43,   528,   618,    44,
      51,    42,    43,    44,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    32,    33,   175,   176,    39,   920,
     921,   922,   181,   182,    45,    44,    44,   928,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    43,  1761,   973,
      28,   975,   986,    28,    29,    29,    31,    31,    26,   174,
      31,    39,    30,    31,   179,    43,    98,    99,    43,   478,
      44,   480,   611,    44,    49,    43,   485,  1011,  1422,  1423,
     938,   939,    39,   611,  1379,    42,  1381,   969,  1372,   611,
      28,  1025,  1468,  1469,  1028,   611,   599,    31,    32,    33,
      28,    39,    26,    27,    28,    29,    30,    31,   611,    44,
      44,    39,    26,    37,    48,    50,    30,  1412,    97,    43,
     164,   979,   980,   981,   982,    49,    50,    26,    52,    43,
      29,    29,    26,   511,    28,    29,    30,    31,    46,   997,
      39,   999,    32,    33,  1002,  1003,  1004,  1005,  1072,    43,
      30,  1033,  1086,   170,    44,    49,    50,   174,    52,    46,
      49,   539,   179,    52,  1022,  1023,   544,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    49,  1036,    46,
      52,   559,    49,   561,    32,    33,   225,    30,   227,   228,
     229,   230,   231,   232,   233,   234,  1491,    45,    46,   577,
      28,    43,   580,   581,   582,    51,    52,    49,    43,  1091,
    1092,    39,   590,   591,    49,    28,    26,   170,    28,    29,
      30,    31,   600,   601,  1082,  1083,    39,  1085,   181,  1087,
     183,   184,    30,    43,   612,   613,    39,   615,   616,    49,
      32,    33,    52,   621,   622,   623,   624,   170,  1139,   985,
      28,   660,  1143,    49,   173,    44,   179,  1148,   181,   182,
     179,    39,    28,    48,   187,   188,   170,   190,    31,    32,
      33,   175,   176,    39,   683,   684,   685,   181,   182,   661,
     662,   663,   664,   665,   666,   667,   668,   669,    46,    46,
      46,    49,    49,    49,  1570,    32,    33,   706,   707,   708,
     709,   710,   711,   712,   713,   714,   715,   716,   717,   718,
     719,    50,   721,   722,   723,   724,    28,    31,    32,    33,
     729,   730,   731,   732,    28,    29,   735,    39,   737,    49,
     739,   740,   741,    49,  1706,   744,   745,  1709,    26,    46,
     749,   750,    49,   618,   619,   620,    28,    29,   757,   758,
     759,   760,   672,   673,    46,   764,   765,   766,   767,   676,
     677,   185,   186,   772,    39,   774,   185,   186,   777,   778,
     748,   177,   178,   782,   171,   172,   564,   565,    52,    27,
     789,    98,    43,    43,    29,    49,   795,    28,   797,    39,
     799,    30,    26,   802,  1689,   804,  1691,   806,    19,  2138,
     809,    55,   811,   812,    26,    29,   815,    50,    50,   818,
      10,    11,    12,    13,    14,    15,    16,    17,    43,   828,
     829,   830,    30,    50,    29,    45,  2254,   836,   837,   838,
     839,   840,   841,   842,   843,   844,   845,   846,   847,   848,
     849,    30,   851,   852,   853,  1379,   855,  1381,    50,  1764,
     859,   860,   861,   862,   863,    30,   865,   866,    29,   868,
     869,   870,    49,    26,  1346,    51,    38,    27,  1817,    39,
      38,    30,    27,    39,   883,   884,    45,    27,  1412,    27,
    1467,  1372,    39,    45,    45,    39,    51,    31,    31,   871,
     872,   900,   901,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    54,     1,    39,    39,     4,    30,    26,
      51,   920,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,  1380,    44,  1382,    44,    39,    51,    51,    26,
    1421,    28,    29,    30,    31,    38,    31,    42,  1462,    54,
      37,    45,    39,    39,    45,    42,    43,    26,    48,    48,
      47,    48,    49,    44,    51,    44,    50,  1491,    48,    52,
       7,    28,  1444,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    50,    46,    27,  1458,    26,    92,  1461,
     989,    29,   621,   961,  1466,    44,   964,   965,   966,   967,
     968,   969,   970,   971,    29,   973,    26,   975,    43,   977,
      26,    98,    99,    31,    92,   983,    37,    92,    42,  1904,
      29,    48,    50,   991,    39,    39,    29,    44,    29,    92,
      49,    29,   661,   662,   663,   664,   665,   666,   667,   668,
     669,    92,    29,    31,    92,  1013,  1014,  1952,    29,    29,
      44,    92,    31,    29,  2357,    39,    49,    43,    30,  1027,
      29,    38,    29,    31,  1032,  1033,    30,    52,    10,    11,
      12,    13,    14,    15,    16,    17,    29,  1045,  1046,  1047,
      31,    44,    26,    25,    92,    31,    52,    92,    45,  1570,
      32,    33,    34,    48,    36,    44,    50,    49,  1066,    39,
    1068,    29,  1070,    39,  1072,    43,    30,    50,    45,    50,
      30,    44,  1997,    48,    39,    49,    39,    29,  1090,    45,
    1119,    39,    64,  1091,  1092,    42,    68,    30,    70,    39,
      72,    39,    74,    44,    39,    30,    30,    39,   772,    39,
      49,    29,   776,   777,  1143,    52,    30,    26,  2451,    50,
      28,    44,    51,    42,    44,  1154,    39,  1156,  1157,  1158,
    1159,    27,    39,    45,    50,  1689,    39,  1691,    39,    45,
      39,    30,    39,   807,    39,    49,    30,    49,    39,  1178,
      29,    39,    44,  1760,    31,    45,  1185,    50,  1187,  1188,
      45,    39,    30,  1192,  1193,  1194,    39,  1196,  1197,    51,
      42,    45,  1201,  1202,    44,    51,    45,    26,    45,    26,
    1209,    39,    30,    39,    30,  1214,    51,    39,    31,    44,
    1219,    30,  1221,  1222,  1223,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,  1234,  1717,    39,  1237,  1238,
    1239,    39,   871,   872,  1692,  1213,   875,    27,   882,    49,
      39,    28,    30,    29,    26,    28,  1255,    39,  1257,    50,
    1259,    55,    28,     7,  1263,  1264,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    39,    44,    92,    44,
      92,    29,  1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,
    1289,  1290,  1291,  1292,    32,  1294,    35,  1296,    33,  1298,
      26,  1300,    44,    28,    45,  1304,  1305,  1306,  1307,  1308,
      42,  1310,  1311,    44,  1313,  1314,  1315,    44,    28,     7,
    1319,  1320,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    44,    30,    92,    92,    30,  1336,    42,    26,
      30,    43,    31,     7,  1316,  1317,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    42,    49,    92,    44,
      44,    44,    92,    92,    44,   576,   577,   578,    45,    28,
      30,    92,    45,  1372,    28,    30,    92,    92,  1346,    29,
    1904,    49,    48,    31,    45,     7,    48,   598,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    28,    92,
      42,    50,    28,    50,    29,    29,    50,    42,    42,  1377,
      42,    29,    43,   624,     7,    52,  1415,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    29,    49,    42,
      45,    30,    44,    44,  1402,  1403,  1404,  1405,  1406,    28,
    1408,  1409,  1410,    28,    30,    26,    92,    30,  1416,    92,
      26,    42,    30,    48,    29,    27,    48,    30,    48,  1088,
    1089,  1090,    49,  1431,    30,    39,    34,    51,    39,    92,
      50,    26,    39,  1997,    30,  1443,  1444,    30,  1446,  1447,
      49,    44,    92,    30,    43,    50,    92,    39,    49,    30,
    1458,  1459,    30,  1461,  1462,  1463,  1464,    45,  1466,  1467,
    1468,  1469,  1470,    39,    55,  1473,  1474,    50,    39,    49,
      92,    39,    49,  1481,  1482,  1483,  1484,  1485,    50,    44,
      39,    50,  1521,    44,    39,    30,    39,  1495,  1496,    51,
      92,  1530,  1531,  1532,  1533,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    92,    92,  1548,
    1549,  1185,  1186,    50,    30,  1554,  1555,  1556,    28,    92,
      50,  1560,  1196,    45,    39,    30,  1200,    39,    39,    30,
      51,    45,  1571,  1572,    30,    39,    30,    39,    28,    30,
      92,    48,    30,  1582,  1583,    30,  1220,    50,    30,    49,
      42,    44,    39,    42,  1562,    39,    92,    43,    92,    30,
      39,  1235,    42,    31,  1603,  1604,    92,  1606,  1580,  1581,
      51,    44,    50,    48,    27,    42,    45,    42,  1617,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    50,    29,    92,    44,  1634,    31,    49,    44,    50,
      48,    48,  1641,    43,    29,    44,    50,    48,    44,  1648,
    1649,  1650,  1651,  1652,    45,    92,    50,    44,    92,  1293,
      29,  1660,  1661,  1297,    45,    44,    31,    43,    92,    28,
      48,    45,    30,    44,    92,    92,    48,  2254,  1677,    45,
      30,    45,    42,    92,    45,    48,    42,  1316,  1317,  1318,
      92,    29,    44,    50,    92,    42,    31,    28,    43,    92,
      92,    27,  1336,    49,   579,    48,    92,    31,   583,    92,
      92,    39,    92,    45,    30,  1714,  1684,   592,    30,    49,
      42,    48,  1690,    31,   599,  1724,    51,    50,  1727,    49,
      39,    49,    45,    30,    28,    49,    51,    39,    49,    49,
       6,    48,  1710,    48,  1712,    51,    46,    51,    39,   960,
      31,   962,   963,    39,    30,  1723,    51,    50,    28,    51,
    1728,   972,    39,    26,    39,   976,    30,    45,   643,    28,
    1738,    92,  1740,    92,    39,    46,    31,    51,    26,    48,
    1748,  1753,    43,  1751,  1752,   996,  1754,   998,    39,  1757,
    1758,  1759,  1760,    92,    92,    39,  1764,  1765,  1797,  1010,
      31,  1012,  1431,    28,    45,    45,    29,    50,    30,    30,
    1809,    30,    51,  1812,  1782,    44,    29,    28,    50,    39,
      92,    30,    30,    51,    51,    39,    44,    30,    43,    55,
      30,    28,    48,    50,  1045,  1046,  1047,    49,  1467,    30,
      50,  1470,    31,    50,    50,  1474,    51,    51,    50,    44,
      51,    50,    44,    51,  1065,  1854,    50,    44,  1069,    43,
      50,    44,    44,    51,  1493,  1494,    45,  1866,  1867,    45,
      44,    44,    26,    45,    45,  1874,    46,    39,    39,    44,
      44,    50,    39,    51,  1856,  1857,    44,    26,    51,    45,
      50,    44,    51,    43,    28,  1894,    48,  1108,    46,    26,
      51,    26,    46,    52,    49,    43,    92,    50,    48,    28,
      30,    28,    44,    48,  1548,    30,    46,    48,  1917,    43,
      48,  1555,    43,    49,    46,    28,    30,    50,    26,    30,
    1942,    49,    46,  1567,    50,    48,    46,    43,    50,    50,
      49,    48,    50,    29,    44,  1579,    51,    46,    42,    51,
      45,  1580,  1581,    50,    50,  1589,    44,    39,    49,    31,
      45,    44,    30,    50,    48,    28,    28,    44,    48,    30,
      51,    50,    50,  1945,  1942,    51,    50,    42,    51,    30,
    1948,  1949,    49,    30,  1952,    51,  1620,    51,    26,    30,
      50,    50,    50,    30,    44,    51,    51,    51,    30,  1633,
      92,    31,    51,    92,  1972,  1639,    50,    38,    51,  1643,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    50,    44,    51,    92,    50,    39,    51,    51,
      50,    48,   917,    51,    92,    50,    42,    51,  2020,  2021,
      51,    50,    50,    29,    51,    30,    39,    50,    31,    51,
      45,    51,    50,    92,    45,    44,    30,    26,    50,    49,
      44,    43,    92,    50,  2073,    26,    92,    39,    49,    44,
      50,   956,    51,   958,   959,    43,    92,    43,    26,    92,
      50,  1725,    44,    43,  1723,    43,    43,    50,    44,  1728,
      26,    29,    26,   978,    29,    50,    43,    50,    92,    50,
      50,    26,    30,    46,    50,    42,    46,    48,    50,    39,
      26,    29,    45,  1752,  1753,  1754,    45,    44,    44,    51,
      44,  1760,    45,    43,    30,    92,    28,    92,    50,    50,
      39,    29,    29,    92,    37,    31,  1021,    29,    92,    50,
      44,    30,    28,    92,  1029,  1030,    45,    28,    49,  1034,
      37,    30,    30,    30,    44,    50,    50,    30,    30,    44,
    2138,    92,    30,    28,    44,    49,  1810,  1811,  1053,  1054,
    1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,
    1401,    30,    92,    45,  1828,    92,  1407,    28,    51,  2198,
      50,  1076,  1077,  1078,  1079,  1080,    51,    45,    44,  1420,
      50,  1422,  1423,    50,    92,    44,    50,    30,    30,  1430,
      92,    49,    28,    28,    44,    29,    52,  1856,  1857,    38,
      30,  1860,    39,    28,  1109,    52,    39,    39,    39,    28,
      39,    92,    38,    30,    29,    28,    28,    44,    29,  1460,
      48,    45,    44,    44,  1465,    28,    44,  1468,  1469,    52,
    1471,    38,    52,    29,  1475,    44,    52,    28,    48,    38,
      42,    27,    50,    44,    50,    50,    29,    45,    52,    26,
      50,    30,    45,    48,    38,    28,  2254,    44,    29,    39,
      30,    50,    52,    26,    45,    50,    30,    29,    42,    51,
      30,    39,    50,    30,    50,    50,    30,    26,    92,    38,
    2309,    39,    30,    50,  1943,  1944,  1945,    30,    39,    50,
      45,    38,    30,    30,    39,    92,    26,    50,    92,    51,
      92,    26,    52,    39,    92,    39,    39,    30,    50,    40,
      39,    39,    28,    30,    28,    39,    51,    44,    39,    28,
      92,    26,    28,    92,    39,    26,    92,    49,    28,    31,
      40,    49,    52,    52,    46,    30,    92,    48,    45,    52,
      39,  1582,  1583,    28,    52,    39,    92,    44,    92,    52,
      48,    92,    44,    43,    45,    44,    44,    39,    44,    39,
      30,  2020,  2021,  2022,    45,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    49,  2407,    34,
      49,    48,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    48,    46,    43,    50,    44,    44,    48,    39,
      30,    39,    30,    44,    43,    39,    30,  2096,  2097,    44,
      55,    38,    44,    50,    30,    48,    39,    31,    30,    45,
      51,    50,    44,    48,    30,    26,    44,    30,    28,    46,
      51,    27,    30,    44,    49,  1706,  1707,  1708,  1709,    26,
      92,    26,    49,    26,    30,    50,    92,    39,    28,    28,
      44,    28,    26,  1724,    44,    46,  1727,    28,    27,    26,
      44,    50,    30,    45,    45,  1400,    44,    28,    30,    45,
      30,    44,    92,    92,    46,    28,    51,    28,    48,    92,
      46,    46,    30,    46,    48,    43,    42,    44,    30,    30,
      30,    46,    30,    30,    46,    44,    30,    30,    28,    92,
      50,    46,    31,    31,    51,    51,    50,  1442,    51,    26,
      42,    46,    26,    49,    26,    30,    26,  1759,  1761,    50,
      42,    44,    48,    50,    49,    44,    50,    45,    39,    42,
      50,  2451,  1477,    92,    29,    43,    45,    44,    92,    44,
      46,    45,    45,    44,    30,    45,  1817,    45,    26,    51,
      31,    50,    48,    92,    45,    49,    49,    54,    46,    48,
      44,    51,    45,    50,    31,  2254,    51,    29,    50,    48,
      46,    92,    26,    50,    48,    30,    92,  1512,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    92,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    92,    92,
      30,    39,    48,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,  1737,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    26,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    26,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    42,    45,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    44,    31,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    48,    44,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    45,    45,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    44,    44,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    45,    55,    26,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    48,    92,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    48,
      28,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    43,    50,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    43,    49,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    39,    48,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    43,    51,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    31,
      45,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    55,    50,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    51,    31,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    50,    48,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    26,    29,    39,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      30,    45,    30,    30,    26,    50,    26,    30,    44,    92,
      45,    44,    26,    45,    43,    52,    50,    48,    30,    44,
      30,    30,    43,    49,    48,    44,    39,    30,    43,    29,
      92,    30,    50,    50,    50,    30,    44,    30,    45,    50,
      44,    31,    30,    30,    50,    30,    48,    52,   966,    50,
      48,    48,    38,    30,    50,  1473,    48,    51,    29,    50,
      49,    45,    44,    42,    45,    48,  1728,    45,    48,  1717,
      42,   939,    48,  1369,    48,    50,    49,   677,  1583,  1382,
    1754,   179,   186,   179,   179,   186,   186,   186,   595,   611,
     620,   179,   186,    -1,   186,    -1,   186,   186,    68,   186,
     186,   186,   186,    -1,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   180,   182,   179,   179,
     186,   179,   179,   182,   179,   179,   182,   179,    -1,   182,
     179,   182
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     1,     4,    26,    28,    29,    30,    31,    37,    39,
      42,    43,    47,    48,    49,    51,   101,   102,   103,   104,
     105,   197,   198,   199,   200,   201,   202,   204,   205,   206,
     207,   208,   209,   214,   215,   219,   220,   221,   222,   297,
     324,   325,   327,   332,    29,    46,    42,    45,    48,    30,
      29,    39,    45,    30,    44,    39,    45,    26,    45,    51,
      30,    30,    51,    44,     0,    98,    99,   195,   196,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,   173,   176,   186,   188,   189,   190,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,    29,    46,    30,    46,    30,    49,    39,    44,    48,
      50,    49,    49,    26,    46,    52,    39,    43,    49,    44,
      50,    27,    43,    49,   195,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,   172,   173,   172,   172,   172,
     172,   172,   172,   172,   172,     4,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,   188,    28,    29,    43,
      49,   106,   223,   278,   288,   300,   302,    26,    28,    29,
      30,    31,    43,    49,    50,    52,   112,   223,   224,   225,
     226,   251,   252,   253,   261,   266,   267,   268,   269,   270,
     271,   273,   274,   275,   276,   277,   278,   279,   280,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   298,
     299,   300,   301,   302,   303,   304,   313,   314,   319,   320,
     323,    43,   109,   286,   110,   286,    43,   116,   287,    29,
      42,   117,   301,   302,   311,    29,   118,   300,   303,   119,
     300,   303,    26,    30,    31,    43,   114,   267,   275,   284,
     298,   120,   267,   275,   284,   298,    26,    30,    31,    43,
      49,   130,   223,   224,   225,   226,   266,   273,   276,   279,
     280,   283,   287,   289,   299,   301,   302,   323,    30,    39,
     121,   259,   261,   111,   223,   278,   288,   298,   300,   303,
      26,    30,    43,    49,   113,   216,   223,   265,   270,   277,
     281,   282,   288,   298,   300,   303,   314,   319,   320,   330,
     122,   278,   123,   278,    26,    27,    28,    29,    30,    31,
      37,    43,    49,    50,    52,   107,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     254,   255,   256,   257,   258,   328,   108,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   254,   255,   256,   257,   258,   328,   131,   223,   125,
     223,    39,   126,   305,   115,   223,    30,    26,    55,    26,
      50,    50,    29,    43,    30,    50,    29,    30,    50,    45,
      30,    29,    49,    26,    51,   173,    38,    27,    39,    30,
     194,   194,   194,   194,   194,    51,    52,    38,    27,    31,
      44,    39,    54,    39,    30,    50,    45,    45,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,    39,   194,   194,
      39,   194,    27,    45,   194,   194,   194,    27,   194,   194,
     194,   194,    51,    31,    54,    39,   194,   194,   194,   194,
     194,   194,   194,   194,    51,    31,    54,    39,    30,   194,
     194,   194,   194,   194,   194,   194,    44,   194,   194,   194,
     194,   194,   194,   194,   194,    51,    28,    31,    39,    38,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,    51,    51,    38,
      27,    30,    31,    54,    42,    39,    30,    50,    45,    45,
     194,   194,    44,   194,    44,    48,    50,    48,    26,    48,
      52,    50,    28,    46,    27,    26,    92,    29,    44,    43,
     294,    11,    12,    13,    14,    15,    16,    17,    18,    19,
     136,   176,    32,    33,   154,   182,    32,    33,   153,   177,
      31,    44,    48,   153,   166,   213,   308,   309,   310,    29,
      26,    26,    92,    31,    37,    42,    92,    29,    44,    48,
      48,    50,    39,    42,    39,    49,   227,   248,   249,   250,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     136,   227,   227,   227,   227,    26,   142,   176,   210,   227,
     227,   227,   227,   148,   176,   227,   153,   309,   153,   309,
     227,   227,   227,    39,   259,   260,    29,    44,   127,   213,
     142,    29,   142,   210,    92,    49,    44,   153,   310,   153,
     310,    29,   240,    92,   153,   153,   153,   153,    29,    31,
      92,    29,   186,   158,   167,   176,   186,   186,   148,   176,
     176,   176,   148,    29,    31,    92,    29,    44,    43,   295,
     176,   142,   210,   148,   153,   309,   153,   309,    39,   260,
      49,   132,   152,   177,   213,   133,   151,   186,   148,   294,
      30,   136,   274,   142,   210,   274,    31,   153,   307,   153,
      29,    38,    31,    29,    30,    26,    29,    39,   217,   218,
     331,    26,    43,    49,    52,   124,   267,   271,   272,   284,
     288,   291,   292,   293,   294,   314,   317,   318,   176,   176,
      52,   316,   176,   176,    44,   329,    26,    42,    43,   271,
     274,   296,   315,   329,    29,   306,   329,    31,   306,   312,
     329,     8,     9,   146,   170,   172,   173,   176,   176,   176,
     169,   176,   186,   274,   274,    29,    31,    26,    92,    52,
      31,    92,    45,    29,    49,    44,    48,    50,    39,    43,
     326,   259,    49,   148,    29,    30,    45,    50,    50,    30,
      48,    49,    44,    39,    39,    29,    39,   194,    34,    36,
     184,   185,   187,   190,   191,   192,   193,   194,   185,    34,
     178,   179,   189,   178,    42,    45,    30,   194,   194,   194,
      39,    39,    44,    39,    30,    39,    30,    49,    39,    29,
      52,    30,    26,    28,    51,    44,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,    42,   194,   194,   194,   194,   194,
     194,   194,   194,   194,    44,   194,   194,    39,    45,   142,
     194,   194,    39,    39,    50,    45,   194,   194,   194,   194,
      27,    39,   194,   194,   194,   194,    39,    30,    49,    39,
     194,   194,   194,   194,   194,    39,    30,    49,    39,    29,
      39,   194,   194,   194,    44,   194,    50,   194,   194,   194,
     194,    31,   194,   194,   194,    45,   194,    39,    45,    30,
      39,    42,    51,    45,    44,   194,   194,   194,    51,    39,
      51,    45,    45,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,    26,   194,   194,
     194,    26,   194,    51,    30,    39,   194,   194,   194,   194,
     194,    30,   194,   194,    39,   194,   194,   194,   172,   172,
       7,   194,   194,    39,    31,    44,    39,    39,    30,    49,
      27,    39,    28,    29,    30,    26,    28,    39,   194,   194,
      50,    55,    39,    28,    44,    44,    92,    92,    29,   148,
      10,    11,    12,    13,    14,    15,    16,    17,    25,    32,
      33,    34,    36,    64,    68,    70,    72,    74,   174,   187,
     194,    32,    34,   184,   190,   191,    33,    32,   180,    35,
      33,    26,    44,    28,   152,    31,   153,   203,   153,   166,
      45,    42,    44,    44,    28,    44,    30,    92,    92,    30,
      26,    42,    30,    43,    31,   148,    27,    46,   129,   321,
     322,   148,   148,   151,   176,   186,   186,   176,   176,   158,
     135,   176,   135,   136,   176,   176,   186,   142,    42,   142,
     186,   176,   148,   153,   153,   153,   153,   176,    49,   227,
     152,    92,    44,   194,   143,   176,    92,    44,    44,   164,
     186,   153,   164,   153,    92,    44,   153,   153,   153,   153,
      45,    28,    30,    92,   128,   186,   152,   168,   128,   145,
     176,   150,   176,    45,    28,    30,    92,    92,    29,   148,
     153,   153,    49,   152,    48,   149,   176,   152,   148,   148,
      31,   159,   176,   136,   148,    48,   153,    45,    28,    92,
      42,    50,    28,    50,   169,   169,   169,    29,    29,    50,
      42,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   186,   176,    42,   135,   186,   176,    43,
     142,    29,    52,    29,   148,   148,   148,   148,   148,    49,
     153,   153,    42,   153,   152,   153,     7,     7,   172,   136,
     136,    45,    30,    44,    44,    28,    28,    30,    26,    92,
      30,    92,    26,    42,    30,    29,   186,   148,    48,    27,
      48,    30,    49,    39,    30,    39,   194,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,   175,
     174,    34,   184,    34,    50,    92,    51,   194,    39,   194,
     194,   194,   194,    92,    26,    30,    49,    50,    30,    44,
      39,    30,    48,    43,    92,    49,    30,    45,    55,    30,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,    50,   194,    39,    92,   143,   194,    39,    49,    92,
     194,     5,   194,   194,    39,    49,    92,    50,    44,    39,
     194,     5,   194,   194,    92,    50,    44,    39,    30,    39,
      50,    51,   194,   194,   194,    30,   194,   194,    28,    92,
      50,    39,    45,    30,    39,    39,    30,    45,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
       5,   194,    51,   194,     5,   194,    30,   194,    39,    30,
      39,   194,   194,   194,   194,   194,    28,   194,   194,    30,
     194,   194,   194,   172,   172,   194,   194,    92,    48,    30,
      49,    30,    50,    44,    42,    39,    42,    30,    43,    92,
      92,    39,   194,    51,    30,    39,    42,    50,    44,    31,
      92,   136,   175,   174,    43,    48,    48,   149,    42,   152,
     153,   152,   153,   203,    45,    27,    42,    50,    92,    29,
      44,    31,    49,    49,    52,    50,    48,   148,   186,   176,
     176,   161,   176,   159,   176,   186,   143,   176,   150,    48,
     152,    44,   194,   144,   176,    44,    50,   186,   165,   182,
     185,   164,   164,    44,    50,    45,    92,    29,    44,   186,
       8,     9,    32,    33,   162,   163,   170,   176,   181,   186,
     157,   182,   156,   182,    45,    92,    29,    44,    31,    92,
      48,    43,   148,   149,   136,    28,   159,   150,    30,    45,
      92,    44,    92,    48,    45,    92,   136,   137,   176,   138,
     186,   136,    29,    44,   142,   211,   212,   139,   176,   140,
     176,   210,   141,   186,   136,   146,   169,   169,   162,   186,
      30,   161,   162,   186,   154,    45,    42,    92,   150,   150,
     150,   150,   150,    48,   182,   182,    92,   182,   152,   182,
     159,   159,    45,    92,    42,    50,    92,    92,    29,    92,
      44,    42,    31,    92,    52,    28,    92,   148,    43,    27,
      48,    49,    31,    39,   194,    45,    30,    49,    30,   194,
     194,   194,   194,    51,    42,    49,    48,    39,    49,    46,
      49,    31,    50,    45,    30,    28,   194,   194,   194,   194,
     194,    51,   194,    49,   144,    49,    48,     5,    49,    48,
      51,    39,    46,     5,   183,   186,   194,   183,     6,    51,
      39,    46,    49,    31,    39,    51,    30,   194,   194,    50,
     194,    51,    39,    46,    28,    26,    92,    39,   194,    30,
      45,   194,     5,   194,     5,    92,    28,   194,   194,   194,
     194,   194,    39,    46,   194,   194,    51,    31,    92,    48,
      43,    39,    92,    39,    46,    49,    26,    31,    28,    45,
      45,    29,   194,    30,    30,    51,    50,    30,    44,   139,
      29,    28,    39,    92,   152,   149,   152,   153,    50,    30,
      51,    44,    51,    50,    30,    48,    39,    49,    30,   128,
     186,   186,   128,   144,    43,   149,    50,   194,    50,    51,
     165,    50,    51,    50,    44,    51,   162,    32,   186,   194,
      33,   163,    50,    44,    51,    50,    30,    44,    43,    44,
     148,   139,   160,   176,    50,    44,    51,    45,    28,    45,
      44,   137,    31,    44,   139,   162,     8,     9,   147,   171,
     172,    44,   162,   211,    45,    45,   137,   159,   145,   146,
     156,    46,    26,   135,   160,    50,    39,    28,    39,    43,
      51,    39,    44,    28,    39,    44,    51,    50,    44,    30,
      45,    39,    51,    26,   176,    44,    43,    48,    28,    46,
      26,    51,    52,    26,   194,    46,    92,    43,    49,    50,
      48,    28,    30,    28,    44,    30,    48,   151,    48,    43,
      48,    43,    46,    49,    50,    46,    49,    50,    48,    28,
      46,    30,    50,    46,    49,    50,    48,    50,    51,    46,
      26,    30,   194,   172,   172,     7,    51,    44,   194,   194,
      50,    50,   194,    46,    42,    45,    44,    39,    43,    29,
      49,    45,    44,    31,    50,    48,    30,    28,    51,    28,
      44,    50,   194,    50,    30,    51,    50,    51,    42,    48,
      30,    49,   152,    51,    26,    30,    43,    30,    50,    92,
      51,    50,    26,    30,    50,    44,    51,   194,    51,    30,
      51,    30,    51,    50,    92,    51,    50,    92,    51,    50,
      51,    44,    51,    50,    92,    48,    50,    51,    51,   140,
       7,     7,   172,    50,    50,   137,   160,    39,    38,   135,
      51,    42,    51,    31,    29,    30,    39,    50,    51,    31,
      45,    92,    51,    45,    50,    44,    30,    50,    26,   176,
      49,    44,    43,    92,    50,    49,    92,    50,    51,    31,
      44,    39,    44,    26,    29,    46,    43,    92,    43,    92,
      26,    50,    43,   152,    43,    44,    43,    44,    50,    26,
      29,    50,    26,    29,    43,    92,    50,    50,    50,    26,
      29,    46,    30,    46,    50,    42,   172,   172,    46,    48,
      45,    50,    44,    45,    39,    44,    26,    44,    45,    29,
      43,    51,    92,    50,    92,    92,    50,    30,    28,    39,
      29,    92,    29,    31,    37,    29,    50,    44,    30,    48,
      45,    30,    28,    92,    28,    37,    49,    30,   194,    30,
      50,    30,    50,    92,    44,    30,    92,    44,    30,    30,
      28,    49,    92,    44,    30,    45,    28,    51,    92,    50,
      51,    45,    44,    92,    50,    50,    44,    50,    30,    30,
      49,    28,    28,    29,    49,    92,    44,    38,    52,    39,
      28,    29,    39,    30,    39,    39,    49,    92,    28,    52,
      39,    48,    44,    38,    28,    45,    30,    44,   149,    44,
      92,    44,    29,    28,    52,    29,    28,    52,    44,    38,
      28,    29,    28,    52,    48,    50,    50,    28,    46,    50,
      50,    42,    29,    49,    92,    28,    52,    44,    38,    45,
      27,    29,    50,    26,    30,    48,    38,    30,    48,    45,
      28,    44,    92,    92,    29,    30,    39,    52,    50,    50,
      26,    45,    51,    50,   194,    50,    29,    50,    30,    30,
      39,    30,    30,    39,    50,    26,    38,    30,    30,    39,
      50,    92,    38,    45,    55,    92,    42,    30,    92,    28,
      29,    39,    30,    39,    50,    26,    51,    39,    92,    39,
      26,    52,    39,    92,    50,    30,    39,    40,    39,    49,
      92,    28,    30,    92,    49,    92,    39,    51,    44,   146,
      39,    92,    52,    28,    52,    28,    92,    39,    26,    52,
      28,    28,    50,    26,    48,    46,    30,    52,    40,    45,
      39,    44,    92,    28,    92,    39,    44,    48,    43,    44,
      44,    39,    52,    45,    49,    49,    44,    45,    48,    39,
      30,    48,    46,    43,    44,    44,    50,   194,    48,    31,
      39,    30,    39,    30,    43,    44,    44,    39,    30,    38,
      55,    44,    50,    30,    48,    39,    45,    51,    48,    31,
      28,    39,    30,    28,    39,    43,    44,    50,    30,    26,
      44,    28,    30,    51,    46,    27,    30,    44,    49,    92,
      26,    26,    49,    50,    92,    45,    46,   134,   155,   182,
     262,   263,   264,    30,    39,    28,    28,    26,    44,    28,
      92,    26,    46,    44,    28,    27,    44,    30,    45,    45,
      44,    92,    45,    44,    26,    92,    28,    46,    30,    30,
      48,    50,    51,    28,    46,    46,    48,    46,    43,    44,
      30,   194,    28,    42,    30,    30,    46,    30,    30,    46,
      44,    30,    30,    30,    92,    50,    28,    51,    31,    28,
      39,    51,    31,    46,    28,    39,    50,    42,    49,    92,
      46,    50,    50,    51,    26,    26,    49,    26,    92,    48,
     155,    50,    30,    42,    92,    26,    44,    42,    92,    39,
      50,    44,    45,    45,    44,    44,    45,    92,    45,    44,
      45,    92,    46,    51,    92,    45,    50,    48,    43,    54,
      29,    49,    45,    49,    46,    48,    30,    28,    39,    44,
      45,    50,    51,    31,    50,    28,    39,    51,    31,    48,
      46,    26,    50,    29,    48,    92,    26,    30,    30,    39,
      48,    26,    26,    42,    45,    44,    31,    48,    44,    45,
      45,    44,    44,    45,    55,    26,    48,    92,    48,    39,
      28,    43,    50,    43,    49,    48,    43,    51,    31,    45,
      55,    50,    51,    31,    50,    48,    26,    29,    39,    30,
      45,    30,    30,    26,    50,    26,    30,    44,    45,    92,
      44,    45,    26,    43,    48,    52,    49,    44,    50,    48,
      44,    30,    43,    50,    50,    28,    39,    50,    43,    30,
      39,    30,    50,    30,    29,    44,    30,    30,    45,    44,
      30,    50,    52,    48,    48,    48,    92,    50,    50,    48,
      51,    31,    50,    30,    30,    49,    45,    38,    30,    44,
      45,    30,    48,    48,    42,    45,    48,    50,    48,    49,
      42,    29
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   100,   101,   101,   102,   102,   103,   103,   103,   104,
     104,   104,   104,   104,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   109,   109,   109,   109,   110,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   114,   114,
     114,   114,   114,   114,   114,   114,   115,   116,   116,   117,
     117,   117,   117,   117,   118,   118,   119,   119,   120,   120,
     120,   120,   121,   121,   122,   123,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     125,   126,   127,   127,   128,   128,   129,   129,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   131,   132,   132,
     133,   134,   134,   134,   135,   136,   137,   138,   139,   140,
     140,   141,   142,   143,   144,   145,   146,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   162,   163,   163,   163,   163,   164,
     164,   165,   165,   166,   166,   166,   167,   168,   169,   169,
     170,   170,   170,   171,   171,   171,   171,   171,   171,   172,
     172,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   174,   174,   174,   174,   174,   174,   174,   174,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   177,   177,   178,   179,   179,   180,   180,   180,   181,
     181,   182,   182,   183,   183,   183,   183,   184,   184,   184,
     184,   185,   185,   185,   185,   186,   186,   187,   187,   187,
     187,   188,   188,   188,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   192,   192,   192,   193,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     3,     0,     1,     1,     1,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     7,
       9,     3,     9,    11,     7,     9,     7,     9,     5,     7,
       1,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,    16,    18,    18,    20,     7,     3,     5,
       7,     9,     3,     3,     5,     9,     7,     5,     3,     3,
       1,     1,     1,     5,     5,     7,     3,     7,     1,     5,
       7,     9,     1,     5,     7,     9,     1,     1,     5,     5,
       7,     9,     7,     9,     1,     5,     7,     5,     5,     5,
       3,     1,     1,     1,     3,     3,     9,     1,     5,     3,
       7,     5,     3,     5,     5,     3,     5,     5,     1,     1,
       1,     5,     9,     7,     9,     7,     9,     9,     9,     3,
       7,     9,    11,     9,     9,    11,     7,     7,     7,     7,
       7,     3,     5,     5,     5,     3,     3,     3,     5,     3,
       5,     3,     5,     7,     3,     7,     3,     3,     3,     7,
       5,     7,     5,     3,     5,     5,     5,     5,     3,     3,
       5,     3,     3,     3,     9,    11,     5,     7,     5,     9,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     1,     0,     2,     3,     5,     1,     1,     1,     1,
       1,     5,     1,     1,     1,     3,     1,     1,     3,     3,
       1,     3,     5,     3,     5,     3,     5,    11,     5,     7,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     3,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     4,     3,     4,     4,     1,     2,     2,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     2,     1,     3,     0,     2,     2,     3,
       3,     3,     3,     1,     1,     2,     2,     1,     1,     2,
       2,     1,     1,     2,     2,     1,     2,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     1,     2,     3,     4,     3,
       4,     1,     1,     1,     3,     3,     3,     5,     5,     4,
      11,     4,     4,     6,     7,     4,     4,     3,     4,     7,
       9,     6,     3,     5,     8,    12,     6,     6,     9,    11,
       7,    17,    30,     8,     4,    25,    24,    23,    22,    25,
      24,    21,    20,    29,    28,    19,    18,    19,    23,    13,
      12,    11,    12,    13,    12,    11,    12,    11,    18,    17,
      21,    11,    21,    20,    23,    22,    10,    11,     6,     9,
      14,    10,    29,    20,    19,    29,    19,    30,    20,    17,
      17,     6,    15,    16,    29,    17,    18,    28,    27,    25,
      18,    17,    27,    15,    16,    19,    20,    17,    15,    18,
      15,    10,    11,    15,     4,     7,     8,    23,    25,    14,
      13,    18,    14,    11,     5,     4,     9,    13,     4,     9,
       6,     6,     5,     5,     4,     4,     6,     7,     5,    10,
       4,     4,     6,     9,     5,    13,     4,     4,     3,     4
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
        yyerror (yyparse_param, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, yyparse_param); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void* yyparse_param)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yyparse_param);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void* yyparse_param)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, yyparse_param);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, void* yyparse_param)
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
                       &yyvsp[(yyi + 1) - (yynrhs)], yyparse_param);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, yyparse_param); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, void* yyparse_param)
{
  YY_USE (yyvaluep);
  YY_USE (yyparse_param);
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
yyparse (void* yyparse_param)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

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

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

/* User initialization code.  */
#line 191 "lscp.y"
{
    yyparse_param_t* p = (yyparse_param_t*) yyparse_param;
    p->ppStackBottom = &yyss;
    p->ppStackTop    = &yyssp;
}

#line 3780 "y.tab.c"

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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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
      yychar = yylex (&yylval);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* input: line  */
#line 227 "lscp.y"
                                { INCREMENT_LINE; if (!(yyvsp[0].String).empty()) LSCPSERVER->AnswerClient((yyvsp[0].String)); return LSCP_DONE; }
#line 3982 "y.tab.c"
    break;

  case 3: /* input: error  */
#line 228 "lscp.y"
                                { INCREMENT_LINE; LSCPSERVER->AnswerClient("ERR:0:" + sLastError + "\r\n"); RESTART; return LSCP_SYNTAX_ERROR; }
#line 3988 "y.tab.c"
    break;

  case 4: /* line: statement LF  */
#line 231 "lscp.y"
                                          { (yyval.String) = (yyvsp[-1].String); }
#line 3994 "y.tab.c"
    break;

  case 5: /* line: statement CR LF  */
#line 232 "lscp.y"
                                          { (yyval.String) = (yyvsp[-2].String); }
#line 4000 "y.tab.c"
    break;

  case 6: /* statement: %empty  */
#line 235 "lscp.y"
                                                                       { (yyval.String) = ""; }
#line 4006 "y.tab.c"
    break;

  case 7: /* statement: comment  */
#line 236 "lscp.y"
                                  { (yyval.String) = ""; }
#line 4012 "y.tab.c"
    break;

  case 14: /* command: ADD SP add_instruction  */
#line 247 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4018 "y.tab.c"
    break;

  case 15: /* command: MAP SP map_instruction  */
#line 248 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4024 "y.tab.c"
    break;

  case 16: /* command: UNMAP SP unmap_instruction  */
#line 249 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4030 "y.tab.c"
    break;

  case 17: /* command: GET SP get_instruction  */
#line 250 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4036 "y.tab.c"
    break;

  case 18: /* command: CREATE SP create_instruction  */
#line 251 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4042 "y.tab.c"
    break;

  case 19: /* command: DESTROY SP destroy_instruction  */
#line 252 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4048 "y.tab.c"
    break;

  case 20: /* command: LIST SP list_instruction  */
#line 253 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4054 "y.tab.c"
    break;

  case 21: /* command: LOAD SP load_instruction  */
#line 254 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4060 "y.tab.c"
    break;

  case 22: /* command: REMOVE SP remove_instruction  */
#line 255 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4066 "y.tab.c"
    break;

  case 23: /* command: SET SP set_instruction  */
#line 256 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4072 "y.tab.c"
    break;

  case 24: /* command: SUBSCRIBE SP subscribe_event  */
#line 257 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4078 "y.tab.c"
    break;

  case 25: /* command: UNSUBSCRIBE SP unsubscribe_event  */
#line 258 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4084 "y.tab.c"
    break;

  case 26: /* command: RESET SP reset_instruction  */
#line 259 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4090 "y.tab.c"
    break;

  case 27: /* command: CLEAR SP clear_instruction  */
#line 260 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4096 "y.tab.c"
    break;

  case 28: /* command: FIND SP find_instruction  */
#line 261 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4102 "y.tab.c"
    break;

  case 29: /* command: MOVE SP move_instruction  */
#line 262 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4108 "y.tab.c"
    break;

  case 30: /* command: COPY SP copy_instruction  */
#line 263 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4114 "y.tab.c"
    break;

  case 31: /* command: EDIT SP edit_instruction  */
#line 264 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4120 "y.tab.c"
    break;

  case 32: /* command: FORMAT SP format_instruction  */
#line 265 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4126 "y.tab.c"
    break;

  case 33: /* command: SEND SP send_instruction  */
#line 266 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4132 "y.tab.c"
    break;

  case 34: /* command: APPEND SP append_instruction  */
#line 267 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4138 "y.tab.c"
    break;

  case 35: /* command: INSERT SP insert_instruction  */
#line 268 "lscp.y"
                                                               { (yyval.String) = (yyvsp[0].String);                                                }
#line 4144 "y.tab.c"
    break;

  case 36: /* command: RESET  */
#line 269 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->ResetSampler();                        }
#line 4150 "y.tab.c"
    break;

  case 37: /* command: QUIT  */
#line 270 "lscp.y"
                                                               { LSCPSERVER->AnswerClient("Bye!\r\n"); return LSCP_QUIT; }
#line 4156 "y.tab.c"
    break;

  case 38: /* add_instruction: CHANNEL  */
#line 273 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->AddChannel();                  }
#line 4162 "y.tab.c"
    break;

  case 39: /* add_instruction: CHANNEL SP MIDI_INPUT SP sampler_channel SP device_index  */
#line 274 "lscp.y"
                                                                                                            { (yyval.String) = LSCPSERVER->AddChannelMidiInput((yyvsp[-2].Number),(yyvsp[0].Number));    }
#line 4168 "y.tab.c"
    break;

  case 40: /* add_instruction: CHANNEL SP MIDI_INPUT SP sampler_channel SP device_index SP midi_input_port_index  */
#line 275 "lscp.y"
                                                                                                            { (yyval.String) = LSCPSERVER->AddChannelMidiInput((yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].Number)); }
#line 4174 "y.tab.c"
    break;

  case 41: /* add_instruction: DB_INSTRUMENT_DIRECTORY SP db_path  */
#line 276 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->AddDbInstrumentDirectory((yyvsp[0].String));  }
#line 4180 "y.tab.c"
    break;

  case 42: /* add_instruction: DB_INSTRUMENTS SP NON_MODAL SP scan_mode SP db_path SP filename  */
#line 277 "lscp.y"
                                                                                                                { (yyval.String) = LSCPSERVER->AddDbInstruments((yyvsp[-4].String),(yyvsp[-2].String),(yyvsp[0].String), true);        }
#line 4186 "y.tab.c"
    break;

  case 43: /* add_instruction: DB_INSTRUMENTS SP NON_MODAL SP scan_mode SP FILE_AS_DIR SP db_path SP filename  */
#line 278 "lscp.y"
                                                                                                                { (yyval.String) = LSCPSERVER->AddDbInstruments((yyvsp[-6].String),(yyvsp[-2].String),(yyvsp[0].String), true, true); }
#line 4192 "y.tab.c"
    break;

  case 44: /* add_instruction: DB_INSTRUMENTS SP scan_mode SP db_path SP filename  */
#line 279 "lscp.y"
                                                                                                                { (yyval.String) = LSCPSERVER->AddDbInstruments((yyvsp[-4].String),(yyvsp[-2].String),(yyvsp[0].String));              }
#line 4198 "y.tab.c"
    break;

  case 45: /* add_instruction: DB_INSTRUMENTS SP scan_mode SP FILE_AS_DIR SP db_path SP filename  */
#line 280 "lscp.y"
                                                                                                                { (yyval.String) = LSCPSERVER->AddDbInstruments((yyvsp[-6].String),(yyvsp[-2].String),(yyvsp[0].String), false, true); }
#line 4204 "y.tab.c"
    break;

  case 46: /* add_instruction: DB_INSTRUMENTS SP NON_MODAL SP db_path SP filename  */
#line 281 "lscp.y"
                                                                                                                { (yyval.String) = LSCPSERVER->AddDbInstruments((yyvsp[-2].String),(yyvsp[0].String), -1, true);       }
#line 4210 "y.tab.c"
    break;

  case 47: /* add_instruction: DB_INSTRUMENTS SP NON_MODAL SP db_path SP filename SP instrument_index  */
#line 282 "lscp.y"
                                                                                                                { (yyval.String) = LSCPSERVER->AddDbInstruments((yyvsp[-4].String),(yyvsp[-2].String),(yyvsp[0].Number), true);        }
#line 4216 "y.tab.c"
    break;

  case 48: /* add_instruction: DB_INSTRUMENTS SP db_path SP filename  */
#line 283 "lscp.y"
                                                                                                                { (yyval.String) = LSCPSERVER->AddDbInstruments((yyvsp[-2].String),(yyvsp[0].String));                 }
#line 4222 "y.tab.c"
    break;

  case 49: /* add_instruction: DB_INSTRUMENTS SP db_path SP filename SP instrument_index  */
#line 284 "lscp.y"
                                                                                                                { (yyval.String) = LSCPSERVER->AddDbInstruments((yyvsp[-4].String),(yyvsp[-2].String),(yyvsp[0].Number));              }
#line 4228 "y.tab.c"
    break;

  case 50: /* add_instruction: MIDI_INSTRUMENT_MAP  */
#line 285 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->AddMidiInstrumentMap();                }
#line 4234 "y.tab.c"
    break;

  case 51: /* add_instruction: MIDI_INSTRUMENT_MAP SP map_name  */
#line 286 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->AddMidiInstrumentMap((yyvsp[0].String));              }
#line 4240 "y.tab.c"
    break;

  case 52: /* add_instruction: SEND_EFFECT_CHAIN SP device_index  */
#line 287 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->AddSendEffectChain((yyvsp[0].Number));                }
#line 4246 "y.tab.c"
    break;

  case 53: /* subscribe_event: AUDIO_OUTPUT_DEVICE_COUNT  */
#line 290 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_audio_device_count);   }
#line 4252 "y.tab.c"
    break;

  case 54: /* subscribe_event: AUDIO_OUTPUT_DEVICE_INFO  */
#line 291 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_audio_device_info);    }
#line 4258 "y.tab.c"
    break;

  case 55: /* subscribe_event: MIDI_INPUT_DEVICE_COUNT  */
#line 292 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_midi_device_count);    }
#line 4264 "y.tab.c"
    break;

  case 56: /* subscribe_event: MIDI_INPUT_DEVICE_INFO  */
#line 293 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_midi_device_info);     }
#line 4270 "y.tab.c"
    break;

  case 57: /* subscribe_event: CHANNEL_COUNT  */
#line 294 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_channel_count);        }
#line 4276 "y.tab.c"
    break;

  case 58: /* subscribe_event: CHANNEL_MIDI  */
#line 295 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_channel_midi);         }
#line 4282 "y.tab.c"
    break;

  case 59: /* subscribe_event: DEVICE_MIDI  */
#line 296 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_device_midi);          }
#line 4288 "y.tab.c"
    break;

  case 60: /* subscribe_event: VOICE_COUNT  */
#line 297 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_voice_count);          }
#line 4294 "y.tab.c"
    break;

  case 61: /* subscribe_event: STREAM_COUNT  */
#line 298 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_stream_count);         }
#line 4300 "y.tab.c"
    break;

  case 62: /* subscribe_event: BUFFER_FILL  */
#line 299 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_buffer_fill);          }
#line 4306 "y.tab.c"
    break;

  case 63: /* subscribe_event: CHANNEL_INFO  */
#line 300 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_channel_info);         }
#line 4312 "y.tab.c"
    break;

  case 64: /* subscribe_event: FX_SEND_COUNT  */
#line 301 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_fx_send_count);        }
#line 4318 "y.tab.c"
    break;

  case 65: /* subscribe_event: FX_SEND_INFO  */
#line 302 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_fx_send_info);         }
#line 4324 "y.tab.c"
    break;

  case 66: /* subscribe_event: MIDI_INSTRUMENT_MAP_COUNT  */
#line 303 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_midi_instr_map_count); }
#line 4330 "y.tab.c"
    break;

  case 67: /* subscribe_event: MIDI_INSTRUMENT_MAP_INFO  */
#line 304 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_midi_instr_map_info);  }
#line 4336 "y.tab.c"
    break;

  case 68: /* subscribe_event: MIDI_INSTRUMENT_COUNT  */
#line 305 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_midi_instr_count);     }
#line 4342 "y.tab.c"
    break;

  case 69: /* subscribe_event: MIDI_INSTRUMENT_INFO  */
#line 306 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_midi_instr_info);      }
#line 4348 "y.tab.c"
    break;

  case 70: /* subscribe_event: DB_INSTRUMENT_DIRECTORY_COUNT  */
#line 307 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_db_instr_dir_count);   }
#line 4354 "y.tab.c"
    break;

  case 71: /* subscribe_event: DB_INSTRUMENT_DIRECTORY_INFO  */
#line 308 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_db_instr_dir_info);    }
#line 4360 "y.tab.c"
    break;

  case 72: /* subscribe_event: DB_INSTRUMENT_COUNT  */
#line 309 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_db_instr_count);       }
#line 4366 "y.tab.c"
    break;

  case 73: /* subscribe_event: DB_INSTRUMENT_INFO  */
#line 310 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_db_instr_info);        }
#line 4372 "y.tab.c"
    break;

  case 74: /* subscribe_event: DB_INSTRUMENTS_JOB_INFO  */
#line 311 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_db_instrs_job_info);   }
#line 4378 "y.tab.c"
    break;

  case 75: /* subscribe_event: MISCELLANEOUS  */
#line 312 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_misc);                 }
#line 4384 "y.tab.c"
    break;

  case 76: /* subscribe_event: TOTAL_STREAM_COUNT  */
#line 313 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_total_stream_count);   }
#line 4390 "y.tab.c"
    break;

  case 77: /* subscribe_event: TOTAL_VOICE_COUNT  */
#line 314 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_total_voice_count);    }
#line 4396 "y.tab.c"
    break;

  case 78: /* subscribe_event: GLOBAL_INFO  */
#line 315 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_global_info);          }
#line 4402 "y.tab.c"
    break;

  case 79: /* subscribe_event: EFFECT_INSTANCE_COUNT  */
#line 316 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_fx_instance_count);    }
#line 4408 "y.tab.c"
    break;

  case 80: /* subscribe_event: EFFECT_INSTANCE_INFO  */
#line 317 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_fx_instance_info);     }
#line 4414 "y.tab.c"
    break;

  case 81: /* subscribe_event: SEND_EFFECT_CHAIN_COUNT  */
#line 318 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_send_fx_chain_count);  }
#line 4420 "y.tab.c"
    break;

  case 82: /* subscribe_event: SEND_EFFECT_CHAIN_INFO  */
#line 319 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->SubscribeNotification(LSCPEvent::event_send_fx_chain_info);   }
#line 4426 "y.tab.c"
    break;

  case 83: /* unsubscribe_event: AUDIO_OUTPUT_DEVICE_COUNT  */
#line 322 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_audio_device_count);   }
#line 4432 "y.tab.c"
    break;

  case 84: /* unsubscribe_event: AUDIO_OUTPUT_DEVICE_INFO  */
#line 323 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_audio_device_info);    }
#line 4438 "y.tab.c"
    break;

  case 85: /* unsubscribe_event: MIDI_INPUT_DEVICE_COUNT  */
#line 324 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_midi_device_count);    }
#line 4444 "y.tab.c"
    break;

  case 86: /* unsubscribe_event: MIDI_INPUT_DEVICE_INFO  */
#line 325 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_midi_device_info);     }
#line 4450 "y.tab.c"
    break;

  case 87: /* unsubscribe_event: CHANNEL_COUNT  */
#line 326 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_channel_count);        }
#line 4456 "y.tab.c"
    break;

  case 88: /* unsubscribe_event: CHANNEL_MIDI  */
#line 327 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_channel_midi);         }
#line 4462 "y.tab.c"
    break;

  case 89: /* unsubscribe_event: DEVICE_MIDI  */
#line 328 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_device_midi);          }
#line 4468 "y.tab.c"
    break;

  case 90: /* unsubscribe_event: VOICE_COUNT  */
#line 329 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_voice_count);          }
#line 4474 "y.tab.c"
    break;

  case 91: /* unsubscribe_event: STREAM_COUNT  */
#line 330 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_stream_count);         }
#line 4480 "y.tab.c"
    break;

  case 92: /* unsubscribe_event: BUFFER_FILL  */
#line 331 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_buffer_fill);          }
#line 4486 "y.tab.c"
    break;

  case 93: /* unsubscribe_event: CHANNEL_INFO  */
#line 332 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_channel_info);         }
#line 4492 "y.tab.c"
    break;

  case 94: /* unsubscribe_event: FX_SEND_COUNT  */
#line 333 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_fx_send_count);        }
#line 4498 "y.tab.c"
    break;

  case 95: /* unsubscribe_event: FX_SEND_INFO  */
#line 334 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_fx_send_info);         }
#line 4504 "y.tab.c"
    break;

  case 96: /* unsubscribe_event: MIDI_INSTRUMENT_MAP_COUNT  */
#line 335 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_midi_instr_map_count); }
#line 4510 "y.tab.c"
    break;

  case 97: /* unsubscribe_event: MIDI_INSTRUMENT_MAP_INFO  */
#line 336 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_midi_instr_map_info);  }
#line 4516 "y.tab.c"
    break;

  case 98: /* unsubscribe_event: MIDI_INSTRUMENT_COUNT  */
#line 337 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_midi_instr_count);     }
#line 4522 "y.tab.c"
    break;

  case 99: /* unsubscribe_event: MIDI_INSTRUMENT_INFO  */
#line 338 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_midi_instr_info);      }
#line 4528 "y.tab.c"
    break;

  case 100: /* unsubscribe_event: DB_INSTRUMENT_DIRECTORY_COUNT  */
#line 339 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_db_instr_dir_count);   }
#line 4534 "y.tab.c"
    break;

  case 101: /* unsubscribe_event: DB_INSTRUMENT_DIRECTORY_INFO  */
#line 340 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_db_instr_dir_info);    }
#line 4540 "y.tab.c"
    break;

  case 102: /* unsubscribe_event: DB_INSTRUMENT_COUNT  */
#line 341 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_db_instr_count);       }
#line 4546 "y.tab.c"
    break;

  case 103: /* unsubscribe_event: DB_INSTRUMENT_INFO  */
#line 342 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_db_instr_info);        }
#line 4552 "y.tab.c"
    break;

  case 104: /* unsubscribe_event: DB_INSTRUMENTS_JOB_INFO  */
#line 343 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_db_instrs_job_info);   }
#line 4558 "y.tab.c"
    break;

  case 105: /* unsubscribe_event: MISCELLANEOUS  */
#line 344 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_misc);                 }
#line 4564 "y.tab.c"
    break;

  case 106: /* unsubscribe_event: TOTAL_STREAM_COUNT  */
#line 345 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_total_stream_count);   }
#line 4570 "y.tab.c"
    break;

  case 107: /* unsubscribe_event: TOTAL_VOICE_COUNT  */
#line 346 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_total_voice_count);    }
#line 4576 "y.tab.c"
    break;

  case 108: /* unsubscribe_event: GLOBAL_INFO  */
#line 347 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_global_info);          }
#line 4582 "y.tab.c"
    break;

  case 109: /* unsubscribe_event: EFFECT_INSTANCE_COUNT  */
#line 348 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_fx_instance_count);    }
#line 4588 "y.tab.c"
    break;

  case 110: /* unsubscribe_event: EFFECT_INSTANCE_INFO  */
#line 349 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_fx_instance_info);     }
#line 4594 "y.tab.c"
    break;

  case 111: /* unsubscribe_event: SEND_EFFECT_CHAIN_COUNT  */
#line 350 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_send_fx_chain_count);  }
#line 4600 "y.tab.c"
    break;

  case 112: /* unsubscribe_event: SEND_EFFECT_CHAIN_INFO  */
#line 351 "lscp.y"
                                                               { (yyval.String) = LSCPSERVER->UnsubscribeNotification(LSCPEvent::event_send_fx_chain_info);   }
#line 4606 "y.tab.c"
    break;

  case 113: /* map_instruction: MIDI_INSTRUMENT SP modal_arg midi_map SP midi_bank SP midi_prog SP engine_name SP filename SP instrument_index SP volume_value  */
#line 354 "lscp.y"
                                                                                                                                                        { (yyval.String) = LSCPSERVER->AddOrReplaceMIDIInstrumentMapping((yyvsp[-12].Number),(yyvsp[-10].Number),(yyvsp[-8].Number),(yyvsp[-6].String),(yyvsp[-4].String),(yyvsp[-2].Number),(yyvsp[0].Dotnum),MidiInstrumentMapper::DONTCARE,"",(yyvsp[-13].Bool)); }
#line 4612 "y.tab.c"
    break;

  case 114: /* map_instruction: MIDI_INSTRUMENT SP modal_arg midi_map SP midi_bank SP midi_prog SP engine_name SP filename SP instrument_index SP volume_value SP instr_load_mode  */
#line 355 "lscp.y"
                                                                                                                                                                           { (yyval.String) = LSCPSERVER->AddOrReplaceMIDIInstrumentMapping((yyvsp[-14].Number),(yyvsp[-12].Number),(yyvsp[-10].Number),(yyvsp[-8].String),(yyvsp[-6].String),(yyvsp[-4].Number),(yyvsp[-2].Dotnum),(yyvsp[0].LoadMode),"",(yyvsp[-15].Bool)); }
#line 4618 "y.tab.c"
    break;

  case 115: /* map_instruction: MIDI_INSTRUMENT SP modal_arg midi_map SP midi_bank SP midi_prog SP engine_name SP filename SP instrument_index SP volume_value SP entry_name  */
#line 356 "lscp.y"
                                                                                                                                                                      { (yyval.String) = LSCPSERVER->AddOrReplaceMIDIInstrumentMapping((yyvsp[-14].Number),(yyvsp[-12].Number),(yyvsp[-10].Number),(yyvsp[-8].String),(yyvsp[-6].String),(yyvsp[-4].Number),(yyvsp[-2].Dotnum),MidiInstrumentMapper::DONTCARE,(yyvsp[0].String),(yyvsp[-15].Bool)); }
#line 4624 "y.tab.c"
    break;

  case 116: /* map_instruction: MIDI_INSTRUMENT SP modal_arg midi_map SP midi_bank SP midi_prog SP engine_name SP filename SP instrument_index SP volume_value SP instr_load_mode SP entry_name  */
#line 357 "lscp.y"
                                                                                                                                                                                         { (yyval.String) = LSCPSERVER->AddOrReplaceMIDIInstrumentMapping((yyvsp[-16].Number),(yyvsp[-14].Number),(yyvsp[-12].Number),(yyvsp[-10].String),(yyvsp[-8].String),(yyvsp[-6].Number),(yyvsp[-4].Dotnum),(yyvsp[-2].LoadMode),(yyvsp[0].String),(yyvsp[-17].Bool)); }
#line 4630 "y.tab.c"
    break;

  case 117: /* unmap_instruction: MIDI_INSTRUMENT SP midi_map SP midi_bank SP midi_prog  */
#line 360 "lscp.y"
                                                                                { (yyval.String) = LSCPSERVER->RemoveMIDIInstrumentMapping((yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].Number)); }
#line 4636 "y.tab.c"
    break;

  case 118: /* remove_instruction: CHANNEL SP sampler_channel  */
#line 363 "lscp.y"
                                                                      { (yyval.String) = LSCPSERVER->RemoveChannel((yyvsp[0].Number));                      }
#line 4642 "y.tab.c"
    break;

  case 119: /* remove_instruction: CHANNEL SP MIDI_INPUT SP sampler_channel  */
#line 364 "lscp.y"
                                                                                                            { (yyval.String) = LSCPSERVER->RemoveChannelMidiInput((yyvsp[0].Number));       }
#line 4648 "y.tab.c"
    break;

  case 120: /* remove_instruction: CHANNEL SP MIDI_INPUT SP sampler_channel SP device_index  */
#line 365 "lscp.y"
                                                                                                            { (yyval.String) = LSCPSERVER->RemoveChannelMidiInput((yyvsp[-2].Number),(yyvsp[0].Number));    }
#line 4654 "y.tab.c"
    break;

  case 121: /* remove_instruction: CHANNEL SP MIDI_INPUT SP sampler_channel SP device_index SP midi_input_port_index  */
#line 366 "lscp.y"
                                                                                                            { (yyval.String) = LSCPSERVER->RemoveChannelMidiInput((yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].Number)); }
#line 4660 "y.tab.c"
    break;

  case 122: /* remove_instruction: MIDI_INSTRUMENT_MAP SP midi_map  */
#line 367 "lscp.y"
                                                                      { (yyval.String) = LSCPSERVER->RemoveMidiInstrumentMap((yyvsp[0].Number));            }
#line 4666 "y.tab.c"
    break;

  case 123: /* remove_instruction: MIDI_INSTRUMENT_MAP SP ALL  */
#line 368 "lscp.y"
                                                                      { (yyval.String) = LSCPSERVER->RemoveAllMidiInstrumentMaps();          }
#line 4672 "y.tab.c"
    break;

  case 124: /* remove_instruction: SEND_EFFECT_CHAIN SP device_index SP effect_chain  */
#line 369 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->RemoveSendEffectChain((yyvsp[-2].Number),(yyvsp[0].Number));     }
#line 4678 "y.tab.c"
    break;

  case 125: /* remove_instruction: SEND_EFFECT_CHAIN SP EFFECT SP device_index SP effect_chain SP chain_pos  */
#line 370 "lscp.y"
                                                                                                   { (yyval.String) = LSCPSERVER->RemoveSendEffectChainEffect((yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].Number)); }
#line 4684 "y.tab.c"
    break;

  case 126: /* remove_instruction: FX_SEND SP EFFECT SP sampler_channel SP fx_send_id  */
#line 371 "lscp.y"
                                                                             { (yyval.String) = LSCPSERVER->SetFxSendEffect((yyvsp[-2].Number),(yyvsp[0].Number),-1,-1);    }
#line 4690 "y.tab.c"
    break;

  case 127: /* remove_instruction: DB_INSTRUMENT_DIRECTORY SP FORCE SP db_path  */
#line 372 "lscp.y"
                                                                      { (yyval.String) = LSCPSERVER->RemoveDbInstrumentDirectory((yyvsp[0].String), true);  }
#line 4696 "y.tab.c"
    break;

  case 128: /* remove_instruction: DB_INSTRUMENT_DIRECTORY SP db_path  */
#line 373 "lscp.y"
                                                                      { (yyval.String) = LSCPSERVER->RemoveDbInstrumentDirectory((yyvsp[0].String));        }
#line 4702 "y.tab.c"
    break;

  case 129: /* remove_instruction: DB_INSTRUMENT SP db_path  */
#line 374 "lscp.y"
                                                                      { (yyval.String) = LSCPSERVER->RemoveDbInstrument((yyvsp[0].String));                 }
#line 4708 "y.tab.c"
    break;

  case 130: /* get_instruction: AVAILABLE_ENGINES  */
#line 377 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAvailableEngines();                          }
#line 4714 "y.tab.c"
    break;

  case 131: /* get_instruction: AVAILABLE_EFFECTS  */
#line 378 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAvailableEffects();                          }
#line 4720 "y.tab.c"
    break;

  case 132: /* get_instruction: EFFECT_INSTANCES  */
#line 379 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetEffectInstances();                           }
#line 4726 "y.tab.c"
    break;

  case 133: /* get_instruction: EFFECT SP INFO SP effect_index  */
#line 380 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetEffectInfo((yyvsp[0].Number));                              }
#line 4732 "y.tab.c"
    break;

  case 134: /* get_instruction: EFFECT_INSTANCE SP INFO SP effect_instance  */
#line 381 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetEffectInstanceInfo((yyvsp[0].Number));                      }
#line 4738 "y.tab.c"
    break;

  case 135: /* get_instruction: EFFECT_INSTANCE_INPUT_CONTROL SP INFO SP effect_instance SP input_control  */
#line 382 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetEffectInstanceInputControlInfo((yyvsp[-2].Number),(yyvsp[0].Number));       }
#line 4744 "y.tab.c"
    break;

  case 136: /* get_instruction: SEND_EFFECT_CHAINS SP device_index  */
#line 383 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetSendEffectChains((yyvsp[0].Number));                        }
#line 4750 "y.tab.c"
    break;

  case 137: /* get_instruction: SEND_EFFECT_CHAIN SP INFO SP device_index SP effect_chain  */
#line 384 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetSendEffectChainInfo((yyvsp[-2].Number),(yyvsp[0].Number));                  }
#line 4756 "y.tab.c"
    break;

  case 138: /* get_instruction: AVAILABLE_MIDI_INPUT_DRIVERS  */
#line 385 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAvailableMidiInputDrivers();                 }
#line 4762 "y.tab.c"
    break;

  case 139: /* get_instruction: MIDI_INPUT_DRIVER SP INFO SP string  */
#line 386 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetMidiInputDriverInfo((yyvsp[0].String));                     }
#line 4768 "y.tab.c"
    break;

  case 140: /* get_instruction: MIDI_INPUT_DRIVER_PARAMETER SP INFO SP string SP string  */
#line 387 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetMidiInputDriverParameterInfo((yyvsp[-2].String), (yyvsp[0].String));        }
#line 4774 "y.tab.c"
    break;

  case 141: /* get_instruction: MIDI_INPUT_DRIVER_PARAMETER SP INFO SP string SP string SP key_val_list  */
#line 388 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetMidiInputDriverParameterInfo((yyvsp[-4].String), (yyvsp[-2].String), (yyvsp[0].KeyValList));    }
#line 4780 "y.tab.c"
    break;

  case 142: /* get_instruction: AVAILABLE_AUDIO_OUTPUT_DRIVERS  */
#line 389 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAvailableAudioOutputDrivers();               }
#line 4786 "y.tab.c"
    break;

  case 143: /* get_instruction: AUDIO_OUTPUT_DRIVER SP INFO SP string  */
#line 390 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAudioOutputDriverInfo((yyvsp[0].String));                   }
#line 4792 "y.tab.c"
    break;

  case 144: /* get_instruction: AUDIO_OUTPUT_DRIVER_PARAMETER SP INFO SP string SP string  */
#line 391 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAudioOutputDriverParameterInfo((yyvsp[-2].String), (yyvsp[0].String));      }
#line 4798 "y.tab.c"
    break;

  case 145: /* get_instruction: AUDIO_OUTPUT_DRIVER_PARAMETER SP INFO SP string SP string SP key_val_list  */
#line 392 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAudioOutputDriverParameterInfo((yyvsp[-4].String), (yyvsp[-2].String), (yyvsp[0].KeyValList));  }
#line 4804 "y.tab.c"
    break;

  case 146: /* get_instruction: AUDIO_OUTPUT_DEVICES  */
#line 393 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAudioOutputDeviceCount();                    }
#line 4810 "y.tab.c"
    break;

  case 147: /* get_instruction: MIDI_INPUT_DEVICES  */
#line 394 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetMidiInputDeviceCount();                      }
#line 4816 "y.tab.c"
    break;

  case 148: /* get_instruction: AUDIO_OUTPUT_DEVICE SP INFO SP number  */
#line 395 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAudioOutputDeviceInfo((yyvsp[0].Number));                   }
#line 4822 "y.tab.c"
    break;

  case 149: /* get_instruction: MIDI_INPUT_DEVICE SP INFO SP number  */
#line 396 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetMidiInputDeviceInfo((yyvsp[0].Number));                     }
#line 4828 "y.tab.c"
    break;

  case 150: /* get_instruction: MIDI_INPUT_PORT SP INFO SP number SP number  */
#line 397 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetMidiInputPortInfo((yyvsp[-2].Number), (yyvsp[0].Number));                   }
#line 4834 "y.tab.c"
    break;

  case 151: /* get_instruction: MIDI_INPUT_PORT_PARAMETER SP INFO SP number SP number SP string  */
#line 398 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetMidiInputPortParameterInfo((yyvsp[-4].Number), (yyvsp[-2].Number), (yyvsp[0].String));      }
#line 4840 "y.tab.c"
    break;

  case 152: /* get_instruction: AUDIO_OUTPUT_CHANNEL SP INFO SP number SP number  */
#line 399 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAudioOutputChannelInfo((yyvsp[-2].Number), (yyvsp[0].Number));              }
#line 4846 "y.tab.c"
    break;

  case 153: /* get_instruction: AUDIO_OUTPUT_CHANNEL_PARAMETER SP INFO SP number SP number SP string  */
#line 400 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAudioOutputChannelParameterInfo((yyvsp[-4].Number), (yyvsp[-2].Number), (yyvsp[0].String)); }
#line 4852 "y.tab.c"
    break;

  case 154: /* get_instruction: CHANNELS  */
#line 401 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetChannels();                                  }
#line 4858 "y.tab.c"
    break;

  case 155: /* get_instruction: CHANNEL SP INFO SP sampler_channel  */
#line 402 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetChannelInfo((yyvsp[0].Number));                             }
#line 4864 "y.tab.c"
    break;

  case 156: /* get_instruction: CHANNEL SP BUFFER_FILL SP buffer_size_type SP sampler_channel  */
#line 403 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetBufferFill((yyvsp[-2].FillResponse), (yyvsp[0].Number));                          }
#line 4870 "y.tab.c"
    break;

  case 157: /* get_instruction: CHANNEL SP STREAM_COUNT SP sampler_channel  */
#line 404 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetStreamCount((yyvsp[0].Number));                             }
#line 4876 "y.tab.c"
    break;

  case 158: /* get_instruction: CHANNEL SP VOICE_COUNT SP sampler_channel  */
#line 405 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetVoiceCount((yyvsp[0].Number));                              }
#line 4882 "y.tab.c"
    break;

  case 159: /* get_instruction: ENGINE SP INFO SP engine_name  */
#line 406 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetEngineInfo((yyvsp[0].String));                              }
#line 4888 "y.tab.c"
    break;

  case 160: /* get_instruction: SERVER SP INFO  */
#line 407 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetServerInfo();                                }
#line 4894 "y.tab.c"
    break;

  case 161: /* get_instruction: TOTAL_STREAM_COUNT  */
#line 408 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetTotalStreamCount();                           }
#line 4900 "y.tab.c"
    break;

  case 162: /* get_instruction: TOTAL_VOICE_COUNT  */
#line 409 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetTotalVoiceCount();                           }
#line 4906 "y.tab.c"
    break;

  case 163: /* get_instruction: TOTAL_VOICE_COUNT_MAX  */
#line 410 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetTotalVoiceCountMax();                        }
#line 4912 "y.tab.c"
    break;

  case 164: /* get_instruction: MIDI_INSTRUMENTS SP midi_map  */
#line 411 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetMidiInstrumentMappings((yyvsp[0].Number));                  }
#line 4918 "y.tab.c"
    break;

  case 165: /* get_instruction: MIDI_INSTRUMENTS SP ALL  */
#line 412 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetAllMidiInstrumentMappings();                 }
#line 4924 "y.tab.c"
    break;

  case 166: /* get_instruction: MIDI_INSTRUMENT SP INFO SP midi_map SP midi_bank SP midi_prog  */
#line 413 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetMidiInstrumentMapping((yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].Number));             }
#line 4930 "y.tab.c"
    break;

  case 167: /* get_instruction: MIDI_INSTRUMENT_MAPS  */
#line 414 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetMidiInstrumentMaps();                        }
#line 4936 "y.tab.c"
    break;

  case 168: /* get_instruction: MIDI_INSTRUMENT_MAP SP INFO SP midi_map  */
#line 415 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetMidiInstrumentMap((yyvsp[0].Number));                       }
#line 4942 "y.tab.c"
    break;

  case 169: /* get_instruction: FX_SENDS SP sampler_channel  */
#line 416 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetFxSends((yyvsp[0].Number));                                 }
#line 4948 "y.tab.c"
    break;

  case 170: /* get_instruction: FX_SEND SP INFO SP sampler_channel SP fx_send_id  */
#line 417 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetFxSendInfo((yyvsp[-2].Number),(yyvsp[0].Number));                           }
#line 4954 "y.tab.c"
    break;

  case 171: /* get_instruction: DB_INSTRUMENT_DIRECTORIES SP RECURSIVE SP db_path  */
#line 418 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetDbInstrumentDirectoryCount((yyvsp[0].String), true);        }
#line 4960 "y.tab.c"
    break;

  case 172: /* get_instruction: DB_INSTRUMENT_DIRECTORIES SP db_path  */
#line 419 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetDbInstrumentDirectoryCount((yyvsp[0].String), false);       }
#line 4966 "y.tab.c"
    break;

  case 173: /* get_instruction: DB_INSTRUMENT_DIRECTORY SP INFO SP db_path  */
#line 420 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetDbInstrumentDirectoryInfo((yyvsp[0].String));               }
#line 4972 "y.tab.c"
    break;

  case 174: /* get_instruction: DB_INSTRUMENTS SP RECURSIVE SP db_path  */
#line 421 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetDbInstrumentCount((yyvsp[0].String), true);                 }
#line 4978 "y.tab.c"
    break;

  case 175: /* get_instruction: DB_INSTRUMENTS SP db_path  */
#line 422 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetDbInstrumentCount((yyvsp[0].String), false);                }
#line 4984 "y.tab.c"
    break;

  case 176: /* get_instruction: DB_INSTRUMENT SP INFO SP db_path  */
#line 423 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetDbInstrumentInfo((yyvsp[0].String));                        }
#line 4990 "y.tab.c"
    break;

  case 177: /* get_instruction: DB_INSTRUMENTS_JOB SP INFO SP number  */
#line 424 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetDbInstrumentsJobInfo((yyvsp[0].Number));                    }
#line 4996 "y.tab.c"
    break;

  case 178: /* get_instruction: VOLUME  */
#line 425 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetGlobalVolume();                              }
#line 5002 "y.tab.c"
    break;

  case 179: /* get_instruction: VOICES  */
#line 426 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetGlobalMaxVoices();                           }
#line 5008 "y.tab.c"
    break;

  case 180: /* get_instruction: STREAMS  */
#line 427 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetGlobalMaxStreams();                          }
#line 5014 "y.tab.c"
    break;

  case 181: /* get_instruction: FILE SP INSTRUMENTS SP filename  */
#line 428 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetFileInstruments((yyvsp[0].String));                         }
#line 5020 "y.tab.c"
    break;

  case 182: /* get_instruction: FILE SP INSTRUMENT SP INFO SP filename SP instrument_index  */
#line 429 "lscp.y"
                                                                                                    { (yyval.String) = LSCPSERVER->GetFileInstrumentInfo((yyvsp[-2].String),(yyvsp[0].Number));                   }
#line 5026 "y.tab.c"
    break;

  case 183: /* set_instruction: AUDIO_OUTPUT_DEVICE_PARAMETER SP number SP string '=' param_val_list  */
#line 432 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetAudioOutputDeviceParameter((yyvsp[-4].Number), (yyvsp[-2].String), (yyvsp[0].String));      }
#line 5032 "y.tab.c"
    break;

  case 184: /* set_instruction: AUDIO_OUTPUT_CHANNEL_PARAMETER SP number SP number SP string '=' param_val_list  */
#line 433 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetAudioOutputChannelParameter((yyvsp[-6].Number), (yyvsp[-4].Number), (yyvsp[-2].String), (yyvsp[0].String)); }
#line 5038 "y.tab.c"
    break;

  case 185: /* set_instruction: MIDI_INPUT_DEVICE_PARAMETER SP number SP string '=' param_val_list  */
#line 434 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetMidiInputDeviceParameter((yyvsp[-4].Number), (yyvsp[-2].String), (yyvsp[0].String));        }
#line 5044 "y.tab.c"
    break;

  case 186: /* set_instruction: MIDI_INPUT_PORT_PARAMETER SP number SP number SP string '=' NONE  */
#line 435 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetMidiInputPortParameter((yyvsp[-6].Number), (yyvsp[-4].Number), (yyvsp[-2].String), "");      }
#line 5050 "y.tab.c"
    break;

  case 187: /* set_instruction: MIDI_INPUT_PORT_PARAMETER SP number SP number SP string '=' param_val_list  */
#line 436 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetMidiInputPortParameter((yyvsp[-6].Number), (yyvsp[-4].Number), (yyvsp[-2].String), (yyvsp[0].String));      }
#line 5056 "y.tab.c"
    break;

  case 188: /* set_instruction: EFFECT_INSTANCE_INPUT_CONTROL SP VALUE SP effect_instance SP input_control SP control_value  */
#line 437 "lscp.y"
                                                                                                                      { (yyval.String) = LSCPSERVER->SetEffectInstanceInputControlValue((yyvsp[-4].Number), (yyvsp[-2].Number), (yyvsp[0].Dotnum)); }
#line 5062 "y.tab.c"
    break;

  case 189: /* set_instruction: CHANNEL SP set_chan_instruction  */
#line 438 "lscp.y"
                                                                                                          { (yyval.String) = (yyvsp[0].String);                                                         }
#line 5068 "y.tab.c"
    break;

  case 190: /* set_instruction: MIDI_INSTRUMENT_MAP SP NAME SP midi_map SP map_name  */
#line 439 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetMidiInstrumentMapName((yyvsp[-2].Number), (yyvsp[0].String));               }
#line 5074 "y.tab.c"
    break;

  case 191: /* set_instruction: FX_SEND SP NAME SP sampler_channel SP fx_send_id SP fx_send_name  */
#line 440 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetFxSendName((yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].String));                        }
#line 5080 "y.tab.c"
    break;

  case 192: /* set_instruction: FX_SEND SP AUDIO_OUTPUT_CHANNEL SP sampler_channel SP fx_send_id SP audio_channel_index SP audio_channel_index  */
#line 441 "lscp.y"
                                                                                                                                         { (yyval.String) = LSCPSERVER->SetFxSendAudioOutputChannel((yyvsp[-6].Number),(yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].Number)); }
#line 5086 "y.tab.c"
    break;

  case 193: /* set_instruction: FX_SEND SP MIDI_CONTROLLER SP sampler_channel SP fx_send_id SP midi_ctrl  */
#line 442 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetFxSendMidiController((yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].Number));              }
#line 5092 "y.tab.c"
    break;

  case 194: /* set_instruction: FX_SEND SP LEVEL SP sampler_channel SP fx_send_id SP volume_value  */
#line 443 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetFxSendLevel((yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].Dotnum));                       }
#line 5098 "y.tab.c"
    break;

  case 195: /* set_instruction: FX_SEND SP EFFECT SP sampler_channel SP fx_send_id SP effect_chain SP chain_pos  */
#line 444 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetFxSendEffect((yyvsp[-6].Number),(yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].Number));                  }
#line 5104 "y.tab.c"
    break;

  case 196: /* set_instruction: DB_INSTRUMENT_DIRECTORY SP NAME SP db_path SP stringval_escaped  */
#line 445 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetDbInstrumentDirectoryName((yyvsp[-2].String),(yyvsp[0].String));            }
#line 5110 "y.tab.c"
    break;

  case 197: /* set_instruction: DB_INSTRUMENT_DIRECTORY SP DESCRIPTION SP db_path SP stringval_escaped  */
#line 446 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetDbInstrumentDirectoryDescription((yyvsp[-2].String),(yyvsp[0].String));     }
#line 5116 "y.tab.c"
    break;

  case 198: /* set_instruction: DB_INSTRUMENT SP NAME SP db_path SP stringval_escaped  */
#line 447 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetDbInstrumentName((yyvsp[-2].String),(yyvsp[0].String));                     }
#line 5122 "y.tab.c"
    break;

  case 199: /* set_instruction: DB_INSTRUMENT SP DESCRIPTION SP db_path SP stringval_escaped  */
#line 448 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetDbInstrumentDescription((yyvsp[-2].String),(yyvsp[0].String));              }
#line 5128 "y.tab.c"
    break;

  case 200: /* set_instruction: DB_INSTRUMENT SP FILE_PATH SP filename SP filename  */
#line 449 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetDbInstrumentFilePath((yyvsp[-2].String),(yyvsp[0].String));                 }
#line 5134 "y.tab.c"
    break;

  case 201: /* set_instruction: ECHO SP boolean  */
#line 450 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetEcho((yyparse_param_t*) yyparse_param, (yyvsp[0].Dotnum));  }
#line 5140 "y.tab.c"
    break;

  case 202: /* set_instruction: SHELL SP INTERACT SP boolean  */
#line 451 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetShellInteract((yyparse_param_t*) yyparse_param, (yyvsp[0].Dotnum)); }
#line 5146 "y.tab.c"
    break;

  case 203: /* set_instruction: SHELL SP AUTO_CORRECT SP boolean  */
#line 452 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetShellAutoCorrect((yyparse_param_t*) yyparse_param, (yyvsp[0].Dotnum)); }
#line 5152 "y.tab.c"
    break;

  case 204: /* set_instruction: SHELL SP DOC SP boolean  */
#line 453 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetShellDoc((yyparse_param_t*) yyparse_param, (yyvsp[0].Dotnum)); }
#line 5158 "y.tab.c"
    break;

  case 205: /* set_instruction: VOLUME SP volume_value  */
#line 454 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetGlobalVolume((yyvsp[0].Dotnum));                            }
#line 5164 "y.tab.c"
    break;

  case 206: /* set_instruction: VOICES SP number  */
#line 455 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetGlobalMaxVoices((yyvsp[0].Number));                         }
#line 5170 "y.tab.c"
    break;

  case 207: /* set_instruction: STREAMS SP number  */
#line 456 "lscp.y"
                                                                                                          { (yyval.String) = LSCPSERVER->SetGlobalMaxStreams((yyvsp[0].Number));                        }
#line 5176 "y.tab.c"
    break;

  case 208: /* create_instruction: AUDIO_OUTPUT_DEVICE SP string SP key_val_list  */
#line 459 "lscp.y"
                                                                        { (yyval.String) = LSCPSERVER->CreateAudioOutputDevice((yyvsp[-2].String),(yyvsp[0].KeyValList)); }
#line 5182 "y.tab.c"
    break;

  case 209: /* create_instruction: AUDIO_OUTPUT_DEVICE SP string  */
#line 460 "lscp.y"
                                                                        { (yyval.String) = LSCPSERVER->CreateAudioOutputDevice((yyvsp[0].String));    }
#line 5188 "y.tab.c"
    break;

  case 210: /* create_instruction: MIDI_INPUT_DEVICE SP string SP key_val_list  */
#line 461 "lscp.y"
                                                                        { (yyval.String) = LSCPSERVER->CreateMidiInputDevice((yyvsp[-2].String),(yyvsp[0].KeyValList));   }
#line 5194 "y.tab.c"
    break;

  case 211: /* create_instruction: MIDI_INPUT_DEVICE SP string  */
#line 462 "lscp.y"
                                                                        { (yyval.String) = LSCPSERVER->CreateMidiInputDevice((yyvsp[0].String));      }
#line 5200 "y.tab.c"
    break;

  case 212: /* create_instruction: FX_SEND SP sampler_channel SP midi_ctrl  */
#line 463 "lscp.y"
                                                                        { (yyval.String) = LSCPSERVER->CreateFxSend((yyvsp[-2].Number),(yyvsp[0].Number));            }
#line 5206 "y.tab.c"
    break;

  case 213: /* create_instruction: FX_SEND SP sampler_channel SP midi_ctrl SP fx_send_name  */
#line 464 "lscp.y"
                                                                                  { (yyval.String) = LSCPSERVER->CreateFxSend((yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].String)); }
#line 5212 "y.tab.c"
    break;

  case 214: /* create_instruction: EFFECT_INSTANCE SP effect_index  */
#line 465 "lscp.y"
                                                                        { (yyval.String) = LSCPSERVER->CreateEffectInstance((yyvsp[0].Number));       }
#line 5218 "y.tab.c"
    break;

  case 215: /* create_instruction: EFFECT_INSTANCE SP effect_system SP module SP effect_name  */
#line 466 "lscp.y"
                                                                                    { (yyval.String) = LSCPSERVER->CreateEffectInstance((yyvsp[-4].String),(yyvsp[-2].String),(yyvsp[0].String)); }
#line 5224 "y.tab.c"
    break;

  case 216: /* reset_instruction: CHANNEL SP sampler_channel  */
#line 469 "lscp.y"
                                                     { (yyval.String) = LSCPSERVER->ResetChannel((yyvsp[0].Number)); }
#line 5230 "y.tab.c"
    break;

  case 217: /* clear_instruction: MIDI_INSTRUMENTS SP midi_map  */
#line 472 "lscp.y"
                                                        { (yyval.String) = LSCPSERVER->ClearMidiInstrumentMappings((yyvsp[0].Number));  }
#line 5236 "y.tab.c"
    break;

  case 218: /* clear_instruction: MIDI_INSTRUMENTS SP ALL  */
#line 473 "lscp.y"
                                                        { (yyval.String) = LSCPSERVER->ClearAllMidiInstrumentMappings(); }
#line 5242 "y.tab.c"
    break;

  case 219: /* find_instruction: DB_INSTRUMENTS SP NON_RECURSIVE SP db_path SP query_val_list  */
#line 476 "lscp.y"
                                                                                                   { (yyval.String) = LSCPSERVER->FindDbInstruments((yyvsp[-2].String),(yyvsp[0].KeyValList), false);           }
#line 5248 "y.tab.c"
    break;

  case 220: /* find_instruction: DB_INSTRUMENTS SP db_path SP query_val_list  */
#line 477 "lscp.y"
                                                                                                   { (yyval.String) = LSCPSERVER->FindDbInstruments((yyvsp[-2].String),(yyvsp[0].KeyValList), true);            }
#line 5254 "y.tab.c"
    break;

  case 221: /* find_instruction: DB_INSTRUMENT_DIRECTORIES SP NON_RECURSIVE SP db_path SP query_val_list  */
#line 478 "lscp.y"
                                                                                                   { (yyval.String) = LSCPSERVER->FindDbInstrumentDirectories((yyvsp[-2].String),(yyvsp[0].KeyValList), false); }
#line 5260 "y.tab.c"
    break;

  case 222: /* find_instruction: DB_INSTRUMENT_DIRECTORIES SP db_path SP query_val_list  */
#line 479 "lscp.y"
                                                                                                   { (yyval.String) = LSCPSERVER->FindDbInstrumentDirectories((yyvsp[-2].String),(yyvsp[0].KeyValList), true);  }
#line 5266 "y.tab.c"
    break;

  case 223: /* find_instruction: LOST SP DB_INSTRUMENT_FILES  */
#line 480 "lscp.y"
                                                                                                   { (yyval.String) = LSCPSERVER->FindLostDbInstrumentFiles();                 }
#line 5272 "y.tab.c"
    break;

  case 224: /* move_instruction: DB_INSTRUMENT_DIRECTORY SP db_path SP db_path  */
#line 483 "lscp.y"
                                                                          { (yyval.String) = LSCPSERVER->MoveDbInstrumentDirectory((yyvsp[-2].String),(yyvsp[0].String)); }
#line 5278 "y.tab.c"
    break;

  case 225: /* move_instruction: DB_INSTRUMENT SP db_path SP db_path  */
#line 484 "lscp.y"
                                                                          { (yyval.String) = LSCPSERVER->MoveDbInstrument((yyvsp[-2].String),(yyvsp[0].String));          }
#line 5284 "y.tab.c"
    break;

  case 226: /* copy_instruction: DB_INSTRUMENT_DIRECTORY SP db_path SP db_path  */
#line 487 "lscp.y"
                                                                          { (yyval.String) = LSCPSERVER->CopyDbInstrumentDirectory((yyvsp[-2].String),(yyvsp[0].String)); }
#line 5290 "y.tab.c"
    break;

  case 227: /* copy_instruction: DB_INSTRUMENT SP db_path SP db_path  */
#line 488 "lscp.y"
                                                                          { (yyval.String) = LSCPSERVER->CopyDbInstrument((yyvsp[-2].String),(yyvsp[0].String));          }
#line 5296 "y.tab.c"
    break;

  case 228: /* destroy_instruction: AUDIO_OUTPUT_DEVICE SP number  */
#line 491 "lscp.y"
                                                        { (yyval.String) = LSCPSERVER->DestroyAudioOutputDevice((yyvsp[0].Number)); }
#line 5302 "y.tab.c"
    break;

  case 229: /* destroy_instruction: MIDI_INPUT_DEVICE SP number  */
#line 492 "lscp.y"
                                                        { (yyval.String) = LSCPSERVER->DestroyMidiInputDevice((yyvsp[0].Number));   }
#line 5308 "y.tab.c"
    break;

  case 230: /* destroy_instruction: FX_SEND SP sampler_channel SP fx_send_id  */
#line 493 "lscp.y"
                                                                   { (yyval.String) = LSCPSERVER->DestroyFxSend((yyvsp[-2].Number),(yyvsp[0].Number)); }
#line 5314 "y.tab.c"
    break;

  case 231: /* destroy_instruction: EFFECT_INSTANCE SP number  */
#line 494 "lscp.y"
                                                        { (yyval.String) = LSCPSERVER->DestroyEffectInstance((yyvsp[0].Number));    }
#line 5320 "y.tab.c"
    break;

  case 232: /* load_instruction: INSTRUMENT SP load_instr_args  */
#line 497 "lscp.y"
                                                        { (yyval.String) = (yyvsp[0].String); }
#line 5326 "y.tab.c"
    break;

  case 233: /* load_instruction: ENGINE SP load_engine_args  */
#line 498 "lscp.y"
                                                        { (yyval.String) = (yyvsp[0].String); }
#line 5332 "y.tab.c"
    break;

  case 234: /* append_instruction: SEND_EFFECT_CHAIN SP EFFECT SP device_index SP effect_chain SP effect_instance  */
#line 501 "lscp.y"
                                                                                                         { (yyval.String) = LSCPSERVER->AppendSendEffectChainEffect((yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].Number)); }
#line 5338 "y.tab.c"
    break;

  case 235: /* insert_instruction: SEND_EFFECT_CHAIN SP EFFECT SP device_index SP effect_chain SP chain_pos SP effect_instance  */
#line 504 "lscp.y"
                                                                                                                      { (yyval.String) = LSCPSERVER->InsertSendEffectChainEffect((yyvsp[-6].Number),(yyvsp[-4].Number),(yyvsp[-2].Number),(yyvsp[0].Number)); }
#line 5344 "y.tab.c"
    break;

  case 236: /* set_chan_instruction: AUDIO_OUTPUT_DEVICE SP sampler_channel SP device_index  */
#line 507 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetAudioOutputDevice((yyvsp[0].Number), (yyvsp[-2].Number));      }
#line 5350 "y.tab.c"
    break;

  case 237: /* set_chan_instruction: AUDIO_OUTPUT_CHANNEL SP sampler_channel SP audio_channel_index SP audio_channel_index  */
#line 508 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetAudioOutputChannel((yyvsp[-2].Number), (yyvsp[0].Number), (yyvsp[-4].Number)); }
#line 5356 "y.tab.c"
    break;

  case 238: /* set_chan_instruction: AUDIO_OUTPUT_TYPE SP sampler_channel SP audio_output_type_name  */
#line 509 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetAudioOutputType((yyvsp[0].String), (yyvsp[-2].Number));        }
#line 5362 "y.tab.c"
    break;

  case 239: /* set_chan_instruction: MIDI_INPUT SP sampler_channel SP device_index SP midi_input_port_index SP midi_input_channel_index  */
#line 510 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetMIDIInput((yyvsp[-4].Number), (yyvsp[-2].Number), (yyvsp[0].Number), (yyvsp[-6].Number));      }
#line 5368 "y.tab.c"
    break;

  case 240: /* set_chan_instruction: MIDI_INPUT_DEVICE SP sampler_channel SP device_index  */
#line 511 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetMIDIInputDevice((yyvsp[0].Number), (yyvsp[-2].Number));        }
#line 5374 "y.tab.c"
    break;

  case 241: /* set_chan_instruction: MIDI_INPUT_PORT SP sampler_channel SP midi_input_port_index  */
#line 512 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetMIDIInputPort((yyvsp[0].Number), (yyvsp[-2].Number));          }
#line 5380 "y.tab.c"
    break;

  case 242: /* set_chan_instruction: MIDI_INPUT_CHANNEL SP sampler_channel SP midi_input_channel_index  */
#line 513 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetMIDIInputChannel((yyvsp[0].Number), (yyvsp[-2].Number));       }
#line 5386 "y.tab.c"
    break;

  case 243: /* set_chan_instruction: MIDI_INPUT_TYPE SP sampler_channel SP midi_input_type_name  */
#line 514 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetMIDIInputType((yyvsp[0].String), (yyvsp[-2].Number));          }
#line 5392 "y.tab.c"
    break;

  case 244: /* set_chan_instruction: VOLUME SP sampler_channel SP volume_value  */
#line 515 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetVolume((yyvsp[0].Dotnum), (yyvsp[-2].Number));                 }
#line 5398 "y.tab.c"
    break;

  case 245: /* set_chan_instruction: MUTE SP sampler_channel SP boolean  */
#line 516 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetChannelMute((yyvsp[0].Dotnum), (yyvsp[-2].Number));            }
#line 5404 "y.tab.c"
    break;

  case 246: /* set_chan_instruction: SOLO SP sampler_channel SP boolean  */
#line 517 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetChannelSolo((yyvsp[0].Dotnum), (yyvsp[-2].Number));            }
#line 5410 "y.tab.c"
    break;

  case 247: /* set_chan_instruction: MIDI_INSTRUMENT_MAP SP sampler_channel SP midi_map  */
#line 518 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetChannelMap((yyvsp[-2].Number), (yyvsp[0].Number));             }
#line 5416 "y.tab.c"
    break;

  case 248: /* set_chan_instruction: MIDI_INSTRUMENT_MAP SP sampler_channel SP NONE  */
#line 519 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetChannelMap((yyvsp[-2].Number), -1);             }
#line 5422 "y.tab.c"
    break;

  case 249: /* set_chan_instruction: MIDI_INSTRUMENT_MAP SP sampler_channel SP DEFAULT  */
#line 520 "lscp.y"
                                                                                                                             { (yyval.String) = LSCPSERVER->SetChannelMap((yyvsp[-2].Number), -2);             }
#line 5428 "y.tab.c"
    break;

  case 250: /* edit_instruction: CHANNEL SP INSTRUMENT SP sampler_channel  */
#line 523 "lscp.y"
                                                                   { (yyval.String) = LSCPSERVER->EditSamplerChannelInstrument((yyvsp[0].Number)); }
#line 5434 "y.tab.c"
    break;

  case 251: /* format_instruction: INSTRUMENTS_DB  */
#line 526 "lscp.y"
                                         { (yyval.String) = LSCPSERVER->FormatInstrumentsDb(); }
#line 5440 "y.tab.c"
    break;

  case 252: /* modal_arg: %empty  */
#line 529 "lscp.y"
                                                         { (yyval.Bool) = true;  }
#line 5446 "y.tab.c"
    break;

  case 253: /* modal_arg: NON_MODAL SP  */
#line 530 "lscp.y"
                                                         { (yyval.Bool) = false; }
#line 5452 "y.tab.c"
    break;

  case 254: /* key_val_list: string '=' param_val_list  */
#line 533 "lscp.y"
                                                                    { (yyval.KeyValList)[(yyvsp[-2].String)] = (yyvsp[0].String);          }
#line 5458 "y.tab.c"
    break;

  case 255: /* key_val_list: key_val_list SP string '=' param_val_list  */
#line 534 "lscp.y"
                                                                    { (yyval.KeyValList) = (yyvsp[-4].KeyValList); (yyval.KeyValList)[(yyvsp[-2].String)] = (yyvsp[0].String); }
#line 5464 "y.tab.c"
    break;

  case 256: /* buffer_size_type: BYTES  */
#line 537 "lscp.y"
                                     { (yyval.FillResponse) = fill_response_bytes;      }
#line 5470 "y.tab.c"
    break;

  case 257: /* buffer_size_type: PERCENTAGE  */
#line 538 "lscp.y"
                                     { (yyval.FillResponse) = fill_response_percentage; }
#line 5476 "y.tab.c"
    break;

  case 258: /* list_instruction: AUDIO_OUTPUT_DEVICES  */
#line 541 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->GetAudioOutputDevices();              }
#line 5482 "y.tab.c"
    break;

  case 259: /* list_instruction: MIDI_INPUT_DEVICES  */
#line 542 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->GetMidiInputDevices();                }
#line 5488 "y.tab.c"
    break;

  case 260: /* list_instruction: CHANNELS  */
#line 543 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListChannels();                       }
#line 5494 "y.tab.c"
    break;

  case 261: /* list_instruction: CHANNEL SP MIDI_INPUTS SP sampler_channel  */
#line 544 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListChannelMidiInputs((yyvsp[0].Number));            }
#line 5500 "y.tab.c"
    break;

  case 262: /* list_instruction: AVAILABLE_ENGINES  */
#line 545 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListAvailableEngines();               }
#line 5506 "y.tab.c"
    break;

  case 263: /* list_instruction: AVAILABLE_EFFECTS  */
#line 546 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListAvailableEffects();               }
#line 5512 "y.tab.c"
    break;

  case 264: /* list_instruction: EFFECT_INSTANCES  */
#line 547 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListEffectInstances();                }
#line 5518 "y.tab.c"
    break;

  case 265: /* list_instruction: SEND_EFFECT_CHAINS SP number  */
#line 548 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListSendEffectChains((yyvsp[0].Number));             }
#line 5524 "y.tab.c"
    break;

  case 266: /* list_instruction: AVAILABLE_MIDI_INPUT_DRIVERS  */
#line 549 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListAvailableMidiInputDrivers();      }
#line 5530 "y.tab.c"
    break;

  case 267: /* list_instruction: AVAILABLE_AUDIO_OUTPUT_DRIVERS  */
#line 550 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListAvailableAudioOutputDrivers();    }
#line 5536 "y.tab.c"
    break;

  case 268: /* list_instruction: MIDI_INSTRUMENTS SP midi_map  */
#line 551 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListMidiInstrumentMappings((yyvsp[0].Number));       }
#line 5542 "y.tab.c"
    break;

  case 269: /* list_instruction: MIDI_INSTRUMENTS SP ALL  */
#line 552 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListAllMidiInstrumentMappings();      }
#line 5548 "y.tab.c"
    break;

  case 270: /* list_instruction: MIDI_INSTRUMENT_MAPS  */
#line 553 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListMidiInstrumentMaps();             }
#line 5554 "y.tab.c"
    break;

  case 271: /* list_instruction: FX_SENDS SP sampler_channel  */
#line 554 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListFxSends((yyvsp[0].Number));                      }
#line 5560 "y.tab.c"
    break;

  case 272: /* list_instruction: DB_INSTRUMENT_DIRECTORIES SP RECURSIVE SP db_path  */
#line 555 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->GetDbInstrumentDirectories((yyvsp[0].String), true); }
#line 5566 "y.tab.c"
    break;

  case 273: /* list_instruction: DB_INSTRUMENT_DIRECTORIES SP db_path  */
#line 556 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->GetDbInstrumentDirectories((yyvsp[0].String));       }
#line 5572 "y.tab.c"
    break;

  case 274: /* list_instruction: DB_INSTRUMENTS SP RECURSIVE SP db_path  */
#line 557 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->GetDbInstruments((yyvsp[0].String), true);           }
#line 5578 "y.tab.c"
    break;

  case 275: /* list_instruction: DB_INSTRUMENTS SP db_path  */
#line 558 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->GetDbInstruments((yyvsp[0].String));                 }
#line 5584 "y.tab.c"
    break;

  case 276: /* list_instruction: FILE SP INSTRUMENTS SP filename  */
#line 559 "lscp.y"
                                                                            { (yyval.String) = LSCPSERVER->ListFileInstruments((yyvsp[0].String));              }
#line 5590 "y.tab.c"
    break;

  case 277: /* send_instruction: CHANNEL SP MIDI_DATA SP string SP sampler_channel SP number SP number  */
#line 562 "lscp.y"
                                                                                                { (yyval.String) = LSCPSERVER->SendChannelMidiData((yyvsp[-6].String), (yyvsp[-4].Number), (yyvsp[-2].Number), (yyvsp[0].Number)); }
#line 5596 "y.tab.c"
    break;

  case 278: /* load_instr_args: filename SP instrument_index SP sampler_channel  */
#line 565 "lscp.y"
                                                                                       { (yyval.String) = LSCPSERVER->LoadInstrument((yyvsp[-4].String), (yyvsp[-2].Number), (yyvsp[0].Number));       }
#line 5602 "y.tab.c"
    break;

  case 279: /* load_instr_args: NON_MODAL SP filename SP instrument_index SP sampler_channel  */
#line 566 "lscp.y"
                                                                                       { (yyval.String) = LSCPSERVER->LoadInstrument((yyvsp[-4].String), (yyvsp[-2].Number), (yyvsp[0].Number), true); }
#line 5608 "y.tab.c"
    break;

  case 280: /* load_engine_args: engine_name SP sampler_channel  */
#line 569 "lscp.y"
                                                         { (yyval.String) = LSCPSERVER->SetEngineType((yyvsp[-2].String), (yyvsp[0].Number)); }
#line 5614 "y.tab.c"
    break;

  case 281: /* instr_load_mode: ON_DEMAND  */
#line 572 "lscp.y"
                                         { (yyval.LoadMode) = MidiInstrumentMapper::ON_DEMAND;      }
#line 5620 "y.tab.c"
    break;

  case 282: /* instr_load_mode: ON_DEMAND_HOLD  */
#line 573 "lscp.y"
                                         { (yyval.LoadMode) = MidiInstrumentMapper::ON_DEMAND_HOLD; }
#line 5626 "y.tab.c"
    break;

  case 283: /* instr_load_mode: PERSISTENT  */
#line 574 "lscp.y"
                                         { (yyval.LoadMode) = MidiInstrumentMapper::PERSISTENT;     }
#line 5632 "y.tab.c"
    break;

  case 290: /* midi_input_channel_index: ALL  */
#line 593 "lscp.y"
                                  { (yyval.Number) = 16; }
#line 5638 "y.tab.c"
    break;

  case 297: /* volume_value: number  */
#line 612 "lscp.y"
                                     { (yyval.Dotnum) = (yyvsp[0].Number); }
#line 5644 "y.tab.c"
    break;

  case 303: /* filename: path  */
#line 630 "lscp.y"
                                   {
                                 #if WIN32
                                 (yyval.String) = (yyvsp[0].UniversalPath).toWindows();
                                 #else
                                 // assuming POSIX
                                 (yyval.String) = (yyvsp[0].UniversalPath).toPosix();
                                 #endif
                             }
#line 5657 "y.tab.c"
    break;

  case 304: /* db_path: path  */
#line 640 "lscp.y"
                                   { (yyval.String) = (yyvsp[0].UniversalPath).toDbPath(); }
#line 5663 "y.tab.c"
    break;

  case 314: /* param_val_list: param_val_list ',' param_val  */
#line 668 "lscp.y"
                                                         { (yyval.String) = (yyvsp[-2].String) + "," + (yyvsp[0].String); }
#line 5669 "y.tab.c"
    break;

  case 315: /* param_val: string  */
#line 672 "lscp.y"
                                               { (yyval.String) = "\'" + (yyvsp[0].String) + "\'"; }
#line 5675 "y.tab.c"
    break;

  case 316: /* param_val: stringval  */
#line 673 "lscp.y"
                                               { (yyval.String) = "\'" + (yyvsp[0].String) + "\'"; }
#line 5681 "y.tab.c"
    break;

  case 317: /* param_val: number  */
#line 674 "lscp.y"
                                               { std::stringstream ss; ss << "\'" << (yyvsp[0].Number) << "\'"; (yyval.String) = ss.str(); }
#line 5687 "y.tab.c"
    break;

  case 318: /* param_val: dotnum  */
#line 675 "lscp.y"
                                               { std::stringstream ss; ss << "\'" << (yyvsp[0].Dotnum) << "\'"; (yyval.String) = ss.str(); }
#line 5693 "y.tab.c"
    break;

  case 319: /* query_val_list: string '=' query_val  */
#line 678 "lscp.y"
                                                                     { (yyval.KeyValList)[(yyvsp[-2].String)] = (yyvsp[0].String);          }
#line 5699 "y.tab.c"
    break;

  case 320: /* query_val_list: query_val_list SP string '=' query_val  */
#line 679 "lscp.y"
                                                                     { (yyval.KeyValList) = (yyvsp[-4].KeyValList); (yyval.KeyValList)[(yyvsp[-2].String)] = (yyvsp[0].String); }
#line 5705 "y.tab.c"
    break;

  case 323: /* scan_mode: RECURSIVE  */
#line 686 "lscp.y"
                                            { (yyval.String) = "RECURSIVE"; }
#line 5711 "y.tab.c"
    break;

  case 324: /* scan_mode: NON_RECURSIVE  */
#line 687 "lscp.y"
                                            { (yyval.String) = "NON_RECURSIVE"; }
#line 5717 "y.tab.c"
    break;

  case 325: /* scan_mode: FLAT  */
#line 688 "lscp.y"
                                            { (yyval.String) = "FLAT"; }
#line 5723 "y.tab.c"
    break;

  case 328: /* boolean: number  */
#line 702 "lscp.y"
                                 { (yyval.Dotnum) = (yyvsp[0].Number); }
#line 5729 "y.tab.c"
    break;

  case 329: /* boolean: string  */
#line 703 "lscp.y"
                                 { (yyval.Dotnum) = -1; }
#line 5735 "y.tab.c"
    break;

  case 330: /* dotnum: digits '.' digits  */
#line 706 "lscp.y"
                                                { std::stringstream ss((yyvsp[-2].String) + "." + (yyvsp[0].String)); ss.imbue(std::locale::classic()); ss >> (yyval.Dotnum); }
#line 5741 "y.tab.c"
    break;

  case 331: /* dotnum: '+' digits '.' digits  */
#line 707 "lscp.y"
                                                { std::stringstream ss((yyvsp[-2].String) + "." + (yyvsp[0].String)); ss.imbue(std::locale::classic()); ss >> (yyval.Dotnum); }
#line 5747 "y.tab.c"
    break;

  case 332: /* dotnum: '-' digits '.' digits  */
#line 708 "lscp.y"
                                                { std::stringstream ss("-" + (yyvsp[-2].String) + "." + (yyvsp[0].String)); ss.imbue(std::locale::classic()); ss >> (yyval.Dotnum); }
#line 5753 "y.tab.c"
    break;

  case 333: /* real: digits '.' digits  */
#line 711 "lscp.y"
                                                { std::stringstream ss((yyvsp[-2].String) + "." + (yyvsp[0].String)); ss.imbue(std::locale::classic()); ss >> (yyval.Dotnum); }
#line 5759 "y.tab.c"
    break;

  case 334: /* real: '+' digits '.' digits  */
#line 712 "lscp.y"
                                                { std::stringstream ss((yyvsp[-2].String) + "." + (yyvsp[0].String)); ss.imbue(std::locale::classic()); ss >> (yyval.Dotnum); }
#line 5765 "y.tab.c"
    break;

  case 335: /* real: '-' digits '.' digits  */
#line 713 "lscp.y"
                                                { std::stringstream ss("-" + (yyvsp[-2].String) + "." + (yyvsp[0].String)); ss.imbue(std::locale::classic()); ss >> (yyval.Dotnum); }
#line 5771 "y.tab.c"
    break;

  case 336: /* real: digits  */
#line 714 "lscp.y"
                                                { std::stringstream ss((yyvsp[0].String)); ss.imbue(std::locale::classic()); ss >> (yyval.Dotnum);                  }
#line 5777 "y.tab.c"
    break;

  case 337: /* real: '+' digits  */
#line 715 "lscp.y"
                                                { std::stringstream ss((yyvsp[0].String)); ss.imbue(std::locale::classic()); ss >> (yyval.Dotnum);                  }
#line 5783 "y.tab.c"
    break;

  case 338: /* real: '-' digits  */
#line 716 "lscp.y"
                                                { std::stringstream ss("-" + (yyvsp[0].String)); ss.imbue(std::locale::classic()); ss >> (yyval.Dotnum);            }
#line 5789 "y.tab.c"
    break;

  case 339: /* digits: digit  */
#line 720 "lscp.y"
                                       { (yyval.String) = (yyvsp[0].Char);      }
#line 5795 "y.tab.c"
    break;

  case 340: /* digits: digits digit  */
#line 721 "lscp.y"
                                       { (yyval.String) = (yyvsp[-1].String) + (yyvsp[0].Char); }
#line 5801 "y.tab.c"
    break;

  case 341: /* digit: '0'  */
#line 724 "lscp.y"
                              { (yyval.Char) = '0'; }
#line 5807 "y.tab.c"
    break;

  case 342: /* digit: '1'  */
#line 725 "lscp.y"
                              { (yyval.Char) = '1'; }
#line 5813 "y.tab.c"
    break;

  case 343: /* digit: '2'  */
#line 726 "lscp.y"
                              { (yyval.Char) = '2'; }
#line 5819 "y.tab.c"
    break;

  case 344: /* digit: '3'  */
#line 727 "lscp.y"
                              { (yyval.Char) = '3'; }
#line 5825 "y.tab.c"
    break;

  case 345: /* digit: '4'  */
#line 728 "lscp.y"
                              { (yyval.Char) = '4'; }
#line 5831 "y.tab.c"
    break;

  case 346: /* digit: '5'  */
#line 729 "lscp.y"
                              { (yyval.Char) = '5'; }
#line 5837 "y.tab.c"
    break;

  case 347: /* digit: '6'  */
#line 730 "lscp.y"
                              { (yyval.Char) = '6'; }
#line 5843 "y.tab.c"
    break;

  case 348: /* digit: '7'  */
#line 731 "lscp.y"
                              { (yyval.Char) = '7'; }
#line 5849 "y.tab.c"
    break;

  case 349: /* digit: '8'  */
#line 732 "lscp.y"
                              { (yyval.Char) = '8'; }
#line 5855 "y.tab.c"
    break;

  case 350: /* digit: '9'  */
#line 733 "lscp.y"
                              { (yyval.Char) = '9'; }
#line 5861 "y.tab.c"
    break;

  case 351: /* digit_oct: '0'  */
#line 736 "lscp.y"
                              { (yyval.Char) = '0'; }
#line 5867 "y.tab.c"
    break;

  case 352: /* digit_oct: '1'  */
#line 737 "lscp.y"
                              { (yyval.Char) = '1'; }
#line 5873 "y.tab.c"
    break;

  case 353: /* digit_oct: '2'  */
#line 738 "lscp.y"
                              { (yyval.Char) = '2'; }
#line 5879 "y.tab.c"
    break;

  case 354: /* digit_oct: '3'  */
#line 739 "lscp.y"
                              { (yyval.Char) = '3'; }
#line 5885 "y.tab.c"
    break;

  case 355: /* digit_oct: '4'  */
#line 740 "lscp.y"
                              { (yyval.Char) = '4'; }
#line 5891 "y.tab.c"
    break;

  case 356: /* digit_oct: '5'  */
#line 741 "lscp.y"
                              { (yyval.Char) = '5'; }
#line 5897 "y.tab.c"
    break;

  case 357: /* digit_oct: '6'  */
#line 742 "lscp.y"
                              { (yyval.Char) = '6'; }
#line 5903 "y.tab.c"
    break;

  case 358: /* digit_oct: '7'  */
#line 743 "lscp.y"
                              { (yyval.Char) = '7'; }
#line 5909 "y.tab.c"
    break;

  case 359: /* digit_hex: '0'  */
#line 746 "lscp.y"
                              { (yyval.Char) = '0'; }
#line 5915 "y.tab.c"
    break;

  case 360: /* digit_hex: '1'  */
#line 747 "lscp.y"
                              { (yyval.Char) = '1'; }
#line 5921 "y.tab.c"
    break;

  case 361: /* digit_hex: '2'  */
#line 748 "lscp.y"
                              { (yyval.Char) = '2'; }
#line 5927 "y.tab.c"
    break;

  case 362: /* digit_hex: '3'  */
#line 749 "lscp.y"
                              { (yyval.Char) = '3'; }
#line 5933 "y.tab.c"
    break;

  case 363: /* digit_hex: '4'  */
#line 750 "lscp.y"
                              { (yyval.Char) = '4'; }
#line 5939 "y.tab.c"
    break;

  case 364: /* digit_hex: '5'  */
#line 751 "lscp.y"
                              { (yyval.Char) = '5'; }
#line 5945 "y.tab.c"
    break;

  case 365: /* digit_hex: '6'  */
#line 752 "lscp.y"
                              { (yyval.Char) = '6'; }
#line 5951 "y.tab.c"
    break;

  case 366: /* digit_hex: '7'  */
#line 753 "lscp.y"
                              { (yyval.Char) = '7'; }
#line 5957 "y.tab.c"
    break;

  case 367: /* digit_hex: '8'  */
#line 754 "lscp.y"
                              { (yyval.Char) = '8'; }
#line 5963 "y.tab.c"
    break;

  case 368: /* digit_hex: '9'  */
#line 755 "lscp.y"
                              { (yyval.Char) = '9'; }
#line 5969 "y.tab.c"
    break;

  case 369: /* digit_hex: 'a'  */
#line 756 "lscp.y"
                              { (yyval.Char) = 'a'; }
#line 5975 "y.tab.c"
    break;

  case 370: /* digit_hex: 'b'  */
#line 757 "lscp.y"
                              { (yyval.Char) = 'b'; }
#line 5981 "y.tab.c"
    break;

  case 371: /* digit_hex: 'c'  */
#line 758 "lscp.y"
                              { (yyval.Char) = 'c'; }
#line 5987 "y.tab.c"
    break;

  case 372: /* digit_hex: 'd'  */
#line 759 "lscp.y"
                              { (yyval.Char) = 'd'; }
#line 5993 "y.tab.c"
    break;

  case 373: /* digit_hex: 'e'  */
#line 760 "lscp.y"
                              { (yyval.Char) = 'e'; }
#line 5999 "y.tab.c"
    break;

  case 374: /* digit_hex: 'f'  */
#line 761 "lscp.y"
                              { (yyval.Char) = 'f'; }
#line 6005 "y.tab.c"
    break;

  case 375: /* digit_hex: 'A'  */
#line 762 "lscp.y"
                              { (yyval.Char) = 'a'; }
#line 6011 "y.tab.c"
    break;

  case 376: /* digit_hex: 'B'  */
#line 763 "lscp.y"
                              { (yyval.Char) = 'b'; }
#line 6017 "y.tab.c"
    break;

  case 377: /* digit_hex: 'C'  */
#line 764 "lscp.y"
                              { (yyval.Char) = 'c'; }
#line 6023 "y.tab.c"
    break;

  case 378: /* digit_hex: 'D'  */
#line 765 "lscp.y"
                              { (yyval.Char) = 'd'; }
#line 6029 "y.tab.c"
    break;

  case 379: /* digit_hex: 'E'  */
#line 766 "lscp.y"
                              { (yyval.Char) = 'e'; }
#line 6035 "y.tab.c"
    break;

  case 380: /* digit_hex: 'F'  */
#line 767 "lscp.y"
                              { (yyval.Char) = 'f'; }
#line 6041 "y.tab.c"
    break;

  case 381: /* number: digit  */
#line 770 "lscp.y"
                                     { (yyval.Number) = atoi(String(1, (yyvsp[0].Char)).c_str());      }
#line 6047 "y.tab.c"
    break;

  case 382: /* number: '1' digits  */
#line 771 "lscp.y"
                                     { (yyval.Number) = atoi(String(String("1") + (yyvsp[0].String)).c_str()); }
#line 6053 "y.tab.c"
    break;

  case 383: /* number: '2' digits  */
#line 772 "lscp.y"
                                     { (yyval.Number) = atoi(String(String("2") + (yyvsp[0].String)).c_str()); }
#line 6059 "y.tab.c"
    break;

  case 384: /* number: '3' digits  */
#line 773 "lscp.y"
                                     { (yyval.Number) = atoi(String(String("3") + (yyvsp[0].String)).c_str()); }
#line 6065 "y.tab.c"
    break;

  case 385: /* number: '4' digits  */
#line 774 "lscp.y"
                                     { (yyval.Number) = atoi(String(String("4") + (yyvsp[0].String)).c_str()); }
#line 6071 "y.tab.c"
    break;

  case 386: /* number: '5' digits  */
#line 775 "lscp.y"
                                     { (yyval.Number) = atoi(String(String("5") + (yyvsp[0].String)).c_str()); }
#line 6077 "y.tab.c"
    break;

  case 387: /* number: '6' digits  */
#line 776 "lscp.y"
                                     { (yyval.Number) = atoi(String(String("6") + (yyvsp[0].String)).c_str()); }
#line 6083 "y.tab.c"
    break;

  case 388: /* number: '7' digits  */
#line 777 "lscp.y"
                                     { (yyval.Number) = atoi(String(String("7") + (yyvsp[0].String)).c_str()); }
#line 6089 "y.tab.c"
    break;

  case 389: /* number: '8' digits  */
#line 778 "lscp.y"
                                     { (yyval.Number) = atoi(String(String("8") + (yyvsp[0].String)).c_str()); }
#line 6095 "y.tab.c"
    break;

  case 390: /* number: '9' digits  */
#line 779 "lscp.y"
                                     { (yyval.Number) = atoi(String(String("9") + (yyvsp[0].String)).c_str()); }
#line 6101 "y.tab.c"
    break;

  case 391: /* path: '\'' path_base '\''  */
#line 782 "lscp.y"
                                              { (yyval.UniversalPath) = (yyvsp[-1].UniversalPath); }
#line 6107 "y.tab.c"
    break;

  case 392: /* path: '"' path_base '"'  */
#line 783 "lscp.y"
                                              { (yyval.UniversalPath) = (yyvsp[-1].UniversalPath); }
#line 6113 "y.tab.c"
    break;

  case 393: /* path_base: path_prefix path_body  */
#line 786 "lscp.y"
                                                { (yyval.UniversalPath) = (yyvsp[-1].UniversalPath) + (yyvsp[0].UniversalPath); }
#line 6119 "y.tab.c"
    break;

  case 394: /* path_prefix: '/'  */
#line 789 "lscp.y"
                                             { (yyval.UniversalPath) = Path();                    }
#line 6125 "y.tab.c"
    break;

  case 395: /* path_prefix: alpha_char ':' '/'  */
#line 790 "lscp.y"
                                             { Path p; p.setDrive((yyvsp[-2].Char)); (yyval.UniversalPath) = p; }
#line 6131 "y.tab.c"
    break;

  case 396: /* path_body: %empty  */
#line 793 "lscp.y"
                                                        { (yyval.UniversalPath) = Path();                           }
#line 6137 "y.tab.c"
    break;

  case 397: /* path_body: path_body '/'  */
#line 794 "lscp.y"
                                                        { (yyval.UniversalPath) = (yyvsp[-1].UniversalPath);                               }
#line 6143 "y.tab.c"
    break;

  case 398: /* path_body: path_body text_escaped_base  */
#line 795 "lscp.y"
                                                        { Path p; p.appendNode((yyvsp[0].String)); (yyval.UniversalPath) = (yyvsp[-1].UniversalPath) + p; }
#line 6149 "y.tab.c"
    break;

  case 399: /* stringval: '\'' text '\''  */
#line 798 "lscp.y"
                                         { (yyval.String) = (yyvsp[-1].String); }
#line 6155 "y.tab.c"
    break;

  case 400: /* stringval: '"' text '"'  */
#line 799 "lscp.y"
                                         { (yyval.String) = (yyvsp[-1].String); }
#line 6161 "y.tab.c"
    break;

  case 401: /* stringval_escaped: '\'' text_escaped '\''  */
#line 802 "lscp.y"
                                                 { (yyval.String) = (yyvsp[-1].String); }
#line 6167 "y.tab.c"
    break;

  case 402: /* stringval_escaped: '"' text_escaped '"'  */
#line 803 "lscp.y"
                                                 { (yyval.String) = (yyvsp[-1].String); }
#line 6173 "y.tab.c"
    break;

  case 403: /* text: SP  */
#line 806 "lscp.y"
                                      { (yyval.String) = " ";      }
#line 6179 "y.tab.c"
    break;

  case 405: /* text: text SP  */
#line 808 "lscp.y"
                                      { (yyval.String) = (yyvsp[-1].String) + " "; }
#line 6185 "y.tab.c"
    break;

  case 406: /* text: text string  */
#line 809 "lscp.y"
                                      { (yyval.String) = (yyvsp[-1].String) + (yyvsp[0].String);  }
#line 6191 "y.tab.c"
    break;

  case 407: /* text_escaped_base: SP  */
#line 813 "lscp.y"
                                                           { (yyval.String) = " ";      }
#line 6197 "y.tab.c"
    break;

  case 409: /* text_escaped_base: text_escaped_base SP  */
#line 815 "lscp.y"
                                                           { (yyval.String) = (yyvsp[-1].String) + " "; }
#line 6203 "y.tab.c"
    break;

  case 410: /* text_escaped_base: text_escaped_base string_escaped  */
#line 816 "lscp.y"
                                                           { (yyval.String) = (yyvsp[-1].String) + (yyvsp[0].String);  }
#line 6209 "y.tab.c"
    break;

  case 411: /* text_escaped: '/'  */
#line 819 "lscp.y"
                                                          { (yyval.String) = "/";      }
#line 6215 "y.tab.c"
    break;

  case 413: /* text_escaped: text_escaped '/'  */
#line 821 "lscp.y"
                                                          { (yyval.String) = (yyvsp[-1].String) + "/"; }
#line 6221 "y.tab.c"
    break;

  case 414: /* text_escaped: text_escaped text_escaped_base  */
#line 822 "lscp.y"
                                                          { (yyval.String) = (yyvsp[-1].String) + (yyvsp[0].String);  }
#line 6227 "y.tab.c"
    break;

  case 415: /* string: char  */
#line 825 "lscp.y"
                                       { std::string s; s = (yyvsp[0].Char); (yyval.String) = s; }
#line 6233 "y.tab.c"
    break;

  case 416: /* string: string char  */
#line 826 "lscp.y"
                                       { (yyval.String) = (yyvsp[-1].String) + (yyvsp[0].Char);                  }
#line 6239 "y.tab.c"
    break;

  case 417: /* string_escaped: char_base  */
#line 829 "lscp.y"
                                                     { std::string s; s = (yyvsp[0].Char); (yyval.String) = s; }
#line 6245 "y.tab.c"
    break;

  case 418: /* string_escaped: escape_seq  */
#line 830 "lscp.y"
                                                     { std::string s; s = (yyvsp[0].Char); (yyval.String) = s; }
#line 6251 "y.tab.c"
    break;

  case 419: /* string_escaped: string_escaped char_base  */
#line 831 "lscp.y"
                                                     { (yyval.String) = (yyvsp[-1].String) + (yyvsp[0].Char);                  }
#line 6257 "y.tab.c"
    break;

  case 420: /* string_escaped: string_escaped escape_seq  */
#line 832 "lscp.y"
                                                     { (yyval.String) = (yyvsp[-1].String) + (yyvsp[0].Char);                  }
#line 6263 "y.tab.c"
    break;

  case 422: /* char: '\\'  */
#line 837 "lscp.y"
                               { (yyval.Char) = '\\'; }
#line 6269 "y.tab.c"
    break;

  case 423: /* char: '/'  */
#line 838 "lscp.y"
                               { (yyval.Char) = '/';  }
#line 6275 "y.tab.c"
    break;

  case 424: /* alpha_char: 'A'  */
#line 842 "lscp.y"
                             { (yyval.Char) = 'A'; }
#line 6281 "y.tab.c"
    break;

  case 425: /* alpha_char: 'B'  */
#line 842 "lscp.y"
                                                 { (yyval.Char) = 'B'; }
#line 6287 "y.tab.c"
    break;

  case 426: /* alpha_char: 'C'  */
#line 842 "lscp.y"
                                                                     { (yyval.Char) = 'C'; }
#line 6293 "y.tab.c"
    break;

  case 427: /* alpha_char: 'D'  */
#line 842 "lscp.y"
                                                                                         { (yyval.Char) = 'D'; }
#line 6299 "y.tab.c"
    break;

  case 428: /* alpha_char: 'E'  */
#line 842 "lscp.y"
                                                                                                             { (yyval.Char) = 'E'; }
#line 6305 "y.tab.c"
    break;

  case 429: /* alpha_char: 'F'  */
#line 842 "lscp.y"
                                                                                                                                 { (yyval.Char) = 'F'; }
#line 6311 "y.tab.c"
    break;

  case 430: /* alpha_char: 'G'  */
#line 842 "lscp.y"
                                                                                                                                                     { (yyval.Char) = 'G'; }
#line 6317 "y.tab.c"
    break;

  case 431: /* alpha_char: 'H'  */
#line 842 "lscp.y"
                                                                                                                                                                         { (yyval.Char) = 'H'; }
#line 6323 "y.tab.c"
    break;

  case 432: /* alpha_char: 'I'  */
#line 842 "lscp.y"
                                                                                                                                                                                             { (yyval.Char) = 'I'; }
#line 6329 "y.tab.c"
    break;

  case 433: /* alpha_char: 'J'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                 { (yyval.Char) = 'J'; }
#line 6335 "y.tab.c"
    break;

  case 434: /* alpha_char: 'K'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                     { (yyval.Char) = 'K'; }
#line 6341 "y.tab.c"
    break;

  case 435: /* alpha_char: 'L'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                         { (yyval.Char) = 'L'; }
#line 6347 "y.tab.c"
    break;

  case 436: /* alpha_char: 'M'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                             { (yyval.Char) = 'M'; }
#line 6353 "y.tab.c"
    break;

  case 437: /* alpha_char: 'N'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                 { (yyval.Char) = 'N'; }
#line 6359 "y.tab.c"
    break;

  case 438: /* alpha_char: 'O'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                     { (yyval.Char) = 'O'; }
#line 6365 "y.tab.c"
    break;

  case 439: /* alpha_char: 'P'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                                         { (yyval.Char) = 'P'; }
#line 6371 "y.tab.c"
    break;

  case 440: /* alpha_char: 'Q'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                             { (yyval.Char) = 'Q'; }
#line 6377 "y.tab.c"
    break;

  case 441: /* alpha_char: 'R'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                 { (yyval.Char) = 'R'; }
#line 6383 "y.tab.c"
    break;

  case 442: /* alpha_char: 'S'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                     { (yyval.Char) = 'S'; }
#line 6389 "y.tab.c"
    break;

  case 443: /* alpha_char: 'T'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                         { (yyval.Char) = 'T'; }
#line 6395 "y.tab.c"
    break;

  case 444: /* alpha_char: 'U'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                             { (yyval.Char) = 'U'; }
#line 6401 "y.tab.c"
    break;

  case 445: /* alpha_char: 'V'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                 { (yyval.Char) = 'V'; }
#line 6407 "y.tab.c"
    break;

  case 446: /* alpha_char: 'W'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     { (yyval.Char) = 'W'; }
#line 6413 "y.tab.c"
    break;

  case 447: /* alpha_char: 'X'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         { (yyval.Char) = 'X'; }
#line 6419 "y.tab.c"
    break;

  case 448: /* alpha_char: 'Y'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             { (yyval.Char) = 'Y'; }
#line 6425 "y.tab.c"
    break;

  case 449: /* alpha_char: 'Z'  */
#line 842 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 { (yyval.Char) = 'Z'; }
#line 6431 "y.tab.c"
    break;

  case 450: /* alpha_char: 'a'  */
#line 843 "lscp.y"
                             { (yyval.Char) = 'a'; }
#line 6437 "y.tab.c"
    break;

  case 451: /* alpha_char: 'b'  */
#line 843 "lscp.y"
                                                 { (yyval.Char) = 'b'; }
#line 6443 "y.tab.c"
    break;

  case 452: /* alpha_char: 'c'  */
#line 843 "lscp.y"
                                                                     { (yyval.Char) = 'c'; }
#line 6449 "y.tab.c"
    break;

  case 453: /* alpha_char: 'd'  */
#line 843 "lscp.y"
                                                                                         { (yyval.Char) = 'd'; }
#line 6455 "y.tab.c"
    break;

  case 454: /* alpha_char: 'e'  */
#line 843 "lscp.y"
                                                                                                             { (yyval.Char) = 'e'; }
#line 6461 "y.tab.c"
    break;

  case 455: /* alpha_char: 'f'  */
#line 843 "lscp.y"
                                                                                                                                 { (yyval.Char) = 'f'; }
#line 6467 "y.tab.c"
    break;

  case 456: /* alpha_char: 'g'  */
#line 843 "lscp.y"
                                                                                                                                                     { (yyval.Char) = 'g'; }
#line 6473 "y.tab.c"
    break;

  case 457: /* alpha_char: 'h'  */
#line 843 "lscp.y"
                                                                                                                                                                         { (yyval.Char) = 'h'; }
#line 6479 "y.tab.c"
    break;

  case 458: /* alpha_char: 'i'  */
#line 843 "lscp.y"
                                                                                                                                                                                             { (yyval.Char) = 'i'; }
#line 6485 "y.tab.c"
    break;

  case 459: /* alpha_char: 'j'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                 { (yyval.Char) = 'j'; }
#line 6491 "y.tab.c"
    break;

  case 460: /* alpha_char: 'k'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                     { (yyval.Char) = 'k'; }
#line 6497 "y.tab.c"
    break;

  case 461: /* alpha_char: 'l'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                         { (yyval.Char) = 'l'; }
#line 6503 "y.tab.c"
    break;

  case 462: /* alpha_char: 'm'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                             { (yyval.Char) = 'm'; }
#line 6509 "y.tab.c"
    break;

  case 463: /* alpha_char: 'n'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                 { (yyval.Char) = 'n'; }
#line 6515 "y.tab.c"
    break;

  case 464: /* alpha_char: 'o'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                     { (yyval.Char) = 'o'; }
#line 6521 "y.tab.c"
    break;

  case 465: /* alpha_char: 'p'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                                         { (yyval.Char) = 'p'; }
#line 6527 "y.tab.c"
    break;

  case 466: /* alpha_char: 'q'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                             { (yyval.Char) = 'q'; }
#line 6533 "y.tab.c"
    break;

  case 467: /* alpha_char: 'r'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                 { (yyval.Char) = 'r'; }
#line 6539 "y.tab.c"
    break;

  case 468: /* alpha_char: 's'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                     { (yyval.Char) = 's'; }
#line 6545 "y.tab.c"
    break;

  case 469: /* alpha_char: 't'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                         { (yyval.Char) = 't'; }
#line 6551 "y.tab.c"
    break;

  case 470: /* alpha_char: 'u'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                             { (yyval.Char) = 'u'; }
#line 6557 "y.tab.c"
    break;

  case 471: /* alpha_char: 'v'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                 { (yyval.Char) = 'v'; }
#line 6563 "y.tab.c"
    break;

  case 472: /* alpha_char: 'w'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     { (yyval.Char) = 'w'; }
#line 6569 "y.tab.c"
    break;

  case 473: /* alpha_char: 'x'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         { (yyval.Char) = 'x'; }
#line 6575 "y.tab.c"
    break;

  case 474: /* alpha_char: 'y'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             { (yyval.Char) = 'y'; }
#line 6581 "y.tab.c"
    break;

  case 475: /* alpha_char: 'z'  */
#line 843 "lscp.y"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 { (yyval.Char) = 'z'; }
#line 6587 "y.tab.c"
    break;

  case 477: /* char_base: '0'  */
#line 848 "lscp.y"
                             { (yyval.Char) = '0'; }
#line 6593 "y.tab.c"
    break;

  case 478: /* char_base: '1'  */
#line 848 "lscp.y"
                                                 { (yyval.Char) = '1'; }
#line 6599 "y.tab.c"
    break;

  case 479: /* char_base: '2'  */
#line 848 "lscp.y"
                                                                     { (yyval.Char) = '2'; }
#line 6605 "y.tab.c"
    break;

  case 480: /* char_base: '3'  */
#line 848 "lscp.y"
                                                                                         { (yyval.Char) = '3'; }
#line 6611 "y.tab.c"
    break;

  case 481: /* char_base: '4'  */
#line 848 "lscp.y"
                                                                                                             { (yyval.Char) = '4'; }
#line 6617 "y.tab.c"
    break;

  case 482: /* char_base: '5'  */
#line 848 "lscp.y"
                                                                                                                                 { (yyval.Char) = '5'; }
#line 6623 "y.tab.c"
    break;

  case 483: /* char_base: '6'  */
#line 848 "lscp.y"
                                                                                                                                                     { (yyval.Char) = '6'; }
#line 6629 "y.tab.c"
    break;

  case 484: /* char_base: '7'  */
#line 848 "lscp.y"
                                                                                                                                                                         { (yyval.Char) = '7'; }
#line 6635 "y.tab.c"
    break;

  case 485: /* char_base: '8'  */
#line 848 "lscp.y"
                                                                                                                                                                                             { (yyval.Char) = '8'; }
#line 6641 "y.tab.c"
    break;

  case 486: /* char_base: '9'  */
#line 848 "lscp.y"
                                                                                                                                                                                                                 { (yyval.Char) = '9'; }
#line 6647 "y.tab.c"
    break;

  case 487: /* char_base: '!'  */
#line 849 "lscp.y"
                             { (yyval.Char) = '!'; }
#line 6653 "y.tab.c"
    break;

  case 488: /* char_base: '#'  */
#line 849 "lscp.y"
                                                 { (yyval.Char) = '#'; }
#line 6659 "y.tab.c"
    break;

  case 489: /* char_base: '$'  */
#line 849 "lscp.y"
                                                                     { (yyval.Char) = '$'; }
#line 6665 "y.tab.c"
    break;

  case 490: /* char_base: '%'  */
#line 849 "lscp.y"
                                                                                         { (yyval.Char) = '%'; }
#line 6671 "y.tab.c"
    break;

  case 491: /* char_base: '&'  */
#line 849 "lscp.y"
                                                                                                             { (yyval.Char) = '&'; }
#line 6677 "y.tab.c"
    break;

  case 492: /* char_base: '('  */
#line 849 "lscp.y"
                                                                                                                                 { (yyval.Char) = '('; }
#line 6683 "y.tab.c"
    break;

  case 493: /* char_base: ')'  */
#line 849 "lscp.y"
                                                                                                                                                     { (yyval.Char) = ')'; }
#line 6689 "y.tab.c"
    break;

  case 494: /* char_base: '*'  */
#line 849 "lscp.y"
                                                                                                                                                                         { (yyval.Char) = '*'; }
#line 6695 "y.tab.c"
    break;

  case 495: /* char_base: '+'  */
#line 849 "lscp.y"
                                                                                                                                                                                             { (yyval.Char) = '+'; }
#line 6701 "y.tab.c"
    break;

  case 496: /* char_base: '-'  */
#line 849 "lscp.y"
                                                                                                                                                                                                                 { (yyval.Char) = '-'; }
#line 6707 "y.tab.c"
    break;

  case 497: /* char_base: '.'  */
#line 849 "lscp.y"
                                                                                                                                                                                                                                     { (yyval.Char) = '.'; }
#line 6713 "y.tab.c"
    break;

  case 498: /* char_base: ','  */
#line 849 "lscp.y"
                                                                                                                                                                                                                                                         { (yyval.Char) = ','; }
#line 6719 "y.tab.c"
    break;

  case 499: /* char_base: ':'  */
#line 850 "lscp.y"
                             { (yyval.Char) = ':'; }
#line 6725 "y.tab.c"
    break;

  case 500: /* char_base: ';'  */
#line 850 "lscp.y"
                                                 { (yyval.Char) = ';'; }
#line 6731 "y.tab.c"
    break;

  case 501: /* char_base: '<'  */
#line 850 "lscp.y"
                                                                     { (yyval.Char) = '<'; }
#line 6737 "y.tab.c"
    break;

  case 502: /* char_base: '='  */
#line 850 "lscp.y"
                                                                                         { (yyval.Char) = '='; }
#line 6743 "y.tab.c"
    break;

  case 503: /* char_base: '>'  */
#line 850 "lscp.y"
                                                                                                             { (yyval.Char) = '>'; }
#line 6749 "y.tab.c"
    break;

  case 504: /* char_base: '?'  */
#line 850 "lscp.y"
                                                                                                                                 { (yyval.Char) = '?'; }
#line 6755 "y.tab.c"
    break;

  case 505: /* char_base: '@'  */
#line 850 "lscp.y"
                                                                                                                                                     { (yyval.Char) = '@'; }
#line 6761 "y.tab.c"
    break;

  case 506: /* char_base: '['  */
#line 851 "lscp.y"
                             { (yyval.Char) = '['; }
#line 6767 "y.tab.c"
    break;

  case 507: /* char_base: ']'  */
#line 851 "lscp.y"
                                                 { (yyval.Char) = ']'; }
#line 6773 "y.tab.c"
    break;

  case 508: /* char_base: '^'  */
#line 851 "lscp.y"
                                                                     { (yyval.Char) = '^'; }
#line 6779 "y.tab.c"
    break;

  case 509: /* char_base: '_'  */
#line 851 "lscp.y"
                                                                                         { (yyval.Char) = '_'; }
#line 6785 "y.tab.c"
    break;

  case 510: /* char_base: '{'  */
#line 852 "lscp.y"
                             { (yyval.Char) = '{'; }
#line 6791 "y.tab.c"
    break;

  case 511: /* char_base: '|'  */
#line 852 "lscp.y"
                                                 { (yyval.Char) = '|'; }
#line 6797 "y.tab.c"
    break;

  case 512: /* char_base: '}'  */
#line 852 "lscp.y"
                                                                     { (yyval.Char) = '}'; }
#line 6803 "y.tab.c"
    break;

  case 513: /* char_base: '~'  */
#line 852 "lscp.y"
                                                                                         { (yyval.Char) = '~'; }
#line 6809 "y.tab.c"
    break;

  case 515: /* escape_seq: '\\' '\''  */
#line 856 "lscp.y"
                                    { (yyval.Char) = '\''; }
#line 6815 "y.tab.c"
    break;

  case 516: /* escape_seq: '\\' '"'  */
#line 857 "lscp.y"
                                    { (yyval.Char) = '\"'; }
#line 6821 "y.tab.c"
    break;

  case 517: /* escape_seq: '\\' '\\'  */
#line 858 "lscp.y"
                                    { (yyval.Char) = '\\'; }
#line 6827 "y.tab.c"
    break;

  case 518: /* escape_seq: '\\' '/'  */
#line 859 "lscp.y"
                                    { (yyval.Char) = '/';  }
#line 6833 "y.tab.c"
    break;

  case 519: /* escape_seq: '\\' 'n'  */
#line 860 "lscp.y"
                                    { (yyval.Char) = '\n'; }
#line 6839 "y.tab.c"
    break;

  case 520: /* escape_seq: '\\' 'r'  */
#line 861 "lscp.y"
                                    { (yyval.Char) = '\r'; }
#line 6845 "y.tab.c"
    break;

  case 521: /* escape_seq: '\\' 'f'  */
#line 862 "lscp.y"
                                    { (yyval.Char) = '\f'; }
#line 6851 "y.tab.c"
    break;

  case 522: /* escape_seq: '\\' 't'  */
#line 863 "lscp.y"
                                    { (yyval.Char) = '\t'; }
#line 6857 "y.tab.c"
    break;

  case 523: /* escape_seq: '\\' 'v'  */
#line 864 "lscp.y"
                                    { (yyval.Char) = '\v'; }
#line 6863 "y.tab.c"
    break;

  case 526: /* escape_seq_octal: '\\' digit_oct  */
#line 869 "lscp.y"
                                                             { (yyval.Char) = (char) octalsToNumber((yyvsp[0].Char));       }
#line 6869 "y.tab.c"
    break;

  case 527: /* escape_seq_octal: '\\' digit_oct digit_oct  */
#line 870 "lscp.y"
                                                             { (yyval.Char) = (char) octalsToNumber((yyvsp[0].Char),(yyvsp[-1].Char));    }
#line 6875 "y.tab.c"
    break;

  case 528: /* escape_seq_octal: '\\' digit_oct digit_oct digit_oct  */
#line 871 "lscp.y"
                                                             { (yyval.Char) = (char) octalsToNumber((yyvsp[0].Char),(yyvsp[-1].Char),(yyvsp[-2].Char)); }
#line 6881 "y.tab.c"
    break;

  case 529: /* escape_seq_hex: '\\' 'x' digit_hex  */
#line 874 "lscp.y"
                                                       { (yyval.Char) = (char) hexsToNumber((yyvsp[0].Char));    }
#line 6887 "y.tab.c"
    break;

  case 530: /* escape_seq_hex: '\\' 'x' digit_hex digit_hex  */
#line 875 "lscp.y"
                                                       { (yyval.Char) = (char) hexsToNumber((yyvsp[0].Char),(yyvsp[-1].Char)); }
#line 6893 "y.tab.c"
    break;


#line 6897 "y.tab.c"

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
          = {yyssp, yytoken};
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
        yyerror (yyparse_param, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, yyparse_param);
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


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yyparse_param);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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
  yyerror (yyparse_param, YY_("memory exhausted"));
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
                  yytoken, &yylval, yyparse_param);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yyparse_param);
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

#line 1297 "lscp.y"


// TODO: actually would be fine to have the following bunch of source code in a separate file, however those functions are a) accessing private Bison tables like yytable and b) including the functions from another file here would make the line numbers incorrect on compile errors in auto generated lscpparser.cpp

/**
 * Additional informations of a grammar symbol.
 */
struct BisonSymbolInfo {
    bool isTerminalSymbol; ///< Whether the symbol is a terminal or non-termianl symbol. NOTE: Read comment regarding this in _isRuleTerminalSymbol() !!
    String nextExpectedChars; ///< According to current parser position: sequence of characters expected next for satisfying this grammar symbol.
};

#if HAVE_BISON_MAJ >= 3 || !defined(HAVE_BISON_MAJ) // Bison 3.x or younger ...

/**
 * Must ONLY be called just before a so called "reduce" parser action:
 * Returns true if the grammar rule, which is just about to be "reduced", is a
 * terminal symbol (in *our* terms).
 *
 * Please note that the term "terminal symbol" is a bit confusingly used in
 * this source code here around. In Bison's terms, "terminal symbols" are (more
 * or less) just the numbers returned by the YYLEX function. Since we decided
 * though to use a convenient solution without a separate lexer, and all its
 * caveats, all numbers by the yylex() function here are just the ASCII
 * numbers of the individual characters received. Based on that however, one
 * single character is not what one would intuitively expect of being a 
 * "terminal symbol", because it is simply too primitive.
 *
 * So in this LSCP parser source code a "terminal symbol" rather means a
 * keyword like "CREATE" or "GET". In the grammal definition above, those are
 * however defined as grammar rules (non-terminals in Bison's terms). So this
 * function decides like this: if the given grammar rule just contains
 * individual characters on the right side of its grammar rule, then it is a
 * "terminal symbol" in *our* terms.
 *
 * @param rule - Bison grammar rule number
 * @param stack - reflecting current Bison parser state
 */
inline static bool _isRuleTerminalSymbol(int rule, const std::vector<YYTYPE_INT16>& stack) {
    int nrhs = yyr2[rule];
    for (int i = 0; i < nrhs; ++i)
        if (yystos[*(stack.end() - nrhs + i)] >= YYNTOKENS) return false;
    return true;
}

/**
 * Must ONLY be called just before a so called "reduce" parser action: Returns
 * additional informations to the given grammar rule that is about to be
 * "reduced".
 *
 * @param rule - Bison grammar rule number
 * @param stack - reflecting current Bison parser state
 * @param nextExpectedChars - must already be filled with the characters
 *                            expected to be coming next
 */
inline static BisonSymbolInfo _symbolInfoForRule(int rule, const std::vector<YYTYPE_INT16>& stack, const String& nextExpectedChars) {
    BisonSymbolInfo info;
    info.isTerminalSymbol = _isRuleTerminalSymbol(rule, stack);
    if (info.isTerminalSymbol) info.nextExpectedChars  = nextExpectedChars;
    return info;
}

#else // Bison 2.x or older ...

//TODO: The Bison 2.x code below can probably soon just be deleted. Most Bisonx 2.x versions should be able to compile successfully with the Bison 3.x code above as well (just requires the existence of table yystos[] in the auto generated lscpparser.cpp).

/**
 * Returns true if the given grammar @a rule is a terminal symbol (in *our*
 * terms).
 *
 * Please note that the term "terminal symbol" is a bit confusingly used in
 * this source code here around. In Bison's terms, "terminal symbols" are (more
 * or less) just the numbers returned by the YYLEX function. Since we decided
 * though to use a convenient solution without a separate lexer, and all its
 * caveats, all numbers by the yylex() function here are just the ASCII
 * numbers of the individual characters received. Based on that however, one
 * single character is not what one would intuitively expect of being a 
 * "terminal symbol", because it is simply too primitive.
 *
 * So in this LSCP parser source code a "terminal symbol" rather means a
 * keyword like "CREATE" or "GET". In the grammal definition above, those are
 * however defined as grammar rules (non-terminals in Bison's terms). So this
 * function decides like this: if the given grammar rule just contains
 * individual characters on the right side of its grammar rule, then it is a
 * "terminal symbol" in *our* terms.
 *
 * @param rule - Bison grammar rule number
 */
inline static bool _isRuleTerminalSymbol(int rule) {
    for (int i = yyprhs[rule]; yyrhs[i] != -1; ++i)
        if (yyrhs[i] >= YYNTOKENS) return false;
    return true;
}

/**
 * Returns additional informations to the given grammar @a rule.
 *
 * @param rule - grammar rule index to retrieve informations about
 * @param nextExpectedChars - must already be filled with the characters
 *                            expected to be coming next
 */
inline static BisonSymbolInfo _symbolInfoForRule(int rule, const String& nextExpectedChars) {
    BisonSymbolInfo info;
    info.isTerminalSymbol = _isRuleTerminalSymbol(rule);
    if (info.isTerminalSymbol) info.nextExpectedChars  = nextExpectedChars;
    return info;
}

#endif // HAVE_BISON_MAJ >= 3

/**
 * Returns the human readable name of the given @a token.
 */
inline static String _tokenName(int token) {
    String s = yytname[token];
    // remove leading and trailing apostrophes that Bison usually adds to
    // ASCII characters used directly in grammar rules
    if (s.empty()) return s;
    if (s[0] == '\'') s.erase(0, 1);
    if (s.empty()) return s;
    if (s[s.size() - 1] == '\'') s.erase(s.size() - 1);
    return s;
}

/**
 * Assumes the given @a token is exactly one character and returns that
 * character. This must be changed in future, i.e. in case Unicode characters
 * will be introduced in the LSCP grammar one day.
 */
inline static char _tokenChar(int token) {
    String s = _tokenName(token);
    if (s == "\\n") return '\n';
    if (s == "\\r") return '\r';
    return _tokenName(token)[0];
}

/**
 * Implements Bison's so called "reduce" action, according to Bison's LALR(1)
 * parser algorithm.
 */
inline static int _yyReduce(std::vector<YYTYPE_INT16>& stack, const int& rule) {
    if (stack.empty()) throw 1; // severe error
    const int len = yyr2[rule];
    stack.resize(stack.size() - len);
    YYTYPE_INT16 newState = yypgoto[yyr1[rule] - YYNTOKENS] + stack.back();
    if (0 <= newState && newState <= YYLAST && yycheck[newState] == stack.back())
        newState = yytable[newState];
    else
        newState = yydefgoto[yyr1[rule] - YYNTOKENS];
    stack.push_back(newState);
    return newState;
}

/**
 * Implements Bison's so called "default reduce" action, according to Bison's
 * LALR(1) parser algorithm.
 */
inline static int _yyDefaultReduce(std::vector<YYTYPE_INT16>& stack) {
    if (stack.empty()) throw 2; // severe error
    int rule = yydefact[stack.back()];
    if (rule <= 0 || rule >= YYNRULES) throw 3; // no rule, something is wrong
    return _yyReduce(stack, rule);
}

static bool yyValid(std::vector<YYTYPE_INT16>& stack, char ch);

/**
 * A set of parser symbol stacks. This type is used for the recursive algorithms
 * in a) yyAutoComplete() and b) walkAndFillExpectedSymbols() for detecting
 * endless recursions.
 * 
 * This unique container is used to keep track of all previous parser states
 * (stacks), for detecting a parser symbol stack that has already been
 * encountered before. Because if yyAutoComplete() or
 * walkAndFillExpectedSymbols() reach the exactly same parser symbol stack
 * again, that means there is an endless recursion in that part of the grammar
 * tree branch and shall not be evaluated any further, since it would end up in
 * an endless loop of the algorithm otherwise.
 *
 * This solution consumes a lot of memory, but unfortunately there is no other
 * easy way to solve it. With our grammar and today's usual memory heap size &
 * memory stack size in modern devices, it should be fine though.
 */
typedef std::set< std::vector<YYTYPE_INT16> > YYStackHistory;

/*
 * YYTERROR macro was removed in Bison 3.6.0, we need it in function below.
 */
#ifndef YYTERROR
# define YYTERROR YYSYMBOL_YYerror
#endif

#define DEBUG_BISON_SYNTAX_ERROR_WALKER 0

/**
 * Tries to find the next expected grammar symbols according to the given
 * precise parse position & state represented by @a stack, according to Bison's
 * LALR(1) parser algorithm.
 *
 * This function is given a Bison parser symbol stack, reflecting the parser's
 * entire state at a certain point, i.e. when a syntax error occured. This
 * function will then walk ahead the potential parse tree starting from the
 * current head of the given symbol stack. This function will call itself
 * recursively to scan the individual parse tree branches. As soon as it hits
 * on the next non-terminal grammar symbol in one parse tree branch, it adds the
 * found non-terminal symbol to @a expectedSymbols and aborts scanning the
 * respective tree branch further. If any local parser state is reached a second
 * time, the respective parse tree is aborted to avoid any endless recursion.
 *
 * @param stack - current Bison (yacc) symbol stack to be examined
 * @param expectedSymbols - will be filled with next expected grammar symbols
 * @param nextExpectedChars - just for internal purpose, due to the recursive
 *                            implementation of this function, do supply an
 *                            empty string for this argument
 * @param history - only for internal purpose, keeps a history of all previous
 *                  parser symbol stacks (just for avoiding endless recursion in
 *                  this recursive algorithm), do supply an empty history
 * @param depth - just for internal debugging purposes, do not supply it
 */
static void walkAndFillExpectedSymbols(
    std::vector<YYTYPE_INT16>& stack,
    std::map<String,BisonSymbolInfo>& expectedSymbols,
    String& nextExpectedChars, YYStackHistory& history, int depth = 0)
{
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
    printf("\n");
    for (int i = 0; i < depth; ++i) printf("\t");
    printf("Symbol stack:");
    for (int i = 0; i < stack.size(); ++i) {
        printf(" %d", stack[i]);
    }
    printf("\n");
#endif
    startLabel:

    // detect endless recursion
    if (history.count(stack)) return;
    history.insert(stack);

    if (stack.empty()) {
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
        for (int i = 0; i < depth; ++i) printf("\t");
        printf("(EMPTY STACK)\n");
#endif
        return;
    }

    int state = stack[stack.size() - 1];
    int n = yypact[state];
    if (n == YYPACT_NINF) { // default reduction required ...
        // get default reduction rule for this state
        n = yydefact[state];
        if (n <= 0 || n >= YYNRULES) {
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
            for (int i = 0; i < depth; ++i) printf("\t");
            printf("(EMPTY RULE)\n");
#endif
            return; // no rule, something is wrong
        }
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
        for (int i = 0; i < depth; ++i) printf("\t");
        printf("(default reduction)\n");
#endif
        #if HAVE_BISON_MAJ >= 3 || !defined(HAVE_BISON_MAJ)
        if (!nextExpectedChars.empty() || !_isRuleTerminalSymbol(n, stack)) {
        #else
        if (!nextExpectedChars.empty() || !_isRuleTerminalSymbol(n)) {
        #endif
            // Return the new resolved expected symbol (left-hand symbol of grammar
            // rule), then we're done in this state. (If the same symbol can be
            // matched on different ways, then it is non-terminal symbol.)
            bool ambigious =
                expectedSymbols.count(yytname[yyr1[n]]) &&
                expectedSymbols[yytname[yyr1[n]]].nextExpectedChars != nextExpectedChars;
            #if HAVE_BISON_MAJ >= 3 || !defined(HAVE_BISON_MAJ)
            expectedSymbols[yytname[yyr1[n]]] = _symbolInfoForRule(n, stack, nextExpectedChars);
            #else
            expectedSymbols[yytname[yyr1[n]]] = _symbolInfoForRule(n, nextExpectedChars);
            #endif
            if (ambigious)
                expectedSymbols[yytname[yyr1[n]]].isTerminalSymbol = false;
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
            for (int i = 0; i < depth; ++i) printf("\t");
            printf("(empty expectedChars. sym = %s)\n", yytname[yyr1[n]]);
#endif
            return;
        }
        _yyReduce(stack, n);
        goto startLabel;
    }
    if (!(YYPACT_NINF < n && n <= YYLAST)) {
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
        for (int i = 0; i < depth; ++i) printf("\t");
        printf("(invalid action B)\n");
#endif
        return;
    }

    // Check for duplicate states, if duplicates exist return
    // (this check is necessary since the introduction of the yyValid() call
    // below, which does not care about duplicates).
    for (int i = 0; i < stack.size(); ++i)
        for (int k = i + 1; k < stack.size(); ++k)
            if (stack[i] == stack[k])
                return;

#if DEBUG_BISON_SYNTAX_ERROR_WALKER
    for (int i = 0; i < depth; ++i) printf("\t");
    printf("Expected tokens:");
#endif
    int begin = n < 0 ? -n : 0;
    //int checklim = YYLAST - n + 1;
    int end = YYNTOKENS;//checklim < YYNTOKENS ? checklim : YYNTOKENS;
    int rule, action, stackSize, nextExpectedCharsLen;
    for (int token = begin; token < end; ++token) {
        if (token <= YYTERROR) continue;
        if (yytname[token] == String("$undefined")) continue;
        if (yytname[token] == String("EXT_ASCII_CHAR")) continue;
        //if (yycheck[n + token] != token) goto default_reduction;
        if (yycheck[n + token] != token) { // default reduction suggested ...
            // If we are here, it means the current token in the loop would not
            // cause a "shift", however we don't already know whether this token
            // is valid or not. Because there might be several reductions
            // involved until one can determine whether the token causes an
            // error or is valid. So we use this heavy check instead:
            std::vector<YYTYPE_INT16> stackCopy = stack; // copy required, since reduction will take place
            if (!yyValid(stackCopy, _tokenChar(token))) continue; // invalid token
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
            printf(" ETdr(%s)", yytname[token]);
#endif
            // the token is valid, "stackCopy" has been reduced accordingly
            // and now do recurse ...
            nextExpectedChars += _tokenName(token);
            nextExpectedCharsLen = (int)nextExpectedChars.size();
            walkAndFillExpectedSymbols( //FIXME: could cause stack overflow (should be a loop instead), is probably fine with our current grammar though
                stackCopy, expectedSymbols, nextExpectedChars, history, depth + 1
            );
            nextExpectedChars.resize(nextExpectedCharsLen); // restore 'nextExpectedChars'
            continue;
        }
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
        printf(" ET(%s)", yytname[token]);
#endif

        action = yytable[n + token];
        if (action == 0 || action == YYTABLE_NINF) {
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
            printf(" (invalid action A) "); fflush(stdout);
#endif
            continue; // error, ignore
        }
        if (action < 0) { // reduction with rule -action required ...
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
            printf(" (reduction) "); fflush(stdout);
#endif
            rule = -action;
            goto reduce;
        }
        if (action == YYFINAL) {
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
            printf(" (ACCEPT) "); fflush(stdout);
#endif
            continue; // "accept" state, we don't care about it here
        }

        // "shift" required ...

        if (std::find(stack.begin(), stack.end(), action) != stack.end()) {
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
            printf(" (duplicate state %d) ", action); fflush(stdout);
#endif
            continue; // duplicate state, ignore it to avoid endless recursions
        }

        // "shift" / push the new state on the symbol stack and call this
        // function recursively, and restore the stack after the recurse return
        stackSize = (int)stack.size();
        nextExpectedCharsLen = (int)nextExpectedChars.size();
        stack.push_back(action);
        nextExpectedChars += _tokenName(token);
        walkAndFillExpectedSymbols( //FIXME: could cause stack overflow (should be a loop instead), is probably fine with our current grammar though
            stack, expectedSymbols, nextExpectedChars, history, depth + 1
        );
        stack.resize(stackSize); // restore stack
        nextExpectedChars.resize(nextExpectedCharsLen); // restore 'nextExpectedChars'
        continue;

    //default_reduction: // resolve default reduction for this state
    //    printf(" (default red.) "); fflush(stdout);
    //    rule = yydefact[state];

    reduce: // "reduce" required
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
        printf(" (reduce by %d) ", rule); fflush(stdout);
#endif
        if (rule == 0 || rule >= YYNRULES) {
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
            printf(" (invalid rule) "); fflush(stdout);
#endif
            continue; // invalid rule, something is wrong
        }
        // Store the left-hand symbol of the grammar rule. (If the same symbol
        // can be matched on different ways, then it is non-terminal symbol.)
        bool ambigious =
            expectedSymbols.count(yytname[yyr1[rule]]) &&
            expectedSymbols[yytname[yyr1[rule]]].nextExpectedChars != nextExpectedChars;
        #if HAVE_BISON_MAJ >= 3 || !defined(HAVE_BISON_MAJ)
        expectedSymbols[yytname[yyr1[rule]]] = _symbolInfoForRule(rule, stack, nextExpectedChars);
        #else
        expectedSymbols[yytname[yyr1[rule]]] = _symbolInfoForRule(rule, nextExpectedChars);
        #endif
        if (ambigious)
            expectedSymbols[yytname[yyr1[n]]].isTerminalSymbol = false;
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
        printf(" (SYM %s) ", yytname[yyr1[rule]]); fflush(stdout);
#endif
    }
#if DEBUG_BISON_SYNTAX_ERROR_WALKER
    printf("\n");
#endif
}

/**
 * Just a convenience wrapper on top of the actual walkAndFillExpectedSymbols()
 * implementation above, which can be called with less parameters than the
 * implementing function above actually requires.
 */
static void walkAndFillExpectedSymbols(
    std::vector<YYTYPE_INT16>& stack,
    std::map<String,BisonSymbolInfo>& expectedSymbols)
{
    String nextExpectedChars;
    YYStackHistory history;

    walkAndFillExpectedSymbols(
        stack, expectedSymbols, nextExpectedChars, history
    );
}

#define DEBUG_PUSH_PARSE 0

/**
 * Implements parsing exactly one character (given by @a ch), continueing at the
 * parser position reflected by @a stack. The @a stack will hold the new parser
 * state after this call.
 *
 * This function is implemented according to Bison's LALR(1) parser algorithm.
 */
static bool yyPushParse(std::vector<YYTYPE_INT16>& stack, char ch) {
    startLabel:

#if DEBUG_PUSH_PARSE
    //printf("\n");
    //for (int i = 0; i < depth; ++i) printf("\t");
    printf("Symbol stack:");
    for (int i = 0; i < stack.size(); ++i) {
        printf(" %d", stack[i]);
    }
    printf(" char='%c'(%d)\n", ch, (int)ch);
#endif

    if (stack.empty()) return false;

    int state = stack.back();
    int n = yypact[state];
    if (n == YYPACT_NINF) { // default reduction required ...
#if DEBUG_PUSH_PARSE
        printf("(def reduce 1)\n");
#endif
        state = _yyDefaultReduce(stack);
        goto startLabel;
    }
    if (!(YYPACT_NINF < n && n <= YYLAST)) return false;

    YYTYPE_INT16 token = (ch == YYEOF) ? YYEOF : yytranslate[ch];
    n += token;
    if (n < 0 || YYLAST < n || yycheck[n] != token) {
#if DEBUG_PUSH_PARSE
        printf("(def reduce 2) n=%d token=%d\n", n, token);
#endif
        state = _yyDefaultReduce(stack);
        goto startLabel;
    }
    int action = yytable[n]; // yytable[yypact[state] + token]
    if (action == 0 || action == YYTABLE_NINF) throw 4;
    if (action < 0) {
#if DEBUG_PUSH_PARSE
        printf("(reduce)\n");
#endif
        int rule = -action;
        state = _yyReduce(stack, rule);
        goto startLabel;
    }
    if (action == YYFINAL) return true; // final state reached

#if DEBUG_PUSH_PARSE
    printf("(push)\n");
#endif
    // push new state
    state = action;
    stack.push_back(state);
    return true;
}

/**
 * Returns true if parsing ahead with given character @a ch is syntactically
 * valid according to the LSCP grammar, it returns false if it would create a
 * parse error.
 *
 * The @a stack will reflect the new parser state after this call.
 *
 * This is just a wrapper ontop of yyPushParse() which converts parser
 * exceptions thrown by yyPushParse() into @c false return value.
 */
static bool yyValid(std::vector<YYTYPE_INT16>& stack, char ch) {
    try {
        return yyPushParse(stack, ch);
    } catch (int i) {
#if DEBUG_PUSH_PARSE
        printf("exception %d\n", i);
#endif
        return false;
    } catch (...) {
        return false;
    }
}

/**
 * Returns the amount of correct characters of given @a line from the left,
 * according to the LSCP grammar.
 *
 * @param stack - a Bison symbol stack to work with
 * @param line  - the input line to check
 * @param bAutoCorrect - if true: try to correct obvious, trivial syntax errors
 */
static int yyValidCharacters(std::vector<YYTYPE_INT16>& stack, String& line, bool bAutoCorrect) {
    int i;
    for (i = 0; i < line.size(); ++i) {
        // since we might check the same parser state twice against the current
        // char here below, and since the symbol stack might be altered
        // (i.e. shifted or reduced) on syntax errors, we have to backup the
        // current symbol stack and restore it on syntax errors below
        std::vector<YYTYPE_INT16> stackCopy = stack;
        if (yyValid(stackCopy, line[i])) {
            stack = stackCopy;
            continue;
        }
        if (bAutoCorrect) {
            // try trivial corrections, i.e. upper case character instead of
            // lower case, subline instead of space and vice versa
            char c;
            if      (line[i] == ' ') c = '_';
            else if (line[i] == '_') c = ' ';
            else if (isLowerCaseAlphaChar(line[i]))
                c = alphaCharToUpperCase(line[i]);
            else return i;
            if (yyValid(stack, c)) {
                line[i] = c;
                continue;
            }
        }
        return i;
    }
    return i;
}

/**
 * Should only be called on syntax errors: returns a set of non-terminal
 * symbols expected to appear now/next, just at the point where the syntax
 * error appeared.
 *
 * @returns names of the non-terminal symbols expected at this parse position
 */
static std::set<String> yyExpectedSymbols() {
    std::map<String,BisonSymbolInfo> expectedSymbols;
    yyparse_param_t* param = GetCurrentYaccSession();
    YYTYPE_INT16* ss = (*param->ppStackBottom);
    YYTYPE_INT16* sp = (*param->ppStackTop);
    int iStackSize   = int(sp - ss + 1);
    // copy and wrap parser's symbol stack into a convenient STL container
    std::vector<YYTYPE_INT16> stack;
    for (int i = 0; i < iStackSize; ++i) {
        stack.push_back(ss[i]);
    }
    // do the actual parser work
    walkAndFillExpectedSymbols(stack, expectedSymbols);

    // convert expectedSymbols to the result set
    std::set<String> result;
    for (std::map<String,BisonSymbolInfo>::const_iterator it = expectedSymbols.begin();
         it != expectedSymbols.end(); ++it) result.insert(it->first);
    return result;
}

#define DEBUG_YY_AUTO_COMPLETE 0

/**
 * Generates and returns an auto completion string for the current parser
 * state given by @a stack. That means, this function will return the longest
 * sequence of characters that is uniqueley expected to be sent next by the LSCP
 * client. Or in other words, if the LSCP client would send any other
 * character(s) than returned here, it would result in a syntax error.
 *
 * This function takes a Bison symbol @a stack as argument, reflecting the
 * current Bison parser state, and evaluates the individual grammar tree
 * branches starting from that particular position. It walks along the grammar
 * tree as long as there is only one possible tree branch and assembles a string
 * of input characters that would lead to that walk through the grammar tree. As
 * soon as a position in the grammar tree is reached where there are multiple
 * possible tree branches, this algorithm will stop, since the user could have
 * multiple possible valid characters he could type at that point, thus auto
 * completion would no longer be unique at that point.
 *
 * Regarding @a history argument: read the description on YYStackHistory for the
 * purpose behind this argument.
 *
 * @param stack - current Bison (yacc) symbol stack to create auto completion for
 * @param history - only for internal purpose, keeps a history of all previous
 *                  parser symbol stacks (just for avoiding endless recursion in
 *                  this auto completion algorithm), do supply an empty history
 * @param depth - just for internal debugging purposes, do not supply anything
 * @returns auto completion for current, given parser state
 */
static String yyAutoComplete(std::vector<YYTYPE_INT16>& stack, YYStackHistory& history, int depth = 0) {
    std::map<String,BisonSymbolInfo> expectedSymbols;
    walkAndFillExpectedSymbols(stack, expectedSymbols);
    if (expectedSymbols.size() == 1) {
        String name          = expectedSymbols.begin()->first;
        BisonSymbolInfo info = expectedSymbols.begin()->second;
#if DEBUG_YY_AUTO_COMPLETE
        for (int q = 0; q < depth; ++q) printf("  ");
        printf("(%d) Suggested Sub Completion (sz=%d): type=%s %s -> '%s'\n", depth, expectedSymbols.size(), (info.isTerminalSymbol) ? "T" : "NT", name.c_str(), info.nextExpectedChars.c_str());
#endif
        if (info.nextExpectedChars.empty() || !info.isTerminalSymbol) return "";
        // parse forward with the suggested auto completion
        std::vector<YYTYPE_INT16> stackCopy = stack;
        yyValidCharacters(stackCopy, info.nextExpectedChars, false);
        // detect endless recursion
        if (history.count(stackCopy)) return "";
        history.insert(stackCopy);
        // recurse and return the expanded auto completion with maximum length
        return info.nextExpectedChars + yyAutoComplete(stackCopy, history, depth + 1);
    } else if (expectedSymbols.size() == 0) {
#if DEBUG_YY_AUTO_COMPLETE
        for (int q = 0; q < depth; ++q) printf("  ");
        printf("(%d) No sub suggestion.\n", depth);
#endif
        return "";
    } else if (expectedSymbols.size() > 1) {
#if DEBUG_YY_AUTO_COMPLETE
        for (int q = 0; q < depth; ++q) printf("  ");
        printf("(%d) Multiple sub possibilities (before expansion):", depth);
        for (std::map<String,BisonSymbolInfo>::const_iterator it = expectedSymbols.begin();
             it != expectedSymbols.end(); ++it)
        {
            printf(" %s (..%s)", it->first.c_str(), it->second.nextExpectedChars.c_str());
        }
        printf("\n");
#endif
        // check if any of the possibilites is a non-terminal symbol, if so, we
        // have no way for auto completion at this point
        for (std::map<String,BisonSymbolInfo>::const_iterator it = expectedSymbols.begin();
             it != expectedSymbols.end(); ++it)
        {
            if (!it->second.isTerminalSymbol) {
#if DEBUG_YY_AUTO_COMPLETE
                for (int q = 0; q < depth; ++q) printf("  ");
                printf("(%d) Non-terminal exists. Stop.", depth);
#endif
                return "";
            }
        }
#if 0 // commented out for now, since practically irrelevant and VERY slow ...
        // all possibilities are terminal symbols, so expand all possiblities to
        // maximum length with a recursive call for each possibility
        for (std::map<String,BisonSymbolInfo>::iterator it = expectedSymbols.begin();
             it != expectedSymbols.end(); ++it)
        {
            if (it->second.nextExpectedChars.empty() || !it->second.isTerminalSymbol) continue;
            // parse forward with this particular suggested auto completion
            std::vector<YYTYPE_INT16> stackCopy = stack;
            yyValidCharacters(stackCopy, it->second.nextExpectedChars, false);
            // detect endless recursion
            if (history.count(stackCopy)) continue;
            history.insert(stackCopy);
            // recurse and return the total possible auto completion for this
            // grammar tree branch
            it->second.nextExpectedChars += yyAutoComplete(stackCopy, history, depth + 1);
        }
#endif
        // try to find the longest common string all possibilities start with
        // (from the left)
        String sCommon;
        for (int i = 0; true; ++i) {
            char c = '\0';
            for (std::map<String,BisonSymbolInfo>::const_iterator it = expectedSymbols.begin();
                 it != expectedSymbols.end(); ++it)
            {
                if (i >= it->second.nextExpectedChars.size())
                    goto commonSearchEndLabel;
                if (it == expectedSymbols.begin())
                    c = it->second.nextExpectedChars[i];
                if (c != it->second.nextExpectedChars[i])
                    goto commonSearchEndLabel;
                if (it == --expectedSymbols.end())
                    sCommon += c;
            }
        }
        commonSearchEndLabel:
#if DEBUG_YY_AUTO_COMPLETE
        for (int q = 0; q < depth; ++q) printf("  ");
        printf("(%d) Multiple sub possibilities (after expansion):", depth);
        for (std::map<String,BisonSymbolInfo>::const_iterator it = expectedSymbols.begin();
             it != expectedSymbols.end(); ++it)
        {
            printf(" %s (..%s)", it->first.c_str(), it->second.nextExpectedChars.c_str());
        }
        printf("\n");
        for (int q = 0; q < depth; ++q) printf("  ");
        printf("(%d) Common sub possibility: '%s'\n", depth, sCommon.c_str());
#endif
        return sCommon;
    }
    return ""; // just pro forma, should never happen though
}

/**
 * Just a convenience wrapper on top of the actual yyAutoComplete()
 * implementation. See its description above for details.
 */
static String yyAutoComplete(std::vector<YYTYPE_INT16>& stack) {
    YYStackHistory history;
    return yyAutoComplete(stack, history);
}

namespace LinuxSampler {

#define DEBUG_SHELL_INTERACTION 0

/**
 * If LSCP shell mode is enabled for the respective LSCP client connection, then
 * this function is called on every new byte received from that client. It will
 * check the current total input line and reply to the LSCP shell with a
 * specially crafted string, which allows the shell to provide colored syntax
 * highlighting and potential auto completion in the shell.
 *
 * It also performs auto correction of obvious & trivial syntax mistakes if
 * requested.
 *
 * The return value of this function will be sent to the client. It contains one
 * line specially formatted for the LSCP shell application, which can easily be
 * processed by the client/shell for extracting its necessary informations like
 * which part of the current command line is syntactically correct, which part
 * is incorrect, what could be auto completed right now, etc. So all the heavy
 * grammar evaluation tasks are peformed by the LSCP server for the LSCP shell
 * application (which is desgined as a thin client), so the LSCP shell
 * application will only have to show the results of the LSCP server's
 * evaluation to the user on the screen.
 *
 * @param line - the current command line to be evaluated by LSCP parser
 * @param param = reentrant parser session parameters
 * @param possibilities - whether all possibilities shall be shown
 * @returns LSCP shell response line to be returned to the client
 */
String lscpParserProcessShellInteraction(String& line, yyparse_param_t* param, bool possibilities) {
    // first, determine how many characters (starting from the left) of the
    // given input line are already syntactically correct
    std::vector<YYTYPE_INT16> stack;
    stack.push_back(0); // every Bison symbol stack starts with state zero
    String l = line + '\n'; // '\n' to pretend ENTER as if the line was now complete
    int n = yyValidCharacters(stack, l, param->bShellAutoCorrect);

    // if auto correction is enabled, apply the auto corrected string to
    // intput/output string 'line'
    if (param->bShellAutoCorrect) {
        int nMin = int( (n < line.length()) ? n : line.length() );
        line.replace(0, nMin, l.substr(0, nMin));
    }

    ssize_t cursorPos = line.size() + param->iCursorOffset;
    if (cursorPos < 0) cursorPos = 0;

    // generate an info string that will be sent to the LSCP shell for letting
    // it know which part is correct, which one is wrong, where is the cursor, etc.
    String result = line;
    result.insert(n <= result.length() ? n : result.length(), LSCP_SHK_GOOD_FRONT);
    result.insert(cursorPos <= n ? cursorPos : cursorPos + String(LSCP_SHK_GOOD_FRONT).length(), LSCP_SHK_CURSOR);
    int code = (n > line.length()) ? LSCP_SHU_COMPLETE : (n < line.length()) ?
               LSCP_SHU_SYNTAX_ERR : LSCP_SHU_INCOMPLETE;
    result = "SHU:" + ToString(code) + ":" + result;
    //if (n > line.length()) result += " [OK]";

    // get a clean parser stack to the last valid parse position
    // (due to the appended '\n' character above, and on syntax errors, the
    // symbol stack might be in undesired, i.e. reduced state)
    stack.clear();
    stack.push_back(0); // every Bison symbol stack starts with state zero
    l = line.substr(0, n);
    if (!l.empty()) yyValidCharacters(stack, l, param->bShellAutoCorrect);

    // generate auto completion suggestion (based on the current parser stack)
    std::vector<YYTYPE_INT16> stackCopy = stack; // make a copy, since yyAutoComplete() might alter the stack
    String sSuggestion = yyAutoComplete(stackCopy);
    if (!sSuggestion.empty()) result += LSCP_SHK_SUGGEST_BACK + sSuggestion;

    if (possibilities) {
        // append all possible terminals and non-terminals according to
        // current parser state
        std::map<String,BisonSymbolInfo> expectedSymbols;
        walkAndFillExpectedSymbols(stack, expectedSymbols);

        // pretend to LSCP shell that the following terminal symbols were
        // non-terminal symbols (since they are not human visible for auto
        // completion on the shell's screen)
        std::set<String> specialNonTerminals;
        specialNonTerminals.insert("SP");
        specialNonTerminals.insert("CR");
        specialNonTerminals.insert("LF");

        String sPossibilities;
        int iNonTerminals = 0;
        int iTerminals    = 0;
        for (std::map<String,BisonSymbolInfo>::const_iterator it = expectedSymbols.begin();
             it != expectedSymbols.end(); ++it)
        {
            if (!sPossibilities.empty()) sPossibilities += " | ";
            if (it->second.isTerminalSymbol && !specialNonTerminals.count(it->first)) {
                sPossibilities += it->first;
                iTerminals++;
            } else {
                sPossibilities += "<" + it->first + ">";
                iNonTerminals++;
            }
        }
        if (!sPossibilities.empty() && (iNonTerminals || iTerminals > 1)) {
            result += LSCP_SHK_POSSIBILITIES_BACK + sPossibilities;
        }
    }

#if DEBUG_SHELL_INTERACTION
    printf("%s\n", result.c_str());
#endif

    return result;
}

/**
 * Clears input buffer.
 */
void restart(yyparse_param_t* pparam, int& yychar) {
    bytes = 0;
    ptr   = 0;
    sLastError = "";
    sParsed = "";
}

}
