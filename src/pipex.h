/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 20:42:24 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/30 15:30:52 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# include <unistd.h> /* write, read, fork */
# include <stdio.h> /* perror */
# include <paths.h> /* _PATH_DEFPATH */
# include <fcntl.h> /* open */
# include <errno.h> /* errno */

# include "libft.h"

# define WR 1
# define RD 0

void	ft_exit(char *err_str, int err_num);
char	*ft_getenv(const char *name, const char **env);
char	*ft_which(const char *exec, char *path);
int		ft_execvpe(const char *cmd, char const *args[], char const *envp[]);
int		check(int argc, char **argv, char **envp);

#endif
