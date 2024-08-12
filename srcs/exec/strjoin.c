/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strjoin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 13:27:06 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/12 12:55:42 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	*strjoin1(char *s1, char *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*ptr;

	ptr = NULL;
	if (!s1 && !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	ptr = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (!ptr)
		return (ft_free(s1));
	ft_strlcpy(ptr, s1, len_s1 + 1);
	ft_strlcpy((ptr + len_s1), s2, (len_s2 + 1));
	return (ptr);
}

void	free_int_array(int **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i] != NULL)
		free(array[i++]);
	free(array);
}

int	check_exit_status(int status)
{
	if (WIFSIGNALED(status))
	{
		write(1, "\n", 1);
		g_exit_stat = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		g_exit_stat = WEXITSTATUS(status);
	return (g_exit_stat);
}

int	check(t_command *node)
{
	t_command	*c;

	c = node;
	while (c)
	{
		if (c->type == PIPE)
			return (1);
		else if (c->type == RED_OUT || c->type == APP)
			return (2);
		c = c->next;
	}
	return (0);
}
