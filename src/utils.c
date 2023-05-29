/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 12:29:19 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/29 12:11:15 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_exit(int err_num)
{
	perror("Error");
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

char	*ft_getenv(const char *name, const char **env)
{
	char	*variable;
	int		i;

	i = 0;
	variable = ft_strjoin(name, "=");
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", ft_strlen("PATH=")) == 0)
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
		dir = ft_strndup(path, dir_len);
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
