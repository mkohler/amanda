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
 * Author: James da Silva, Systems Design and Analysis Group
 *			   Computer Science Department
 *			   University of Maryland at College Park
 */
/*
 * $Id: dgram.h,v 1.6.2.2.4.1.2.1 2002/03/24 19:23:23 jrjackson Exp $
 *
 * interface for datagram module
 */
#ifndef DGRAM_H
#define DGRAM_H

#include "amanda.h"

/*
 * Maximum datagram (UDP packet) we can generate.  Size is limited by
 * a 16 bit length field in an IPv4 header (65535), which must include
 * the 24 byte IP header and the 8 byte UDP header.
 */
#define MAX_DGRAM      (((1<<16)-1)-24-8)

typedef struct dgram_s {
    char *cur;
    int socket;
    int len;
    char data[MAX_DGRAM+1];
} dgram_t;

int dgram_bind P((dgram_t *dgram, int *portp));
void dgram_socket P((dgram_t *dgram, int sock));
int dgram_send P((char *hostname, int port, dgram_t *dgram));
int dgram_send_addr P((struct sockaddr_in addr, dgram_t *dgram));
int dgram_recv P((dgram_t *dgram, int timeout, struct sockaddr_in *fromaddr));
void dgram_zero P((dgram_t *dgram));
void dgram_cat P((dgram_t *dgram, const char *str));
void dgram_eatline P((dgram_t *dgram));

extern dgram_t  *debug_dgram_alloc  P((char *c, int l));

#define	dgram_alloc()		debug_dgram_alloc(__FILE__, __LINE__)

#endif /* ! DGRAM_H */
