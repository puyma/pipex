/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:43:57 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/26 13:24:58 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <string.h> /* strndup */

int		check(int argc, char **argv, char **envp);
char	*ft_getenv(const char *name, const char **env);
char	*ft_which(const char *exec, char *path);
int		ft_execvpe(const char *cmd, char const *args[], char const *envp[]);

int	main(int argc, char **argv, char **envp)
{
	int		fildes[3][2];
	char	buf[3][1];
	pid_t	pid1;
	int		pid2;

	if (check(argc, argv, envp) == -1 || pipe(fildes[0]) == -1
		|| pipe(fildes[1]) == -1 || pipe(fildes[2]) == -1
		|| file_to_fd(argv[1], fildes[0][1]) == -1)
		ft_exit (2);
	close(fildes[0][1]);
	pid1 = fork();

	if (pid1 < 0)
		ft_exit(0);
	if (pid1 == 0)
	{
		close(fildes[0][1]);
		close(fildes[1][0]);
		close(fildes[2][0]);
		close(fildes[2][1]);
		while (read(fildes[0][0], buf[0], 1) > 0)
			write(fildes[1][1], buf[0], 1);
		close(fildes[0][0]);
		close(fildes[1][1]);
		exit(0);
	}

	waitpid(pid1, NULL, 0);
	close(fildes[0][0]);
	close(fildes[1][1]);

	pid2 = fork();
	if (pid2 < 0)
		ft_exit (0);
	if (pid2 == 0)
	{
		close(fildes[0][0]);
		close(fildes[0][1]);
		close(fildes[1][1]);
		close(fildes[2][0]);
		while (read(fildes[1][0], buf[1], 1) > 0)
			write(fildes[2][1], buf[1], 1);
		close(fildes[1][0]);
		close(fildes[2][1]);
		exit(0);
	}

	waitpid(pid2, NULL, 0);
	close(fildes[1][0]);
	close(fildes[2][1]);

	if (fd_to_file(fildes[2][0], argv[4]) == -1)
		ft_exit (4);
	close(fildes[2][0]);
	close(fildes[2][1]);
	exit(0);
}

int	check(int argc, char **argv, char **envp)
{
	char	*infile;
	char	*path;

	(void) argc;
	(void) argv;
	(void) envp;
	if (argc != 5)
		return (-1);
	infile = argv[1];
	path = ft_getenv("PATH=", (const char **) envp);
	//use ft_which to get absolute path for each executable (cmd1, cmd2)
	if (access(infile, R_OK) == -1
		|| access(argv[2], X_OK) == -1
		|| access(argv[3], X_OK) == -1)
		return (-1);
	return (0);
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

char	*ft_which(const char *exec, char *path)
{
	size_t	dir_len;
	char	*dir;
	char	*filename;
	char	*temp;

	path = ft_strchr(path, '=') + 1;
	while (path != NULL)
	{
		dir_len = ft_strchr(path, ':') - path;
		dir = strndup(path, dir_len);
		temp = ft_strjoin(dir, "/");
		filename = ft_strjoin(temp, exec);
		free(temp);
		free(dir);
		if (access(filename, X_OK) == 0)
			return (filename);
		free(filename);
		path = ft_strchr(path, ':');
		if (path != NULL)
			++path;
	}
	return (NULL);
}

char	*ft_getenv(const char *name, const char **env)
{
	char	*variable;
	int		i;

	i = 0;
	variable = ft_strjoin(name, "=");
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", strlen("PATH=")) == 0)
		{
			free(variable);
			return ((char *) env[i]);
		}
		++i;
	}
	free(variable);
	variable = NULL;
	return (variable);
}
