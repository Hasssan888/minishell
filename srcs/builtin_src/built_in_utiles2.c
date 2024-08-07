/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:12:03 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/07 09:50:26 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

int	env_c_len(t_env *env_)
{
	int	len;

	len = 0;
	while (env_ != NULL)
	{
		len++;
		env_ = env_->next;
	}
	return (len);
}

void	print_export_env(t_env **env, int env_len)
{
	int	i;

	i = 0;
	while (i < env_len)
	{
		if (!env[i]->env_key)
			printf("declare -x %s\n", env[i]->env_value);
		else if (env[i]->env_key != NULL)
			printf("declare -x %s=\"%s\"\n", env[i]->env_value,
				env[i]->env_key);
		i++;
	}
}

void	print_sorted_env(t_env *env)
{
	t_env	*env_;
	int		env_len;
	int		i;
	t_env	**env__;

	env_ = env;
	env_len = env_c_len(env_);
	i = 0;
	env__ = malloc((env_len + 1) * sizeof(t_env *));
	while (i < env_len)
	{
		env__[i] = env_;
		env_ = env_->next;
		i++;
	}
	env__[i] = NULL;
	env__ = sort_env(env__, env_len);
	print_export_env(env__, env_len);
	free(env__);
}

int	valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (str[++i] && str[i] != '=')
	{
		if (str[i] == '+')
			break ;
		else if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
	}
	if (str[i] && str[i] == '+' && str[i + 1] != '=')
		return (0);
	return (1);
}

t_env	**sort_env(t_env **env_, int env_len)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	j = 0;
	while (i < env_len - 1)
	{
		j = 0;
		while (j < env_len - i - 1)
		{
			if (ft_strcmp(env_[j]->env_value, env_[j + 1]->env_value) > 0)
			{
				temp = env_[j];
				env_[j] = env_[j + 1];
				env_[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	env_[env_len] = NULL;
	return (env_);
}
