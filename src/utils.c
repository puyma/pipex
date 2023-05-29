/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 12:29:19 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/29 16:24:21 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_exit(char *err_str, int err_num)
{
	perror(err_str);
	exit(err_num);
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
