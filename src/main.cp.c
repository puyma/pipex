/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 20:39:10 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/19 20:28:15 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h" /* libft.h, ... */
#include <unistd.h> /* access, pipe, write, execve */
#include <fcntl.h> /* open */
#include <string.h> /* strerror */
#include <errno.h> /* errno */
#include <paths.h> /* _PATH_STDPATH */

#include <stdio.h> /* printf */

int		check(int argc, char **argv);
int		ft_redirect_fd(char *infile, int fd);
int		ft_execvpe(const char *cmd, char const *args[], const char *envp[]);
char	*ft_getenv(const char *name, const char **env);
char	*ft_which(const char *exec, char *path);

int	main(int argc, char **argv, const char **envp)
{
	char	*infile;
	char	*outfile;
	int		fd[2];
	int		pid1;
	int		pid2;
	int		out_fd;

	(void) outfile;
	check(argc, argv);
	infile = argv[1];
	outfile = argv[4];

	if (argc != 5)
		exit (0);
	if (pipe(fd) == -1)
		exit (3);
	
	ft_redirect_fd(infile, fd[1]);
	//ft_printf("redirect OK\n");
	
	const char **cmd1_argv = (const char **) ft_split(argv[2], ' ');
	const char **cmd2_argv = (const char **) ft_split(argv[3], ' ');

	close(fd[1]);

	pid1 = fork();
	if (pid1 < 0) { exit (2); }

	if (pid1 == 0)
	{
		ft_printf("pid1\n");
		dup2(fd[0], STDIN_FILENO);
		ft_execvpe(cmd1_argv[0], cmd1_argv, envp);
		return (0);
	}
	waitpid(pid1, NULL, 0);
	ft_printf("here\n");

	out_fd = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (out_fd == -1) { perror("Error"); }

	pid2 = fork();
	if (pid2 < 0) { exit (4); }
	if (pid2 == 0)
	{
		close(fd[0]);
		ft_printf("pid2\n");
		char *buf[1];
		while (read(fd[1], buf, 1) > 0)
		{
			write(1, buf, 1);
			write(out_fd, buf, 1);
		}
		close(fd[0]);
		close(fd[1]);
		ft_execvpe(cmd2_argv[0], cmd2_argv, envp);
	}

	waitpid(pid2, NULL, 0);
	close(fd[0]);
	close(fd[1]);
	return (0);
}

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
