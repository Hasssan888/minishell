/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utillis.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 18:32:21 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/09 12:53:03 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	file_info_2(char **av, int *flag, struct stat file_start)
{
	if (av[0][ft_strlen(av[0]) - 1] == '/' && !S_ISDIR(file_start.st_mode))
	{
		perror(av[0]);
		*flag = 0;
	}
	else
	{
		perror(av[0]);
		*flag = 1;
	}
}
void	file_info(char **av, int *flag)
{
	struct stat	file_start;

	stat(av[0], &file_start);
	if (ft_strchr(av[0], '/') != NULL && access(av[0], F_OK) == -1)
		file_info_2(av, flag, file_start);
	else	if (ft_strchr(av[0], '/') != NULL && access(av[0], F_OK) != -1)
	{
		if (S_ISREG(file_start.st_mode) && access(av[0], X_OK) != -1)
			*flag = 2;
		else if (S_ISREG(file_start.st_mode) && access(av[0], X_OK) == -1)
		{
			ft_putstr_fd(av[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			*flag = 0;
		}
		else if (S_ISDIR(file_start.st_mode))
		{
			ft_putstr_fd(av[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			*flag = 0;
		}
	}
	else
		*flag = 2;
}

int	handle_direct(char **av)
{
	int	flag;

	flag = 0;
	file_info(av, &flag);
	if (flag == 0)
	{
		g_exit_stat = 126;
		return (0);
	}
	else if (flag == 1)
	{
		g_exit_stat = 127;
		return (0);
	}
	return (1);
}
