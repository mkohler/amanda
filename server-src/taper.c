/*
 * Amanda, The Advanced Maryland Automatic Network Disk Archiver
 * Copyright (c) 1991-1998, 2000 University of Maryland at College Park
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
/* $Id: taper.c,v 1.47.2.14.4.8.2.17.2.4 2005/10/11 11:10:20 martinea Exp $
 *
 * moves files from holding disk to tape, or from a socket to tape
 */

#include "amanda.h"
#include "conffile.h"
#include "tapefile.h"
#include "clock.h"
#include "stream.h"
#include "logfile.h"
#include "tapeio.h"
#include "changer.h"
#include "version.h"
#include "arglist.h"
#include "token.h"
#include "amfeatures.h"
#include "fileheader.h"
#include "server_util.h"
#ifdef HAVE_LIBVTBLC
#include <vtblc.h>
#include <strings.h>

static int vtbl_no   = -1;
static int len       =  0;
static int offset    =  0;
static char *datestr = NULL;
static char start_datestr[20];
time_t raw_time;
struct tm tape_time;
struct tm backup_time;
struct tm *tape_timep = &tape_time;
typedef struct vtbl_lbls {
    u_int8_t  label[45];
    u_int8_t  date[20];
} vtbl_lbls;
static vtbl_lbls vtbl_entry[MAX_VOLUMES];
#endif /* HAVE_LIBVTBLC */
/*
 * XXX update stat collection/printing
 * XXX advance to next tape first in next_tape
 * XXX label is being read twice?
 */

/* NBUFS replaced by conf_tapebufs */
/* #define NBUFS		20 */
int conf_tapebufs;

/* This is now the number of empties, not full bufs */
#define THRESHOLD	1

#define CONNECT_TIMEOUT 2*60



#define EMPTY 1
#define FILLING 2
#define FULL 3

typedef struct buffer_s {
    long status;
    unsigned int size;
    char *buffer;
} buffer_t;

#define nextbuf(p)    ((p) == buftable+conf_tapebufs-1? buftable : (p)+1)
#define prevbuf(p)    ((p) == buftable? buftable+conf_tapebufs-1 : (p)-1)

/* major modules */
int main P((int main_argc, char **main_argv));
void file_reader_side P((int rdpipe, int wrpipe));
void tape_writer_side P((int rdpipe, int wrpipe));

/* shared-memory routines */
char *attach_buffers P((unsigned int size));
void detach_buffers P((char *bufp));
void destroy_buffers P((void));

/* synchronization pipe routines */
void syncpipe_init P((int rd, int wr));
char syncpipe_get P((void));
int  syncpipe_getint P((void));
char *syncpipe_getstr P((void));
void syncpipe_put P((int ch));
void syncpipe_putint P((int i));
void syncpipe_putstr P((char *str));

/* tape manipulation subsystem */
int first_tape P((char *new_datestamp));
int next_tape P((int writerr));
int end_tape P((int writerr));
int write_filemark P((void));

/*
 * ========================================================================
 * GLOBAL STATE
 *
 */
int interactive;
int writerpid;
times_t total_wait;
#ifdef TAPER_DEBUG
int bufdebug = 1;
#else
int bufdebug = 0;
#endif

char *buffers = NULL;
buffer_t *buftable = NULL;

char *procname = "parent";

char *taper_datestamp = NULL;
char *label = NULL;
int filenum;
char *errstr = NULL;
int tape_fd = -1;
char *tapedev = NULL;
char *tapetype = NULL;
tapetype_t *tt = NULL;
long tt_blocksize;
long tt_blocksize_kb;
long buffer_size;
int tt_file_pad;
static unsigned long malloc_hist_1, malloc_size_1;
static unsigned long malloc_hist_2, malloc_size_2;

am_feature_t *their_features = NULL;

int runtapes, cur_tape, have_changer, tapedays;
char *labelstr, *conf_tapelist;
#ifdef HAVE_LIBVTBLC
char *rawtapedev;
int first_seg, last_seg;
#endif /* HAVE_LIBVTBLC */

/*
 * ========================================================================
 * MAIN PROGRAM
 *
 */
int main(main_argc, main_argv)
int main_argc;
char **main_argv;
{
    int p2c[2], c2p[2];		/* parent-to-child, child-to-parent pipes */
    char *conffile;
    unsigned int size;
    int i;
    int j;
    int page_size;
    char *first_buffer;

    safe_fd(-1, 0);

    set_pname("taper");

    malloc_size_1 = malloc_inuse(&malloc_hist_1);

    fprintf(stderr, "%s: pid %ld executable %s version %s\n",
	    get_pname(), (long) getpid(), main_argv[0], version());
    fflush(stderr);

    if (main_argc > 1 && main_argv[1][0] != '-') {
	config_name = stralloc(main_argv[1]);
	config_dir = vstralloc(CONFIG_DIR, "/", main_argv[1], "/", NULL);
	main_argc--;
	main_argv++;
    } else {
	char my_cwd[STR_SIZE];

	if (getcwd(my_cwd, sizeof(my_cwd)) == NULL) {
	    error("cannot determine current working directory");
	}
	config_dir = stralloc2(my_cwd, "/");
	if ((config_name = strrchr(my_cwd, '/')) != NULL) {
	    config_name = stralloc(config_name + 1);
	}
    }

    safe_cd();

    /* print prompts and debug messages if running interactive */

    interactive = (main_argc > 1 && strcmp(main_argv[1],"-t") == 0);
    if(interactive) {
	erroutput_type = ERR_INTERACTIVE;
    } else {
	erroutput_type = ERR_AMANDALOG;
	set_logerror(logerror);
    }

    conffile = stralloc2(config_dir, CONFFILE_NAME);
    if(read_conffile(conffile)) {
	error("errors processing config file \"%s\"", conffile);
    }
    amfree(conffile);

    conf_tapelist = getconf_str(CNF_TAPELIST);
    if (*conf_tapelist == '/') {
	conf_tapelist = stralloc(conf_tapelist);
    } else {
	conf_tapelist = stralloc2(config_dir, conf_tapelist);
    }
    if(read_tapelist(conf_tapelist)) {
	error("could not load tapelist \"%s\"", conf_tapelist);
    }

    tapedev	= getconf_str(CNF_TAPEDEV);
    tapetype    = getconf_str(CNF_TAPETYPE);
    tt		= lookup_tapetype(tapetype);
#ifdef HAVE_LIBVTBLC
    rawtapedev = getconf_str(CNF_RAWTAPEDEV);
#endif /* HAVE_LIBVTBLC */
    tapedays	= getconf_int(CNF_TAPECYCLE);
    labelstr	= getconf_str(CNF_LABELSTR);

    runtapes	= getconf_int(CNF_RUNTAPES);
    cur_tape	= 0;

    conf_tapebufs = getconf_int(CNF_TAPEBUFS);

    tt_blocksize_kb = tt->blocksize;
    tt_blocksize = tt_blocksize_kb * 1024;
    tt_file_pad = tt->file_pad;

    if(interactive) {
	fprintf(stderr,"taper: running in interactive test mode\n");
	fflush(stderr);
    }

    /* create read/write syncronization pipes */

    if(pipe(p2c) || pipe(c2p))
	error("creating sync pipes: %s", strerror(errno));

    /* create shared memory segment */

#if defined(HAVE_GETPAGESIZE)
    page_size = getpagesize();
    fprintf(stderr, "%s: page size is %d\n", get_pname(), page_size);
#else
    page_size = 1024;
    fprintf(stderr, "%s: getpagesize() not available, using %d\n",
	    get_pname(),
	    page_size);
#endif
    buffer_size = am_round(tt_blocksize, page_size);
    fprintf(stderr, "%s: buffer size is %ld\n", get_pname(), buffer_size);
    while(conf_tapebufs > 0) {
	size  = page_size;
	size += conf_tapebufs * buffer_size;
	size += conf_tapebufs * sizeof(buffer_t);
	if((buffers = attach_buffers(size)) != NULL) {
	    break;
	}
	log_add(L_INFO, "attach_buffers: (%d tapebuf%s: %d bytes) %s",
			conf_tapebufs,
			(conf_tapebufs == 1) ? "" : "s",
			size,
			strerror(errno));
	conf_tapebufs--;
    }
    if(buffers == NULL) {
	error("cannot allocate shared memory");
    }
    i = (buffers - (char *)0) & (page_size - 1);  /* page boundary offset */
    if(i != 0) {
	first_buffer = buffers + page_size - i;
	fprintf(stderr, "%s: shared memory at %p, first buffer at %p\n",
		get_pname(),
		buffers,
		first_buffer);
    } else {
	first_buffer = buffers;
    }
    buftable = (buffer_t *)(first_buffer + conf_tapebufs * buffer_size);
    memset(buftable, 0, conf_tapebufs * sizeof(buffer_t));
    if(conf_tapebufs < 10) {
	j = 1;
    } else if(conf_tapebufs < 100) {
	j = 2;
    } else {
	j = 3;
    }
    for(i = 0; i < conf_tapebufs; i++) {
	buftable[i].buffer = first_buffer + i * buffer_size;
	fprintf(stderr, "%s: buffer[%0*d] at %p\n",
		get_pname(),
		j, i,
		buftable[i].buffer);
    }
    fprintf(stderr, "%s: buffer structures at %p for %d bytes\n",
	    get_pname(),
	    buftable,
	    (int)(conf_tapebufs * sizeof(buffer_t)));

    /* fork off child writer process, parent becomes reader process */

    switch(writerpid = fork()) {
    case -1:
	error("fork: %s", strerror(errno));

    case 0:	/* child */
	aclose(p2c[1]);
	aclose(c2p[0]);

	tape_writer_side(p2c[0], c2p[1]);
	error("tape writer terminated unexpectedly");

    default:	/* parent */
	aclose(p2c[0]);
	aclose(c2p[1]);

	file_reader_side(c2p[0], p2c[1]);
	error("file reader terminated unexpectedly");
    }

    /* NOTREACHED */
    return 0;
}


/*
 * ========================================================================
 * FILE READER SIDE
 *
 */
void read_file P((int fd, char *handle,
		  char *host, char *disk, char *datestamp, 
		  int level, int port_flag));
int taper_fill_buffer P((int fd, buffer_t *bp, int buflen));
void dumpbufs P((char *str1));
void dumpstatus P((buffer_t *bp));

void file_reader_side(rdpipe, wrpipe)
int rdpipe, wrpipe;
{
    cmd_t cmd;
    struct cmdargs cmdargs;
    char *handle = NULL;
    char *filename = NULL;
    char *hostname = NULL;
    char *diskname = NULL;
    char *result = NULL;
    char *datestamp = NULL;
    char tok;
    char *q = NULL;
    int level, fd, data_port, data_socket, wpid;
    struct stat stat_file;
    int tape_started;
    int a;

    procname = "reader";
    syncpipe_init(rdpipe, wrpipe);

    /* must get START_TAPER before beginning */

    startclock();
    cmd = getcmd(&cmdargs);
    total_wait = stopclock();

    if(cmd != START_TAPER || cmdargs.argc != 2) {
	error("error [file_reader_side cmd %d argc %d]", cmd, cmdargs.argc);
    }

    /* pass start command on to tape writer */

    taper_datestamp = newstralloc(taper_datestamp, cmdargs.argv[2]);

    tape_started = 0;
    syncpipe_put('S');
    syncpipe_putstr(taper_datestamp);

    /* get result of start command */

    tok = syncpipe_get();
    switch(tok) {
    case 'S':
	putresult(TAPER_OK, "\n");
	tape_started = 1;
	/* start is logged in writer */
	break;
    case 'E':
	/* no tape, bail out */
	result = syncpipe_getstr();
	q = squotef("[%s]", result ? result : "(null)");
	putresult(TAPE_ERROR, "%s\n", q);
	amfree(q);
	log_add(L_ERROR,"no-tape [%s]", result);
	amfree(result);
	syncpipe_put('e');			/* ACK error */
	break;
    default:
	error("expected 'S' or 'E' for START-TAPER, got '%c'", tok);
    }

    /* process further commands */

    while(1) {
	startclock();
	cmd = getcmd(&cmdargs);
	if(cmd != QUIT && !tape_started) {
	    error("error [file_reader_side cmd %d without tape ready]", cmd);
	}
	total_wait = timesadd(total_wait, stopclock());

	switch(cmd) {
	case PORT_WRITE:
	    /*
	     * PORT-WRITE
	     *   handle
	     *   hostname
	     *   features
	     *   diskname
	     *   level
	     *   datestamp
	     */
	    cmdargs.argc++;			/* true count of args */
	    a = 2;

	    if(a >= cmdargs.argc) {
		error("error [taper PORT-WRITE: not enough args: handle]");
	    }
	    handle = newstralloc(handle, cmdargs.argv[a++]);

	    if(a >= cmdargs.argc) {
		error("error [taper PORT-WRITE: not enough args: hostname]");
	    }
	    hostname = newstralloc(hostname, cmdargs.argv[a++]);

	    if(a >= cmdargs.argc) {
		error("error [taper PORT-WRITE: not enough args: features]");
	    }
	    am_release_feature_set(their_features);
	    their_features = am_string_to_feature(cmdargs.argv[a++]);

	    if(a >= cmdargs.argc) {
		error("error [taper PORT-WRITE: not enough args: diskname]");
	    }
	    diskname = newstralloc(diskname, cmdargs.argv[a++]);

	    if(a >= cmdargs.argc) {
		error("error [taper PORT-WRITE: not enough args: level]");
	    }
	    level = atoi(cmdargs.argv[a++]);

	    if(a >= cmdargs.argc) {
		error("error [taper PORT-WRITE: not enough args: datestamp]");
	    }
	    datestamp = newstralloc(datestamp, cmdargs.argv[a++]);

	    if(a != cmdargs.argc) {
		error("error [taper file_reader_side PORT-WRITE: too many args: %d != %d]",
		      cmdargs.argc, a);
	    }

	    data_port = 0;
	    data_socket = stream_server(&data_port,
					-1,
					STREAM_BUFSIZE);	
	    if(data_socket < 0) {
		char *m;

		m = vstralloc("[port create failure: ",
			      strerror(errno),
			      "]",
			      NULL);
		q = squote(m);
		putresult(TAPE_ERROR, "%s %s\n", handle, q);
		amfree(m);
		break;
	    }
	    putresult(PORT, "%d\n", data_port);

	    if((fd = stream_accept(data_socket, CONNECT_TIMEOUT,
				   -1, NETWORK_BLOCK_BYTES)) == -1) {
		q = squote("[port connect timeout]");
		putresult(TAPE_ERROR, "%s %s\n", handle, q);
		aclose(data_socket);
		break;
	    }
	    read_file(fd, handle, hostname, diskname, datestamp, level, 1);
	    aclose(data_socket);
	    break;

	case FILE_WRITE:
	    /*
	     * FILE-WRITE
	     *   handle
	     *   filename
	     *   hostname
	     *   features
	     *   diskname
	     *   level
	     *   datestamp
	     */
	    cmdargs.argc++;			/* true count of args */
	    a = 2;

	    if(a >= cmdargs.argc) {
		error("error [taper FILE-WRITE: not enough args: handle]");
	    }
	    handle = newstralloc(handle, cmdargs.argv[a++]);

	    if(a >= cmdargs.argc) {
		error("error [taper FILE-WRITE: not enough args: filename]");
	    }
	    filename = newstralloc(filename, cmdargs.argv[a++]);

	    if(a >= cmdargs.argc) {
		error("error [taper FILE-WRITE: not enough args: hostname]");
	    }
	    hostname = newstralloc(hostname, cmdargs.argv[a++]);

	    if(a >= cmdargs.argc) {
		error("error [taper FILE-WRITE: not enough args: features]");
	    }
	    am_release_feature_set(their_features);
	    their_features = am_string_to_feature(cmdargs.argv[a++]);

	    if(a >= cmdargs.argc) {
		error("error [taper FILE-WRITE: not enough args: diskname]");
	    }
	    diskname = newstralloc(diskname, cmdargs.argv[a++]);

	    if(a >= cmdargs.argc) {
		error("error [taper FILE-WRITE: not enough args: level]");
	    }
	    level = atoi(cmdargs.argv[a++]);

	    if(a >= cmdargs.argc) {
		error("error [taper FILE-WRITE: not enough args: datestamp]");
	    }
	    datestamp = newstralloc(datestamp, cmdargs.argv[a++]);

	    if(a != cmdargs.argc) {
		error("error [taper file_reader_side FILE-WRITE: too many args: %d != %d]",
		      cmdargs.argc, a);
	    }

	    if(stat(filename,&stat_file)!=0) {
		q = squotef("[%s]", strerror(errno));
		putresult(TAPE_ERROR, "%s %s\n", handle, q);
		break;
	    }
	    if((fd = open(filename, O_RDONLY)) == -1) {
		q = squotef("[%s]", strerror(errno));
		putresult(TAPE_ERROR, "%s %s\n", handle, q);
		break;
	    }
	    read_file(fd, handle, hostname, diskname, datestamp, level, 0);
	    break;

	case QUIT:
	    putresult(QUITTING, "\n");
	    fprintf(stderr,"taper: DONE [idle wait: %s secs]\n",
		    walltime_str(total_wait));
	    fflush(stderr);
	    syncpipe_put('Q');	/* tell writer we're exiting gracefully */
	    aclose(wrpipe);

	    if((wpid = wait(NULL)) != writerpid) {
		fprintf(stderr,
			"taper: writer wait returned %d instead of %d: %s\n",
			wpid, writerpid, strerror(errno));
		fflush(stderr);
	    }

	    detach_buffers(buffers);
	    destroy_buffers();
	    amfree(datestamp);
	    amfree(label);
	    amfree(errstr);
	    amfree(changer_resultstr);
	    amfree(tapedev);
	    amfree(config_dir);
	    amfree(config_name);

	    malloc_size_2 = malloc_inuse(&malloc_hist_2);

	    if(malloc_size_1 != malloc_size_2) {
		malloc_list(fileno(stderr), malloc_hist_1, malloc_hist_2);
	    }

	    exit(0);

	default:
	    if(cmdargs.argc >= 1) {
		q = squote(cmdargs.argv[1]);
	    } else if(cmdargs.argc >= 0) {
		q = squote(cmdargs.argv[0]);
	    } else {
		q = stralloc("(no input?)");
	    }
	    putresult(BAD_COMMAND, "%s\n", q);
	    break;
	}
    }
    amfree(q);
    amfree(handle);
    amfree(filename);
    amfree(hostname);
    amfree(diskname);
}

void dumpbufs(str1)
char *str1;
{
    int i,j;
    long v;

    fprintf(stderr, "%s: state", str1);
    for(i = j = 0; i < conf_tapebufs; i = j+1) {
	v = buftable[i].status;
	for(j = i; j < conf_tapebufs && buftable[j].status == v; j++);
	j--;
	if(i == j) fprintf(stderr, " %d:", i);
	else fprintf(stderr, " %d-%d:", i, j);
	switch(v) {
	case FULL:	fputc('F', stderr); break;
	case FILLING:	fputc('f', stderr); break;
	case EMPTY:	fputc('E', stderr); break;
	default:
	    fprintf(stderr, "%ld", v);
	    break;
	}

    }
    fputc('\n', stderr);
    fflush(stderr);
}

void dumpstatus(bp)
buffer_t *bp;
{
    char pn[2];
    char bt[NUM_STR_SIZE];
    char status[NUM_STR_SIZE + 1];
    char *str = NULL;

    pn[0] = procname[0];
    pn[1] = '\0';
    ap_snprintf(bt, sizeof(bt), "%d", (int)(bp-buftable));

    switch(bp->status) {
    case FULL:		ap_snprintf(status, sizeof(status), "F%d", bp->size);
			break;
    case FILLING:	status[0] = 'f'; status[1] = '\0'; break;
    case EMPTY:		status[0] = 'E'; status[1] = '\0'; break;
    default:
	ap_snprintf(status, sizeof(status), "%ld", bp->status);
	break;
    }

    str = vstralloc("taper: ", pn, ": [buf ", bt, ":=", status, "]", NULL);
    dumpbufs(str);
    amfree(str);
}


void read_file(fd, handle, hostname, diskname, datestamp, level, port_flag)
    int fd, level, port_flag;
    char *handle, *hostname, *diskname, *datestamp;
{
    buffer_t *bp;
    char tok;
    int rc, err, opening, closing, bufnum, need_closing;
    long filesize;
    times_t runtime;
    char *strclosing = NULL;
    char *str;
    int header_read = 0;
    int buflen;
    dumpfile_t file;

    char *q = NULL;

#ifdef HAVE_LIBVTBLC
    static char desc[45];
    static char vol_date[20];
    static char vol_label[45];
#endif /* HAVE_LIBVTBLC */


    /* initialize */

    filesize = 0;
    closing = 0;
    need_closing = 0;
    err = 0;
    fh_init(&file);

    if(bufdebug) {
	fprintf(stderr, "taper: r: start file\n");
	fflush(stderr);
    }

    for(bp = buftable; bp < buftable + conf_tapebufs; bp++) {
	bp->status = EMPTY;
    }

    bp = buftable;
    if(interactive || bufdebug) dumpstatus(bp);

    /* tell writer to open tape */

    opening = 1;
    syncpipe_put('O');
    syncpipe_putstr(datestamp);
    syncpipe_putstr(hostname);
    syncpipe_putstr(diskname);
    syncpipe_putint(level);

    startclock();

    /* read file in loop */

    while(1) {
	tok = syncpipe_get();
	switch(tok) {

	case 'O':
	    assert(opening);
	    opening = 0;
	    err = 0;
	    break;

	case 'R':
	    bufnum = syncpipe_getint();

	    if(bufdebug) {
		fprintf(stderr, "taper: r: got R%d\n", bufnum);
		fflush(stderr);
	    }

	    if(need_closing) {
		syncpipe_put('C');
		closing = 1;
		need_closing = 0;
		break;
	    }

	    if(closing) break;	/* ignore extra read tokens */

	    assert(!opening);
	    if(bp->status != EMPTY || bufnum != bp-buftable) {
		/* XXX this SHOULD NOT HAPPEN.  Famous last words. */
		fprintf(stderr,"taper: panic: buffer mismatch at ofs %ld:\n",
			filesize);
		if(bufnum != bp-buftable) {
		    fprintf(stderr, "    my buf %d but writer buf %d\n",
			    (int)(bp-buftable), bufnum);
		}
		else {
		    fprintf(stderr,"buf %d state %s (%ld) instead of EMPTY\n",
			    (int)(bp-buftable),
			    bp->status == FILLING? "FILLING" :
			    bp->status == FULL? "FULL" : "EMPTY!?!?",
			    (long)bp->status);
		}
		dumpbufs("taper");
		sleep(1);
		dumpbufs("taper: after 1 sec");
		if(bp->status == EMPTY)
		    fprintf(stderr, "taper: result now correct!\n");
		fflush(stderr);

		errstr = newstralloc(errstr,
				     "[fatal buffer mismanagement bug]");
		q = squote(errstr);
		putresult(TRYAGAIN, "%s %s\n", handle, q);
		amfree(q);
		log_add(L_INFO, "retrying %s:%s.%d on new tape due to: %s",
		        hostname, diskname, level, errstr);
		closing = 1;
		syncpipe_put('X');	/* X == buffer snafu, bail */
		do {
		    tok = syncpipe_get();
		    if(tok == 'R')
			bufnum = syncpipe_getint();
		} while(tok != 'x');
		aclose(fd);
		return;
	    }

	    bp->status = FILLING;
	    buflen = header_read ? tt_blocksize : DISK_BLOCK_BYTES;
	    if(interactive || bufdebug) dumpstatus(bp);
	    if((rc = taper_fill_buffer(fd, bp, buflen)) < 0) {
		err = errno;
		closing = 1;
		strclosing = newvstralloc(strclosing,"Can't read data: ",NULL);
		syncpipe_put('C');
	    } else {
		if(rc < buflen) { /* switch to next file */
		    int save_fd;
		    struct stat stat_file;

		    save_fd = fd;
		    close(fd);
		    if(file.cont_filename[0] == '\0') {	/* no more file */
			err = 0;
			need_closing = 1;
		    } else if(stat(file.cont_filename, &stat_file) != 0) {
			err = errno;
			need_closing = 1;
			strclosing = newvstralloc(strclosing,"can't stat: ",file.cont_filename,NULL);
		    } else if((fd = open(file.cont_filename,O_RDONLY)) == -1) {
			err = errno;
			need_closing = 1;
			strclosing = newvstralloc(strclosing,"can't open: ",file.cont_filename,NULL);
		    } else if((fd != save_fd) && dup2(fd, save_fd) == -1) {
			err = errno;
			need_closing = 1;
			strclosing = newvstralloc(strclosing,"can't dup2: ",file.cont_filename,NULL);
		    } else {
			buffer_t bp1;
			int rc1;

			bp1.status = EMPTY;
			bp1.size = DISK_BLOCK_BYTES;
			bp1.buffer = malloc(DISK_BLOCK_BYTES);

			if(fd != save_fd) {
			    close(fd);
			    fd = save_fd;
			}

			rc1 = taper_fill_buffer(fd, &bp1, DISK_BLOCK_BYTES);
			if(rc1 <= 0) {
			    amfree(bp1.buffer);
			    err = (rc1 < 0) ? errno : 0;
			    need_closing = 1;
			    strclosing = newvstralloc(strclosing,
						      "Can't read header: ",
						      file.cont_filename,
						      NULL);
			} else {
			    parse_file_header(bp1.buffer, &file, rc1);

			    amfree(bp1.buffer);
			    bp1.buffer = bp->buffer + rc;

			    rc1 = taper_fill_buffer(fd, &bp1, tt_blocksize - rc);
			    if(rc1 <= 0) {
				err = (rc1 < 0) ? errno : 0;
				need_closing = 1;
				if(rc1 < 0) {
			    	    strclosing = newvstralloc(strclosing,
							      "Can't read data: ",
							      file.cont_filename,
							      NULL);
				}
			    }
			    else {
				rc += rc1;
				bp->size = rc;
			    }
			}
		    }
		}
		if(rc > 0) {
		    bp->status = FULL;
		    if(header_read == 0) {
			char *cont_filename;

			parse_file_header(bp->buffer, &file, rc);
			cont_filename = stralloc(file.cont_filename);
			file.cont_filename[0] = '\0';
			file.blocksize = tt_blocksize;
			build_header(bp->buffer, &file, tt_blocksize);

			/* add CONT_FILENAME back to in-memory header */
			strncpy(file.cont_filename, cont_filename, 
				sizeof(file.cont_filename));
			if(interactive || bufdebug) dumpstatus(bp);
			bp->size = tt_blocksize; /* output a full tape block */
			header_read = 1;
			amfree(cont_filename);
		    }
		    else {
			filesize += am_round(rc, 1024) / 1024;
		    }
		    if(interactive || bufdebug) dumpstatus(bp);
		    if(bufdebug) {
			fprintf(stderr,"taper: r: put W%d\n",(int)(bp-buftable));
			fflush(stderr);
		    }
		    syncpipe_put('W');
		    syncpipe_putint(bp-buftable);
		    bp = nextbuf(bp);
		}
		if(need_closing && rc <= 0) {
		    syncpipe_put('C');
		    need_closing = 0;
		    closing = 1;
		}
	    }
	    break;

	case 'T':
	case 'E':
	    syncpipe_put('e');	/* ACK error */

	    aclose(fd);
	    str = syncpipe_getstr();
	    errstr = newvstralloc(errstr, "[", str ? str : "(null)", "]", NULL);
	    amfree(str);

	    q = squote(errstr);
	    if(tok == 'T') {
		putresult(TRYAGAIN, "%s %s\n", handle, q);
		log_add(L_INFO, "retrying %s:%s.%d on new tape due to: %s",
		        hostname, diskname, level, errstr);
	    } else {
		putresult(TAPE_ERROR, "%s %s\n", handle, q);
		log_add(L_FAIL, "%s %s %s %d [out of tape]",
			hostname, diskname, datestamp, level);
		log_add(L_ERROR,"no-tape [%s]", errstr);
	    }
	    amfree(q);
	    return;

	case 'C':
	    assert(!opening);
	    assert(closing);

	    str = syncpipe_getstr();
	    label = newstralloc(label, str ? str : "(null)");
	    amfree(str);
	    str = syncpipe_getstr();
	    filenum = atoi(str ? str : "-9876");	/* ??? */
	    amfree(str);
	    fprintf(stderr, "taper: reader-side: got label %s filenum %d\n",
		    label, filenum);
	    fflush(stderr);

	    aclose(fd);
	    runtime = stopclock();
	    if(err) {
		if(strclosing) {
		    errstr = newvstralloc(errstr,
				          "[input: ", strclosing, ": ",
					  strerror(err), "]", NULL);
		    amfree(strclosing);
		}
		else
		    errstr = newvstralloc(errstr,
				          "[input: ", strerror(err), "]",
				          NULL);
		q = squote(errstr);
		putresult(TAPE_ERROR, "%s %s\n", handle, q);
		amfree(q);
		log_add(L_FAIL, "%s %s %s %d %s",
			hostname, diskname, datestamp, level, errstr);
		str = syncpipe_getstr();	/* reap stats */
		amfree(str);
	    } else {
		char kb_str[NUM_STR_SIZE];
		char kps_str[NUM_STR_SIZE];
		double rt;

		rt = runtime.r.tv_sec+runtime.r.tv_usec/1000000.0;
		ap_snprintf(kb_str, sizeof(kb_str), "%ld", filesize);
		ap_snprintf(kps_str, sizeof(kps_str), "%3.1f",
				     rt ? filesize / rt : 0.0);
		str = syncpipe_getstr();
		errstr = newvstralloc(errstr,
				      "[sec ", walltime_str(runtime),
				      " kb ", kb_str,
				      " kps ", kps_str,
				      " ", str ? str : "(null)",
				      "]",
				      NULL);
		amfree(str);
		q = squote(errstr);
		putresult(DONE, "%s %s %d %s\n",
			  handle, label, filenum, q);
		amfree(q);
		log_add(L_SUCCESS, "%s %s %s %d %s",
		        hostname, diskname, datestamp, level, errstr);
#ifdef HAVE_LIBVTBLC
		/* 
		 *  We have 44 characters available for the label string:
		 *  use max 20 characters for hostname
		 *      max 20 characters for diskname 
		 *             (it could contain a samba share or dos path)
		 *           2 for level
		 */
		memset(desc, '\0', 45);

		strncpy(desc, hostname, 20);

		if ((len = strlen(hostname)) <= 20) {
		    memset(desc + len, ' ', 1);
		    offset = len + 1;
		}
		else{
		    memset(desc + 20, ' ', 1);
		    offset = 21;
		}

		strncpy(desc + offset, diskname, 20);

		if ((len = strlen(diskname)) <= 20) {
		    memset(desc + offset + len, ' ', 1);
		    offset = offset + len + 1;
		}
		else{
		    memset(desc + offset + 20, ' ', 1);
		    offset = offset + 21;
		}

		sprintf(desc + offset, "%i", level);

	        strncpy(vol_label, desc, 44);
		fprintf(stderr, "taper: added vtbl label string %i: \"%s\"\n",
			filenum, vol_label);
		fflush(stderr);

		/* pass label string on to tape writer */
		syncpipe_put('L');
		syncpipe_putint(filenum);
		syncpipe_putstr(vol_label);		

		/* 
		 * reformat datestamp for later use with set_date from vtblc 
		 */
		strptime(datestamp, "%Y%m%d", &backup_time);
		strftime(vol_date, 20, "%T %D", &backup_time);
		fprintf(stderr, 
			"taper: reformatted vtbl date string: \"%s\"->\"%s\"\n",
			datestamp,
			vol_date);

		/* pass date string on to tape writer */		
		syncpipe_put('D');
		syncpipe_putint(filenum);
		syncpipe_putstr(vol_date);

#endif /* HAVE_LIBVTBLC */
	    }
	    return;

	default:
	    assert(0);
	}
    }
}

int taper_fill_buffer(fd, bp, buflen)
int fd;
buffer_t *bp;
int buflen;
{
    char *curptr;
    int spaceleft, cnt;

    curptr = bp->buffer;
    bp->size = 0;
    spaceleft = buflen;

    do {
	cnt = read(fd, curptr, spaceleft);
	switch(cnt) {
	case 0:	/* eof */
	    if(interactive) fputs("r0", stderr);
	    return bp->size;
	case -1:	/* error on read, punt */
	    if(interactive) fputs("rE", stderr);
	    return -1;
	default:
	    spaceleft -= cnt;
	    curptr += cnt;
	    bp->size += cnt;
	}

    } while(spaceleft > 0);

    if(interactive) fputs("R", stderr);
    return bp->size;
}



/*
 * ========================================================================
 * TAPE WRITER SIDE
 *
 */
times_t idlewait, rdwait, wrwait, fmwait;
long total_writes;
double total_tape_used;
int total_tape_fm;

void write_file P((void));
int write_buffer P((buffer_t *bp));

void tape_writer_side(getp, putp)
int getp, putp;
{
    char tok;
    int tape_started;
    char *str;
    char *hostname;
    char *diskname;
    char *datestamp;
    int level;

#ifdef HAVE_LIBVTBLC
    char *vol_label;
    char *vol_date;
#endif /* HAVE_LIBVTBLC */

    procname = "writer";
    syncpipe_init(getp, putp);

    tape_started = 0;
    idlewait = times_zero;

    while(1) {
	startclock();
	tok = syncpipe_get();
	idlewait = timesadd(idlewait, stopclock());
	if(tok != 'S' && tok != 'Q' && !tape_started) {
	    error("writer: token '%c' before start", tok);
	}

	switch(tok) {
	case 'S':		/* start-tape */
	    if(tape_started) {
		error("writer: multiple start requests");
	    }
	    str = syncpipe_getstr();
	    if(!first_tape(str ? str : "bad-datestamp")) {
		if(tape_fd >= 0) {
		    tapefd_close(tape_fd);
		    tape_fd = -1;
		}
		syncpipe_put('E');
		syncpipe_putstr(errstr);
		/* wait for reader to acknowledge error */
		do {
		    tok = syncpipe_get();
		    if(tok != 'e') {
			error("writer: got '%c' unexpectedly after error", tok);
		    }
		} while(tok != 'e');
	    } else {
		syncpipe_put('S');
		tape_started = 1;
	    }
	    amfree(str);

	    break;

	case 'O':		/* open-output */
	    datestamp = syncpipe_getstr();
	    tapefd_setinfo_datestamp(tape_fd, datestamp);
	    amfree(datestamp);
	    hostname = syncpipe_getstr();
	    tapefd_setinfo_host(tape_fd, hostname);
	    amfree(hostname);
	    diskname = syncpipe_getstr();
	    tapefd_setinfo_disk(tape_fd, diskname);
	    amfree(diskname);
	    level = syncpipe_getint();
	    tapefd_setinfo_level(tape_fd, level);
	    write_file();
	    break;

#ifdef HAVE_LIBVTBLC
	case 'L':		/* read vtbl label */
	    vtbl_no = syncpipe_getint();
	    vol_label = syncpipe_getstr();
	    fprintf(stderr, "taper: read label string \"%s\" from pipe\n", 
		    vol_label);
	    strncpy(vtbl_entry[vtbl_no].label, vol_label, 45);
	    break;

	case 'D':		/* read vtbl date */
	    vtbl_no = syncpipe_getint();
	    vol_date = syncpipe_getstr();
	    fprintf(stderr, "taper: read date string \"%s\" from pipe\n", 
		    vol_date);
	    strncpy(vtbl_entry[vtbl_no].date, vol_date, 20);
	    break;
#endif /* HAVE_LIBVTBLC */

	case 'Q':
	    end_tape(0);	/* XXX check results of end tape ?? */
	    clear_tapelist();
	    detach_buffers(buffers);
	    amfree(taper_datestamp);
	    amfree(label);
	    amfree(errstr);
	    amfree(changer_resultstr);
	    amfree(tapedev);
	    amfree(config_dir);
	    amfree(config_name);

	    malloc_size_2 = malloc_inuse(&malloc_hist_2);

	    if(malloc_size_1 != malloc_size_2) {
		malloc_list(fileno(stderr), malloc_hist_1, malloc_hist_2);
	    }

	    exit(0);

	default:
	    assert(0);
	}
    }
}

void write_file()
{
    buffer_t *bp;
    int full_buffers, i, bufnum;
    char tok;
    char number[NUM_STR_SIZE];
    char *rdwait_str, *wrwait_str, *fmwait_str;

    rdwait = wrwait = times_zero;
    total_writes = 0;

    bp = buftable;
    full_buffers = 0;
    tok = '?';

    if(bufdebug) {
	fprintf(stderr, "taper: w: start file\n");
	fflush(stderr);
    }

    /*
     * Tell the reader that the tape is open, and give it all the buffers.
     */
    syncpipe_put('O');
    for(i = 0; i < conf_tapebufs; i++) {
	if(bufdebug) {
	    fprintf(stderr, "taper: w: put R%d\n", i);
	    fflush(stderr);
	}
	syncpipe_put('R'); syncpipe_putint(i);
    }

    /*
     * We write the filemark at the start of the file rather than at the end,
     * so that it can proceed in parallel with the reader's initial filling
     * up of the buffers.
     */

    startclock();
    if(!write_filemark())
	goto tape_error;
    fmwait = stopclock();

    filenum += 1;

    do {

	/*
	 * STOPPED MODE
	 *
	 * At the start of the file, or if the input can't keep up with the
	 * tape, we enter STOPPED mode, which waits for most of the buffers
	 * to fill up before writing to tape.  This maximizes the amount of
	 * data written in chunks to the tape drive, minimizing the number
	 * of starts/stops, which in turn saves tape and time.
	 */

	if(interactive) fputs("[WS]", stderr);
	startclock();
	while(full_buffers < conf_tapebufs - THRESHOLD) {
	    tok = syncpipe_get();
	    if(tok != 'W') break;
	    bufnum = syncpipe_getint();
	    if(bufdebug) {
		fprintf(stderr,"taper: w: got W%d\n",bufnum);
		fflush(stderr);
	    }
	    full_buffers++;
	}
	rdwait = timesadd(rdwait, stopclock());

	/*
	 * STARTING MODE
	 *
	 * We start output when sufficient buffers have filled up, or at
	 * end-of-file, whichever comes first.  Here we drain all the buffers
	 * that were waited on in STOPPED mode.  If more full buffers come
	 * in, then we will be STREAMING.
	 */

	while(full_buffers) {
	    if(tt_file_pad && bp->size < tt_blocksize) {
		memset(bp->buffer+bp->size, 0, tt_blocksize - bp->size);
		bp->size = tt_blocksize;
	    }
	    if(!write_buffer(bp)) goto tape_error;
	    full_buffers--;
	    bp = nextbuf(bp);
	}

	/*
	 * STREAMING MODE
	 *
	 * With any luck, the input source is faster than the tape drive.  In
	 * this case, full buffers will appear in the circular queue faster
	 * than we can write them, so the next buffer in the queue will always
	 * be marked FULL by the time we get to it.  If so, we'll stay in
	 * STREAMING mode.
	 *
	 * On the other hand, if we catch up to the input and thus would have
	 * to wait for buffers to fill, we are then STOPPED again.
	 */

	while(tok == 'W' && bp->status == FULL) {
	    tok = syncpipe_get();
	    if(tok == 'W') {
		bufnum = syncpipe_getint();
		if(bufdebug) {
		    fprintf(stderr,"taper: w: got W%d\n",bufnum);
		    fflush(stderr);
		}
		if(bufnum != bp-buftable) {
		    fprintf(stderr,
			    "taper: tape-writer: my buf %d reader buf %d\n",
			    (int)(bp-buftable), bufnum);
		    fflush(stderr);
		    syncpipe_put('E');
		    syncpipe_putstr("writer-side buffer mismatch");
		    goto error_ack;
		}
		if(tt_file_pad && bp->size < tt_blocksize) {
		    memset(bp->buffer+bp->size, 0, tt_blocksize - bp->size);
		    bp->size = tt_blocksize;
		}
		if(!write_buffer(bp)) goto tape_error;
		bp = nextbuf(bp);
	    }
	    else if(tok == 'Q')
		return;
	    else if(tok == 'X')
		goto reader_buffer_snafu;
	    else
		error("writer-side not expecting token: %c", tok);
	}
    } while(tok == 'W');

    /* got close signal from reader, acknowledge it */

    if(tok == 'X')
	goto reader_buffer_snafu;

    assert(tok == 'C');
    syncpipe_put('C');

    /* tell reader the tape and file number */

    syncpipe_putstr(label);
    ap_snprintf(number, sizeof(number), "%d", filenum);
    syncpipe_putstr(number);

    ap_snprintf(number, sizeof(number), "%ld", total_writes);
    rdwait_str = stralloc(walltime_str(rdwait));
    wrwait_str = stralloc(walltime_str(wrwait));
    fmwait_str = stralloc(walltime_str(fmwait));
    errstr = newvstralloc(errstr,
			  "{wr:",
			  " writers ", number,
			  " rdwait ", rdwait_str,
			  " wrwait ", wrwait_str,
			  " filemark ", fmwait_str,
			  "}",
			  NULL);
    amfree(rdwait_str);
    amfree(wrwait_str);
    amfree(fmwait_str);
    syncpipe_putstr(errstr);

    /* XXX go to next tape if past tape size? */

    return;

 tape_error:
    /* got tape error */
    if(next_tape(1)) syncpipe_put('T');	/* next tape in place, try again */
    else syncpipe_put('E');		/* no more tapes, fail */
    syncpipe_putstr(errstr);

 error_ack:
    /* wait for reader to acknowledge error */
    do {
	tok = syncpipe_get();
	if(tok != 'W' && tok != 'C' && tok != 'e')
	    error("writer: got '%c' unexpectedly after error", tok);
	if(tok == 'W')
	    syncpipe_getint();	/* eat buffer number */
    } while(tok != 'e');
    return;

 reader_buffer_snafu:
    syncpipe_put('x');
    return;
}

int write_buffer(bp)
buffer_t *bp;
{
    int rc;

    if(bp->status != FULL) {
	/* XXX buffer management snafu */
	assert(0);
    }

    startclock();
    rc = tapefd_write(tape_fd, bp->buffer, bp->size);
    if(rc == bp->size) {
#if defined(NEED_RESETOFS)
	static double tape_used_modulus_2gb = 0;

	/*
	 * If the next write will go over the 2 GByte boundary, reset
	 * the kernel concept of where we are to make sure it does not
	 * go silly on us.
	 */
	tape_used_modulus_2gb += (double)rc;
	if(tape_used_modulus_2gb + (double)rc > (double)0x7fffffff) {
	    tape_used_modulus_2gb = 0;
	    tapefd_resetofs(tape_fd);
	}
#endif
	wrwait = timesadd(wrwait, stopclock());
	total_writes += 1;
	total_tape_used += (double)rc;
	bp->status = EMPTY;
	if(interactive || bufdebug) dumpstatus(bp);
	if(interactive) fputs("W", stderr);

	if(bufdebug) {
	    fprintf(stderr, "taper: w: put R%d\n", (int)(bp-buftable));
	    fflush(stderr);
	}
	syncpipe_put('R'); syncpipe_putint(bp-buftable);
	return 1;
    } else {
	errstr = newvstralloc(errstr,
			      "writing file: ",
			      (rc != -1) ? "short write" : strerror(errno),
			      NULL);
	wrwait = timesadd(wrwait, stopclock());
	if(interactive) fputs("[WE]", stderr);
	return 0;
    }
}


/*
 * ========================================================================
 * SHARED-MEMORY BUFFER SUBSYSTEM
 *
 */

#ifdef HAVE_SYSVSHM

int shmid = -1;

char *attach_buffers(size)
    unsigned int size;
{
    char *result;

    shmid = shmget(IPC_PRIVATE, size, IPC_CREAT|0700);
    if(shmid == -1) {
	return NULL;
    }

    result = (char *)shmat(shmid, (SHM_ARG_TYPE *)NULL, 0);

    if(result == (char *)-1) {
	int save_errno = errno;

	destroy_buffers();
	errno = save_errno;
	error("shmat: %s", strerror(errno));
    }

    return result;
}


void detach_buffers(bufp)
    char *bufp;
{
    if(shmdt((SHM_ARG_TYPE *)bufp) == -1) {
	error("shmdt: %s", strerror(errno));
    }
}

void destroy_buffers()
{
    if(shmid == -1) return;	/* nothing to destroy */
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
	error("shmctl: %s", strerror(errno));
    }
}

#else
#ifdef HAVE_MMAP

#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#ifndef MAP_ANON
#  ifdef MAP_ANONYMOUS			/* OSF/1-style */
#    define MAP_ANON MAP_ANONYMOUS
#  else					/* SunOS4-style */
#    define MAP_ANON 0
#    define ZERO_FILE "/dev/zero"
#  endif
#endif

int shmfd = -1;
unsigned int saved_size;

char *attach_buffers(size)
    unsigned int size;
{
    char *shmbuf;

#ifdef ZERO_FILE
    shmfd = open(ZERO_FILE, O_RDWR);
    if(shmfd == -1) {
	error("attach_buffers: could not open %s: %s",
	      ZERO_FILE,
	      strerror(errno));
    }
#endif

    saved_size = size;
    shmbuf = (char *) mmap((void *) 0,
			   size,
			   PROT_READ|PROT_WRITE,
			   MAP_ANON|MAP_SHARED,
			   shmfd, 0);

    return shmbuf;
}

void detach_buffers(bufp)
char *bufp;
{
    if(munmap((void *)bufp, saved_size) == -1) {
	error("detach_buffers: munmap: %s", strerror(errno));
    }

    aclose(shmfd);
}

void destroy_buffers()
{
}

#else
error: must define either HAVE_SYSVSHM or HAVE_MMAP!
#endif
#endif



/*
 * ========================================================================
 * SYNC-PIPE SUBSYSTEM
 *
 */

int getpipe, putpipe;

void syncpipe_init(rd, wr)
int rd, wr;
{
    getpipe = rd;
    putpipe = wr;
}

char syncpipe_get()
{
    int rc;
    char buf[1];

    rc = read(getpipe, buf, sizeof(buf));
    if(rc == 0)		/* EOF */
	error("syncpipe_get: %c: unexpected EOF", *procname);
    else if(rc < 0)
	error("syncpipe_get: %c: %s", *procname, strerror(errno));

    if(bufdebug && *buf != 'R' && *buf != 'W') {
	fprintf(stderr,"taper: %c: getc %c\n",*procname,*buf);
	fflush(stderr);
    }

    return buf[0];
}

int syncpipe_getint()
{
    int rc;
    int i;
    int len = sizeof(i);
    char *p;

    for(p = (char *)&i; len > 0; len -= rc, p += rc) {
	if ((rc = read(getpipe, p, len)) <= 0) {
	    error("syncpipe_getint: %s",
		  rc < 0 ? strerror(errno) : "short read");
	}
    }

    return i;
}


char *syncpipe_getstr()
{
    int rc;
    int len;
    char *p;
    char *str;

    if((len = syncpipe_getint()) <= 0) {
	return NULL;
    }

    str = alloc(len);

    for(p = str; len > 0; len -= rc, p += rc) {
	if ((rc = read(getpipe, p, len)) <= 0) {
	    error("syncpipe_getstr: %s",
		  rc < 0 ? strerror(errno) : "short read");
	}
    }

    return str;
}


void syncpipe_put(chi)
int chi;
{
    int l, n, s;
    char ch = chi;
    char *item = &ch;

    if(bufdebug && chi != 'R' && chi != 'W') {
	fprintf(stderr,"taper: %c: putc %c\n",*procname,chi);
	fflush(stderr);
    }

    for(l = 0, n = sizeof(ch); l < n; l += s) {
	if((s = write(putpipe, item + l, n - l)) < 0) {
	    error("syncpipe_put: %s", strerror(errno));
	}
    }
}

void syncpipe_putint(i)
int i;
{
    int l, n, s;
    char *item = (char *)&i;

    for(l = 0, n = sizeof(i); l < n; l += s) {
	if((s = write(putpipe, item + l, n - l)) < 0) {
	    error("syncpipe_putint: %s", strerror(errno));
	}
    }
}

void syncpipe_putstr(item)
char *item;
{
    int l, n, s;

    n = strlen(item)+1;				/* send '\0' as well */
    syncpipe_putint(n);
    for(l = 0, n = strlen(item)+1; l < n; l += s) {
	if((s = write(putpipe, item + l, n - l)) < 0) {
	    error("syncpipe_putstr: %s", strerror(errno));
	}
    }
}


/*
 * ========================================================================
 * TAPE MANIPULATION SUBSYSTEM
 *
 */

/* local functions */
int scan_init P((int rc, int ns, int bk));
int taperscan_slot P((int rc, char *slotstr, char *device));
char *taper_scan P((void));
int label_tape P((void));

int label_tape()
{
    char *conf_tapelist_old = NULL;
    char *olddatestamp = NULL;
    char *result;
    tape_t *tp;
    static int first_call = 1;

    if(have_changer) {
	amfree(tapedev);
	if ((tapedev = taper_scan()) == NULL) {
	    errstr = newstralloc(errstr, changer_resultstr);
	    return 0;
	}
    }

#ifdef HAVE_LINUX_ZFTAPE_H
    if (is_zftape(tapedev) == 1){
	if((tape_fd = tape_open(tapedev, O_RDONLY)) == -1) {
	    errstr = newstralloc2(errstr, "taper: ",
				  (errno == EACCES) ? "tape is write-protected"
				  : strerror(errno));
	    return 0;
	}
	if((result = tapefd_rdlabel(tape_fd, &olddatestamp, &label)) != NULL) {
	    amfree(olddatestamp);
	    errstr = newstralloc(errstr, result);
	    return 0;
	}
	if(tapefd_rewind(tape_fd) == -1) { 
	    return 0;
	} 
	tapefd_close(tape_fd);
	tape_fd = -1;
    }
    else
#endif /* !HAVE_LINUX_ZFTAPE_H */
    if((result = tape_rdlabel(tapedev, &olddatestamp, &label)) != NULL) {
	amfree(olddatestamp);
	errstr = newstralloc(errstr, result);
	return 0;
    }

    fprintf(stderr, "taper: read label `%s' date `%s'\n", label, olddatestamp);
    fflush(stderr);
    amfree(olddatestamp);

    /* check against tape list */
    if (strcmp(label, FAKE_LABEL) != 0) {
	tp = lookup_tapelabel(label);
	if(tp == NULL) {
	    errstr = newvstralloc(errstr,
				  "label ", label,
		" match labelstr but it not listed in the tapelist file",
				  NULL);
	    return 0;
	}
	else if(tp != NULL && !reusable_tape(tp)) {
	    errstr = newvstralloc(errstr,
			          "cannot overwrite active tape ", label,
			          NULL);
	    return 0;
	}

	if(!match(labelstr, label)) {
	    errstr = newvstralloc(errstr,
			          "label ", label,
			          " doesn\'t match labelstr \"", labelstr, "\"",
			          NULL);
	    return 0;
	}
    }

    if((tape_fd = tape_open(tapedev, O_WRONLY)) == -1) {
	if(errno == EACCES) {
	    errstr = newstralloc(errstr,
				 "writing label: tape is write protected");
	} else {
	    errstr = newstralloc2(errstr,
				  "writing label: ", strerror(errno));
	}
	return 0;
    }

    tapefd_setinfo_length(tape_fd, tt->length);

    tapefd_setinfo_datestamp(tape_fd, taper_datestamp);
    tapefd_setinfo_disk(tape_fd, label);
    result = tapefd_wrlabel(tape_fd, taper_datestamp, label, tt_blocksize);
    if(result != NULL) {
	errstr = newstralloc(errstr, result);
	return 0;
    }

    fprintf(stderr, "taper: wrote label `%s' date `%s'\n", label, taper_datestamp);
    fflush(stderr);

#ifdef HAVE_LIBVTBLC
    /* store time for the first volume entry */ 
    time(&raw_time);
    tape_timep = localtime(&raw_time);
    strftime(start_datestr, 20, "%T %D", tape_timep);
    fprintf(stderr, "taper: got vtbl start time: %s\n", start_datestr);
    fflush(stderr);
#endif /* HAVE_LIBVTBLC */

    /* write tape list */

    /* XXX add cur_tape number to tape list structure */
    if (strcmp(label, FAKE_LABEL) != 0) {

	if(cur_tape == 0) {
	    conf_tapelist_old = stralloc2(conf_tapelist, ".yesterday");
	} else {
	    char cur_str[NUM_STR_SIZE];

	    ap_snprintf(cur_str, sizeof(cur_str), "%d", cur_tape - 1);
	    conf_tapelist_old = vstralloc(conf_tapelist,
					".today.", cur_str, NULL);
	}
	if(write_tapelist(conf_tapelist_old)) {
	    error("could not write tapelist: %s", strerror(errno));
	}
	amfree(conf_tapelist_old);

	remove_tapelabel(label);
	add_tapelabel(atoi(taper_datestamp), label);
	if(write_tapelist(conf_tapelist)) {
	    error("could not write tapelist: %s", strerror(errno));
	}
    }

    log_add(L_START, "datestamp %s label %s tape %d",
	    taper_datestamp, label, cur_tape);
    if (first_call && strcmp(label, FAKE_LABEL) == 0) {
	first_call = 0;
	log_add(L_WARNING, "tapedev is %s, dumps will be thrown away", tapedev);
    }

    total_tape_used=0.0;
    total_tape_fm = 0;

    return 1;
}

int first_tape(new_datestamp)
char *new_datestamp;
{
    if((have_changer = changer_init()) < 0) {
	error("changer initialization failed: %s", strerror(errno));
    }
    changer_debug = 1;

    taper_datestamp = newstralloc(taper_datestamp, new_datestamp);

    if(!label_tape())
	return 0;

    filenum = 0;
    return 1;
}

int next_tape(writerror)
int writerror;
{
    end_tape(writerror);

    if(++cur_tape >= runtapes)
	return 0;

    if(!label_tape()) {
	return 0;
    }

    filenum = 0;
    return 1;
}


int end_tape(writerror)
int writerror;
{
    char *result;
    int rc = 0;

    if(tape_fd >= 0) {
	log_add(L_INFO, "tape %s kb %ld fm %d %s", 
		label,
		(long) ((total_tape_used+1023.0) / 1024.0),
		total_tape_fm,
		writerror? errstr : "[OK]");

	fprintf(stderr, "taper: writing end marker. [%s %s kb %ld fm %d]\n",
		label,
		writerror? "ERR" : "OK",
		(long) ((total_tape_used+1023.0) / 1024.0),
		total_tape_fm);
	fflush(stderr);
	if(! writerror) {
	    if(! write_filemark()) {
		rc = 1;
		goto common_exit;
	    }

	    result = tapefd_wrendmark(tape_fd, taper_datestamp, tt_blocksize);
	    if(result != NULL) {
		errstr = newstralloc(errstr, result);
		rc = 1;
		goto common_exit;
	    }
	}
    }

#ifdef HAVE_LINUX_ZFTAPE_H
    if (tape_fd >= 0 && is_zftape(tapedev) == 1) {
	/* rewind the tape */

	if(tapefd_rewind(tape_fd) == -1 ) {
	    errstr = newstralloc2(errstr, "rewinding tape: ", strerror(errno));
	    rc = 1;
	    goto common_exit;
	}
	/* close the tape */

	if(tapefd_close(tape_fd) == -1) {
	    errstr = newstralloc2(errstr, "closing tape: ", strerror(errno));
	    rc = 1;
	    goto common_exit;
	}
	tape_fd = -1;

#ifdef HAVE_LIBVTBLC
	/* update volume table */
	fprintf(stderr, "taper: updating volume table ...\n");
	fflush(stderr);
    
	if ((tape_fd = raw_tape_open(rawtapedev, O_RDWR)) == -1) {
	    if(errno == EACCES) {
		errstr = newstralloc(errstr,
				     "updating volume table: tape is write protected");
	    } else {
		errstr = newstralloc2(errstr,
				      "updating volume table: ", 
				      strerror(errno));
	    }
	    rc = 1;
	    goto common_exit;
	}
	/* read volume table */
	if ((num_volumes = read_vtbl(tape_fd, volumes, vtbl_buffer,
				     &first_seg, &last_seg)) == -1 ) {
	    errstr = newstralloc2(errstr,
				  "reading volume table: ", 
				  strerror(errno));
	    rc = 1;
	    goto common_exit;
	}
	/* set volume label and date for first entry */
	vtbl_no = 0;
	if(set_label(label, volumes, num_volumes, vtbl_no)){
	    errstr = newstralloc2(errstr,
				  "setting label for entry 1: ",
				  strerror(errno));
	    rc = 1;
	    goto common_exit;
	}
	/* date of start writing this tape */
	if (set_date(start_datestr, volumes, num_volumes, vtbl_no)){
	    errstr = newstralloc2(errstr,
				  "setting date for entry 1: ", 
				  strerror(errno));
	    rc = 1;
	    goto common_exit;
	}
	/* set volume labels and dates for backup files */
	for (vtbl_no = 1; vtbl_no <= num_volumes - 2; vtbl_no++){ 
	    fprintf(stderr,"taper: label %i: %s, date %s\n", 
		    vtbl_no,
		    vtbl_entry[vtbl_no].label,
		    vtbl_entry[vtbl_no].date);
	    fflush(stderr);
	    if(set_label(vtbl_entry[vtbl_no].label, 
			 volumes, num_volumes, vtbl_no)){
		errstr = newstralloc2(errstr,
				      "setting label for entry i: ", 
				      strerror(errno));
		rc = 1;
		goto common_exit;
	    }
	    if(set_date(vtbl_entry[vtbl_no].date, 
			volumes, num_volumes, vtbl_no)){
		errstr = newstralloc2(errstr,
				      "setting date for entry i: ",
				      strerror(errno));
		rc = 1;
		goto common_exit;
	    }
	}
	/* set volume label and date for last entry */
	vtbl_no = num_volumes - 1;
	if(set_label("AMANDA Tape End", volumes, num_volumes, vtbl_no)){
	    errstr = newstralloc2(errstr,
				  "setting label for last entry: ", 
				  strerror(errno));
	    rc = 1;
	    goto common_exit;
	}
	datestr = NULL; /* take current time */ 
	if (set_date(datestr, volumes, num_volumes, vtbl_no)){
	    errstr = newstralloc2(errstr,
				  "setting date for last entry 1: ", 
				  strerror(errno));
	    rc = 1;
	    goto common_exit;
	}
	/* write volume table back */
	if (write_vtbl(tape_fd, volumes, vtbl_buffer, num_volumes, first_seg,
		       op_mode == trunc)) {
	    errstr = newstralloc2(errstr,
				  "writing volume table: ", 
				  strerror(errno));
	    rc = 1;
	    goto common_exit;
	}  

	fprintf(stderr, "taper: updating volume table: done.\n");
	fflush(stderr);
#endif /* HAVE_LIBVTBLC */
    }
#endif /* !HAVE_LINUX_ZFTAPE_H */

    /* close the tape and let the OS write the final filemarks */

common_exit:

    if(tape_fd >= 0 && tapefd_close(tape_fd) == -1 && ! writerror) {
	errstr = newstralloc2(errstr, "closing tape: ", strerror(errno));
	rc = 1;
    }
    tape_fd = -1;
    amfree(label);

    return rc;
}


int write_filemark()
{
    if(tapefd_weof(tape_fd, 1) == -1) {
	errstr = newstralloc2(errstr, "writing filemark: ", strerror(errno));
	return 0;
    }
    total_tape_fm++;
    return 1;
}


/*
 * ========================================================================
 * TAPE CHANGER SCAN
 *
 */
int nslots, backwards, found, got_match, tapedays;
char *first_match_label = NULL, *first_match = NULL, *found_device = NULL;
char *searchlabel, *labelstr;
tape_t *tp;

int scan_init(rc, ns, bk)
int rc, ns, bk;
{
    if(rc) {
	fprintf(stderr, "%s: could not get changer info: %s\n",
		get_pname(), changer_resultstr);
	return rc;
    }

    nslots = ns;
    backwards = bk;

    return 0;
}

int taperscan_slot(rc, slotstr, device)
     int rc;
     char *slotstr;
     char *device;
{
    char *t_errstr;
    char *scan_datestamp = NULL;

    if(rc == 2) {
	fprintf(stderr, "%s: fatal slot %s: %s\n",
		get_pname(), slotstr, changer_resultstr);
	fflush(stderr);
	return 1;
    }
    else if(rc == 1) {
	fprintf(stderr, "%s: slot %s: %s\n", get_pname(),
		slotstr, changer_resultstr);
	fflush(stderr);
	return 0;
    }
    else {
	if((t_errstr = tape_rdlabel(device, &scan_datestamp, &label)) != NULL) {
	    amfree(scan_datestamp);
	    fprintf(stderr, "%s: slot %s: %s\n",
		    get_pname(), slotstr, t_errstr);
	    fflush(stderr);
	}
	else {
	    /* got an amanda tape */
	    fprintf(stderr, "%s: slot %s: date %-8s label %s",
		    get_pname(), slotstr, scan_datestamp, label);
	    fflush(stderr);
	    amfree(scan_datestamp);
	    if(searchlabel != NULL
	       && (strcmp(label, FAKE_LABEL) == 0
		   || strcmp(label, searchlabel) == 0)) {
		/* it's the one we are looking for, stop here */
		fprintf(stderr, " (exact label match)\n");
		fflush(stderr);
		found_device = newstralloc(found_device, device);
		found = 1;
		return 1;
	    }
	    else if(!match(labelstr, label)) {
		fprintf(stderr, " (no match)\n");
		fflush(stderr);
	    }
	    else {
		/* not an exact label match, but a labelstr match */
		/* check against tape list */
		tp = lookup_tapelabel(label);
		if(tp == NULL) {
		    fprintf(stderr, "(not in tapelist)\n");
		    fflush(stderr);
		}
		else if(!reusable_tape(tp)) {
		    fprintf(stderr, " (active tape)\n");
		    fflush(stderr);
		}
		else if(got_match == 0 && tp->datestamp == 0) {
		    got_match = 1;
		    first_match = newstralloc(first_match, slotstr);
		    first_match_label = newstralloc(first_match_label, label);
		    fprintf(stderr, " (new tape)\n");
		    fflush(stderr);
		    found = 3;
		    found_device = newstralloc(found_device, device);
		    return 1;
		}
		else if(got_match) {
		    fprintf(stderr, " (labelstr match)\n");
		    fflush(stderr);
		}
		else {
		    got_match = 1;
		    first_match = newstralloc(first_match, slotstr);
		    first_match_label = newstralloc(first_match_label, label);
		    fprintf(stderr, " (first labelstr match)\n");
		    fflush(stderr);
		    if(!backwards || !searchlabel) {
			found = 2;
			found_device = newstralloc(found_device, device);
			return 1;
		    }
		}
	    }
	}
    }
    return 0;
}

char *taper_scan()
{
    char *outslot = NULL;

    if((tp = lookup_last_reusable_tape(0)) == NULL)
	searchlabel = NULL;
    else
	searchlabel = tp->label;

    found = 0;
    got_match = 0;

    if (searchlabel != NULL)
      changer_find(scan_init, taperscan_slot, searchlabel);
    else
      changer_scan(scan_init, taperscan_slot);

    if(found == 2 || found == 3)
	searchlabel = first_match_label;
    else if(!found && got_match) {
	searchlabel = first_match_label;
	amfree(found_device);
	if(changer_loadslot(first_match, &outslot, &found_device) == 0) {
	    found = 1;
	}
	amfree(outslot);
    }
    else if(!found) {
	if(searchlabel) {
	    changer_resultstr = newvstralloc(changer_resultstr,
					     "label ", searchlabel,
					     " or new tape not found in rack",
					     NULL);
	} else {
	    changer_resultstr = newstralloc(changer_resultstr,
					    "new tape not found in rack");
	}
    }

    if(found) {
	outslot = found_device;
	found_device = NULL;		/* forget about our copy */
    } else {
	outslot = NULL;
	amfree(found_device);
    }
    return outslot;
}
