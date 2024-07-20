/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:17:53 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/20 13:46:34 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
}

int	export(t_command *cmd, t_env *envir)
{
	char	*word;
	t_env	*env_var;

	if (!cmd->args[1])
		sort_list(envir);
	else
	{
		data->str1 = ft_strchr(cmd->args[1], '=');
		word = get_word_(cmd->args[1], "+=");
		env_var = get_env_ele_ptr(word);
		free(word);
		// if (!data->str1)
		// 	return (0);
		if (env_var != NULL && data->str1[0] == '='
			&& data->str1[-1] != '+')
		{
			if (env_var->value != NULL)
				free(env_var->value);
			env_var->value = ft_strdup(cmd->args[1]);
		}
		else if (env_var != NULL && data->str1[-1] == '+')
			env_var->value = ft_strjoin(env_var->value,
					ft_strdup(&data->str1[1]));
		else
			add_back(&data->env, lstnew(ft_strdup(cmd->args[1])));
	}
	return (0);
}

void	del_one(t_env **env, t_env *env_var)
{
	t_env	*current;

	current = env_var->next;
	free(env_var->value);
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
			free(current->value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

int	unset(char *env_var, t_env *envirenement)
{
	t_env	*env_ptr;

	if (!env_var)
		return (0);
	env_ptr = get_env_ele_ptr(env_var);
	if (!env_ptr)
		return (1);
	del_node(&envirenement, env_ptr);
	return (0);
}
