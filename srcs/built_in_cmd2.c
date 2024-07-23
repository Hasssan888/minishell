/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:17:53 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/23 15:14:23 by aelkheta         ###   ########.fr       */
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

int	export(t_command *cmd, t_env *env)
{
	// char	*word;
	// t_env	*env_var;

	if (!cmd->args[1])
	{
		while(env != NULL)
		{
			if (env->env_key != NULL)
				printf("%s=\"%s\"\n", env->env_value, env->env_key);
			else
				printf("%s=\n", env->env_value);
			env = env->next;	
		}
		return (0);
	}
	else if (ft_isdigit(cmd->args[1][0]) || cmd->args[2] != NULL)
	{
		ft_perror ("minishell: export: not a valid identifier\n");
		data->exit_status = 1;
		return (0);
	}
		// sort_list(envir);
	// else
	// {
		// word = get_word_(cmd->args[1], "+=");
		// env_var = get_env_ele_ptr(word);
		data->str1 = ft_strchr(cmd->args[1], '=');
		char **splited = ft_split(cmd->args[1], '=');
		// free(word);
		// if (!data->str1)
		// 	return (0);
		char *trimed = ft_strtrim(splited[0], "+");
		t_env *env_ptr = get_env_ele_ptr(trimed);
		free(trimed);
		if (env_ptr != NULL && splited != NULL && splited[0] != NULL)
		{
			// if (env_ptr != NULL)
			// printf("value %s %c\n", splited[0], splited[0][ft_strlen(splited[0]) -1]);
			if (splited[0][ft_strlen(splited[0]) - 1] == '+' && env_ptr != NULL)
				env_ptr->env_key = ft_strjoin(env_ptr->env_key, ft_strdup(splited[1]));
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
			// if (env_var->env_value != NULL)
			// 	free(env_var->env_value);
			// env_var->env_value = ft_strdup(cmd->args[1]);
			// else if (env_var != NULL && data->str1[-1] == '+')
			// 	env_var->env_value = ft_strjoin(env_var->env_key,
			// 			ft_strdup(&data->str1[1]));
		}
			// if (splited[0][ft_strlen(splited[0])] == '+' && env_ptr != NULL)
		else
		{
			printf("%s\n", cmd->args[1]);
			add_back(&data->env, lstnew(ft_strdup(cmd->args[1]), NULL));
		}
		free_array(splited);
	// }
	return (0);
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
