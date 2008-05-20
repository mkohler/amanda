/* Pipespawn can create up to three pipes; These defines set which pointers
 * should have the other end assigned for a new pipe. If not set, then
 * pipespawn will use a preexisting fd.
 */
#ifndef PIPESPAWN_H
#define PIPESPAWN_H 1

extern char skip_argument[1];

#define STDIN_PIPE	(1 << 0)
#define STDOUT_PIPE	(1 << 1)
#define STDERR_PIPE	(1 << 2)
#define PASSWD_PIPE	(1 << 3)

int pipespawn P((char *prog, int pipedef,
		 int *stdinfd, int *stdoutfd, int *stderrfd,
		 ...));
int pipespawnv P((char *prog, int pipedef,
		  int *stdinfd, int *stdoutfd, int *stderrfd,
		  char **my_argv));
int pipespawnv_passwd P((char *prog, int pipedef,
		  int *stdinfd, int *stdoutfd, int *stderrfd,
		  char *passwdvar, int *passwdfd,
		  char **my_argv));


#endif /* PIPESPAWN_H */
