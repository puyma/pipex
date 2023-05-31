/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 17:25:53 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/31 17:26:02 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	here_doc(char *limiter)
{
	int	fildes[2];

	pipe(fildes);
	read_stdin(fildes[WR], limiter);
	close(fildes[WR]);
	return (fildes[RD]);
}

int	read_stdin(int wr_fd, char *limiter)
{
	char	*line;

	write(STDOUT_FILENO, "> ", 2);
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (*line == *limiter
			&& ft_strncmp(limiter, line, ft_strlen(line) - 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, wr_fd);
		free(line);
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
	}
	return (0);
}

int	write_output(int fd, char *output)
{
	int		outfd;
	char	buf[1];

	outfd = open(output, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	while (read(fd, buf, 1) > 0)
		write(outfd, buf, 1);
	return (0);
}
