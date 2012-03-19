/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
     SETTRANSLATE = 290,
     NL = 291,
     STRING = 292
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
#define SETTRANSLATE 290
#define NL 291
#define STRING 292




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 43 "uparse.y"

	int	intval;
	double	floatval;
	char *	strval;
	int	subtok;



/* Line 2068 of yacc.c  */
#line 133 "uparse.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


