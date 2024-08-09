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

int	file_info_2(char **av)
{
	if (ft_strchr(av[0], '/') != NULL && access(av[0], F_OK) == -1)
	{
		ft_putstr_fd(av[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}

int	file_info(char **av)
{
	struct stat	file_start;

	if (file_info_2(av))
		return (1);
	if (ft_strchr(av[0], '/') != NULL && access(av[0], F_OK) != -1)
	{
		stat(av[0], &file_start);
		if (S_ISREG(file_start.st_mode) && access(av[0], X_OK) != -1)
			return (2);
		else if (S_ISREG(file_start.st_mode) && access(av[0], X_OK) == -1)
		{
			ft_putstr_fd(av[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			return (0);
		}
		else if (S_ISDIR(file_start.st_mode))
		{
			ft_putstr_fd(av[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			return (0);
		}
	}
	return (2);
}

int	handle_direct(char **av)
{
	int	flag;

	flag = file_info(av);
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
