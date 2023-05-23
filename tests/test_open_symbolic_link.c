#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int
main(void)
{
	int fd = open ("symbolic_link", O_RDONLY | O_SYMLINK | O_CLOEXEC);
	if (fd == -1) { exit (2); }
	else { write (1, "OK\n", 3); }
	return (0);
}
