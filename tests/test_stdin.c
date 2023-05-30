#include <unistd.h>
#include <stdio.h>

int
main (void)
{
	int fd = dup(STDIN_FILENO);
	if (write(fd, "hey\n", 4) == -1)
		printf("write -1\n");
	return (0);
}
