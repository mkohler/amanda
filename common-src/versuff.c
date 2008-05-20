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
 * $Id: versuff.c.in,v 1.6.4.1.4.1 2001/07/20 19:37:20 jrjackson Exp $
 *
 * provide amanda version number and suffix appended to program names
 */
#include "amanda.h"
#include "version.h"

const int   VERSION_MAJOR   = 2;
const int   VERSION_MINOR   = 4;
const int   VERSION_PATCH   = 4;
const char *const VERSION_COMMENT = "p3";

char *
versionsuffix()
{
#ifdef USE_VERSION_SUFFIXES
	static char *vsuff = NULL;

	if(vsuff) return vsuff;			/* been here once already */

	vsuff = stralloc2("-", version());
	malloc_mark(vsuff);
	return vsuff;
#else
	return "";
#endif
}

char *
version()
{
	static char *vsuff = NULL;
	char major_str[NUM_STR_SIZE];
	char minor_str[NUM_STR_SIZE];
	char patch_str[NUM_STR_SIZE];

	if(vsuff) return vsuff;			/* been here once already */

	ap_snprintf(major_str, sizeof(major_str), "%d", VERSION_MAJOR);
	ap_snprintf(minor_str, sizeof(minor_str), "%d", VERSION_MINOR);
	ap_snprintf(patch_str, sizeof(patch_str), "%d", VERSION_PATCH);

	vsuff = vstralloc(major_str, ".", minor_str, ".", patch_str,
			  VERSION_COMMENT,
			  NULL);
	malloc_mark(vsuff);
	return vsuff;
}
