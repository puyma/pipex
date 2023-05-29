/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:43:57 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/29 17:34:12 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	do_pid1(int fildes[3][2], char *arg, char **envp);
static int	do_pid2(int fildes[3][2], char *arg, char **envp);

int	main(int argc, char **argv, char **envp)
{
	int	fildes[3][2];

	if (check(argc, argv, envp) == -1)
		ft_exit(*argv, 2);
	if (pipe(fildes[0]) == -1 || pipe(fildes[1]) == -1
		|| pipe(fildes[2]) == -1)
		ft_exit(*argv, 3);
	if (file_to_fd(argv[1], fildes[0][1]) == -1)
		ft_exit (*argv, 4);
	close(fildes[0][1]);
	do_pid1(fildes, argv[2], envp);
	close(fildes[0][0]);
	close(fildes[1][1]);
	do_pid2(fildes, argv[3], envp);
	close(fildes[1][0]);
	close(fildes[2][1]);
	if (fd_to_file(fildes[2][0], argv[4]) == -1)
		ft_exit (*argv, 4);
	close(fildes[2][0]);
	close(fildes[2][1]);
	exit(0);
}

/*
static int	do_pid(int fildes[2], char *arg, char **envp)
{
	char		buf[1];
	const char	**cmd = (const char **) ft_split(arg, ' ');
	const pid_t	pid = fork();

	if (pid < 0)
		ft_exit("pipex", 6);
	if (pid == 0)
	{
		close(STDIN_FILENO);
		dup(fildes[0][0]);
		close(STDOUT_FILENO);
		dup(fildes[1][1]);
		ft_execvpe(cmd[0], (const char **) cmd, (const char **) envp);
		while (read(fildes[0][0], buf, 1) > 0)
			write(fildes[1][1], buf, 1);
		close(fildes[0][0]);
		close(fildes[1][1]);
		return (0);
	}
	waitpid(pid, NULL, 0);
	return (0);
}
*/

static int	do_pid1(int fildes[3][2], char *arg, char **envp)
{
	char		buf[1];
	const char	**cmd = (const char **) ft_split(arg, ' ');
	const pid_t	pid = fork();

	if (pid < 0)
		ft_exit("pipex", 6);
	if (pid == 0)
	{
		close(fildes[0][1]);
		close(fildes[1][0]);
		close(fildes[2][0]);
		close(fildes[2][1]);

		//close(STDIN_FILENO);
		//dup(fildes[0][0]);
		dup2(fildes[0][0], STDIN_FILENO);

		//close(STDOUT_FILENO);
		//dup(fildes[1][1]);
		dup2(fildes[1][1], STDOUT_FILENO);

		ft_execvpe(cmd[0], (const char **) cmd, (const char **) envp);
		while (read(fildes[0][0], buf, 1) > 0)
			write(fildes[1][1], buf, 1);
		close(fildes[0][0]);
		close(fildes[1][1]);
		return (0);
	}
	waitpid(pid, NULL, 0);
	return (0);
}

static int	do_pid2(int fildes[3][2], char *arg, char **envp)
{
	char		buf[1];
	const char	**cmd = (const char **) ft_split(arg, ' ');
	const pid_t	pid = fork();

	if (pid < 0)
		ft_exit("pipex", 6);
	if (pid == 0)
	{
		close(fildes[0][0]);
		close(fildes[0][1]);
		close(fildes[1][1]);
		close(fildes[2][0]);
		close(STDIN_FILENO);
		dup(fildes[1][0]);
		close(STDOUT_FILENO);
		dup(fildes[2][1]);
		ft_execvpe(cmd[0], (const char **) cmd, (const char **) envp);
		while (read(fildes[1][0], buf, 1) > 0)
			write(fildes[2][1], buf, 1);
		close(fildes[1][0]);
		close(fildes[2][1]);
		return (0);
	}
	waitpid(pid, NULL, 0);
	return (0);
}
