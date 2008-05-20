/*
 * Amanda, The Advanced Maryland Automatic Network Disk Archiver
 * Copyright (c) 1991-1999 University of Maryland at College Park
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
 * $Id: noop.c,v 1.3 2006/01/14 04:37:18 paddy_s Exp $
 *
 * send back features.  This was pulled out to it's own program for
 * consistancy and because it's a hell of a lot easier to code in
 * a fork()-less environment.
 */

#include "amanda.h"
#include "amfeatures.h"
#include "util.h"

int main P((int argc, char **argv));

int
main(argc, argv)
    int argc;
    char **argv;
{
    char ch;
    am_feature_t *our_features = NULL;
    char *our_feature_string = NULL;
    char *options;
    int n;

    /* Don't die when child closes pipe */
    signal(SIGPIPE, SIG_IGN);

    do {
 	/* soak up any stdin */
	n = read(0, &ch, 1);
    } while ((n > 0) || ((n < 0) && ((errno == EINTR) || (errno == EAGAIN))));
    our_features = am_init_feature_set();
    our_feature_string = am_feature_to_string(our_features);
    options = vstralloc("OPTIONS features=",
			our_feature_string,
			";\n",
			NULL);
    amfree(our_feature_string);
    am_release_feature_set(our_features);
    our_features = NULL;
    if (fullwrite(1, options, strlen(options)) < 0) {
	error("error sending noop response: %s", strerror(errno));
    }
    amfree(options);
    exit(0);
}
