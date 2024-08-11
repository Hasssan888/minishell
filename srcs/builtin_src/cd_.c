/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:02:24 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/11 21:09:02 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	get_old_current_pwd(t_data *data)
{
	t_env	*old_pwd;
	t_env	*pwd;

	old_pwd = get_env_ele_ptr(data->env, "OLDPWD");
	if (!old_pwd)
		return ;
	pwd = get_env_ele_ptr(data->env, "PWD");
	if (!pwd)
	{
		// free_env_ptr(&old_pwd);
		return ;
	}
	if (old_pwd != NULL && old_pwd->env_key != NULL)
		free(old_pwd->env_key);
	old_pwd->env_key = ft_strdup(pwd->env_key);
	if (pwd != NULL && pwd->env_key != NULL)
		free(pwd->env_key);
	pwd->env_key = getcwd(NULL, 0);
}

int	change_dir(t_data *data, t_env *env, char *path)
{
	if (path != NULL)
	{
		if (chdir(path) != 0)
			return (0);
	}
	else if (env != NULL)
	{
		if (chdir(env->env_key) != 0)
			return (0);
	}
	free(data->prompt);
	get_old_current_pwd(data);
	return (1);
}

int	chdir_home_prev(t_data *data, t_env *env, char **args)
{
	if (args[0] != NULL && args[0][0] == '-' && args[0][1] == '\0')
	{
		env = get_env_ele_ptr(data->env, "OLDPWD");
		if (env != NULL && env->env_key != NULL)
		{
			printf("%s\n", env->env_key);
			change_dir(data, env, NULL);
		}
		return (1);
	}
	else if (args[0] == NULL || (args[0][0] == '~' && args[0][1] == '\0'))
	{
		env = get_env_ele_ptr(data->env, "HOME");
		if (!env || !env->env_key)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			g_exit_stat = 1;
			return (0);
		}
		change_dir(data, env, NULL);
		return (1);
	}
	return (0);
}

int	cd(t_data *data, char **args)
{
	t_env	*env;

	env = NULL;
	if (args[0] && args[0][0] != '-' && args[0][0] != '~' && access(args[0],
			F_OK) != 0)
	{
		perror("minishell: cd");
		g_exit_stat = 1;
		return (0);
	}
	if (args[0] != NULL && args[1] != NULL)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		g_exit_stat = 1;
		return (0);
	}
	else if (args[0] != NULL && args[0][0] == '-' && args[0][1] == '\0')
		return (chdir_home_prev(data, env, args));
	else if (args[0] == NULL || (args[0][0] == '~' && args[0][1] == '\0'))
		return (chdir_home_prev(data, env, args));
	else if (args != NULL && change_dir(data, NULL, args[0]))
		return (1);
	return (0);
}
