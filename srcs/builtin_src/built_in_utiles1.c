/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:31:33 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/05 11:48:42 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	*get_word_(char *line, char *del)
{
	int		i;
	char	*word;

	i = 0;
	if (!line)
		return (NULL);
	while (line[i] && !ft_strchr(del, line[i]))
		i++;
	word = malloc((i + 1) * sizeof(char));
	ft_strlcpy(word, line, i + 1);
	return (word);
}

t_env	*get_env_ele_ptr(t_data *data, char *env_val)
{
	int len = 0;
	int len1 = 0;
	int len2 = 0;
	t_env	*env;

	env = data->env;
	while (env != NULL)
	{
		len1 = ft_strlen(env->env_value);
		len2 = ft_strlen(env_val);
		len = len1 > len2 ? len1 : len2;
		if (ft_strncmp(env->env_value, env_val, len) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	print_array(char **array)
{
	int	i;

	i = 0;
	if (!array || !array[0])
		return ;
	while (array != NULL && array[i] != NULL)
		printf("%s\n", array[i++]);
}

char	**env_to_array_(t_env *env)
{
	int		i;
	int		env_len;
	char	**array;

	i = 0;
	env_len = env_c_len(env);
	array = malloc((env_len + 1) * sizeof(char *));
	if (!array || !env)
		return (NULL);
	while (env != NULL)
	{
		if (!env->env_key)
			array[i++] = ft_strjoin(ft_strdup(env->env_value), ft_strdup(""));
		else if (!env->env_key[0])
			array[i++] = ft_strjoin(ft_strdup(env->env_value), ft_strdup("="));
		else
			array[i++] = ft_strjoin(ft_strjoin(ft_strdup(env->env_value),
						ft_strdup("=")), ft_strdup(env->env_key));
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}

int	ft_is_str_digit(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
	}
	return (1);
}
