/*
 * Amanda, The Advanced Maryland Automatic Network Disk Archiver
 * Copyright (c) 1997-1998 University of Maryland at College Park
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of U.M. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  U.M. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * U.M. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL U.M.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors: the Amanda Development Team.  Its members are listed in a
 * file named AUTHORS, in the root directory of this distribution.
 */
/*
 * $Id: token.h,v 1.12 2003/03/26 20:17:13 kovert Exp $
 *
 * interface to token module
 */
#ifndef TOKEN_H
#define TOKEN_H

#include "amanda.h"

typedef struct {char *word; int value;} table_t;

extern int split P((char *str, char **token, int toklen, char *sep));
extern char *squotef P((char *format, ...))
    __attribute__ ((format (printf, 1, 2)));
extern char *squote P((char *str));
extern char *quotef P((char *sep, char *format, ...))
    __attribute__ ((format (printf, 2, 3)));
extern char *quote P((char *sep, char *str));
extern char *rxquote P((char *str));
#ifndef HAVE_SHQUOTE
extern char *shquote P((char *str));
#endif
extern int table_lookup P((table_t *table, char *str));
extern char *table_lookup_r P((table_t *table, int val));

#endif
