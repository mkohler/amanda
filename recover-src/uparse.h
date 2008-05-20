#ifndef BISON_UPARSE_H
# define BISON_UPARSE_H

#ifndef YYSTYPE
typedef union {
  int intval;
  double floatval;
  char *strval;
  int subtok;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	LISTDISK	257
# define	SETHOST	258
# define	SETDISK	259
# define	SETDATE	260
# define	SETTAPE	261
# define	SETMODE	262
# define	CD	263
# define	CDX	264
# define	QUIT	265
# define	DHIST	266
# define	LS	267
# define	ADD	268
# define	ADDX	269
# define	EXTRACT	270
# define	LIST	271
# define	DELETE	272
# define	DELETEX	273
# define	PWD	274
# define	CLEAR	275
# define	HELP	276
# define	LCD	277
# define	LPWD	278
# define	MODE	279
# define	SMB	280
# define	TAR	281
# define	PATH	282
# define	DATE	283


extern YYSTYPE yylval;

#endif /* not BISON_UPARSE_H */
