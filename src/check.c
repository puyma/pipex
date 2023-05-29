/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 15:54:28 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/29 15:55:02 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	check(int argc, char **argv, char **envp)
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
