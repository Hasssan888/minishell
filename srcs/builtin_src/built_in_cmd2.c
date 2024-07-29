/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:17:53 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/29 09:34:28 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	data->expanded = 0;
	free(cwd);
}

char **env_to_array_(t_env *env)
{
	int i = 0;
	int env_len = env_c_len(env);
	char **array = malloc((env_len + 1) *sizeof(char *));
	if (!array || !env)
		return NULL;
	while (env != NULL)
	{
		if (!env->env_key)
			array[i++] = ft_strjoin(ft_strdup(env->env_value), ft_strdup(""));
		else if (!env->env_key[0])
			array[i++] = ft_strjoin(ft_strdup(env->env_value), ft_strdup("="));
		else
			array[i++] = ft_strjoin(ft_strjoin(ft_strdup(env->env_value), ft_strdup("=")), ft_strdup(env->env_key));
		env = env->next;
	}
	array[i] = NULL;
	return (array);
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
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

int	unset(char **env_var, t_env *envirenement)
{
	int i = 0;
	t_env	*env_ptr;
	
	while(env_var[i] != NULL)
	{
		env_ptr = get_env_ele_ptr(env_var[i++]);
		if (!env_ptr)
			continue;
		del_node(&envirenement, env_ptr);		
	}
	data->expanded = 0;
	return (0);
}
