/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utiles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 09:33:27 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/29 15:07:22 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	*get_var(char *env_var, int *i)
{
	int		k;
	char	*env_val;

	k = 0;
	if (!env_var || !env_var[0])
		return (NULL);
	while (env_var[k] && (ft_isalnum(env_var[k]) || env_var[k] == '_'))
		k++;
	*i += k;
	env_val = malloc((k + 1) * sizeof(char));
	ft_strlcpy(env_val, env_var, k + 1);
	return (env_val);
}

char	*unquote_arg(t_command *list, char *arg, int j, int k)
{
	char	quote;
	char	*argument;
	
	if (!list || !arg)
		return (NULL);
	argument = ft_calloc(ft_strlen(arg) + 1, sizeof(char));
	while (arg[j])
	{
		quote = arg[j];
		if (arg[j] == '$' && (arg[j + 1] == '"' || arg[j + 1] == '\''))
			j++;
		else if (arg[j] == '\'' || arg[j] == '"')
		{
			quote = arg[j++];
			while (arg[j] && arg[j] != quote)
				argument[k++] = arg[j++];
			j++;
		}
		else
			argument[k++] = arg[j++];
		if (arg[0] == quote && quote == '\'')
			list->quoted = 1;
		else if (arg[0] == quote && quote == '"')
			list->quoted = 2;
	}
	free(arg);
	return (argument);
}

// char	*ft_strnstr_l(const char *big, const char *little, size_t len)
// {
// 	size_t	i;
// 	size_t	j;

// 	i = 0;
// 	if (*little == '\0')
// 		return ((char *)big);
// 	while (big[i] && i < len)
// 	{
// 		j = 0;
// 		while (big[i + j] == little[j] && little[j] && (i + j) < len)
// 			j++;
// 		if (little[j] == '\0')
// 			return ((char *)&big[i + j]);
// 		i++;
// 	}
// 	return (NULL);
// }

char	*get_word(char *argument, int *i)
{
	char	*str;
	int		j;

	str = NULL;
	j = *i;
	if (argument[j] == '$')
		j++;
	while (argument[j] && argument[j] != '$')
		j++;
	str = malloc((j - *i + 1) * sizeof(char));
	ft_strlcpy(str, &argument[*i], (j - *i + 1));
	*i = j;
	return (str);
}

int	get_expanded(char *argument, int *i)
{
	data->str1 = get_var(&argument[++(*i)], i);
	data->str2 = get_env_element(data->str1);
	free(data->str1);
	if (!data->str2 || !data->str2[0])
	{
		free(data->str2);
		return (1);
	}
	data->expanded = ft_strjoin(data->expanded, data->str2);
	return (0);
}
