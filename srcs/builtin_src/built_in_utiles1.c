/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:31:33 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/06 17:50:35 by aelkheta         ###   ########.fr       */
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

t_env	*get_env_ele_ptr(t_env *env, char *env_val)
{
	t_env	*env_;

	env_ = env;
	while (env_ != NULL)
	{
		if (ft_strncmp(env_->env_value, env_val, ft_strlen(env_->env_value)
				+ ft_strlen(env_val)) == 0)
			return (env_);
		env_ = env_->next;
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
	if ((str[0] == '-' || str[0] == '+') && str[1])
		++i;
	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
	}
	return (1);
}
