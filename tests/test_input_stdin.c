#include <unistd.h>
#include <stdio.h>

#include <string.h>

#include "pipex.h"

#define WR 1
#define RD 0

int	here_doc(char **argv);
int	read_stdin(int wr_fd, char *limiter);

int	main(int argc, char **argv)
{
	int		fd;
	char	buf[1];

	if (argc < 2)
		return (write(STDERR_FILENO, "argc < 2\n", 9));
	fd = here_doc(argv);
	while (read(fd, buf, 1) > 0)
		write(STDOUT_FILENO, buf, 1);
	close(fd);
	return (0);
}

int	here_doc(char **argv)
{
	int 	fildes[2];
	int		pid;

	pipe(fildes);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		close(fildes[RD]);
		read_stdin(fildes[WR], argv[1]);
		close(fildes[WR]);
		exit (0);
	}
	waitpid(pid, NULL, 0);
	close(fildes[WR]);
	return (fildes[RD]);
}

int	read_stdin(int wr_fd, char *limiter)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (*line == *limiter
			&& ft_strncmp(limiter, line, ft_strlen(line) - 1) == 0)
			break ;
		ft_putstr_fd(line, wr_fd);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	return (0);
}
