/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:43:57 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/23 19:29:16 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int	file_to_fd(char *file, int fildes);
int	fd_to_file(int fildes, char *file);

int	main(int argc, char **argv, char **envp)
{
	int	fildes[3][2];
	char buf[3][1];
	int	pid1;
	int	pid2;

	(void) argc;
	(void) argv;
	(void) envp;

	if (pipe(fildes[0]) == -1) { perror("Error"); exit(10); }
	if (pipe(fildes[1]) == -1) { perror("Error"); exit(11); }
	if (pipe(fildes[2]) == -1) { perror("Error"); exit(12); }

	file_to_fd(argv[1], fildes[0][1]);
	close(fildes[0][1]);

	pid1 = fork();
	if (pid1 < 0) { perror("Error"); exit(0); }
	if (pid1 == 0)
	{
		close(fildes[0][1]);
		close(fildes[1][0]);
		close(fildes[2][0]);
		close(fildes[2][1]);
		close(fildes[0][0]);
		while (read(fildes[0][0], buf[0], 1) > 0) { write(fildes[1][1], buf[0], 1); }
		close(fildes[1][1]);
		exit(0);
	}

	close(fildes[0][0]);
	waitpid(pid1, NULL, 0);

	pid2 = fork();
	if (pid2 < 0) { perror("Error"); exit (0); }
	if (pid2 == 0)
	{
		close(fildes[0][0]);
		close(fildes[0][1]);
		close(fildes[1][1]);
		close(fildes[2][0]);
		close(fildes[1][0]);
		while (read(fildes[1][0], buf[1], 1) > 0) { write(fildes[2][1], buf[1], 1); }
		close(fildes[2][1]);
		exit(0);
	}

	close(fildes[2][0]);
	waitpid(pid2, NULL, 0);
	fd_to_file(fildes[2][0], argv[4]);
	close(fildes[1][0]);
	close(fildes[1][1]);
	close(fildes[2][0]);
	close(fildes[2][1]);
	return (0);
}

int
file_to_fd(char *file, int fildes)
{
	char	buf[1];
	int		fd;

	fd = open(file, O_RDONLY);
	if (fd == -1) { perror("Error"); exit (2); }
	while (read(fd, buf, 1) > 0) { write(fildes, buf, 1); }
	close(fd);
	return (0);
}

int
fd_to_file(int fildes, char *file)
{
	char	buf[1];
	int		fd;

	fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	while (read(fildes, buf, 1) > 0) { write(STDOUT_FILENO, buf, 1); }
	close(fd);
	return (0);
}
