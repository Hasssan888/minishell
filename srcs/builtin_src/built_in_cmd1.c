/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:43:58 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/06 10:16:26 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

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
			ft_perror("cd: HOME not set\n");
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
	if (args[0] != NULL && args[1] != NULL)
	{
		ft_perror("cd: too many arguments\n");
		g_exit_stat = 1;
		return (0);
	}
	else if (args[0] != NULL && args[0][0] == '-' && args[0][1] == '\0')
		return (chdir_home_prev(data, env, args));
	else if (args[0] == NULL || (args[0][0] == '~' && args[0][1] == '\0'))
		return (chdir_home_prev(data, env, args));
	else if (args != NULL && change_dir(data, NULL, args[0]))
		return (1);
	else
		check_cd_errors();
	return (0);
}

int	echo(char **cmd)
{
	int		i;
	bool	flag;

	i = 0;
	flag = false;
	if (check_echo_options(cmd[i]))
	{
		flag = true;
		while (check_echo_options(cmd[++i]))
			;
	}
	echo_it(cmd, i);
	if (!flag)
		printf("\n");
	g_exit_stat = 0;
	return (0);
}

void	exit_(t_data *data, t_command *command)
{
	if (command->args[1] != NULL && !ft_is_str_digit(command->args[1]))
	{
		printf("exit\n");
		ft_perror("minishell: exit: numeric argument required\n");
		exit(2);
	}
	else if (command->args[1] != NULL && command->args[2] != NULL)
	{
		ft_perror("minishell: exit: too many arguments\n");
		g_exit_stat = 1;
		return ;
	}
	else if (command->args[1] != NULL)
		g_exit_stat = ft_atoi(command->args[1]);
	printf("exit\n");
	clear_list(&data->list);
	exit(g_exit_stat);
}

int	is_builtin_cmd(t_data *data, t_command *command)
{
	if (!command)
		return (0);
	if (ft_strcmp(command->value, "echo") == 0)
		echo(&command->args[1]);
	else if (ft_strcmp(command->value, "pwd") == 0)
		pwd();
	else if (ft_strcmp(command->value, "cd") == 0)
		cd(data, &command->args[1]);
	else if (ft_strcmp(command->value, "env") == 0)
		env(data->env);
	else if (ft_strcmp(command->value, "export") == 0)
		export(data, command, data->env);
	else if (ft_strcmp(command->value, "unset") == 0)
		unset(data, &command->args[1], data->env);
	else if (ft_strcmp(command->value, "exit") == 0)
		exit_(data, command);
	else
		return (0);
	return (1);
}
