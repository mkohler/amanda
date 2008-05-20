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
 * $Id: amandates.h,v 1.3 1998/07/04 00:18:10 oliva Exp $
 *
 * interface for amandates file
 */
#ifndef AMANDATES_H
#define AMANDATES_H

#include "amanda.h"

#define DUMP_LEVELS	10	/* XXX should be in amanda.h */
#define EPOCH		((time_t)0)

#ifndef AMANDATES_FILE
#define AMANDATES_FILE "/etc/amandates"
#endif

typedef struct amandates_s {
    struct amandates_s *next;
    char *name;				/* filesystem name */
    time_t dates[DUMP_LEVELS];		/* dump dates */
} amandates_t;

int  start_amandates P((int open_readwrite));
void finish_amandates P((void));
void free_amandates P((void));
amandates_t *amandates_lookup P((char *name));
void amandates_updateone P((char *name, int level, time_t dumpdate));

#endif /* ! AMANDATES_H */
