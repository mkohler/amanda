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
 * $Id: sendbackup.c,v 1.44.2.9.4.4.2.16 2004/01/14 12:59:12 martinea Exp $
 *
 * common code for the sendbackup-* programs.
 */

#include "amanda.h"
#include "sendbackup.h"
#include "clock.h"
#include "pipespawn.h"
#include "amfeatures.h"
#include "stream.h"
#include "arglist.h"
#include "getfsent.h"
#include "version.h"

#define TIMEOUT 30

int comppid = -1;
int dumppid = -1;
int tarpid = -1;
int encpid = -1;
int indexpid = -1;
char *errorstr = NULL;

int data_socket, data_port, dataf;
int mesg_socket, mesg_port, mesgf;
int index_socket, index_port, indexf;

option_t *options;

#ifdef KRB4_SECURITY
#include "sendbackup-krb4.h"
#else					/* I'd tell you what this does */
#define NAUGHTY_BITS			/* but then I'd have to kill you */
#endif

long dump_size = -1;

backup_program_t *program = NULL;

static am_feature_t *our_features = NULL;
static char *our_feature_string = NULL;
static g_option_t *g_options = NULL;

/* local functions */
int main P((int argc, char **argv));
char *optionstr P((option_t *options));
char *childstr P((int pid));
int check_status P((int pid, amwait_t w));

int pipefork P((void (*func) P((void)), char *fname, int *stdinfd,
		int stdoutfd, int stderrfd));
void parse_backup_messages P((int mesgin));
static void process_dumpline P((char *str));


char *optionstr(option_t *options)
{
    static char *optstr = NULL;
    char *compress_opt = "";
    char *record_opt = "";
    char *bsd_opt = "";
    char *krb4_opt = "";
    char *kencrypt_opt = "";
    char *index_opt = "";
    char *exclude_file_opt;
    char *exclude_list_opt;
    char *exc = NULL;
    sle_t *excl;

    if(options->compress == COMPR_BEST)
	compress_opt = "compress-best;";
    else if(options->compress == COMPR_FAST)
	compress_opt = "compress-fast;";
    else if(options->compress == COMPR_SERVER_BEST)
	compress_opt = "srvcomp-best;";
    else if(options->compress == COMPR_SERVER_FAST)
	compress_opt = "srvcomp-fast;";
    if(options->no_record) record_opt = "no-record;";
    if(options->bsd_auth) bsd_opt = "bsd-auth;";
#ifdef KRB4_SECURITY
    if(options->krb4_auth) krb4_opt = "krb4-auth;";
    if(options->kencrypt) kencrypt_opt = "kencrypt;";
#endif
    if(options->createindex) index_opt = "index;";

    exclude_file_opt = stralloc("");
    if(options->exclude_file) {
	for(excl = options->exclude_file->first; excl != NULL; excl=excl->next){
	    exc = newvstralloc(exc, "exclude-file=", excl->name, ";", NULL);
	    strappend(exclude_file_opt, exc);
	}
    }
    exclude_list_opt = stralloc("");
    if(options->exclude_list) {
	for(excl = options->exclude_list->first; excl != NULL; excl=excl->next){
	    exc = newvstralloc(exc, "exclude-list=", excl->name, ";", NULL);
	    strappend(exclude_list_opt, exc);
	}
    }
    optstr = newvstralloc(optstr,
			  compress_opt,
			  record_opt,
			  bsd_opt,
			  krb4_opt,
			  kencrypt_opt,
			  index_opt,
			  exclude_file_opt,
			  exclude_list_opt,
			  NULL);
    return optstr;
}


int main(argc, argv)
int argc;
char **argv;
{
    int interactive = 0;
    int level, mesgpipe[2];
    char *prog, *disk, *amdevice, *dumpdate, *stroptions;
    char *line = NULL;
    char *err_extra = NULL;
    char *s;
    int i;
    int ch;
    unsigned long malloc_hist_1, malloc_size_1;
    unsigned long malloc_hist_2, malloc_size_2;
    int fd;

    /* initialize */

    for(fd = 3; fd < FD_SETSIZE; fd++) {
	/*
	 * Make sure nobody spoofs us with a lot of extra open files
	 * that would cause an open we do to get a very high file
	 * descriptor, which in turn might be used as an index into
	 * an array (e.g. an fd_set).
	 */
#ifdef KRB4_SECURITY
	if (fd != KEY_PIPE)	/* XXX interface needs to be fixed */
#endif
		close(fd);
    }

    safe_cd();

    set_pname("sendbackup");

    malloc_size_1 = malloc_inuse(&malloc_hist_1);

    interactive = (argc > 1 && strcmp(argv[1],"-t") == 0);
    erroutput_type = (ERR_INTERACTIVE|ERR_SYSLOG);
    dbopen();
    startclock();
    dbprintf(("%s: version %s\n", argv[0], version()));

    our_features = am_init_feature_set();
    our_feature_string = am_feature_to_string(our_features);

    if(interactive) {
	/*
	 * In interactive (debug) mode, the backup data is sent to
	 * /dev/null and none of the network connections back to driver
	 * programs on the tape host are set up.  The index service is
	 * run and goes to stdout.
	 */
	fprintf(stderr, "%s: running in interactive test mode\n", get_pname());
	fflush(stderr);
    }

    prog = NULL;
    disk = NULL;
    amdevice = NULL;
    dumpdate = NULL;
    stroptions = NULL;

    /* parse dump request */

    for(; (line = agets(stdin)) != NULL; free(line)) {
	if(interactive) {
	    fprintf(stderr, "%s> ", get_pname());
	    fflush(stderr);
	}
#define sc "OPTIONS "
	if(strncmp(line, sc, sizeof(sc)-1) == 0) {
#undef sc
	    g_options = parse_g_options(line+8, 1);
	    if(!g_options->hostname) {
		g_options->hostname = alloc(MAX_HOSTNAME_LENGTH+1);
		gethostname(g_options->hostname, MAX_HOSTNAME_LENGTH);
		g_options->hostname[MAX_HOSTNAME_LENGTH] = '\0';
	    }
	    continue;
	}

	if (prog != NULL) {
	    err_extra = "multiple requests";
	    goto err;
	}

	s = line;
	ch = *s++;

	skip_whitespace(s, ch);			/* find the program name */
	if(ch == '\0') {
	    err_extra = "no program name";
	    goto err;				/* no program name */
	}
	prog = s - 1;
	skip_non_whitespace(s, ch);
	s[-1] = '\0';
	prog = stralloc(prog);

	skip_whitespace(s, ch);			/* find the disk name */
	if(ch == '\0') {
	    err_extra = "no disk name";
	    goto err;				/* no disk name */
	}
	amfree(disk);
	disk = s - 1;
	skip_non_whitespace(s, ch);
	s[-1] = '\0';
	disk = stralloc(disk);

	skip_whitespace(s, ch);			/* find the device or level */
	if (ch == '\0') {
	    err_extra = "bad level";
	    goto err;
	}

	if(!isdigit((int)s[-1])) {
	    amfree(amdevice);
	    amdevice = s - 1;
	    skip_non_whitespace(s, ch);
	    s[-1] = '\0';
	    amdevice = stralloc(amdevice);
	    skip_whitespace(s, ch);		/* find level number */
	}
	else {
	    amdevice = stralloc(disk);
	}

						/* find the level number */
	if(ch == '\0' || sscanf(s - 1, "%d", &level) != 1) {
	    err_extra = "bad level";
	    goto err;				/* bad level */
	}
	skip_integer(s, ch);

	skip_whitespace(s, ch);			/* find the dump date */
	if(ch == '\0') {
	    err_extra = "no dumpdate";
	    goto err;				/* no dumpdate */
	}
	amfree(dumpdate);
	dumpdate = s - 1;
	skip_non_whitespace(s, ch);
	s[-1] = '\0';
	dumpdate = stralloc(dumpdate);

	skip_whitespace(s, ch);			/* find the options keyword */
	if(ch == '\0') {
	    err_extra = "no options";
	    goto err;				/* no options */
	}
#define sc "OPTIONS "
	if(strncmp(s - 1, sc, sizeof(sc)-1) != 0) {
	    err_extra = "no OPTIONS keyword";
	    goto err;				/* no options */
	}
	s += sizeof(sc)-1;
	ch = s[-1];
#undef sc
	skip_whitespace(s, ch);			/* find the options string */
	if(ch == '\0') {
	    err_extra = "bad options string";
	    goto err;				/* no options */
	}
	amfree(stroptions);
	stroptions = stralloc(s - 1);
    }
    amfree(line);

    dbprintf(("  parsed request as: program `%s'\n", prog));
    dbprintf(("                     disk `%s'\n", disk));
    dbprintf(("                     device `%s'\n", amdevice));
    dbprintf(("                     level %d\n", level));
    dbprintf(("                     since %s\n", dumpdate));
    dbprintf(("                     options `%s'\n", stroptions));

    for(i = 0; programs[i] != NULL; i++) {
	if (strcmp(programs[i]->name, prog) == 0) {
	    break;
	}
    }
    if (programs[i] == NULL) {
	error("ERROR [%s: unknown program %s]", get_pname(), prog);
    }
    program = programs[i];

    options = parse_options(stroptions, disk, amdevice, g_options->features, 0);

#ifdef KRB4_SECURITY
    /* modification by BIS@BBN 4/25/2003:
     * with the option processing changes in amanda 2.4.4, must change
     * the conditional from krb4_auth to options->krb4_auth */
    if(options->krb4_auth) {
	if(read(KEY_PIPE, session_key, sizeof session_key) 
	   != sizeof session_key) {
	  error("ERROR [%s: could not read session key]", get_pname());
	}
    }
#endif

    if(!interactive) {
      data_socket = stream_server(&data_port, STREAM_BUFSIZE, -1);
      if(data_socket < 0) {
	error("ERROR [%s: could not create data socket: %s]",
	      get_pname(), strerror(errno));
      }
      mesg_socket = stream_server(&mesg_port, -1, -1);
      if(mesg_socket < 0) {
	error("ERROR [%s: could not create mesg socket: %s]",
	      get_pname(), strerror(errno));
      }
    }
    if (!interactive && options->createindex) {
      index_socket = stream_server(&index_port, -1, -1);
      if(index_socket < 0) {
	error("ERROR [%s: could not create index socket: %s]",
	      get_pname(), strerror(errno));
      }
    } else {
      index_port = -1;
    }

    printf("CONNECT DATA %d MESG %d INDEX %d\n",
	   data_port, mesg_port, index_port);
    printf("OPTIONS ");
    if(am_has_feature(g_options->features, fe_rep_options_features)) {
	printf("features=%s;", our_feature_string);
    }
    if(am_has_feature(g_options->features, fe_rep_options_hostname)) {
	printf("hostname=%s;", g_options->hostname);
    }
    if(am_has_feature(g_options->features, fe_rep_options_sendbackup_options)) {
	printf("%s", optionstr(options));
    }
    printf("\n");
    fflush(stdout);
    freopen("/dev/null", "w", stdout);

    if (options->createindex)
      dbprintf(("%s: waiting for connect on %d, then %d, then %d\n",
		debug_prefix_time(NULL), data_port, mesg_port, index_port));
    else
      dbprintf(("%s: waiting for connect on %d, then %d\n",
		debug_prefix_time(NULL), data_port, mesg_port));

    if(interactive) {
      if((dataf = open("/dev/null", O_RDWR)) < 0) {
	error("ERROR [%s: open of /dev/null for debug data stream: %s]",
	      get_pname(), strerror(errno));
      }
      mesgf = 2;
    } else {
      dataf = stream_accept(data_socket, TIMEOUT, -1, -1);
      if(dataf == -1) {
	dbprintf(("%s: timeout on data port %d\n",
		  debug_prefix_time(NULL), data_port));
      }
      mesgf = stream_accept(mesg_socket, TIMEOUT, -1, -1);
      if(mesgf == -1) {
        dbprintf(("%s: timeout on mesg port %d\n",
		  debug_prefix_time(NULL), mesg_port));
      }
    }
    if(interactive) {
      indexf = 1;
    } else if (options->createindex) {
      indexf = stream_accept(index_socket, TIMEOUT, -1, -1);
      if (indexf == -1) {
	dbprintf(("%s: timeout on index port %d\n",
		  debug_prefix_time(NULL), index_port));
      }
    }

    if(!interactive) {
      if(dataf == -1 || mesgf == -1 || (options->createindex && indexf == -1)) {
        dbclose();
        exit(1);
      }
    }

    dbprintf(("%s: got all connections\n", debug_prefix_time(NULL)));

#ifdef KRB4_SECURITY
    if(!interactive) {
      /* modification by BIS@BBN 4/25/2003:
       * with the option processing changes in amanda 2.4.4, must change
       * the conditional from krb4_auth to options->krb4_auth */
      if (options->krb4_auth) {
        if(kerberos_handshake(dataf, session_key) == 0) {
	    dbprintf(("%s: kerberos_handshake on data socket failed\n",
		      debug_prefix_time(NULL)));
	    dbclose();
	    exit(1);
        } else {
	    dbprintf(("%s: kerberos_handshake on data socket succeeded\n",
		      debug_prefix_time(NULL)));

	}

        if(kerberos_handshake(mesgf, session_key) == 0) {
	    dbprintf(("%s: kerberos_handshake on mesg socket failed\n",
		      debug_prefix_time(NULL)));
	    dbclose();
	    exit(1);
        } else {
	    dbprintf(("%s: kerberos_handshake on mesg socket succeeded\n",
		      debug_prefix_time(NULL)));

	}

        dbprintf(("%s: kerberos handshakes succeeded!\n",
		  debug_prefix_time(NULL)));
      }
    }
#endif

    if(!interactive) {
      /* redirect stderr */
      if(dup2(mesgf, 2) == -1) {
	  dbprintf(("%s: error redirecting stderr: %s\n",
		    debug_prefix(NULL), strerror(errno)));
	  dbclose();
	  exit(1);
      }
    }

    if(pipe(mesgpipe) == -1) {
      error("error [opening mesg pipe: %s]", strerror(errno));
    }

    program->start_backup(g_options->hostname, disk, amdevice, level, dumpdate,
			  dataf, mesgpipe[1], indexf);
    parse_backup_messages(mesgpipe[0]);

    amfree(prog);
    amfree(disk);
    amfree(amdevice);
    amfree(dumpdate);
    amfree(stroptions);
    amfree(our_feature_string);
    am_release_feature_set(our_features);
    our_features = NULL;
    am_release_feature_set(g_options->features);
    g_options->features = NULL;
    amfree(g_options->hostname);
    amfree(g_options->str);
    amfree(g_options);

    dbclose();

    malloc_size_2 = malloc_inuse(&malloc_hist_2);

    if(malloc_size_1 != malloc_size_2) {
	malloc_list(fileno(stderr), malloc_hist_1, malloc_hist_2);
    }

    return 0;

 err:
    printf("FORMAT ERROR IN REQUEST PACKET\n");
    dbprintf(("%s: REQ packet is bogus%s%s\n",
	      debug_prefix_time(NULL),
	      err_extra ? ": " : "",
	      err_extra ? err_extra : ""));
    dbclose();
    return 1;
}

char *childstr(pid)
int pid;
/*
 * Returns a string for a child process.  Checks the saved dump and
 * compress pids to see which it is.
 */
{
    if(pid == dumppid) return program->backup_name;
    if(pid == comppid) return "compress";
    if(pid == encpid)  return "kencrypt";
    if(pid == indexpid) return "index";
    return "unknown";
}


int check_status(pid, w)
int pid;
amwait_t w;
/*
 * Determine if the child return status really indicates an error.
 * If so, add the error message to the error string; more than one
 * child can have an error.
 */
{
    char *thiserr = NULL;
    char *str;
    int ret, sig, rc;
    char number[NUM_STR_SIZE];

    str = childstr(pid);

    if(WIFSIGNALED(w)) {
	ret = 0;
	rc = sig = WTERMSIG(w);
    } else {
	sig = 0;
	rc = ret = WEXITSTATUS(w);
    }

    if(pid == indexpid) {
	/*
	 * Treat an index failure (other than signal) as a "STRANGE"
	 * rather than an error so the dump goes ahead and gets processed
	 * but the failure is noted.
	 */
	if(ret != 0) {
	    fprintf(stderr, "? %s returned %d\n", str, ret);
	    rc = 0;
	}
    }

#ifndef HAVE_GZIP
    if(pid == comppid) {
	/*
	 * compress returns 2 sometimes, but it is ok.
	 */
	if(ret == 2) {
	    rc = 0;
	}
    }
#endif

#ifdef DUMP_RETURNS_1
    if(pid == dumppid && tarpid == -1) {
        /*
	 * Ultrix dump returns 1 sometimes, but it is ok.
	 */
        if(ret == 1) {
	    rc = 0;
	}
    }
#endif

#ifdef IGNORE_TAR_ERRORS
    if(pid == tarpid) {
	/*
	 * tar bitches about active filesystems, but we do not care.
	 */
        if(ret == 2) {
	    rc = 0;
	}
    }
#endif

    if(rc == 0) {
	return 0;				/* normal exit */
    }

    if(ret == 0) {
	ap_snprintf(number, sizeof(number), "%d", sig);
	thiserr = vstralloc(str, " got signal ", number, NULL);
    } else {
	ap_snprintf(number, sizeof(number), "%d", ret);
	thiserr = vstralloc(str, " returned ", number, NULL);
    }

    if(errorstr) {
	strappend(errorstr, ", ");
	strappend(errorstr, thiserr);
	amfree(thiserr);
    } else {
	errorstr = thiserr;
	thiserr = NULL;
    }
    return 1;
}


/* Send header info to the message file.
*/
void write_tapeheader()
{
    fprintf(stderr, "%s: info BACKUP=%s\n", get_pname(), program->backup_name);

    fprintf(stderr, "%s: info RECOVER_CMD=", get_pname());
    if (options->compress == COMPR_FAST || options->compress == COMPR_BEST)
	fprintf(stderr, "%s %s |", UNCOMPRESS_PATH,
#ifdef UNCOMPRESS_OPT
		UNCOMPRESS_OPT
#else
		""
#endif
		);

    fprintf(stderr, "%s -f... -\n", program->restore_name);

    if (options->compress == COMPR_FAST || options->compress == COMPR_BEST)
	fprintf(stderr, "%s: info COMPRESS_SUFFIX=%s\n",
			get_pname(), COMPRESS_SUFFIX);

    fprintf(stderr, "%s: info end\n", get_pname());
}

int pipefork(func, fname, stdinfd, stdoutfd, stderrfd)
void (*func) P((void));
char *fname;
int *stdinfd;
int stdoutfd, stderrfd;
{
    int pid, inpipe[2];

    dbprintf(("%s: forking function %s in pipeline\n",
	      debug_prefix_time(NULL), fname));

    if(pipe(inpipe) == -1) {
	error("error [open pipe to %s: %s]", fname, strerror(errno));
    }

    switch(pid = fork()) {
    case -1:
	error("error [fork %s: %s]", fname, strerror(errno));
    default:	/* parent process */
	aclose(inpipe[0]);	/* close input side of pipe */
	*stdinfd = inpipe[1];
	break;
    case 0:		/* child process */
	aclose(inpipe[1]);	/* close output side of pipe */

	if(dup2(inpipe[0], 0) == -1) {
	    error("error [dup2 0 %s: dup2 in: %s]", fname, strerror(errno));
	}
	if(dup2(stdoutfd, 1) == -1) {
	    error("error [dup2 1 %s: dup2 out: %s]", fname, strerror(errno));
	}
	if(dup2(stderrfd, 2) == -1) {
	    error("error [dup2 2 %s: dup2 err: %s]", fname, strerror(errno));
	}

	func();
	exit(0);
	/* NOTREACHED */
    }
    return pid;
}

void parse_backup_messages(mesgin)
int mesgin;
{
    int goterror, wpid;
    amwait_t retstat;
    char *line;

    goterror = 0;
    amfree(errorstr);

    for(; (line = areads(mesgin)) != NULL; free(line)) {
	process_dumpline(line);
    }

    if(errno) {
	error("error [read mesg pipe: %s]", strerror(errno));
    }

    while((wpid = wait(&retstat)) != -1) {
	if(check_status(wpid, retstat)) goterror = 1;
    }

    if(errorstr) {
	error("error [%s]", errorstr);
    } else if(dump_size == -1) {
	error("error [no backup size line]");
    }

    program->end_backup(goterror);

    fprintf(stderr, "%s: size %ld\n", get_pname(), dump_size);
    fprintf(stderr, "%s: end\n", get_pname());
}


double first_num P((char *str));

double first_num(str)
char *str;
/*
 * Returns the value of the first integer in a string.
 */
{
    char *num;
    int ch;
    double d;

    ch = *str++;
    while(ch && !isdigit(ch)) ch = *str++;
    num = str - 1;
    while(isdigit(ch) || ch == '.') ch = *str++;
    str[-1] = '\0';
    d = atof(num);
    str[-1] = ch;
    return d;
}

static void process_dumpline(str)
char *str;
{
    regex_t *rp;
    char *type;
    char startchr;

    for(rp = program->re_table; rp->regex != NULL; rp++) {
	if(match(rp->regex, str)) {
	    break;
	}
    }
    if(rp->typ == DMP_SIZE) {
	dump_size = (long)((first_num(str) * rp->scale + 1023.0)/1024.0);
    }
    switch(rp->typ) {
    case DMP_NORMAL:
	type = "normal";
	startchr = '|';
	break;
    case DMP_STRANGE:
	type = "strange";
	startchr = '?';
	break;
    case DMP_SIZE:
	type = "size";
	startchr = '|';
	break;
    case DMP_ERROR:
	type = "error";
	startchr = '?';
	break;
    default:
	/*
	 * Should never get here.
	 */
	type = "unknown";
	startchr = '!';
	break;
    }
    dbprintf(("%s: %3d: %7s(%c): %s\n",
	      debug_prefix_time(NULL),
	      rp->srcline,
	      type,
	      startchr,
	      str));
    fprintf(stderr, "%c %s\n", startchr, str);
}


/* start_index.  Creates an index file from the output of dump/tar.
   It arranges that input is the fd to be written by the dump process.
   If createindex is not enabled, it does nothing.  If it is not, a
   new process will be created that tees input both to a pipe whose
   read fd is dup2'ed input and to a program that outputs an index
   file to `index'.

   make sure that the chat from restore doesn't go to stderr cause
   this goes back to amanda which doesn't expect to see it
   (2>/dev/null should do it)

   Originally by Alan M. McIvor, 13 April 1996

   Adapted by Alexandre Oliva, 1 May 1997

   This program owes a lot to tee.c from GNU sh-utils and dumptee.c
   from the DeeJay backup package.

*/

static volatile int index_finished = 0;

static void index_closed(sig)
int sig;
{
  index_finished = 1;
}

void save_fd(fd, min)
int *fd, min;
{
  int origfd = *fd;

  while (*fd >= 0 && *fd < min) {
    int newfd = dup(*fd);
    if (newfd == -1)
      dbprintf(("%s: unable to save file descriptor [%s]\n",
		debug_prefix(NULL), strerror(errno)));
    *fd = newfd;
  }
  if (origfd != *fd)
    dbprintf(("%s: dupped file descriptor %i to %i\n",
	      debug_prefix(NULL), origfd, *fd));
}

void start_index(createindex, input, mesg, index, cmd)
int createindex, input, mesg, index;
char *cmd;
{
  struct sigaction act, oact;
  int pipefd[2];
  FILE *pipe_fp;
  int exitcode;

  if (!createindex)
    return;

  if (pipe(pipefd) != 0) {
    error("creating index pipe: %s", strerror(errno));
  }

  switch(indexpid = fork()) {
  case -1:
    error("forking index tee process: %s", strerror(errno));

  default:
    aclose(pipefd[0]);
    if (dup2(pipefd[1], input) == -1) {
      error("dup'ping index tee output: %s", strerror(errno));
    }
    aclose(pipefd[1]);
    return;

  case 0:
    break;
  }

  /* now in a child process */
  save_fd(&pipefd[0], 4);
  save_fd(&index, 4);
  save_fd(&mesg, 4);
  save_fd(&input, 4);
  dup2(pipefd[0], 0);
  dup2(index, 1);
  dup2(mesg, 2);
  dup2(input, 3);
  for(index = 4; index < FD_SETSIZE; index++) {
    if (index != dbfd()) {
      close(index);
    }
  }

  /* set up a signal handler for SIGPIPE for when the pipe is finished
     creating the index file */
  /* at that point we obviously want to stop writing to it */
  act.sa_handler = index_closed;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if (sigaction(SIGPIPE, &act, &oact) != 0) {
    error("couldn't set index SIGPIPE handler [%s]", strerror(errno));
  }

  if ((pipe_fp = popen(cmd, "w")) == NULL) {
    error("couldn't start index creator [%s]", strerror(errno));
  }

  dbprintf(("%s: started index creator: \"%s\"\n",
	    debug_prefix_time(NULL), cmd));
  while(1) {
    char buffer[BUFSIZ], *ptr;
    int bytes_read;
    int bytes_written;
    int just_written;

    bytes_read = read(0, buffer, sizeof(buffer));
    if ((bytes_read < 0) && (errno == EINTR))
      continue;

    if (bytes_read < 0) {
      error("index tee cannot read [%s]", strerror(errno));
    }

    if (bytes_read == 0)
      break; /* finished */

    /* write the stuff to the subprocess */
    ptr = buffer;
    bytes_written = 0;
    while (bytes_read > bytes_written && !index_finished) {
      just_written = write(fileno(pipe_fp), ptr, bytes_read - bytes_written);
      if (just_written < 0) {
	  /* the signal handler may have assigned to index_finished
	   * just as we waited for write() to complete. */
	  if (!index_finished) {
	      dbprintf(("%s: index tee cannot write to index creator [%s]\n",
			debug_prefix_time(NULL), strerror(errno)));
	      index_finished = 1;
	}
      } else {
	bytes_written += just_written;
	ptr += just_written;
      }
    }

    /* write the stuff to stdout, ensuring none lost when interrupt
       occurs */
    ptr = buffer;
    bytes_written = 0;
    while (bytes_read > bytes_written) {
      just_written = write(3, ptr, bytes_read - bytes_written);
      if ((just_written < 0) && (errno == EINTR))
	continue;
      if (just_written < 0) {
	error("index tee cannot write [%s]", strerror(errno));
      } else {
	bytes_written += just_written;
	ptr += just_written;
      }
    }
  }

  aclose(pipefd[1]);

  /* finished */
  /* check the exit code of the pipe and moan if not 0 */
  if ((exitcode = pclose(pipe_fp)) != 0) {
    dbprintf(("%s: index pipe returned %d\n",
	      debug_prefix_time(NULL), exitcode));
  } else {
    dbprintf(("%s: index created successfully\n", debug_prefix_time(NULL)));
  }
  pipe_fp = NULL;

  exit(exitcode);
}

extern backup_program_t dump_program, gnutar_program;

backup_program_t *programs[] = {
  &dump_program, &gnutar_program, NULL
};

#ifdef KRB4_SECURITY
#include "sendbackup-krb4.c"
#endif
