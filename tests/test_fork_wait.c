#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
	write(1, "a\n", 2);

	int pid = fork();
	if (pid < 0) { exit (2); }
	if (pid == 0) { write (1, "b\n", 2); exit (0); }
	//else { write (1, "c\n", 2); }
	//else { wait(NULL); write (1, "c\n", 2); }
	//waitpid(pid, NULL, 0);
	wait(NULL);
	write (1, "d\n", 2);
	return (0);
}
