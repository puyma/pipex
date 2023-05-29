/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:43:57 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/29 12:27:12 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	check(int argc, char **argv, char **envp);
static int	do_pid1(int fildes[3][2], char **argv, char **envp);
static int	do_pid2(int fildes[3][2], char **argv, char **envp);

int	main(int argc, char **argv, char **envp)
{
	int		fildes[3][2];
	pid_t	pid1;
	pid_t	pid2;

	if (check(argc, argv, envp) == -1 || pipe(fildes[0]) == -1
		|| pipe(fildes[1]) == -1 || pipe(fildes[2]) == -1
		|| file_to_fd(argv[1], fildes[0][1]) == -1)
		ft_exit (2);
	close(fildes[0][1]);
	pid1 = fork();
	if (pid1 < 0)
		ft_exit(6);
	if (pid1 == 0)
		do_pid1(fildes, argv, envp);
	/*
	if (pid1 == 0)
	{
		close(fildes[0][1]);
		close(fildes[1][0]);
		close(fildes[2][0]);
		close(fildes[2][1]);
		close(STDIN_FILENO);
		dup(fildes[0][0]);
		close(STDOUT_FILENO);
		dup(fildes[1][1]);
		cmd1 = ft_split(argv[2], ' ');
		ft_execvpe(cmd1[0], (const char **) cmd1, (const char **) envp);
		while (read(fildes[0][0], buf[0], 1) > 0)
			write(fildes[1][1], buf[0], 1);
		close(fildes[0][0]);
		close(fildes[1][1]);
		return (0);
	}
	*/
	waitpid(pid1, NULL, 0);
	close(fildes[0][0]);
	close(fildes[1][1]);
	pid2 = fork();
	if (pid2 < 0)
		ft_exit (6);
	if (pid2 == 0)
		do_pid2(fildes, argv, envp);
	/*
	if (pid2 == 0)
	{
		close(fildes[0][0]);
		close(fildes[0][1]);
		close(fildes[1][1]);
		close(fildes[2][0]);
		close(STDIN_FILENO);
		dup(fildes[1][0]);
		close(STDOUT_FILENO);
		dup(fildes[2][1]);
		cmd2 = ft_split(argv[3], ' ');
		ft_execvpe(cmd2[0], (const char **) cmd2, (const char **) envp);
		while (read(fildes[1][0], buf[1], 1) > 0)
			write(fildes[2][1], buf[1], 1);
		close(fildes[1][0]);
		close(fildes[2][1]);
		return (0);
	}
	*/
	waitpid(pid2, NULL, 0);
	close(fildes[1][0]);
	close(fildes[2][1]);
	if (fd_to_file(fildes[2][0], argv[4]) == -1)
		ft_exit (4);
	close(fildes[2][0]);
	close(fildes[2][1]);
	exit(0);
}

static int	do_pid1(int fildes[3][2], char **argv, char **envp)
{
	char	buf[1];
	char	**cmd1;

	close(fildes[0][1]);
	close(fildes[1][0]);
	close(fildes[2][0]);
	close(fildes[2][1]);
	close(STDIN_FILENO);
	dup(fildes[0][0]);
	close(STDOUT_FILENO);
	dup(fildes[1][1]);
	cmd1 = ft_split(argv[2], ' ');
	ft_execvpe(cmd1[0], (const char **) cmd1, (const char **) envp);
	while (read(fildes[0][0], buf, 1) > 0)
		write(fildes[1][1], buf, 1);
	close(fildes[0][0]);
	close(fildes[1][1]);
	return (-1);
}

static int	do_pid2(int fildes[3][2], char **argv, char **envp)
{
	char	buf[1];
	char	**cmd2;

	close(fildes[0][0]);
	close(fildes[0][1]);
	close(fildes[1][1]);
	close(fildes[2][0]);
	close(STDIN_FILENO);
	dup(fildes[1][0]);
	close(STDOUT_FILENO);
	dup(fildes[2][1]);
	cmd2 = ft_split(argv[3], ' ');
	ft_execvpe(cmd2[0], (const char **) cmd2, (const char **) envp);
	while (read(fildes[1][0], buf, 1) > 0)
		write(fildes[2][1], buf, 1);
	close(fildes[1][0]);
	close(fildes[2][1]);
	return (0);
}

static int	check(int argc, char **argv, char **envp)
{
	int		ret;
	char	*cmd[3];
	char	*temp;

	ret = 0;
	cmd[0] = NULL;
	if (argc != 5 || access(argv[1], R_OK) == -1)
		return (-1);
	temp = argv[2];
	if (ft_strchr(argv[2], ' ') != NULL)
		temp = ft_strndup(argv[2], ft_strchr(argv[2], ' ') - argv[2]);
	cmd[1] = ft_which(temp, ft_getenv("PATH=", (const char **) envp));
	if (temp != argv[2])
		free(temp);
	temp = argv[3];
	if (ft_strchr(argv[3], ' ') != NULL)
		temp = ft_strndup(argv[3], ft_strchr(argv[3], ' ') - argv[3]);
	cmd[2] = ft_which(temp, ft_getenv("PATH=", (const char **) envp));
	if (temp != argv[3])
		free(temp);
	if (cmd[1] == NULL || cmd[2] == NULL)
		ret = -1;
	free(cmd[1]);
	free(cmd[2]);
	return (ret);
}

int	ft_execvpe(const char *cmd, char const *args[], char const *envp[])
{
	int		ret;
	char	*path;
	char	*pathcmd;

	path = NULL;
	if (cmd == NULL)
		return (-1);
	if (*cmd != '/')
	{
		path = ft_getenv("PATH", envp);
		if (path == NULL)
			path = _PATH_DEFPATH;
		pathcmd = ft_which(cmd, path);
		if (cmd == NULL)
			exit (99);
		else
			cmd = pathcmd;
	}
	ret = execve(cmd, (char *const *)args, (char *const *)envp);
	return (ret);
}
