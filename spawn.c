#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

char ver[] = "1.01";

/*
v1.01 - chdir() removed, sys_call now defined with || instead of if
*/

void spawn(int argc, char * argv[]);
void sys_call_fail(char * sys_call);

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Err: need something to spawn\n");
		fprintf(stderr, "%s <executable> [args]\n", *argv);
		fprintf(stderr, "%s %s for version\n", *argv, "-v"); 
		exit(EXIT_FAILURE);
	}
	else if ('-' == argv[1][0] && 'v' == argv[1][1])
		printf("v%s\n", ver);
	else
		spawn(--argc, ++argv);
	
	return 0;
}

void spawn(int argc, char * argv[])
{
#define sys_call(val) ((void)(!(((val) < 0)) || (sys_call_fail(#val), 0)))

	pid_t cpid;
	sys_call(cpid = fork());
	
	if (0 == cpid)
	{
		sys_call(setsid());
		sys_call(execvp(*argv, argv));
	}
		
	return;
}

void sys_call_fail(char * sys_call)
{
	fprintf(stderr, "Err: (%s) failed with errno %d:\n", sys_call, errno);
	fprintf(stderr, "%s\n", strerror(errno));
	exit(EXIT_FAILURE);
}
