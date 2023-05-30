/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:43:57 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/30 17:45:37 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void		execute_command(char *argv, char **envp, int *fd);
static int		check_args(int argc, char **argv, char **envp);
static char		*cmd_path(char *argv, const char **envp);

int	main(int argc, char **argv, char **envp)
{
	int		i;
	int		fd;
	int		outfd;
	char	buf[1];

	if (check_args(argc, argv, envp) != 0)
		exit (1);
	i = 2;
	fd = open(argv[1], O_RDONLY);
	while (argv[i + 1] != NULL)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		execute_command(argv[i++], envp, &fd);
	}
	outfd = open(argv[argc - 1], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	while (read(fd, buf, 1) > 0)
		write(outfd, buf, 1);
	close(fd);
	return (0);
}

static void	execute_command(char *argv, char **envp, int *fd)
{
	int		fildes[2];
	char	**cmd;
	pid_t	pid;

	if (pipe(fildes) == -1)
		exit (4);
	pid = fork();
	if (pid == -1)
	{
		perror("pipex");
		exit(1);
	}
	if (pid == 0)
	{
		cmd = ft_split(argv, ' ');
		dup2(fildes[WR], STDOUT_FILENO);
		close(fildes[RD]);
		close(fildes[WR]);
		ft_execvpe(cmd[0], (char const **) cmd, (const char **) envp);
		exit (1);
	}
	waitpid(pid, NULL, 0);
	close(fildes[WR]);
	*fd = fildes[RD];
}

static int	check_args(int argc, char **argv, char **envp)
{
	int		i;
	char	*cmd;

	i = 2;
	if (argc < 5)
		return (write(2, "pipex: invalid number of arguments\n", 36));
	if (open(argv[1], O_DIRECTORY) != -1 || access(argv[1], R_OK) != 0)
		return (write(2, "pipex: invalid input file\n", 27));
	while (argv[i] != NULL && i < argc - 1)
	{
		if (*argv[i] == '\0')
			return (write(2, "pipex: invalid command\n", 24));
		cmd = cmd_path(argv[i], (const char **) envp);
		if (cmd == NULL)
			return (write(2, "pipex: command not found\n", 25));
		if (open(cmd, O_DIRECTORY) != -1 || access(cmd, X_OK) != 0)
		{
			free(cmd);
			return (write(2, "pipex: invalid command\n", 24));
		}
		free(cmd);
		++i;
	}
	return (0);
}

static char	*cmd_path(char *argv, const char **envp)
{
	char	*exec_name;
	char	*cmd;

	if (ft_strchr(argv, ' ') == NULL)
		exec_name = argv;
	else
		exec_name = ft_strndup(argv, ft_strchr(argv, ' ') - argv);
	cmd = ft_which(exec_name, ft_getenv("PATH=", envp));
	if (ft_strchr(argv, ' ') != NULL)
		free(exec_name);
	if (cmd == NULL)
		return (NULL);
	return (cmd);
}
