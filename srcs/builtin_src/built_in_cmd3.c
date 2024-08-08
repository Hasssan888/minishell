/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:09:15 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/08 10:21:21 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

int	check_exit(char *str, long k)
{
	return ((k < 0 && str[0] != '-') || (k > 0 && str[0] == '-'));
}

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

void	export__cnt(t_data *data, t_env *env_ptr, char *arg)
{
	char	**splited;
	char	*trimed;

	trimed = NULL;
	splited = NULL;
	splited = get_exp_splited(arg, '=');
	data->str1 = ft_strchr(arg, '=');
	trimed = ft_strtrim(splited[0], "+");
	env_ptr = get_env_ele_ptr(data->env, trimed);
	if (env_ptr != NULL && splited != NULL && splited[0] != NULL
		&& data->str1 != NULL)
		export_var_app(data, env_ptr, splited);
	else if (!env_ptr)
	{
		if (data->str1 != NULL && splited[1] != NULL)
			add_back(&data->env, lstnew(ft_strdup(trimed),
					ft_strdup(splited[1])));
		else if (data->str1 != NULL)
			add_back(&data->env, lstnew(ft_strdup(trimed), ft_strdup("")));
		else
			add_back(&data->env, lstnew(ft_strdup(arg), NULL));
	}
	free_array(splited);
	free(trimed);
}

int	export(t_data *data, t_command *cmd, t_env *env)
{
	int		i;
	t_env	*env_ptr;

	i = 0;
	env_ptr = NULL;
	if (!cmd->args[1])
	{
		print_sorted_env(env);
		g_exit_stat = 0;
		return (0);
	}
	while (cmd->args[++i] != NULL)
	{
		cmd->args[i] = unquote_arg(data->list, cmd->args[i], 0, 0);
		if (!valid_identifier(cmd->args[i]))
		{
			perror("minishell: export: not a valid identifier\n");
			g_exit_stat = 1;
			continue ;
		}
		export__cnt(data, env_ptr, cmd->args[i]);
	}
	free_array(data->envirenment);
	data->envirenment = env_to_array_(data->env);
	return (0);
}
