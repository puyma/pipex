/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 20:39:10 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/18 17:10:18 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#include <unistd.h> /* access, pipe, write, execve */
#include <fcntl.h> /* open */
#include <string.h> /* strerror */

#include <errno.h> /* errno */
#include <paths.h>

#include <stdio.h> /* printf */

int	check(int argc, char **argv);
int	ft_redirect_fd(char *infile, int fd);

int	main(int argc, char **argv)
{
	//char	*infile;
	//char	*outfile;

	if (argc != 5)
		exit (0);
	check(argc, argv);
	// config() { environ = "..." }

	int fd[2];
	if (pipe(fd) == -1) { exit (3); }
	
	char *newargv[] = {"/bin/echo", "infile", NULL};

	/*
	int pid1 = fork();
	if (pid1 < 0) { exit (2); }
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		ft_printf("here\n");
		execve("/bin/echo", newargv, NULL);
	}
	*/

	int pid2 = fork();
	if (pid2 < 0) { exit (4); }
	if (pid2 == 0)
	{
		//dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execve("/bin/echo", newargv, NULL);
	}

	close(fd[0]);
	close(fd[1]);
	//waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}

int	check(int argc, char **argv)
{
	char	*infile;
	char	*outfile;
	int		out_fd;

	return (9);

	infile = argv[1];
	outfile = argv[argc - 1];
	// check if infile is readable
	if (access(infile, R_OK) == -1) { perror("Error"); }

	// check if cmd1 exist / can be executed 
	if (access(argv[2], X_OK) == -1) { perror("Error"); }

	// check if cmd2 exist / can be executed
	//if (access(argv[3], X_OK) == -1) { perror("Error"); }
	
	// cat infile
	ft_redirect_fd(infile, STDIN_FILENO);

	// write to outfile
	out_fd = open(outfile, O_CREAT|O_TRUNC|O_WRONLY, 0666);
	if (out_fd == -1) { perror("Error"); }
	ft_redirect_fd(infile, out_fd);

	return (0);
}

int	ft_redirect_fd(char *infile, int fd)
{
	char	*line;
	int		in_fd;

	in_fd = open(infile, O_RDONLY);
	if (in_fd == -1)
		return (1);

	line = get_next_line(in_fd);
	while (line != NULL)
	{
		ft_putstr_fd(line, fd);
		free(line);
		line = get_next_line(in_fd);
	}
	close(in_fd);
	return (0);
}

/*
int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		ft_printf("Invalid number of arguments...\n");
		exit(2);
	}
	if (access(argv[1], R_OK) != 0)
	{
		ft_printf("Error with file1\n");
		exit(2);
	}
	//if (access(argv[2], X_OK) != 0)
	//{
	//	ft_printf("Error with cmd1\n");
	//	exit(2);
	//}
	int fd = open(argv[1], O_RDONLY);
	char *line = get_next_line(fd);
	int	pipefd[2];
	pipe(pipefd);
	while (line != NULL)
	{
		write(pipefd[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(fd);
	}
	close(pipefd[1]);
	char *buffer[2];
	while (read(pipefd[0], buffer, 2) > 0)
		write(STDOUT_FILENO, buffer, 2);
	close(pipefd[0]);
	//char **newargv = argv + 1;
	//char *newenv[] = { NULL, "HOME=/Users/mpuig-ma", "PATH=/bin:/usr/bin", NULL };
	//execve(argv[1], newargv, newenv);
	if (errno != 0)
		printf("errno: %s\n", strerror(errno));
	return (0);
}
*/
