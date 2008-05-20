/*
 * Amanda, The Advanced Maryland Automatic Network Disk Archiver
 * Copyright (c) 1991-1998 University of Maryland at College Park
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
 * $Id: client_util.h,v 1.1.2.10 2002/04/22 01:54:34 martinea Exp $
 *
 */

#ifndef CLIENT_UTIL_H
#define CLIENT_UTIL_H

#include "amanda.h"
#include "amfeatures.h"
#include "sl.h"

typedef struct option_s {
    char *str;
    int compress;
    int no_record;
    int bsd_auth;
    int createindex;
    sl_t *exclude_file;
    sl_t *exclude_list;
    sl_t *include_file;
    sl_t *include_list;
    int exclude_optional;
    int include_optional;
#ifdef KRB4_SECURITY
    int krb4_auth;
    int kencrypt;
#endif
} option_t;

typedef struct g_option_s {
    char *str;
    am_feature_t *features;
    char *hostname;
    int maxdumps;
} g_option_t;

#define NO_COMPR   0
#define COMPR_FAST 1
#define COMPR_BEST 2
#define COMPR_SERVER_FAST 3
#define COMPR_SERVER_BEST 4

char *build_exclude P((char *disk, char *device, option_t *options, int verbose));
char *build_include P((char *disk, char *device, option_t *options, int verbose));
void init_options P((option_t *options));
option_t *parse_options P((char *str,
			   char *disk,
			   char *device,
			   am_feature_t *fs,
			   int verbose));

void init_g_options P((g_option_t *g_options));
g_option_t *parse_g_options P((char *str, int verbose));

#endif
