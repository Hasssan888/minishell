/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:17:53 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/06 10:19:58 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	printf("%s\n", cwd);
	g_exit_stat = 0;
	free(cwd);
}

void	del_one(t_env **env, t_env *env_var)
{
	t_env	*current;

	current = env_var->next;
	free(env_var->env_value);
	free(env_var);
	*env = current;
}

void	del_node(t_env **env, t_env *env_var)
{
	t_env	*current;
	t_env	*previous;

	if (!env || !*env || !env_var)
		return ;
	if (*env == env_var)
	{
		del_one(env, env_var);
		return ;
	}
	previous = *env;
	current = (*env)->next;
	while (current)
	{
		if (current == env_var)
		{
			previous->next = current->next;
			free(current->env_value);
			free(current->env_key);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

int	unset(t_data *data, char **env_var, t_env *envirenement)
{
	int		i;
	t_env	*env_ptr;

	i = 0;
	while (env_var[i] != NULL)
	{
		env_ptr = get_env_ele_ptr(data->env, env_var[i++]);
		if (!env_ptr)
			continue ;
		del_node(&envirenement, env_ptr);
	}
	free_array(data->envirenment);
	data->envirenment = env_to_array_(data->env);
	g_exit_stat = 0;
	return (0);
}

int	env(t_env *env)
{
	while (env != NULL)
	{
		if (env->env_key != NULL)
			printf("%s=%s\n", env->env_value, env->env_key);
		env = env->next;
	}
	g_exit_stat = 0;
	return (0);
}
