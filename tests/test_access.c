#include <unistd.h> /* access */
#include <stdio.h> /* printf */
#include <fcntl.h> /* open */

int
main(int argc, char **argv)
{
	if (argc != 2)
		return (0);
	if (access(argv[1], F_OK) != -1)
		printf("OK\n");
	if (open(argv[1], O_DIRECTORY) != -1)
		printf("Is a directory\n");
	return (0);
}
