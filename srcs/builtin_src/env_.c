/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:03:32 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/09 10:14:00 by aelkheta         ###   ########.fr       */
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
	if (!word)
		panic("malloc fail\n", 1);
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

int	env(t_env *env)
{
	while (env != NULL)
	{
		if (env->env_value != NULL && env->env_key != NULL)
			printf("%s=%s\n", env->env_value, env->env_key);
		env = env->next;
	}
	g_exit_stat = 0;
	return (0);
}
