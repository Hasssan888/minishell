/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utiles2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:42:13 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/09 14:44:53 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*get_env_element(char *env_var)
{
	t_env	*env;
	char	*str;

	env = data->env;
	if (!env_var)
		return (NULL);
	while (env != NULL)
	{
		str = ft_strnstr_l(env->value, env_var, ft_strlen(env_var));
		if (str != NULL)
			return (ft_strdup(&str[1]));
		env = env->next;
	}
	return (ft_strdup(""));
}

// char	*get_cmd_path(char *cmd_)
// {
// 	int		i;
// 	char	*cmd_path;
// 	char	*env_ele;
// 	char	**path;
// 	char	*cmd;

// 	i = 0;
// 	env_ele = get_env_element("PATH");
// 	path = ft_split(env_ele, ':');
// 	free(env_ele);
// 	while (path[i])
// 	{
// 		cmd = ft_strjoin("/", cmd_);
// 		cmd_path = ft_strjoin(path[i], cmd);
// 		free(cmd);
// 		if (access(cmd_path, X_OK) == 0)
// 		{
// 			free_array(path);
// 			return (cmd_path);
// 		}
// 		free(cmd_path);
// 		i++;
// 	}
// 	free_array(path);
// 	return (NULL);
// }
