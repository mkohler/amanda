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
 * $Id: diskfile.c,v 1.27.4.6.4.3.2.15.2.8 2005/09/30 19:13:36 martinea Exp $
 *
 * read disklist file
 */
#include "amanda.h"
#include "arglist.h"
#include "conffile.h"
#include "diskfile.h"


static disklist_t lst;
static FILE *diskf;
static char *diskfname = NULL;
static am_host_t *hostlist;
static int line_num, got_parserror;

/* local functions */
static char *upcase P((char *st));
static int read_diskline P((void));
static void parserror P((char *format, ...))
    __attribute__ ((format (printf, 1, 2)));


disklist_t *read_diskfile(filename)
char *filename;
{
    extern int errno;

    /* initialize */

    hostlist = NULL;
    lst.head = lst.tail = NULL;
    diskfname = newstralloc(diskfname, filename);
    malloc_mark(diskfname);
    line_num = got_parserror = 0;

    if((diskf = fopen(filename, "r")) == NULL)
	error("could not open disklist file \"%s\": %s",
	      filename, strerror(errno));

    while(read_diskline());
    afclose(diskf);

    if(got_parserror) return NULL;
    else return &lst;
}

am_host_t *lookup_host(hostname)
char *hostname;
{
    am_host_t *p;

    for(p = hostlist; p != NULL; p = p->next) {
	if(strcasecmp(p->hostname, hostname) == 0) return p;
    }
    return NULL;
}

disk_t *lookup_disk(hostname, diskname)
char *hostname, *diskname;
{
    am_host_t *host;
    disk_t *disk;

    host = lookup_host(hostname);
    if(host == NULL) return NULL;

    for(disk = host->disks; disk != NULL; disk = disk->hostnext) {
	if(strcmp(disk->name, diskname) == 0) return disk;
    }
    return NULL;
}

void enqueue_disk(list, disk)	/* put disk on end of queue */
disklist_t *list;
disk_t *disk;
{
    if(list->tail == NULL) list->head = disk;
    else list->tail->next = disk;
    disk->prev = list->tail;

    list->tail = disk;
    disk->next = NULL;
}

void headqueue_disk(list, disk)	/* put disk on head of queue */
disklist_t *list;
disk_t *disk;
{
    if(list->head == NULL) list->tail = disk;
    else list->head->prev = disk;
    disk->next = list->head;

    list->head = disk;
    disk->prev = NULL;
}

void insert_disk(list, disk, cmp)	/* insert in sorted order */
disklist_t *list;
disk_t *disk;
int (*cmp) P((disk_t *a, disk_t *b));
{
    disk_t *prev, *ptr;

    prev = NULL;
    ptr = list->head;

    while(ptr != NULL) {
	if(cmp(disk, ptr) < 0) break;
	prev = ptr;
	ptr = ptr->next;
    }
    disk->next = ptr;
    disk->prev = prev;

    if(prev == NULL) list->head = disk;
    else prev->next = disk;
    if(ptr == NULL) list->tail = disk;
    else ptr->prev = disk;
}

disk_t *add_disk(hostname, diskname)
char *hostname;
char *diskname;
{
    disk_t *disk;
    am_host_t *host;

    disk = alloc(sizeof(disk_t));
    disk->line = 0;
    disk->name = stralloc(diskname);
    disk->device = stralloc(diskname);
    disk->spindle = -1;
    disk->up = NULL;
    disk->compress = COMP_NONE;
    disk->start_t = 0;
    disk->todo = 1;

    host = lookup_host(hostname);
    if(host == NULL) {
	host = alloc(sizeof(am_host_t));
	host->next = hostlist;
	hostlist = host;

	host->hostname = stralloc(hostname);
	host->disks = NULL;
	host->inprogress = 0;
	host->maxdumps = 1;
	host->netif = NULL;
	host->start_t = 0;
	host->up = NULL;
	host->features = NULL;
    }
    enqueue_disk(&lst, disk);

    disk->host = host;
    disk->hostnext = host->disks;
    host->disks = disk;

    return disk;
}

int find_disk(list, disk)
disklist_t *list;
disk_t *disk;
/* check if disk is present in list. Return true if so, false otherwise. */
{
disk_t *t;

    for( t = list->head; t && t != disk; t = t->next );

    return t == disk;
}

void sort_disk(in, out, cmp)	/* sort a whole queue */
disklist_t *in;
disklist_t *out;
int (*cmp) P((disk_t *a, disk_t *b));
{
    disklist_t *tmp;
    disk_t *disk;

    tmp = in;		/* just in case in == out */

    out->head = (disk_t *)0;
    out->tail = (disk_t *)0;

    while((disk = dequeue_disk(tmp)))
	insert_disk(out, disk, cmp);
}

disk_t *dequeue_disk(list)	/* remove disk from front of queue */
disklist_t *list;
{
    disk_t *disk;

    if(list->head == NULL) return NULL;

    disk = list->head;
    list->head = disk->next;

    if(list->head == NULL) list->tail = NULL;
    else list->head->prev = NULL;

    disk->prev = disk->next = NULL;	/* for debugging */
    return disk;
}

void remove_disk(list, disk)
disklist_t *list;
disk_t *disk;
{
    if(disk->prev == NULL) list->head = disk->next;
    else disk->prev->next = disk->next;

    if(disk->next == NULL) list->tail = disk->prev;
    else disk->next->prev = disk->prev;

    disk->prev = disk->next = NULL;
}

static char *upcase(st)
char *st;
{
    char *s = st;

    while(*s) {
	if(islower((int)*s)) *s = toupper(*s);
	s++;
    }
    return st;
}


static int read_diskline()
{
    am_host_t *host;
    disk_t *disk;
    dumptype_t *dtype;
    interface_t *netif = 0;
    char *hostname = NULL;
    char *diskname, *diskdevice;
    static char *line = NULL;
    char *s = NULL, *fp;
    int ch = '\0', dup = 0;
    char *dn;

    amfree(line);
    for(; (line = agets(diskf)) != NULL; free(line)) {
	line_num += 1;
	s = line;
	ch = *s++;

	skip_whitespace(s, ch);
	if(ch != '\0' && ch != '#') break;
    }
    if(line == NULL) return 0;

    fp = s - 1;
    skip_non_whitespace(s, ch);
    s[-1] = '\0';
    host = lookup_host(fp);
    if (host == NULL) {
      hostname = stralloc(fp);
      malloc_mark(hostname);
    } else {
      hostname = host->hostname;
    }

    skip_whitespace(s, ch);
    if(ch == '\0' || ch == '#') {
	parserror("disk device name expected");
	return 1;
    }
    fp = s - 1;
    skip_non_whitespace(s, ch);
    s[-1] = '\0';
    diskname = stralloc(fp);

    skip_whitespace(s, ch);
    if(ch == '\0' || ch == '#') {
	parserror("disk dumptype expected");
	if(host == NULL) amfree(hostname);
	amfree(diskname);
	return 1;
    }
    fp = s - 1;
    skip_non_whitespace(s, ch);
    s[-1] = '\0';

    /* diskdevice */
    dn = stralloc(fp);
    if(fp[0] != '{' && (dtype = lookup_dumptype(upcase(fp))) == NULL) {
	diskdevice = dn;
	skip_whitespace(s, ch);
	if(ch == '\0' || ch == '#') {
	    parserror("disk dumptype expected");
	    if(host == NULL) amfree(hostname);
	    amfree(diskname);
	    amfree(diskdevice);
	    return 1;
	}
	fp = s - 1;
	skip_non_whitespace(s, ch);
	s[-1] = '\0';
    }
    else {
	diskdevice = NULL;
	amfree(dn);
    }

    /* check for duplicate disk */
    if(host && (disk = lookup_disk(hostname, diskname)) != NULL) {
	parserror("duplicate disk record, previous on line %d", disk->line);
	dup = 1;
    } else {
	disk = alloc(sizeof(disk_t));
	malloc_mark(disk);
	disk->line = line_num;
	disk->name = diskname;
	disk->device = diskdevice;
	malloc_mark(disk->name);
	disk->spindle = -1;
	disk->up = NULL;
	disk->inprogress = 0;
    }

    if (fp[0] == '{') {
	s[-1] = ch;
	s = fp+2;
	skip_whitespace(s, ch);
	if (ch != '\0' && ch != '#') {
	    parserror("expected line break after `{\', ignoring rest of line");
	}

	if (strchr(s-1, '}') &&
	    (strchr(s-1, '#') == NULL ||
	     strchr(s-1, '}') < strchr(s-1, '#'))) {
	    if(host == NULL) amfree(hostname);
	    if(!dup) {
		amfree(disk->name);
		amfree(disk);
	    }
	    return 1;
	}
	amfree(line);

	dtype = read_dumptype(vstralloc("custom(", hostname,
					":", disk->name, ")", 0),
			      diskf, diskfname, &line_num);

	line = agets(diskf);
	/* line_num += 1; */ /* read_dumptype did it already */

	if (dtype == NULL || dup) {
	    if(host == NULL) amfree(hostname);
	    if(!dup) {
	      amfree(disk->name);
	      amfree(disk);
	    }
	    return line != NULL;
	}

	if (line == NULL)
	    line = stralloc("");
	s = line;
	ch = *s++;
    } else if((dtype = lookup_dumptype(upcase(fp))) == NULL) {
	parserror("undefined dumptype `%s'", fp);
	if(host == NULL) amfree(hostname);
	if(!dup) {
	  amfree(disk->name);
	  amfree(disk);
	}
	return 1;
    }

    if (dup) {
	if(host == NULL) amfree(hostname);
	return 1;
    }

    disk->dtype_name	= dtype->name;
    disk->program	= dtype->program;
    disk->exclude_file	= duplicate_sl(dtype->exclude_file);
    disk->exclude_list	= duplicate_sl(dtype->exclude_list);
    disk->include_file	= duplicate_sl(dtype->include_file);
    disk->include_list	= duplicate_sl(dtype->include_list);
    disk->exclude_optional = dtype->exclude_optional;
    disk->include_optional = dtype->include_optional;
    disk->priority	= dtype->priority;
    disk->dumpcycle	= dtype->dumpcycle;
    disk->frequency	= dtype->frequency;
    disk->auth		= dtype->auth;
    disk->maxdumps	= dtype->maxdumps;
    disk->maxpromoteday	= dtype->maxpromoteday;
    disk->bumppercent	= dtype->bumppercent;
    disk->bumpsize	= dtype->bumpsize;
    disk->bumpdays	= dtype->bumpdays;
    disk->bumpmult	= dtype->bumpmult;
    disk->start_t	= dtype->start_t;
    disk->strategy	= dtype->strategy;
    disk->estimate	= dtype->estimate;
    disk->compress	= dtype->compress;
    disk->comprate[0]	= dtype->comprate[0];
    disk->comprate[1]	= dtype->comprate[1];
    disk->record	= dtype->record;
    disk->skip_incr	= dtype->skip_incr;
    disk->skip_full	= dtype->skip_full;
    disk->no_hold	= dtype->no_hold;
    disk->kencrypt	= dtype->kencrypt;
    disk->index		= dtype->index;
    disk->todo		= 1;

    skip_whitespace(s, ch);
    fp = s - 1;
    if(ch && ch != '#') {		/* get optional spindle number */
	char *fp1;
	int is_digit=1;
	skip_non_whitespace(s, ch);
	s[-1] = '\0';
	fp1=fp;
	if (*fp1 == '-') fp1++;
	for(;*fp1!='\0';fp1++) {
	    if(!isdigit((int)*fp1)) {
		is_digit = 0;
	    }
	}
	if(is_digit == 0) {
	    parserror("non-integer spindle `%s'", fp);
	    if(host == NULL) amfree(hostname);
	    amfree(disk->name);
	    amfree(disk);
	    return 1;
	}
	disk->spindle = atoi(fp);
	skip_integer(s, ch);
    }

    skip_whitespace(s, ch);
    fp = s - 1;
    if(ch && ch != '#') {		/* get optional network interface */
	skip_non_whitespace(s, ch);
	s[-1] = '\0';
	if((netif = lookup_interface(upcase(fp))) == NULL) {
	    parserror("undefined network interface `%s'", fp);
	    if(host == NULL) amfree(hostname);
	    amfree(disk->name);
	    amfree(disk);
	    return 1;
	}
    } else {
	netif = lookup_interface("");
    }

    skip_whitespace(s, ch);
    if(ch && ch != '#') {		/* now we have garbage, ignore it */
	parserror("end of line expected");
    }

    if(dtype->ignore || dtype->strategy == DS_SKIP) {
	amfree(diskname);
	free_sl(disk->exclude_file);
	free_sl(disk->exclude_list);
	free_sl(disk->include_file);
	free_sl(disk->include_list);
	amfree(disk);
	return 1;
    }

    /* success, add disk to lists */

    if(host == NULL) {			/* new host */
	host = alloc(sizeof(am_host_t));
	malloc_mark(host);
	host->next = hostlist;
	hostlist = host;

	host->hostname = hostname;
	hostname = NULL;
	host->disks = NULL;
	host->inprogress = 0;
	host->maxdumps = 1;		/* will be overwritten */
	host->netif = NULL;
	host->start_t = 0;
	host->up = NULL;
	host->features = NULL;
    }

    host->netif = netif;

    enqueue_disk(&lst, disk);

    disk->host = host;
    disk->hostnext = host->disks;
    host->disks = disk;
    host->maxdumps = disk->maxdumps;

    return 1;
}


printf_arglist_function(static void parserror, char *, format)
{
    va_list argp;

    /* print error message */

    fprintf(stderr, "\"%s\", line %d: ", diskfname, line_num);
    arglist_start(argp, format);
    vfprintf(stderr, format, argp);
    arglist_end(argp);
    fputc('\n', stderr);

    got_parserror = 1;
}


void dump_queue(st, q, npr, f)
char *st;
disklist_t q;
int npr;	/* we print first npr disks on queue, plus last two */
FILE *f;
{
    disk_t *d,*p;
    int pos;

    if(empty(q)) {
	fprintf(f, "%s QUEUE: empty\n", st);
	return;
    }
    fprintf(f, "%s QUEUE:\n", st);
    for(pos = 0, d = q.head, p = NULL; d != NULL; p = d, d = d->next, pos++) {
	if(pos < npr) fprintf(f, "%3d: %-10s %-4s\n",
			      pos, d->host->hostname, d->name);
    }
    if(pos > npr) {
	if(pos > npr+2) fprintf(f, "  ...\n");
	if(pos > npr+1) {
	    d = p->prev;
	    fprintf(f, "%3d: %-10s %-4s\n", pos-2, d->host->hostname, d->name);
	}
	d = p;
	fprintf(f, "%3d: %-10s %-4s\n", pos-1, d->host->hostname, d->name);
    }
}

char *optionstr(dp, their_features, fdout)
disk_t *dp;
am_feature_t * their_features;
FILE *fdout;
{
    char *auth_opt = NULL;
    char *kencrypt_opt = "";
    char *compress_opt = "";
    char *record_opt = "";
    char *index_opt = "";
    char *exclude_file = NULL;
    char *exclude_list = NULL;
    char *include_file = NULL;
    char *include_list = NULL;
    char *excl_opt = "";
    char *incl_opt = "";
    char *exc = NULL;
    char *result = NULL;
    sle_t *excl;
    int nb_exclude_file;
    int nb_include_file;

    /* modification by BIS@BBN 4/25/2003:
     * The first "if" statement has a number of problems, so I removed the
     * am_has_feature(dp->host->features, fe_options_auth)
     * condition and caused the first case to always fail.
     * 1) If dp->host is a NULL pointer, subsequent tests which look
     *    at dp->host->features or dp->host->hostname will cause a dump.
     *    It appears that there should be an assertion that dp->host
     *    is NOT NULL.
     * 2) The code which checks for the kencrypt feature is only executed
     *    in the case where dp->auth == AUTH_KRB4.  If we enable Kerberos IV
     *    in the first case, then the code checking for kencrypt will never
     *    be executed.
     * 3) The option processing code in server-src/dumper.c and
     *    client-src/sendbackup.c do not yet handle the "auth=krb4"
     *    option style of specifying Kerberos IV.
     * Getting rid of the whole first case seems to take care of problems
     * 2 and 3, but not problem 1.
     */
    if(dp->host && 0)
       /* && am_has_feature(dp->host->features, fe_options_auth)) */ {
	auth_opt = stralloc("auth=");
	if(dp->auth == AUTH_BSD) {
	    strappend(auth_opt, "bsd");
	} else if(dp->auth == AUTH_KRB4) {
	    strappend(auth_opt, "krb4");
	} else {
	    strappend(auth_opt, "unknown");
	}
	strappend(auth_opt, ";");
    } else if(dp->auth == AUTH_BSD) {
	if(am_has_feature(dp->host->features, fe_options_bsd_auth)) {
	    auth_opt = stralloc("bsd-auth;");
	}
	else if(fdout) {
	    fprintf(fdout, "WARNING: %s:%s does not support bsd-auth\n",
		    dp->host->hostname, dp->name);
	}
    } else if(dp->auth == AUTH_KRB4) {
	if(am_has_feature(dp->host->features, fe_options_krb4_auth)) {
	    auth_opt = stralloc("krb4-auth;");
	}
	else if(fdout) {
	    fprintf(fdout, "WARNING: %s:%s does not support krb4-auth\n",
		    dp->host->hostname, dp->name);
	}
	if(dp->kencrypt) {
	    if(am_has_feature(dp->host->features, fe_options_kencrypt)) {
		kencrypt_opt = "kencrypt;";
	    }
	    else if(fdout) {
		fprintf(fdout,
			"WARNING: %s:%s does not support kencrypt\n",
			dp->host->hostname, dp->name);
	    }
	}
    }

    switch(dp->compress) {
    case COMP_FAST:
	if(am_has_feature(their_features, fe_options_compress_fast)) {
	    compress_opt = "compress-fast;";
	}
	else if(fdout) {
	    fprintf(fdout,
		    "WARNING: %s:%s does not support fast compression\n",
		    dp->host->hostname, dp->name);
	}
	break;
    case COMP_BEST:
	if(am_has_feature(their_features, fe_options_compress_best)) {
	    compress_opt = "compress-best;";
	}
	else if(fdout) {
	    fprintf(fdout,
		    "WARNING: %s:%s does not support best compression\n",
		    dp->host->hostname, dp->name);
	}
	break;
    case COMP_SERV_FAST:
	if(am_has_feature(their_features, fe_options_srvcomp_fast)) {
	    compress_opt = "srvcomp-fast;";
	}
	break;
    case COMP_SERV_BEST:
	if(am_has_feature(their_features, fe_options_srvcomp_best)) {
            compress_opt = "srvcomp-best;";
	}
	break;
    }

    if(!dp->record) {
	if(am_has_feature(their_features, fe_options_no_record)) {
	    record_opt = "no-record;";
	}
	else if(fdout) {
	    fprintf(fdout, "WARNING: %s:%s does not support no record\n",
		    dp->host->hostname, dp->name);
	}
    }

    if(dp->index) {
	if(am_has_feature(their_features, fe_options_index)) {
	    index_opt = "index;";
	}
	else if(fdout) {
	    fprintf(fdout, "WARNING: %s:%s does not support index\n",
		    dp->host->hostname, dp->name);
	}
    }

    exclude_file = stralloc("");
    nb_exclude_file = 0;
    if(dp->exclude_file != NULL && dp->exclude_file->nb_element > 0) {
	nb_exclude_file = dp->exclude_file->nb_element;
	if(am_has_feature(their_features, fe_options_exclude_file)) {
	    if(am_has_feature(their_features, fe_options_multiple_exclude) ||
	       dp->exclude_file->nb_element == 1) {
		for(excl = dp->exclude_file->first; excl != NULL;
						    excl = excl->next) {
		    exc = newvstralloc( exc, "exclude-file=", excl->name,
					";", NULL);
		    strappend(exclude_file, exc);
		}
	    } else {
		exc = newvstralloc(exc, "exclude-file=",
				   dp->exclude_file->last->name, ";", NULL);
		strappend(exclude_file, exc);
		if(fdout) {
		    fprintf(fdout,
		       "WARNING: %s:%s does not support multiple exclude\n",
		       dp->host->hostname, dp->name);
		}
	    }
	} else if(fdout) {
	    fprintf(fdout, "WARNING: %s:%s does not support exclude file\n",
		    dp->host->hostname, dp->name);
	}
    }
    exclude_list = stralloc("");
    if(dp->exclude_list != NULL && dp->exclude_list->nb_element > 0) {
	if(am_has_feature(their_features, fe_options_exclude_list)) {
	    if(am_has_feature(their_features, fe_options_multiple_exclude) ||
	       (dp->exclude_list->nb_element == 1 && nb_exclude_file == 0)) {
		for(excl = dp->exclude_list->first; excl != NULL;
						    excl = excl->next) {
		    exc = newvstralloc( exc, "exclude-list=", excl->name,
					";", NULL);
		    strappend(exclude_list, exc);
		}
	    } else {
		exc = newvstralloc(exc, "exclude-list=",
				   dp->exclude_list->last->name, ";", NULL);
		strappend(exclude_list, exc);
		if(fdout) {
			fprintf(fdout,
			 "WARNING: %s:%s does not support multiple exclude\n",
			 dp->host->hostname, dp->name);
		}
	    }
	} else if(fdout) {
	    fprintf(fdout, "WARNING: %s:%s does not support exclude list\n",
		    dp->host->hostname, dp->name);
	}
    }

    include_file = stralloc("");
    nb_include_file = 0;
    if(dp->include_file != NULL && dp->include_file->nb_element > 0) {
	nb_include_file = dp->include_file->nb_element;
	if(am_has_feature(their_features, fe_options_include_file)) {
	    if(am_has_feature(their_features, fe_options_multiple_include) ||
	       dp->include_file->nb_element == 1) {
		for(excl = dp->include_file->first; excl != NULL;
						    excl = excl->next) {
		    exc = newvstralloc( exc, "include-file=", excl->name,
					";", NULL);
		    strappend(include_file, exc);
		}
	    } else {
		exc = newvstralloc(exc, "include-file=",
				   dp->include_file->last->name, ";", NULL);
		strappend(include_file, exc);
		if(fdout) {
		    fprintf(fdout,
			 "WARNING: %s:%s does not support multiple include\n",
			 dp->host->hostname, dp->name);
		}
	    }
	} else if(fdout) {
	    fprintf(fdout, "WARNING: %s:%s does not support include file\n",
		    dp->host->hostname, dp->name);
	}
    }
    include_list = stralloc("");
    if(dp->include_list != NULL && dp->include_list->nb_element > 0) {
	if(am_has_feature(their_features, fe_options_include_list)) {
	    if(am_has_feature(their_features, fe_options_multiple_include) ||
	       (dp->include_list->nb_element == 1 && nb_include_file == 0)) {
		for(excl = dp->include_list->first; excl != NULL;
						    excl = excl->next) {
		    exc = newvstralloc( exc, "include-list=", excl->name,
					";", NULL);
		    strappend(include_list, exc);
		}
	    } else {
		exc = newvstralloc(exc, "include-list=",
				   dp->include_list->last->name, ";", NULL);
		strappend(include_list, exc);
		if(fdout) {
			fprintf(fdout,
			 "WARNING: %s:%s does not support multiple include\n",
			 dp->host->hostname, dp->name);
		}
	    }
	} else if(fdout) {
	    fprintf(fdout, "WARNING: %s:%s does not support include list\n",
		    dp->host->hostname, dp->name);
	}
    }

    if(dp->exclude_optional) {
	if(am_has_feature(their_features, fe_options_optional_exclude)) {
	    excl_opt = "exclude-optional;";
	}
	else if(fdout) {
	    fprintf(fdout,
		    "WARNING: %s:%s does not support optional exclude\n",
		    dp->host->hostname, dp->name);
	}
    }
    if(dp->include_optional) {
	if(am_has_feature(their_features, fe_options_optional_include)) {
	   incl_opt = "include-optional;";
	}
	else if(fdout) {
	    fprintf(fdout,
		    "WARNING: %s:%s does not support optional include\n",
		    dp->host->hostname, dp->name);
	}
    }

    result = vstralloc(";",
		       auth_opt,
		       kencrypt_opt,
		       compress_opt,
		       record_opt,
		       index_opt,
		       exclude_file,
		       exclude_list,
		       include_file,
		       include_list,
		       excl_opt,
		       incl_opt,
		       NULL);
    amfree(auth_opt);
    amfree(exclude_file);
    amfree(exclude_list);
    amfree(include_file);
    amfree(include_list);
    amfree(exc);

    return result;
}

 
void match_disklist(disklist_t *origqp, int sargc, char **sargv)
{
    char *prevhost = NULL;
    int i;
    int match_a_host;
    int match_a_disk;
    int prev_match;
    disk_t *dp;

    if(sargc <= 0)
	return;

    for(dp = origqp->head; dp != NULL; dp = dp->next) {
	if(dp->todo == 1)
	    dp->todo = -1;
    }

    prev_match = 0;
    for(i=0;i<sargc;i++) {
	match_a_host = 0;
	for(dp = origqp->head; dp != NULL; dp = dp->next) {
	    if(match_host(sargv[i], dp->host->hostname))
		match_a_host = 1;
	}
	match_a_disk = 0;
	for(dp = origqp->head; dp != NULL; dp = dp->next) {
	    if(prevhost != NULL &&
	       match_host(prevhost, dp->host->hostname) &&
	       (match_disk(sargv[i], dp->name) ||
		(dp->device && match_disk(sargv[i], dp->device)))) {
		if(match_a_host) {
		    error("Argument %s match a host and a disk",sargv[i]);
		}
		else {
		    if(dp->todo == -1) {
			dp->todo = 1;
			match_a_disk = 1;
			prev_match = 0;
		    }
		}
	    }
	}
	if(!match_a_disk) {
	    if(match_a_host == 1) {
		if(prev_match == 1) { /* all disk of the previous host */
		    for(dp = origqp->head; dp != NULL; dp = dp->next) {
			if(match_host(prevhost,dp->host->hostname))
			    if(dp->todo == -1)
				dp->todo = 1;
		    }
		}
		prevhost = sargv[i];
		prev_match = 1;
	    }
	    else {
		prev_match = 0;
		/*error("%s match nothing",sargv[i]);*/
	    }
	}
    }

    if(prev_match == 1) { /* all disk of the previous host */
	for(dp = origqp->head; dp != NULL; dp = dp->next) {
	    if(match_host(prevhost,dp->host->hostname))
		if(dp->todo == -1)
		    dp->todo = 1;
	}
    }

    for(dp = origqp->head; dp != NULL; dp = dp->next) {
	if(dp->todo == -1)
	    dp->todo = 0;
    }
}

 
#ifdef TEST

void
dump_disk(dp)
disk_t *dp;
{
    printf("  DISK %s (HOST %s, LINE %d) TYPE %s NAME %s SPINDLE %d\n",
	   dp->name, dp->host->hostname, dp->line, dp->dtype_name,
	   dp->name == NULL? "(null)": dp->name,
	   dp->spindle);
}

void
dump_disklist()
{
    disk_t *dp, *prev;
    am_host_t *hp;

    if(hostlist == NULL) {
	printf("DISKLIST not read in\n");
	return;
    }

    printf("DISKLIST BY HOSTNAME:\n");

    for(hp = hostlist; hp != NULL; hp = hp->next) {
	printf("HOST %s INTERFACE %s\n",
	       hp->hostname,
	       (hp->netif == NULL||hp->netif->name == NULL) ? "(null)"
							    : hp->netif->name);
	for(dp = hp->disks; dp != NULL; dp = dp->hostnext)
	    dump_disk(dp);
	putchar('\n');
    }


    printf("DISKLIST IN FILE ORDER:\n");

    prev = NULL;
    for(dp = lst.head; dp != NULL; prev = dp, dp = dp->next) {
	dump_disk(dp);
	/* check pointers */
	if(dp->prev != prev) printf("*** prev pointer mismatch!\n");
	if(dp->next == NULL && lst.tail != dp) printf("tail mismatch!\n");
    }
}

int
main(argc, argv)
int argc;
char *argv[];
{
  char *conffile;
  char *conf_diskfile;
  int result;
  unsigned long malloc_hist_1, malloc_size_1;
  unsigned long malloc_hist_2, malloc_size_2;

  safe_fd(-1, 0);

  set_pname("diskfile");

  malloc_size_1 = malloc_inuse(&malloc_hist_1);

  if (argc>1) {
    config_name = argv[1];
    if (strchr(config_name, '/') != NULL) {
      config_dir = stralloc2(argv[1], "/");
      config_name = strrchr(config_name, '/') + 1;
    } else {
      config_dir = vstralloc(CONFIG_DIR, "/", config_name, "/", NULL);
    }
  } else {
    config_dir = stralloc("");
  }
  conffile = stralloc2(config_dir, CONFFILE_NAME);
  if((result = read_conffile(conffile)) == 0) {
    conf_diskfile = getconf_str(CNF_DISKFILE);
    if (*conf_diskfile == '/') {
      conf_diskfile = stralloc(conf_diskfile);
    } else {
      conf_diskfile = stralloc2(config_dir, conf_diskfile);
    }
    if((result = (read_diskfile(conf_diskfile) == NULL)) == 0) {
      dump_disklist();
    }
    amfree(conf_diskfile);
  }
  amfree(conffile);
  amfree(config_dir);

  malloc_size_2 = malloc_inuse(&malloc_hist_2);

  if(malloc_size_1 != malloc_size_2) {
    malloc_list(fileno(stderr), malloc_hist_1, malloc_hist_2);
  }

  return result;
}

#endif /* TEST */
