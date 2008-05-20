/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LISTHOST = 258,
     LISTDISK = 259,
     SETHOST = 260,
     SETDISK = 261,
     SETDATE = 262,
     SETTAPE = 263,
     SETMODE = 264,
     CD = 265,
     CDX = 266,
     QUIT = 267,
     DHIST = 268,
     LS = 269,
     ADD = 270,
     ADDX = 271,
     EXTRACT = 272,
     LIST = 273,
     DELETE = 274,
     DELETEX = 275,
     PWD = 276,
     CLEAR = 277,
     HELP = 278,
     LCD = 279,
     LPWD = 280,
     MODE = 281,
     SMB = 282,
     TAR = 283,
     PATH = 284,
     DATE = 285
   };
#endif
/* Tokens.  */
#define LISTHOST 258
#define LISTDISK 259
#define SETHOST 260
#define SETDISK 261
#define SETDATE 262
#define SETTAPE 263
#define SETMODE 264
#define CD 265
#define CDX 266
#define QUIT 267
#define DHIST 268
#define LS 269
#define ADD 270
#define ADDX 271
#define EXTRACT 272
#define LIST 273
#define DELETE 274
#define DELETEX 275
#define PWD 276
#define CLEAR 277
#define HELP 278
#define LCD 279
#define LPWD 280
#define MODE 281
#define SMB 282
#define TAR 283
#define PATH 284
#define DATE 285




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 42 "uparse.y"
{
	int	intval;
	double	floatval;
	char *	strval;
	int	subtok;
}
/* Line 1489 of yacc.c.  */
#line 116 "uparse.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

