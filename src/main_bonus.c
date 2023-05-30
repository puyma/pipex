/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:43:57 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/30 16:24:04 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void		ft_execute_command(char *argv, char **envp, int *fd);
static int		ft_check(int argc, char **argv, char **envp);

int	main(int argc, char **argv, char **envp)
{
	int		i;
	int		fd;
	int		outfd;
	char	buf[1];

	if (argc < 4)
		return (1);
	// check infile exists, is readable
	// check commands exist, are executable
	i = 2;
	fd = open(argv[1], O_RDONLY);
	while (argv[i + 1] != NULL)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		ft_execute_command(argv[i++], envp, &fd);
	}
	outfd = open(argv[argc - 1], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	while (read(fd, buf, 1) > 0)
		write(outfd, buf, 1);
	close(fd);
	return (0);
}

static void	ft_execute_command(char *argv, char **envp, int *fd)
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

static int	ft_check(int argc, char **argv, char **envp)
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

void	ft_exit(char *err_str, int err_num)
{
	perror(err_str);
	exit(err_num);
}
