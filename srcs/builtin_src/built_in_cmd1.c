/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:43:58 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/03 13:21:48 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../libraries/minishell.h"

void	get_old_current_pwd(t_data *data)
{
	t_env *old_pwd = get_env_ele_ptr(data, "OLDPWD");
	t_env *pwd = get_env_ele_ptr(data, "PWD");

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
			return 0;	
	}
	else if (env != NULL)
	{
		if (chdir(env->env_key) != 0)
			return 0;
	}
	free(data->prompt);
	get_old_current_pwd(data);
	data->prompt = get_prompt();
	return (1);
}

void	check_cd_errors()
{
	if (errno == EACCES)
		ft_perror("minishell: cd: Permission denied\n");
	else if (errno == ENOENT)
		ft_perror("minishell: cd: No such file or directory\n");
	else if (errno == ENOTDIR)
		ft_perror("minishell: cd: No such file or directory\n");
	// data->exit_status = 1;
	g_exit_stat = 1;
}

int	cd(t_data *data, char **args)
{
	t_env *env = NULL;

	if (args[0] != NULL && args[1] != NULL)
	{
		ft_perror("cd: too many arguments\n");
		// data->exit_status = 1;
		g_exit_stat = 1;
		return (0);
	}
	else if (args[0] != NULL && args[0][0] == '-' && args[0][1] == '\0')
	{
		env = get_env_ele_ptr(data, "OLDPWD");
		printf("path: %s\n", env->env_key);
		if (env != NULL && env->env_key != NULL)
		{
			printf("%s\n", env->env_key);
			change_dir(data, env, NULL);
		}
		return (1);
	}
	else if (args[0] == NULL || (args[0][0] == '~' && args[0][1] == '\0'))
	{
		env = get_env_ele_ptr(data, "HOME");
		if (!env || !env->env_key)
		{
			ft_perror("cd: HOME not set\n");
			// data->exit_status = 1;
			g_exit_stat = 1;
			return (0);
		}
		change_dir(data, env, NULL);
		return (1);
	}
	else if (args != NULL && change_dir(data, NULL, args[0]))
		return (1);
	else
		check_cd_errors();
	return (0);
}

int	env(t_env *env)
{
	while (env != NULL)
	{
		if (env->env_key != NULL)
			printf("%s=%s\n", env->env_value, env->env_key);
		env = env->next;
	}
	g_exit_stat = 0;
	return (0);
}

void	echo_it(char **cmd, int i)
{
	while (cmd[i] != NULL)
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1] != NULL)
			printf(" ");
		i++;
	}
}

bool check_echo_options(char *cmd)
{
	int j = 0;
	bool flag = false;
	if (!cmd)
		return (flag);
	if (cmd[j] == '-')
		while(cmd[++j] == 'n');
	if (!cmd[j])
		flag = true;
	return (flag);
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
		while(check_echo_options(cmd[++i]));
	}
	echo_it(cmd, i);
	if (!flag)
		printf("\n");
	g_exit_stat = 0;
	return (0);
}

int ft_is_str_digit(char *str)
{
	int i = -1;
	while(str[++i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
	}
	return (1);
}

void exit_(t_data *data, t_command *command)
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
		exit(ft_atoi(command->args[1]));
		// exit(ft_atoi(command->args[1]) % 256);
	printf("exit\n");
	clear_list(&data->list);
	exit(g_exit_stat);
}

int	is_builtin_cmd(t_data *data, t_command *command)
{
	if (!command)
		return 0;
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
