/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:47:09 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/18 11:08:07 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	len;

	if (s == NULL || fd < 0)
		return ;
	len = 0;
	while (s[len])
		len++;
	write(fd, s, len);
}

void	ft_error_2(void)
{
	ft_putstr_fd("Error\n", 2);
	g_exit_stat = 127;
}

void	ft_error(t_data *data, char **av)
{
	if (av[0] && ft_strcmp(av[0], ".") == 0)
	{
		ft_putstr_fd(".: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		g_exit_stat = 2;
	}
	else if (av[0] && ft_strcmp(av[0], "..") == 0)
	{
		ft_putstr_fd("command not found: ..\n", 2);
		g_exit_stat = 127;
	}
	else
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putstr_fd(av[0], 2);
		ft_putstr_fd("\n", 2);
		g_exit_stat = 127;
		clear_list(&data->list);
		clear_all(data);
		exit(g_exit_stat);
	}
}

int	ft_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (1);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
