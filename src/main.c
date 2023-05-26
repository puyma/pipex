/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:43:57 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/26 12:17:54 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#include <paths.h> /* _PATH_DEFPATH */
#include <string.h> /* strndup */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int		file_to_fd(char *file, int fildes);
int		fd_to_file(int fildes, char *file);

int		check(int argc, char **argv);
char	*ft_getenv(const char *name, const char **env);
char	*ft_which(const char *exec, char *path);
int		ft_execvpe(const char *cmd, char const *args[], char const *envp[]);

int	main(int argc, char **argv, char **envp)
{
	int		fildes[3][2];
	char	buf[3][1];
	int		pid1;
	int		pid2;

	(void) argc;
	(void) argv;
	(void) envp;
	if (pipe(fildes[0]) == -1 || pipe(fildes[1]) == -1 || pipe(fildes[2]) == -1)
	{
		perror("Error");
		exit(10);
	}
	if (file_to_fd(argv[1], fildes[0][1]) == -1)
		exit (2);
	close(fildes[0][1]);
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("Error");
		exit(0);
	}
	if (pid1 == 0)
	{
		close(fildes[0][1]);
		close(fildes[1][0]);
		close(fildes[2][0]);
		close(fildes[2][1]);
		while (read(fildes[0][0], buf[0], 1) > 0)
		{
			write(fildes[1][1], buf[0], 1);
		}
		close(fildes[0][0]);
		close(fildes[1][1]);
		exit(0);
	}
	waitpid(pid1, NULL, 0);
	close(fildes[0][0]);
	close(fildes[1][1]);
	pid2 = fork();
	if (pid2 < 0)
	{
		perror("Error");
		exit (0);
	}
	if (pid2 == 0)
	{
		close(fildes[0][0]);
		close(fildes[0][1]);
		close(fildes[1][1]);
		close(fildes[2][0]);
		while (read(fildes[1][0], buf[1], 1) > 0)
		{
			write(fildes[2][1], buf[1], 1);
		}
		close(fildes[1][0]);
		close(fildes[2][1]);
		exit(0);
	}
	waitpid(pid2, NULL, 0);
	close(fildes[1][0]);
	close(fildes[2][1]);
	if (fd_to_file(fildes[2][0], argv[4]) == -1)
		exit (4);
	close(fildes[2][0]);
	close(fildes[2][1]);
	return (0);
}

int	file_to_fd(char *file, int fildes)
{
	char	buf[1];
	int		fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (-1);
	while (read(fd, buf, 1) > 0)
		write(fildes, buf, 1);
	close(fd);
	return (0);
}

int	fd_to_file(int fildes, char *file)
{
	char	buf[1];
	int		fd;

	fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd == -1)
		return (-1);
	while (read(fildes, buf, 1) > 0)
		write(fd, buf, 1);
	close(fd);
	return (0);
}

/*
int	check(int argc, char **argv)
{
	char	*infile;
	char	*outfile;
	int		out_fd;

	(void) argc;
	(void) outfile;
	(void) out_fd;
	return (9);
	infile = argv[1];
	//outfile = argv[argc - 1];
	// check if infile is readable
	if (access(infile, R_OK) == -1)
		perror("Error");
	// check if cmd1 exist / can be executed
	if (access(argv[2], X_OK) == -1)
		perror("Error");
	// check if cmd2 exist / can be executed
	//if (access(argv[3], X_OK) == -1) { perror("Error"); }
	// cat infile
	ft_redirect_fd(infile, STDIN_FILENO);
	// write to outfile
	//out_fd = open(outfile, O_CREAT|O_TRUNC|O_WRONLY, 0666);
	//if (out_fd == -1) { perror("Error"); }
	//ft_redirect_fd(infile, out_fd);
	return (0);
}
*/

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
	//ft_printf("executing: %s\n", cmd);
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
