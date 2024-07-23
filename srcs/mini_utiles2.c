/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utiles2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:42:13 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/20 18:37:46 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*get_env_element(char *env_var)
{
	t_env	*env;

	env = data->env;
	if (!env_var)
		return (NULL);
	while (env != NULL)
	{
		if (ft_strcmp(env->env_value, env_var) == 0)
			return (ft_strdup(env->env_key));
		env = env->next;
	}
	return (ft_strdup(""));
}

char	*get_cmd_path(char *cmd_)
{
	int		i;
	char	*cmd_path;
	char	*env_ele;
	char	**path;
	// char	*cmd;

	i = 0;
	// cmd_path = ft_strjoin(ft_strdup("./"), ft_strdup(cmd_));
	// printf("cmd_path: %s\n", cmd_path);
	if (access(cmd_, X_OK) == 0)
		return (ft_strdup(cmd_));
	env_ele = get_env_element("PATH");
	path = ft_split(env_ele, ':');
	free(env_ele);
	while (path[i])
	{
		// cmd = ft_strjoin(ft_strdup("/"), ft_strdup(cmd_));
		cmd_path = ft_strjoin(ft_strdup(path[i]), ft_strjoin(ft_strdup("/"), ft_strdup(cmd_)));
		if (access(cmd_path, X_OK) == 0)
		{
			free_array(path);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	free_array(path);
	return (NULL);
}

void	ft_perror(char *message)
{
	ft_putstr_fd(message, 2);
}
