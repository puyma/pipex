/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:43:57 by mpuig-ma          #+#    #+#             */
/*   Updated: 2023/05/23 18:11:01 by mpuig-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#include <unistd.h>
#include <stdio.h>

int	main(int argc, char **argv, char **envp)
{
	int	fildes[2];
	int	pid1;
	int	pid2;

	(void) argc;
	(void) argv;
	(void) envp;
	(void) pid2;

	if (pipe(fildes) == -1) { perror("Error"); exit(0); }

	pid1 = fork();
	if (pid1 < 0) { perror("Error"); exit(0); }
	if (pid1 == 0)
	{
		close(fildes[0]);
		write(fildes[1], "HEY", 3);
		close(fildes[1]);
		exit(0);
	}
	waitpid(pid1, NULL, 0);
	char buf[100];
	int ret = read(fildes[0], buf, 3);
	printf("ret: %d, %s\n", ret, buf);
	close(fildes[0]);
	return (0);
}

/*	
	if (pipe(fildes) == -1) { perror("Error"); exit(0); }

	ft_putendl_fd("Stuff", fildes[1]);

	pid1 = fork();
	if (pid1 < 0) { perror("Error"); exit(0); }
	if (pid1 > 0)
	{
		waitpid(pid1, NULL, 0);
		ft_putendl_fd("Main", STDOUT_FILENO);
	}
	else
	{
		char str[100];
		read(fildes[0], str, 5);
		ft_putendl_fd("Child", STDOUT_FILENO);
		ft_putendl_fd(str, STDOUT_FILENO);
	}
	return (0);
}
*/
