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
/*
 * $Id: amrecover.c,v 1.29.4.7.4.6.2.7 2003/01/04 04:33:32 martinea Exp $
 *
 * an interactive program for recovering backed-up files
 */

#include "amanda.h"
#include "version.h"
#ifdef HAVE_NETINET_IN_SYSTM_H
#include <netinet/in_systm.h>
#endif
#include <netinet/in.h>
#ifdef HAVE_NETINET_IP_H
#include <netinet/ip.h>
#endif
#include "stream.h"
#include "amfeatures.h"
#include "amrecover.h"
#include "getfsent.h"
#include "dgram.h"

#if defined(KRB4_SECURITY)
#include "krb4-security.h"
#endif
#include "util.h"

#ifdef HAVE_LIBREADLINE
#  ifdef HAVE_READLINE_READLINE_H
#    include <readline/readline.h>
#    ifdef HAVE_READLINE_HISTORY_H
#      include <readline/history.h>
#    endif
#  else
#    ifdef HAVE_READLINE_H
#      include <readline.h>
#      ifdef HAVE_HISTORY_H
#        include <history.h>
#      endif
#    else
#      undef HAVE_LIBREADLINE
#    endif
#  endif
#endif

extern int process_line P((char *line));
int guess_disk P((char *cwd, size_t cwd_len, char **dn_guess, char **mpt_guess));

#define USAGE "Usage: amrecover [[-C] <config>] [-s <index-server>] [-t <tape-server>] [-d <tape-device>]\n"

char *config = NULL;
char *server_name = NULL;
int server_socket;
char *server_line = NULL;
char *dump_datestamp = NULL;		/* date we are restoring */
char *dump_hostname;			/* which machine we are restoring */
char *disk_name = NULL;			/* disk we are restoring */
char *mount_point = NULL;		/* where disk was mounted */
char *disk_path = NULL;			/* path relative to mount point */
char dump_date[STR_SIZE];		/* date on which we are restoring */
int quit_prog;				/* set when time to exit parser */
char *tape_server_name = NULL;
int tape_server_socket;
char *tape_device_name = NULL;
am_feature_t *our_features = NULL;
am_feature_t *their_features = NULL;


#ifndef HAVE_LIBREADLINE
/*
 * simple readline() replacements
 */

char *
readline(prompt)
char *prompt;
{
    printf("%s",prompt);
    fflush(stdout); fflush(stderr);
    return agets(stdin);
}

#define add_history(x)                /* add_history((x)) */

#endif

/* gets a "line" from server and put in server_line */
/* server_line is terminated with \0, \r\n is striped */
/* returns -1 if error */

int get_line ()
{
    char *line = NULL;
    char *part = NULL;
    size_t len;

    while(1) {
	if((part = areads(server_socket)) == NULL) {
	    int save_errno = errno;

	    if(server_line) {
		fputs(server_line, stderr);	/* show the last line read */
		fputc('\n', stderr);
	    }
	    if(save_errno != 0) {
		fprintf(stderr, "%s: Error reading line from server: %s\n",
				get_pname(),
				strerror(save_errno));
	    } else {
		fprintf(stderr, "%s: Unexpected end of file, check amindexd*debug on server %s\n",
			get_pname(),
			server_name);
	    }
	    amfree(line);
	    amfree(server_line);
	    errno = save_errno;
	    return -1;
	}
	if(line) {
	    strappend(line, part);
	    amfree(part);
	} else {
	    line = part;
	    part = NULL;
	}
	if((len = strlen(line)) > 0 && line[len-1] == '\r') {
	    line[len-1] = '\0';
	    server_line = newstralloc(server_line, line);
	    amfree(line);
	    return 0;
	}
	/*
	 * Hmmm.  We got a "line" from areads(), which means it saw
	 * a '\n' (or EOF, etc), but there was not a '\r' before it.
	 * Put a '\n' back in the buffer and loop for more.
	 */
	strappend(line, "\n");
    }
}


/* get reply from server and print to screen */
/* handle multi-line reply */
/* return -1 if error */
/* return code returned by server always occupies first 3 bytes of global
   variable server_line */
int grab_reply (show)
int show;
{
    do {
	if (get_line() == -1) {
	    return -1;
	}
	if(show) puts(server_line);
    } while (server_line[3] == '-');
    if(show) fflush(stdout);

    return 0;
}


/* get 1 line of reply */
/* returns -1 if error, 0 if last (or only) line, 1 if more to follow */
int get_reply_line ()
{
    if (get_line() == -1)
	return -1;
    return server_line[3] == '-';
}


/* returns pointer to returned line */
char *reply_line ()
{
    return server_line;
}



/* returns 0 if server returned an error code (ie code starting with 5)
   and non-zero otherwise */
int server_happy ()
{
    return server_line[0] != '5';
}


int send_command(cmd)
char *cmd;
{
    size_t l, n;
    ssize_t s;
    char *end;

    /*
     * NOTE: this routine is called from sigint_handler, so we must be
     * **very** careful about what we do since there is no way to know
     * our state at the time the interrupt happened.  For instance,
     * do not use any stdio routines here.
     */
    for (l = 0, n = strlen(cmd); l < n; l += s)
	if ((s = write(server_socket, cmd + l, n - l)) < 0) {
	    perror("amrecover: Error writing to server");
	    return -1;
	}
    end = "\r\n";
    for (l = 0, n = strlen(end); l < n; l += s)
	if ((s = write(server_socket, end + l, n - l)) < 0) {
	    perror("amrecover: Error writing to server");
	    return -1;
	}
    return 0;
}


/* send a command to the server, get reply and print to screen */
int converse(cmd)
char *cmd;
{
    if (send_command(cmd) == -1) return -1;
    if (grab_reply(1) == -1) return -1;
    return 0;
}


/* same as converse() but reply not echoed to stdout */
int exchange(cmd)
char *cmd;
{
    if (send_command(cmd) == -1) return -1;
    if (grab_reply(0) == -1) return -1;
    return 0;
}


/* basic interrupt handler for when user presses ^C */
/* Bale out, letting server know before doing so */
void sigint_handler(signum)
int signum;
{
    /*
     * NOTE: we must be **very** careful about what we do here since there
     * is no way to know our state at the time the interrupt happened.
     * For instance, do not use any stdio routines here or in any called
     * routines.  Also, use _exit() instead of exit() to make sure stdio
     * buffer flushing is not attempted.
     */
    if (extract_restore_child_pid != -1)
	(void)kill(extract_restore_child_pid, SIGKILL);
    extract_restore_child_pid = -1;

    (void)send_command("QUIT");
    _exit(1);
}


void clean_pathname(s)
char *s;
{
    size_t length;
    length = strlen(s);

    /* remove "/" at end of path */
    if(length>1 && s[length-1]=='/')
	s[length-1]='\0';

    /* change "/." to "/" */
    if(strcmp(s,"/.")==0)
	s[1]='\0';

    /* remove "/." at end of path */
    if(strcmp(&(s[length-2]),"/.")==0)
	s[length-2]='\0';
}


/* try and guess the disk the user is currently on.
   Return -1 if error, 0 if disk not local, 1 if disk local,
   2 if disk local but can't guess name */
/* do this by looking for the longest mount point which matches the
   current directory */
int guess_disk (cwd, cwd_len, dn_guess, mpt_guess)
     char *cwd, **dn_guess, **mpt_guess;
     size_t cwd_len;
{
    size_t longest_match = 0;
    size_t current_length;
    size_t cwd_length;
    int local_disk = 0;
    generic_fsent_t fsent;
    char *fsname = NULL;
    char *disk_try = NULL;

    *dn_guess = *mpt_guess = NULL;

    if (getcwd(cwd, cwd_len) == NULL)
	return -1;
    cwd_length = strlen(cwd);
    dbprintf(("guess_disk: %d: \"%s\"\n", cwd_length, cwd));

    if (open_fstab() == 0)
	return -1;

    while (get_fstab_nextentry(&fsent))
    {
	current_length = fsent.mntdir ? strlen(fsent.mntdir) : (size_t)0;
	dbprintf(("guess_disk: %d: %d: \"%s\": \"%s\"\n",
		  longest_match,
		  current_length,
		  fsent.mntdir ? fsent.mntdir : "(mntdir null)",
		  fsent.fsname ? fsent.fsname : "(fsname null)"));
	if ((current_length > longest_match)
	    && (current_length <= cwd_length)
	    && (strncmp(fsent.mntdir, cwd, current_length) == 0))
	{
	    longest_match = current_length;
	    amfree(*mpt_guess);
	    *mpt_guess = stralloc(fsent.mntdir);
	    if(strncmp(fsent.fsname,DEV_PREFIX,(strlen(DEV_PREFIX))))
	    {
	        fsname = newstralloc(fsname, fsent.fsname);
            }
	    else
	    {
	        fsname = newstralloc(fsname,fsent.fsname+strlen(DEV_PREFIX));
	    }
	    local_disk = is_local_fstype(&fsent);
	    dbprintf(("guess_disk: local_disk = %d, fsname = \"%s\"\n",
		      local_disk,
		      fsname));
	}
    }
    close_fstab();

    if (longest_match == 0) {
	amfree(*mpt_guess);
	amfree(fsname);
	return -1;			/* ? at least / should match */
    }

    if (!local_disk) {
	amfree(*mpt_guess);
	amfree(fsname);
	return 0;
    }

    /* have mount point now */
    /* disk name may be specified by mount point (logical name) or
       device name, have to determine */
    printf("Trying disk %s ...\n", *mpt_guess);
    disk_try = stralloc2("DISK ", *mpt_guess);		/* try logical name */
    if (exchange(disk_try) == -1)
	exit(1);
    amfree(disk_try);
    if (server_happy())
    {
	*dn_guess = stralloc(*mpt_guess);		/* logical is okay */
	amfree(fsname);
	return 1;
    }
    printf("Trying disk %s ...\n", fsname);
    disk_try = stralloc2("DISK ", fsname);		/* try device name */
    if (exchange(disk_try) == -1)
	exit(1);
    amfree(disk_try);
    if (server_happy())
    {
	*dn_guess = stralloc(fsname);			/* dev name is okay */
	amfree(fsname);
	return 1;
    }

    /* neither is okay */
    amfree(*mpt_guess);
    amfree(fsname);
    return 2;
}


void quit ()
{
    quit_prog = 1;
    (void)converse("QUIT");
}

char *localhost = NULL;

int main(argc, argv)
int argc;
char **argv;
{
    int my_port;
    struct servent *sp;
    int i;
    time_t timer;
    char *lineread = NULL;
    struct sigaction act, oact;
    extern char *optarg;
    extern int optind;
    char cwd[STR_SIZE], *dn_guess = NULL, *mpt_guess = NULL;
    char *service_name;
    char *line = NULL;
    int fd;

    for(fd = 3; fd < FD_SETSIZE; fd++) {
	/*
	 * Make sure nobody spoofs us with a lot of extra open files
	 * that would cause an open we do to get a very high file
	 * descriptor, which in turn might be used as an index into
	 * an array (e.g. an fd_set).
	 */
	close(fd);
    }

    set_pname("amrecover");
    dbopen();

#ifndef IGNORE_UID_CHECK
    if (geteuid() != 0) {
	erroutput_type |= ERR_SYSLOG;
	error("amrecover must be run by root");
    }
#endif

    localhost = alloc(MAX_HOSTNAME_LENGTH+1);
    if (gethostname(localhost, MAX_HOSTNAME_LENGTH) != 0) {
	error("cannot determine local host name\n");
    }
    localhost[MAX_HOSTNAME_LENGTH] = '\0';

    config = newstralloc(config, DEFAULT_CONFIG);
    server_name = newstralloc(server_name, DEFAULT_SERVER);
#ifdef DEFAULT_TAPE_SERVER
    tape_server_name = newstralloc(tape_server_name, DEFAULT_TAPE_SERVER);
#else
    amfree(tape_server_name);
#endif
    if (argc > 1 && argv[1][0] != '-')
    {
	/*
	 * If the first argument is not an option flag, then we assume
	 * it is a configuration name to match the syntax of the other
	 * Amanda utilities.
	 */
	char **new_argv;

	new_argv = (char **) alloc ((argc + 1 + 1) * sizeof (*new_argv));
	new_argv[0] = argv[0];
	new_argv[1] = "-C";
	for (i = 1; i < argc; i++)
	{
	    new_argv[i + 1] = argv[i];
	}
	new_argv[i + 1] = NULL;
	argc++;
	argv = new_argv;
    }
    while ((i = getopt(argc, argv, "C:s:t:d:U")) != EOF)
    {
	switch (i)
	{
	    case 'C':
		config = newstralloc(config, optarg);
		break;

	    case 's':
		server_name = newstralloc(server_name, optarg);
		break;

	    case 't':
		tape_server_name = newstralloc(tape_server_name, optarg);
		break;

	    case 'd':
		tape_device_name = newstralloc(tape_device_name, optarg);
		break;

	    case 'U':
	    case '?':
		(void)printf(USAGE);
		return 0;
	}
    }
    if (optind != argc)
    {
	(void)fprintf(stderr, USAGE);
	exit(1);
    }

    amfree(disk_name);
    amfree(mount_point);
    amfree(disk_path);
    dump_date[0] = '\0';

    /* set up signal handler */
    act.sa_handler = sigint_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(SIGINT, &act, &oact) != 0)
    {
	error("error setting signal handler: %s", strerror(errno));
    }

    service_name = stralloc2("amandaidx", SERVICE_SUFFIX);

    printf("AMRECOVER Version %s. Contacting server on %s ...\n",
	   version(), server_name);  
    if ((sp = getservbyname(service_name, "tcp")) == NULL)
    {
	error("%s/tcp unknown protocol", service_name);
    }
    amfree(service_name);
    server_socket = stream_client_privileged(server_name,
					     ntohs(sp->s_port),
					     -1,
					     -1,
					     &my_port);
    if (server_socket < 0)
    {
	error("cannot connect to %s: %s", server_name, strerror(errno));
    }
    if (my_port >= IPPORT_RESERVED)
    {
	error("did not get a reserved port: %d", my_port);
    }

#if 0
    /*
     * We may need root privilege again later for a reserved port to
     * the tape server, so we will drop down now but might have to
     * come back later.
     */
    setegid(getgid());
    seteuid(getuid());
#endif

    /* get server's banner */
    if (grab_reply(1) == -1)
	exit(1);
    if (!server_happy())
    {
	dbclose();
	aclose(server_socket);
	exit(1);
    }

    /* do the security thing */
#if defined(KRB4_SECURITY)
#if 0 /* not yet implemented */
    if(krb4_auth)
    {
	line = get_krb_security();
    } else
#endif /* 0 */
#endif
    {
	line = get_bsd_security();
    }
    if (converse(line) == -1)
	exit(1);
    if (!server_happy())
	exit(1);
    memset(line, '\0', strlen(line));
    amfree(line);

    /* try to get the features form the server */
    {
	char *our_feature_string = NULL;
	char *their_feature_string = NULL;

	our_features = am_init_feature_set();
	our_feature_string = am_feature_to_string(our_features);
	line = stralloc2("FEATURES ", our_feature_string);
	if(exchange(line) == 0) {
	    their_feature_string = stralloc(server_line+13);
	    their_features = am_string_to_feature(their_feature_string);
	}
	else {
	    their_features = am_set_default_feature_set();
        }
	amfree(our_feature_string);
	amfree(their_feature_string);
	amfree(line);
    }

    /* set the date of extraction to be today */
    (void)time(&timer);
    strftime(dump_date, sizeof(dump_date), "%Y-%m-%d", localtime(&timer));
    printf("Setting restore date to today (%s)\n", dump_date);
    line = stralloc2("DATE ", dump_date);
    if (converse(line) == -1)
	exit(1);
    amfree(line);

    line = stralloc2("SCNF ", config);
    if (converse(line) == -1)
	exit(1);
    amfree(line);

    if (server_happy())
    {
	/* set host we are restoring to this host by default */
	amfree(dump_hostname);
	set_host(localhost);
	if (dump_hostname)
	{
            /* get a starting disk and directory based on where
	       we currently are */
	    switch (guess_disk(cwd, sizeof(cwd), &dn_guess, &mpt_guess))
	    {
		case 1:
		    /* okay, got a guess. Set disk accordingly */
		    printf("$CWD '%s' is on disk '%s' mounted at '%s'.\n",
			   cwd, dn_guess, mpt_guess);
		    set_disk(dn_guess, mpt_guess);
		    set_directory(cwd);
		    if (server_happy() && strcmp(cwd, mpt_guess) != 0)
		        printf("WARNING: not on root of selected filesystem, check man-page!\n");
		    amfree(dn_guess);
		    amfree(mpt_guess);
		    break;

		case 0:
		    printf("$CWD '%s' is on a network mounted disk\n",
			   cwd);
		    printf("so you must 'sethost' to the server\n");
		    /* fake an unhappy server */
		    server_line[0] = '5';
		    break;

		case 2:
		case -1:
		default:
		    printf("Can't determine disk and mount point from $CWD '%s'\n", cwd);
		    /* fake an unhappy server */
		    server_line[0] = '5';
		    break;
	    }
	}
    }

    quit_prog = 0;
    do
    {
	if ((lineread = readline("amrecover> ")) == NULL) {
	    clearerr(stdin);
	    putchar('\n');
	    break;
	}
	if (lineread[0] != '\0') 
	{
	    add_history(lineread);
	    process_line(lineread);	/* act on line's content */
	}
	amfree(lineread);
    } while (!quit_prog);

    dbclose();

    aclose(server_socket);
    return 0;
}
