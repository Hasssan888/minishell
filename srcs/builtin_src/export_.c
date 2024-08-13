/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:03:08 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/13 09:55:43 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	print_sorted_env(t_env *env)
{
	t_env	*env_;
	int		env_len;
	int		i;
	t_env	**env__;

	env_ = env;
	env_len = env_c_len(env_);
	i = 0;
	env__ = malloc((env_len + 1) * sizeof(t_env *));
	if (!env__)
		panic("malloc fail\n", 1);
	while (i < env_len)
	{
		env__[i] = env_;
		env_ = env_->next;
		i++;
	}
	env__[i] = NULL;
	env__ = sort_env(env__, env_len);
	print_export_env(env__, env_len);
	free(env__);
}

int	valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (str[++i] && str[i] != '=')
	{
		if (str[i] == '+')
			break ;
		else if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
	}
	if (str[i] && str[i] == '+' && str[i + 1] != '=')
		return (0);
	return (1);
}

char	**get_exp_splited(char *str, char del)
{
	char	*ch_str;
	char	**arr;

	ch_str = ft_strchr(str, del);
	if (!str)
		return (NULL);
	if (ch_str)
	{
		arr = ft_calloc(3, sizeof(char *));
		if (!arr)
			panic("malloc fail\n", 1);
		arr[0] = ft_substr(str, 0, ft_strlen(str) - ft_strlen(ch_str));
		arr[1] = ft_substr(str, ft_strlen(str) - ft_strlen(ch_str) + 1,
				ft_strlen(str));
		arr[2] = NULL;
	}
	else
	{
		arr = ft_calloc(2, sizeof(char *));
		if (!arr)
			panic("malloc fail\n", 1);
		arr[0] = ft_strdup(str);
		arr[1] = NULL;
	}
	return (arr);
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
		if (!valid_identifier(cmd->args[i]))
		{
			put_error(cmd->args[i]);
			g_exit_stat = 1;
			continue ;
		}
		export__cnt(data, env_ptr, cmd->args[i]);
	}
	free_array(data->envirenment);
	data->envirenment = env_to_array_(data->env);
	return (0);
}
