/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utiles3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:13:31 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/09 10:11:25 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

size_t	ft_len_arr(char **arr)
{
	size_t	len;

	len = -1;
	if (!arr)
		return (0);
	while (arr[++len] != NULL)
		;
	return (len);
}

char	**ft_arr_join(char **arr1, char **arr2)
{
	int		i;
	int		j;
	int		len1;
	int		len2;
	char	**joined;

	i = 0;
	j = 0;
	if (!arr1 && !arr2)
		return (NULL);
	len1 = ft_len_arr(arr1);
	len2 = ft_len_arr(arr2);
	joined = malloc((len1 + len2 + 1) * sizeof(char *));
	if (!joined)
		panic("malloc fail\n", 1);
	while (arr1 != NULL && arr1[j] != NULL)
		joined[i++] = ft_strdup(arr1[j++]);
	j = 0;
	while (arr2 != NULL && arr2[j] != NULL)
		joined[i++] = ft_strdup(arr2[j++]);
	joined[i] = NULL;
	free_array(arr1);
	free_array(arr2);
	return (joined);
}

int	ambigous_red(char *red_file)
{
	int		i;
	char	*str;

	i = 0;
	if (!red_file)
		return (1);
	while (red_file != NULL && red_file[i] && ft_strchr(" \t", red_file[i]))
		i++;
	while (red_file != NULL && red_file[i])
	{
		str = ft_strchr(" \t", red_file[i]);
		if (str != NULL)
		{
			while (red_file[i] && ft_strchr(" \t", red_file[i]))
				i++;
			if (red_file[i] && !ft_strchr(" \t", red_file[i]))
				return (1);
		}
		i++;
	}
	return (0);
}

int	is_ambiguous(t_command *list)
{
	if (!list->quoted && (list->type == RED_OUT || list->type == RED_IN))
	{
		if (list->args != NULL && ambigous_red(list->args[0]))
		{
			ft_putstr_fd("minishell: ambiguous redirect\n", 2);
			list->syntxerr = AMPIGOUS;
			g_exit_stat = 1;
		}
	}
	return (1);
}

int	is_empty(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	if (!str[i])
		return (0);
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (!str[i])
		return (1);
	return (0);
}
