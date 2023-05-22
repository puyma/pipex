#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int
main(void)
{
	int fd = open("hey.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	dup2(fd, STDOUT_FILENO);
	write(STDOUT_FILENO, "Hols\n", 5);
	close(fd);
	return (0);
}
