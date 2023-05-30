/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:43:57 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/30 15:30:41 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_execute_command(char *argv, char **envp, int *fd);

int	main(int argc, char **argv, char **envp)
{
	int	fd;

	if (argc < 4)
		return (0);
	fd = open(argv[1], O_RDONLY);
	int i = 2;
	while (argv[i + 1] != NULL)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		ft_execute_command(argv[i], envp, &fd);
		++i;
	}
	char buf[1];
	while (read(fd, buf, 1) > 0)
		write(STDOUT_FILENO, buf, 1);
	return (0);
}

void	ft_execute_command(char *argv, char **envp, int *fd)
{
	int		fildes[2];
	pid_t	pid;
	char	**cmd;
	
	if (pipe(fildes) == -1) { exit (4); }
	pid = fork();
	if (pid == -1) { perror("pipex"); exit(1); }

	if (pid == 0)
	{
		cmd = ft_split(argv, ' ');
		close(STDOUT_FILENO);
		dup(fildes[WR]);
		close(fildes[RD]);
		close(fildes[WR]);
		ft_execvpe(cmd[0], (char const **) cmd, (const char **) envp);
	}
	else
	{
		waitpid(pid, NULL, 0);
		close(fildes[WR]);
		*fd = fildes[RD];
	}
}
