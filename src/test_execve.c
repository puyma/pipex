/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_execve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 15:38:27 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/19 18:32:19 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <string.h>
#include <paths.h>

#include "libft.h"

// _GNU_SOURCE
int		ft_execvpe(const char *cmd, char const *args[], const char *envp[]);

// STD
char	*ft_getenv(const char *name, const char **env);

// CUSTOM
char	*ft_which(const char *exec, char *path);

int	main(int argc, char **argv, const char **envp)
{
	int			pid1;
	char const	*cmd_args[] = {"ls", "-lh", NULL};

	(void) argc, (void) argv;
	pid1 = fork();
	if (pid1 < 0)
		exit (2);
	if (pid1 == 0)
	{
		if (ft_execvpe(cmd_args[0], cmd_args, envp) == -1)
			perror("Error");
	}
	waitpid(pid1, NULL, 0);
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
