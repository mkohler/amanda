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
 * $Id: infofile.h,v 1.7.4.4.8.2 2005/03/16 18:15:28 martinea Exp $
 *
 * interface for current info file reading code
 */
#ifndef INFOFILE_H
#define INFOFILE_H

#include "amanda.h"

#define DUMP_LEVELS	10
#define MAX_LABEL	80
#define EPOCH		((time_t)0)

#define AVG_COUNT	3
#define NB_HISTORY	100
#define newperf(ary,f)	( ary[2]=ary[1], ary[1]=ary[0], ary[0]=(f) )

typedef struct stats_s {
    /* fields updated by dumper */
    long size;			/* original size of dump in kbytes */
    long csize;			/* compressed size of dump in kbytes */
    long secs;			/* time of dump in secs */
    time_t date;		/* end time of dump */
    /* fields updated by taper */
    int filenum;		/* file number on tape */
    char label[MAX_LABEL];	/* tape label */
} stats_t;

typedef struct history_s {
    int level;			/* level of dump */
    long size;			/* original size of dump in kbytes */
    long csize;			/* compressed size of dump in kbytes */
    time_t date;		/* time of dump */
    long secs;			/* time of dump in secs */
} history_t;

typedef struct perf_s {
    float rate[AVG_COUNT];
    float comp[AVG_COUNT];
} perf_t;

typedef struct info_s {
    unsigned int  command;		/* command word */
#	define NO_COMMAND	0	/* no outstanding commands */
#	define FORCE_FULL	1	/* force level 0 at next run */
#	define FORCE_BUMP	2	/* force bump at next run */
#	define FORCE_NO_BUMP	4	/* force no-bump at next run */
    perf_t  full;
    perf_t  incr;
    stats_t inf[DUMP_LEVELS];
    int last_level, consecutive_runs;
    history_t history[NB_HISTORY+1];
} info_t;


int open_infofile P((char *infofile));
void close_infofile P((void));

char *get_dumpdate P((info_t *info, int level));
double perf_average P((float *array, double def));
int get_info P((char *hostname, char *diskname, info_t *info));
int get_firstkey P((char *hostname, int hostname_size,
		    char *diskname, int diskname_size));
int get_nextkey  P((char *hostname, int hostname_size,
		    char *diskname, int diskname_size));
int put_info P((char *hostname, char *diskname, info_t *info));
int del_info P((char *hostname, char *diskname));

#endif /* ! INFOFILE_H */
