/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:09:15 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/08 22:07:03 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	free_env_ptr(t_env **env_ptr)
{
	t_env	*ptr;

	if (env_ptr != NULL && *env_ptr != NULL)
	{
		ptr = *env_ptr;
		if (ptr->env_value != NULL)
			free(ptr->env_value);
		if (ptr->env_value != NULL)
			free(ptr->env_key);
		free(ptr);
	}
}

void	export_var_app(t_data *data, t_env *env_ptr, char **splited)
{
	if (env_ptr->env_key != NULL && splited[0][ft_strlen(splited[0])
		- 1] == '+')
	{
		if (splited[1] != NULL)
			env_ptr->env_key = ft_strjoin(env_ptr->env_key,
					ft_strdup(splited[1]));
		else
		{
			if (env_ptr->env_key != NULL)
				free(env_ptr->env_key);
			env_ptr->env_key = ft_strdup("");
		}
	}
	else
	{
		if (env_ptr->env_key != NULL)
			free(env_ptr->env_key);
		if (splited[1] != NULL)
			env_ptr->env_key = ft_strdup(splited[1]);
		else if (data->str1 != NULL)
			env_ptr->env_key = ft_strdup("");
		else
			env_ptr->env_key = NULL;
	}
}

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
