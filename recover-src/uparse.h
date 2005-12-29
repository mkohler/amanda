/* A Bison parser, made by GNU Bison 2.0.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LISTDISK = 258,
     SETHOST = 259,
     SETDISK = 260,
     SETDATE = 261,
     SETTAPE = 262,
     SETMODE = 263,
     CD = 264,
     CDX = 265,
     QUIT = 266,
     DHIST = 267,
     LS = 268,
     ADD = 269,
     ADDX = 270,
     EXTRACT = 271,
     LIST = 272,
     DELETE = 273,
     DELETEX = 274,
     PWD = 275,
     CLEAR = 276,
     HELP = 277,
     LCD = 278,
     LPWD = 279,
     MODE = 280,
     SMB = 281,
     TAR = 282,
     PATH = 283,
     DATE = 284
   };
#endif
#define LISTDISK 258
#define SETHOST 259
#define SETDISK 260
#define SETDATE 261
#define SETTAPE 262
#define SETMODE 263
#define CD 264
#define CDX 265
#define QUIT 266
#define DHIST 267
#define LS 268
#define ADD 269
#define ADDX 270
#define EXTRACT 271
#define LIST 272
#define DELETE 273
#define DELETEX 274
#define PWD 275
#define CLEAR 276
#define HELP 277
#define LCD 278
#define LPWD 279
#define MODE 280
#define SMB 281
#define TAR 282
#define PATH 283
#define DATE 284




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 40 "uparse.y"
typedef union YYSTYPE {
  int intval;
  double floatval;
  char *strval;
  int subtok;
} YYSTYPE;
/* Line 1318 of yacc.c.  */
#line 102 "uparse.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



