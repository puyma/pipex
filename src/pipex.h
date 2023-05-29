/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 20:42:24 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/29 11:40:02 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# include <unistd.h> /* write, read, fork */
# include <stdio.h> /* perror */
# include <paths.h> /* _PATH_DEFPATH */
# include <fcntl.h> /* open */

# include "libft.h"

int		file_to_fd(char *file, int fildes);
int		fd_to_file(int fildes, char *file);
void	ft_exit(int err_num);
char	*ft_getenv(const char *name, const char **env);
char	*ft_which(const char *exec, char *path);
int		ft_execvpe(const char *cmd, char const *args[], char const *envp[]);

#endif
