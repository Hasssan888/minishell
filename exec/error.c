/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:47:09 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/05 12:41:08 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

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
	perror("\033[31mError");
	g_exit_stat = 127;
}

void	ft_error(char **av)
{
	int		i;
	char	*str;

	i = -1;
	if (ft_strcmp(av[0], ".") == 0)
	{
		ft_putstr_fd(".: filename argument required", 2);
		write(2, "\n", 1);
		ft_putstr_fd(".: usage: . filename [arguments]", 2);
		write(2, "\n", 1);
		g_exit_stat = 2;
	}
	else
	{
		ft_putstr_fd("Command not found: ", 2);
		while (av[++i])
		{
			str = strjoin1(av[i], " ");
			ft_putstr_fd(str, 2);
			free(str);
		}
		write(2, "\n", 1);
		g_exit_stat = 127;
	}
}

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (0);
	while (*s != '\0')
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
