/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:09:15 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/29 09:34:30 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

int	export(t_command *cmd, t_env *env)
{
	int i = 0;
	if (!cmd->args[1])
	{
		print_sorted_env(env);
		data->exit_status = 0;
		return (0);
	}
	while(cmd->args[++i] != NULL)
	{
		if (!valid_identifier(cmd->args[i]))
		{
			ft_perror ("minishell: export: not a valid identifier\n");
			data->exit_status = 1;
			continue;
		}
		char **splited = ft_split(cmd->args[i], '=');
		data->str1 = ft_strchr(cmd->args[i], '=');
		char *trimed = ft_strtrim(splited[0], "+");
		t_env *env_ptr = get_env_ele_ptr(trimed);
		if (env_ptr != NULL && splited != NULL && splited[0] != NULL && data->str1 != NULL)
		{
			if (env_ptr->env_key != NULL && splited[0][ft_strlen(splited[0]) - 1] == '+')
			{
				if (splited[1] != NULL)
					env_ptr->env_key = ft_strjoin(env_ptr->env_key, ft_strdup(splited[1]));
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
		else if (!env_ptr)
		{
			if (data->str1 != NULL && splited[1] != NULL)
				add_back(&data->env, lstnew(ft_strdup(trimed), ft_strdup(splited[1])));
			else if (data->str1 != NULL)
				add_back(&data->env, lstnew(ft_strdup(trimed), ft_strdup("")));
			else
				add_back(&data->env, lstnew(ft_strdup(cmd->args[i]), NULL));
		}
		free_array(splited);
		free(trimed);
	}

	return (0);
}