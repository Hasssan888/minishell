/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:09:11 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/11 16:08:20 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	print_array(char **array)
{
	int	i;

	i = 0;
	if (!array || !array[0])
		return ;
	while (array != NULL && array[i] != NULL)
		printf("%s\n", array[i++]);
}

void	pwd(t_data *data)
{
	char *cwd;
	t_env *env_ptr = NULL;
	cwd = getcwd(NULL, 0);
	if (cwd)
		printf("%s\n", cwd);
	else
	{
		env_ptr = get_env_ele_ptr(data->env, "PWD");
		if (env_ptr && env_ptr->env_key)
			printf("%s\n", env_ptr->env_key);	
	}
	free(cwd);
	g_exit_stat = 0;
}

void	del_one(t_env **env, t_env *env_var)
{
	t_env	*current;

	current = env_var->next;
	free(env_var->env_value);
	free(env_var->env_key);
	free(env_var);
	*env = current;
}

void	del_node(t_data *data, t_env *env_var)
{
	t_env	**env;
	t_env	*current;
	t_env	*previous;

	env = &data->env;
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
			free_env_ptr(&current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

int	unset(t_data *data, char **env_var)
{
	int		i;
	t_env	*env_ptr;

	i = -1;
	while (env_var[++i] != NULL)
	{
		env_ptr = get_env_ele_ptr(data->env, env_var[i]);
		if (!env_ptr)
			continue ;
		del_node(data, env_ptr);
	}
	free_array(data->envirenment);
	data->envirenment = env_to_array_(data->env);
	g_exit_stat = 0;
	return (0);
}
