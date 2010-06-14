/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LISTHOST = 258,
     LISTDISK = 259,
     LISTPROPERTY = 260,
     SETHOST = 261,
     SETDISK = 262,
     SETDATE = 263,
     SETTAPE = 264,
     SETMODE = 265,
     SETDEVICE = 266,
     SETPROPERTY = 267,
     CD = 268,
     CDX = 269,
     QUIT = 270,
     DHIST = 271,
     LS = 272,
     ADD = 273,
     ADDX = 274,
     EXTRACT = 275,
     DASH_H = 276,
     LIST = 277,
     DELETE = 278,
     DELETEX = 279,
     PWD = 280,
     CLEAR = 281,
     HELP = 282,
     LCD = 283,
     LPWD = 284,
     MODE = 285,
     SMB = 286,
     TAR = 287,
     APPEND = 288,
     PRIORITY = 289,
     NL = 290,
     STRING = 291
   };
#endif
/* Tokens.  */
#define LISTHOST 258
#define LISTDISK 259
#define LISTPROPERTY 260
#define SETHOST 261
#define SETDISK 262
#define SETDATE 263
#define SETTAPE 264
#define SETMODE 265
#define SETDEVICE 266
#define SETPROPERTY 267
#define CD 268
#define CDX 269
#define QUIT 270
#define DHIST 271
#define LS 272
#define ADD 273
#define ADDX 274
#define EXTRACT 275
#define DASH_H 276
#define LIST 277
#define DELETE 278
#define DELETEX 279
#define PWD 280
#define CLEAR 281
#define HELP 282
#define LCD 283
#define LPWD 284
#define MODE 285
#define SMB 286
#define TAR 287
#define APPEND 288
#define PRIORITY 289
#define NL 290
#define STRING 291




/* Copy the first part of user declarations.  */
#line 31 "uparse.y"

#include "amanda.h"
#include "amrecover.h"

#define DATE_ALLOC_SIZE sizeof("YYYY-MM-DD-HH-MM-SS")   /* includes null */

void		yyerror(char *s);
extern int	yylex(void);
extern char *	yytext;


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 43 "uparse.y"
{
	int	intval;
	double	floatval;
	char *	strval;
	int	subtok;
}
/* Line 187 of yacc.c.  */
#line 186 "uparse.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 199 "uparse.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  104
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   166

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  101
/* YYNRULES -- Number of states.  */
#define YYNSTATES  160

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   291

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    30,    33,    37,    40,    44,
      47,    50,    54,    58,    61,    66,    70,    75,    77,    81,
      84,    88,    92,    98,   101,   105,   110,   116,   120,   125,
     130,   136,   139,   143,   147,   152,   156,   160,   163,   167,
     171,   174,   178,   182,   186,   190,   193,   196,   200,   203,
     207,   210,   213,   216,   219,   223,   226,   230,   233,   236,
     239,   242,   245,   248,   251,   254,   258,   261,   263,   267,
     270,   272,   276,   279,   281,   285,   288,   290,   293,   296,
     300,   304,   307,   310,   313,   316,   319,   322,   325,   327,
     330,   333
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      38,     0,    -1,    39,    -1,    40,    -1,    41,    -1,    42,
      -1,    43,    -1,    45,    -1,    47,    -1,    49,    -1,    51,
      -1,    52,    -1,    53,    -1,    54,    -1,     3,    35,    -1,
       3,    56,    -1,     4,    36,    35,    -1,     4,    35,    -1,
       4,    36,    56,    -1,     5,    35,    -1,     5,    56,    -1,
       6,    36,    35,    -1,     6,    36,    56,    -1,     6,    35,
      -1,     7,    36,    36,    35,    -1,     7,    36,    35,    -1,
       7,    36,    36,    56,    -1,     7,    -1,     9,    36,    35,
      -1,     9,    35,    -1,     9,    36,    56,    -1,    11,    36,
      35,    -1,    11,    21,    36,    36,    35,    -1,    11,    35,
      -1,    11,    36,    56,    -1,    11,    21,    36,    35,    -1,
      11,    21,    36,    36,    56,    -1,    12,    36,    55,    -1,
      12,    33,    36,    55,    -1,    12,    34,    36,    55,    -1,
      12,    33,    34,    36,    55,    -1,    12,    35,    -1,    12,
      33,    35,    -1,    12,    34,    35,    -1,    12,    33,    34,
      35,    -1,    13,    36,    35,    -1,    13,    36,    56,    -1,
      13,    35,    -1,    14,    36,    35,    -1,    14,    36,    56,
      -1,    14,    35,    -1,    10,    31,    35,    -1,    10,    32,
      35,    -1,    10,    31,    56,    -1,    10,    32,    56,    -1,
      10,    56,    -1,    10,    35,    -1,     8,    36,    35,    -1,
       8,    35,    -1,     8,    36,    56,    -1,    16,    35,    -1,
      16,    56,    -1,    17,    35,    -1,    17,    56,    -1,    22,
      36,    35,    -1,    22,    35,    -1,    22,    36,    56,    -1,
      25,    35,    -1,    25,    56,    -1,    26,    35,    -1,    26,
      56,    -1,    30,    35,    -1,    30,    56,    -1,    15,    35,
      -1,    15,    56,    -1,    18,    44,    35,    -1,    44,    36,
      -1,    36,    -1,    19,    46,    35,    -1,    46,    36,    -1,
      36,    -1,    23,    48,    35,    -1,    48,    36,    -1,    36,
      -1,    24,    50,    35,    -1,    50,    36,    -1,    36,    -1,
      29,    35,    -1,    29,    56,    -1,    28,    36,    35,    -1,
      28,    36,    56,    -1,    28,    35,    -1,    27,    35,    -1,
      27,    56,    -1,    20,    35,    -1,    20,    56,    -1,    36,
      57,    -1,    36,    55,    -1,    35,    -1,    36,    57,    -1,
      36,    57,    -1,    35,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    68,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   129,   175,   176,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   196,   197,   201,   205,   206,   210,   214,
     215,   219,   223,   224,   228,   232,   233,   237,   238,   239,
     243,   244,   248,   249,   253,   254,   258,   267,   268,   272,
     276,   277
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LISTHOST", "LISTDISK", "LISTPROPERTY",
  "SETHOST", "SETDISK", "SETDATE", "SETTAPE", "SETMODE", "SETDEVICE",
  "SETPROPERTY", "CD", "CDX", "QUIT", "DHIST", "LS", "ADD", "ADDX",
  "EXTRACT", "DASH_H", "LIST", "DELETE", "DELETEX", "PWD", "CLEAR", "HELP",
  "LCD", "LPWD", "MODE", "SMB", "TAR", "APPEND", "PRIORITY", "NL",
  "STRING", "$accept", "ucommand", "set_command", "setdate_command",
  "display_command", "quit_command", "add_command", "add_path",
  "addx_command", "addx_path", "delete_command", "delete_path",
  "deletex_command", "deletex_path", "local_command", "help_command",
  "extract_command", "invalid_command", "property_value", "invalid_string",
  "bogus_string", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    37,    38,    38,    38,    38,    38,    38,    38,    38,
      38,    38,    38,    38,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39,    40,    40,    40,
      41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
      41,    41,    41,    42,    42,    43,    44,    44,    45,    46,
      46,    47,    48,    48,    49,    50,    50,    51,    51,    51,
      51,    51,    52,    52,    53,    53,    54,    55,    55,    56,
      57,    57
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     3,     2,     3,     2,
       2,     3,     3,     2,     4,     3,     4,     1,     3,     2,
       3,     3,     5,     2,     3,     4,     5,     3,     4,     4,
       5,     2,     3,     3,     4,     3,     3,     2,     3,     3,
       2,     3,     3,     3,     3,     2,     2,     3,     2,     3,
       2,     2,     2,     2,     3,     2,     3,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     2,     1,     3,     2,
       1,     3,     2,     1,     3,     2,     1,     2,     2,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     1,     2,
       2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,    27,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,     0,    15,    17,     0,    19,    20,    23,
       0,     0,    58,     0,    29,     0,     0,     0,    56,    55,
       0,    33,     0,     0,     0,    41,     0,    47,     0,    50,
       0,    73,    74,    60,    61,    62,    63,    77,     0,    80,
       0,    94,    95,    65,     0,    83,     0,    86,     0,    67,
      68,    69,    70,    92,    93,    91,     0,    87,    88,    71,
      72,   101,     0,    96,     1,    99,    16,    18,    21,    22,
      25,     0,    57,    59,    28,    30,    51,    53,    52,    54,
       0,    31,    34,     0,    42,     0,    43,     0,    98,     0,
      37,    45,    46,    48,    49,    75,    76,    78,    79,    64,
      66,    81,    82,    84,    85,    89,    90,   100,    24,    26,
      35,     0,    44,     0,    38,    39,    97,    32,    36,    40
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    29,    30,    31,    32,    33,    34,    78,    35,    80,
      36,    86,    37,    88,    38,    39,    40,    41,   130,    44,
     103
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -124
static const yytype_int16 yypact[] =
{
     106,   -22,   -10,     2,     4,   -20,     6,     9,    -4,   -18,
       0,    13,    20,    22,    26,    28,   -14,    -7,    33,    35,
      10,    15,    37,    39,    41,    43,    47,    49,    51,    66,
    -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,
    -124,  -124,  -124,    51,  -124,  -124,    53,  -124,  -124,  -124,
      55,    59,  -124,    61,  -124,    63,    65,    67,  -124,  -124,
      44,  -124,    69,   -27,    71,  -124,   102,  -124,   104,  -124,
     108,  -124,  -124,  -124,  -124,  -124,  -124,  -124,   110,  -124,
     114,  -124,  -124,  -124,   116,  -124,   118,  -124,   120,  -124,
    -124,  -124,  -124,  -124,  -124,  -124,   122,  -124,  -124,  -124,
    -124,  -124,    51,  -124,  -124,  -124,  -124,  -124,  -124,  -124,
    -124,   124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,
     126,  -124,  -124,   128,  -124,   102,  -124,   102,  -124,   102,
    -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,
    -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,
    -124,   130,  -124,   102,  -124,  -124,  -124,  -124,  -124,  -124
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,
    -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -123,    -3,
     -42
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      48,   105,   154,    60,   155,    59,   156,   123,   124,   125,
      72,    74,    76,    42,    43,    82,    51,    61,    62,    90,
      92,    94,    77,    98,   100,    45,    46,    56,    57,    79,
     159,    58,    43,    63,    64,    65,    66,    47,    43,    49,
      50,    52,    53,   107,    54,    55,    85,   109,    67,    68,
     113,    87,   115,   117,   119,    69,    70,    71,    43,   122,
     147,    73,    43,    75,    43,   132,   104,   134,    81,    43,
      83,    84,    89,    43,    91,    43,    93,    43,    95,    96,
     120,   140,    97,    43,    99,    43,   101,   102,   106,    43,
     108,    43,     0,   146,   110,   111,   112,    43,   114,    43,
     116,    43,   118,    43,   121,    43,   126,   127,   149,     1,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,   128,   129,   131,
      43,     0,    28,   133,    43,   135,   136,     0,   158,   137,
     138,   139,    43,   141,   142,   143,   144,   145,    43,   148,
      43,   150,   151,   152,   153,   157,    43
};

static const yytype_int16 yycheck[] =
{
       3,    43,   125,    21,   127,     8,   129,    34,    35,    36,
      13,    14,    15,    35,    36,    18,    36,    35,    36,    22,
      23,    24,    36,    26,    27,    35,    36,    31,    32,    36,
     153,    35,    36,    33,    34,    35,    36,    35,    36,    35,
      36,    35,    36,    46,    35,    36,    36,    50,    35,    36,
      53,    36,    55,    56,    57,    35,    36,    35,    36,    62,
     102,    35,    36,    35,    36,    68,     0,    70,    35,    36,
      35,    36,    35,    36,    35,    36,    35,    36,    35,    36,
      36,    84,    35,    36,    35,    36,    35,    36,    35,    36,
      35,    36,    -1,    96,    35,    36,    35,    36,    35,    36,
      35,    36,    35,    36,    35,    36,    35,    36,   111,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    35,    36,    35,
      36,    -1,    36,    35,    36,    35,    36,    -1,   151,    35,
      36,    35,    36,    35,    36,    35,    36,    35,    36,    35,
      36,    35,    36,    35,    36,    35,    36
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    36,    38,
      39,    40,    41,    42,    43,    45,    47,    49,    51,    52,
      53,    54,    35,    36,    56,    35,    36,    35,    56,    35,
      36,    36,    35,    36,    35,    36,    31,    32,    35,    56,
      21,    35,    36,    33,    34,    35,    36,    35,    36,    35,
      36,    35,    56,    35,    56,    35,    56,    36,    44,    36,
      46,    35,    56,    35,    36,    36,    48,    36,    50,    35,
      56,    35,    56,    35,    56,    35,    36,    35,    56,    35,
      56,    35,    36,    57,     0,    57,    35,    56,    35,    56,
      35,    36,    35,    56,    35,    56,    35,    56,    35,    56,
      36,    35,    56,    34,    35,    36,    35,    36,    35,    36,
      55,    35,    56,    35,    56,    35,    36,    35,    36,    35,
      56,    35,    36,    35,    36,    35,    56,    57,    35,    56,
      35,    36,    35,    36,    55,    55,    55,    35,    56,    55
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 14:
#line 83 "uparse.y"
    { list_host(); }
    break;

  case 15:
#line 84 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 16:
#line 85 "uparse.y"
    { list_disk((yyvsp[(2) - (3)].strval)); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 17:
#line 86 "uparse.y"
    { list_disk(NULL); }
    break;

  case 18:
#line 87 "uparse.y"
    { yyerror("Invalid argument"); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 19:
#line 88 "uparse.y"
    { list_property(); }
    break;

  case 20:
#line 89 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 21:
#line 90 "uparse.y"
    { set_host((yyvsp[(2) - (3)].strval)); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 22:
#line 91 "uparse.y"
    { yyerror("Invalid argument"); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 23:
#line 92 "uparse.y"
    { yyerror("Argument required"); }
    break;

  case 24:
#line 93 "uparse.y"
    { set_disk((yyvsp[(2) - (4)].strval), (yyvsp[(3) - (4)].strval)); amfree((yyvsp[(2) - (4)].strval)); amfree((yyvsp[(3) - (4)].strval)); }
    break;

  case 25:
#line 94 "uparse.y"
    { set_disk((yyvsp[(2) - (3)].strval), NULL); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 26:
#line 95 "uparse.y"
    { yyerror("Invalid argument"); amfree((yyvsp[(2) - (4)].strval)); amfree((yyvsp[(3) - (4)].strval)); }
    break;

  case 27:
#line 96 "uparse.y"
    { yyerror("Argument required"); }
    break;

  case 28:
#line 97 "uparse.y"
    { set_tape((yyvsp[(2) - (3)].strval)); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 29:
#line 98 "uparse.y"
    { set_tape("default"); }
    break;

  case 30:
#line 99 "uparse.y"
    { yyerror("Invalid argument"); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 31:
#line 100 "uparse.y"
    { set_device(NULL, (yyvsp[(2) - (3)].strval)); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 32:
#line 101 "uparse.y"
    { set_device((yyvsp[(3) - (5)].strval), (yyvsp[(4) - (5)].strval)); amfree((yyvsp[(3) - (5)].strval)); amfree((yyvsp[(4) - (5)].strval));  }
    break;

  case 33:
#line 102 "uparse.y"
    { set_device(NULL, NULL); }
    break;

  case 34:
#line 103 "uparse.y"
    { yyerror("Invalid argument"); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 35:
#line 104 "uparse.y"
    { yyerror("Invalid argument"); amfree((yyvsp[(3) - (4)].strval)); }
    break;

  case 36:
#line 105 "uparse.y"
    { yyerror("Invalid argument"); amfree((yyvsp[(3) - (5)].strval)); amfree((yyvsp[(4) - (5)].strval)); }
    break;

  case 37:
#line 106 "uparse.y"
    { set_property_name((yyvsp[(2) - (3)].strval), 0); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 38:
#line 107 "uparse.y"
    { set_property_name((yyvsp[(3) - (4)].strval), 1); amfree((yyvsp[(3) - (4)].strval)); }
    break;

  case 39:
#line 108 "uparse.y"
    { set_property_name((yyvsp[(3) - (4)].strval), 0); amfree((yyvsp[(3) - (4)].strval)); }
    break;

  case 40:
#line 109 "uparse.y"
    { set_property_name((yyvsp[(4) - (5)].strval), 1); amfree((yyvsp[(4) - (5)].strval)); }
    break;

  case 41:
#line 110 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 42:
#line 111 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 43:
#line 112 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 44:
#line 113 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 45:
#line 114 "uparse.y"
    { cd_glob((yyvsp[(2) - (3)].strval), 1); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 46:
#line 115 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 47:
#line 116 "uparse.y"
    { yyerror("Argument required"); }
    break;

  case 48:
#line 117 "uparse.y"
    { cd_regex((yyvsp[(2) - (3)].strval), 1); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 49:
#line 118 "uparse.y"
    { yyerror("Invalid argument"); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 50:
#line 119 "uparse.y"
    { yyerror("Argument required"); }
    break;

  case 51:
#line 120 "uparse.y"
    { set_mode(SAMBA_SMBCLIENT); }
    break;

  case 52:
#line 121 "uparse.y"
    { set_mode(SAMBA_TAR); }
    break;

  case 53:
#line 122 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 54:
#line 123 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 55:
#line 124 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 56:
#line 125 "uparse.y"
    { yyerror("Argument required"); }
    break;

  case 57:
#line 129 "uparse.y"
    {
			time_t now;
			struct tm *t;
			int y=2000, m=0, d=1, h=0, mi=0, s=0;
			int ret;
			char *mydate = (yyvsp[(2) - (3)].strval);

			now = time((time_t *)NULL);
			t = localtime(&now);
			if (t) {
			    y = 1900+t->tm_year;
			    m = t->tm_mon+1;
			    d = t->tm_mday;
			}
			if (sscanf(mydate, "---%d", &d) == 1 ||
			    sscanf(mydate, "--%d-%d", &m, &d) == 2 ||
			    sscanf(mydate, "%d-%d-%d-%d-%d-%d", &y, &m, &d, &h, &mi, &s) >= 3) {
			    if (y < 70) {
				y += 2000;
			    } else if (y < 100) {
				y += 1900;
			    }
			    if(y < 1000 || y > 9999) {
				printf("invalid year");
			    } else if(m < 1 || m > 12) {
				printf("invalid month");
			    } else if(d < 1 || d > 31) {
				printf("invalid day");
			    } else if(h < 0 || h > 24) {
				printf("invalid hour");
			    } else if(mi < 0 || mi > 59) {
				printf("invalid minute");
			    } else if(s < 0 || s > 59) {
				printf("invalid second");
			    } else {
				char result[DATE_ALLOC_SIZE];
				if (h == 0 && mi == 0 && s == 0)
				    g_snprintf(result, DATE_ALLOC_SIZE, "%04d-%02d-%02d", y, m, d);
				else
				    g_snprintf(result, DATE_ALLOC_SIZE, "%04d-%02d-%02d-%02d-%02d-%02d", y, m, d, h, mi, s);
				set_date(result);
			    }
			} else {
			    printf("Invalid date: %s\n", mydate);
			}
		     }
    break;

  case 58:
#line 175 "uparse.y"
    { yyerror("Argument required"); }
    break;

  case 59:
#line 176 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 60:
#line 180 "uparse.y"
    { list_disk_history(); }
    break;

  case 61:
#line 181 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 62:
#line 182 "uparse.y"
    { list_directory(); }
    break;

  case 63:
#line 183 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 64:
#line 184 "uparse.y"
    { display_extract_list((yyvsp[(2) - (3)].strval)); amfree((yyvsp[(2) - (3)].strval)); }
    break;

  case 65:
#line 185 "uparse.y"
    { display_extract_list(NULL); }
    break;

  case 66:
#line 186 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 67:
#line 187 "uparse.y"
    { show_directory(); }
    break;

  case 68:
#line 188 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 69:
#line 189 "uparse.y"
    { clear_extract_list(); }
    break;

  case 70:
#line 190 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 71:
#line 191 "uparse.y"
    { show_mode (); }
    break;

  case 72:
#line 192 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 73:
#line 196 "uparse.y"
    { quit(); }
    break;

  case 74:
#line 197 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 76:
#line 205 "uparse.y"
    { add_glob((yyvsp[(2) - (2)].strval)); amfree((yyvsp[(2) - (2)].strval)); }
    break;

  case 77:
#line 206 "uparse.y"
    { add_glob((yyvsp[(1) - (1)].strval)); amfree((yyvsp[(1) - (1)].strval)); }
    break;

  case 79:
#line 214 "uparse.y"
    { add_regex((yyvsp[(2) - (2)].strval)); amfree((yyvsp[(2) - (2)].strval)); }
    break;

  case 80:
#line 215 "uparse.y"
    { add_regex((yyvsp[(1) - (1)].strval)); amfree((yyvsp[(1) - (1)].strval)); }
    break;

  case 82:
#line 223 "uparse.y"
    { delete_glob((yyvsp[(2) - (2)].strval)); amfree((yyvsp[(2) - (2)].strval)); }
    break;

  case 83:
#line 224 "uparse.y"
    { delete_glob((yyvsp[(1) - (1)].strval)); amfree((yyvsp[(1) - (1)].strval)); }
    break;

  case 85:
#line 232 "uparse.y"
    { delete_regex((yyvsp[(2) - (2)].strval)); amfree((yyvsp[(2) - (2)].strval)); }
    break;

  case 86:
#line 233 "uparse.y"
    { delete_regex((yyvsp[(1) - (1)].strval)); amfree((yyvsp[(1) - (1)].strval)); }
    break;

  case 87:
#line 237 "uparse.y"
    { char * buf= g_get_current_dir(); puts(buf); free(buf); }
    break;

  case 88:
#line 238 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 89:
#line 239 "uparse.y"
    {
		local_cd((yyvsp[(2) - (3)].strval));
		amfree((yyvsp[(2) - (3)].strval));
	}
    break;

  case 90:
#line 243 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 91:
#line 244 "uparse.y"
    { yyerror("Argument required"); }
    break;

  case 92:
#line 248 "uparse.y"
    { help_list(); }
    break;

  case 93:
#line 249 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 94:
#line 253 "uparse.y"
    { extract_files(); }
    break;

  case 95:
#line 254 "uparse.y"
    { yyerror("Invalid argument"); }
    break;

  case 96:
#line 258 "uparse.y"
    {
	    char * errstr = vstralloc("Invalid command: ", (yyvsp[(1) - (2)].strval), NULL);
	    yyerror(errstr);
	    amfree(errstr);
	    YYERROR;
	}
    break;

  case 97:
#line 267 "uparse.y"
    { add_property_value((yyvsp[(1) - (2)].strval)); amfree( (yyvsp[(1) - (2)].strval)); }
    break;

  case 98:
#line 268 "uparse.y"
    { ; }
    break;

  case 99:
#line 272 "uparse.y"
    { amfree((yyvsp[(1) - (2)].strval)); }
    break;

  case 100:
#line 276 "uparse.y"
    { amfree((yyvsp[(1) - (2)].strval)); }
    break;

  case 101:
#line 277 "uparse.y"
    { ; }
    break;


/* Line 1267 of yacc.c.  */
#line 2021 "uparse.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 280 "uparse.y"


void
yyerror(
    char *	s)
{
	g_printf("%s\n", s);
}

