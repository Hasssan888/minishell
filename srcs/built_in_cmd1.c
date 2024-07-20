/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:43:58 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/19 16:14:47 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	get_old_current_pwd(void)
{
	char *old;
	if (data->old_pwd != NULL)
	{
		if (!*(data->old_pwd))
			free(*(data->old_pwd));
		old = *(data->current_pwd);
		*(data->old_pwd) = old;
	}
	if (data->current_pwd != NULL)
	{
		if (!*(data->current_pwd))
			free(*(data->current_pwd));
		*(data->current_pwd) = ft_strjoin(ft_strdup("PWD="), getcwd(NULL, 0));
	}
}

int	change_dir(char *path)
{
	if (chdir(path))
		return (0);
	free(data->prompt);
	get_old_current_pwd();
	data->prompt = get_prompt();
	return (1);
}

void	check_errors(void)
{
	if (errno == EACCES)
		ft_perror("minishell: cd: Permission denied\n");
	else if (errno == ENOENT)
		ft_perror("minishell: cd: No such file or directory\n");
	else if (errno == ENOTDIR)
		ft_perror("minishell: cd: No such file or directory\n");
}

char *getenvval(char **env_ptr)
{
	char *old = NULL;
	if (env_ptr != NULL)
	{
		old = *(env_ptr);
		old = ft_strchr(old, '=');
		old = ft_strdup(&old[1]); 	
		free(*env_ptr);
	}
	return (old);
}

int	cd(char **args)
{
	char *path = NULL;
	if (args[0] != NULL && args[1] != NULL)
	{
		ft_perror("cd: too many arguments\n");
		return (0);
	}
	else if (args[0] != NULL && args[0][0] == '-' && args[0][1] == '\0')
	{
		path = getenvval(data->old_pwd);
		if (path != NULL)
		{
			// printf("%s\n", *(data->old_pwd));
			printf("%s\n", path);
			change_dir(path);
			free(path);
			// free(args[0]);
		}
		return (1);
	}
	else if (args[0] == NULL || (args[0][0] == '~' && args[0][1] == '\0'))
	{
		path = get_env_element("HOME");
		// printf("%s\n", path);
		if (!path || !path[0])
		{
			free(path);
			ft_perror("cd: HOME not set\n");
			return (0);
		}
		change_dir(path);
		return (1);
	}
	else if (args != NULL && change_dir(args[0]))
		return (1);
	else
		check_errors();
	return (0);
}

int	env(t_env *env)
{
	while (env != NULL)
	{
		if (ft_strchr(env->value, '='))
			printf("%s\n", env->value);
		env = env->next;
	}
	return (0);
}

void	echo_it(char **cmd, int i)
{
	while (cmd[i] != NULL)
	{
		printf("%s", cmd[i]);
		printf(" ");
		i++;
	}
}

int	echo(char **cmd)
{
	int		i;
	bool	flag;

	i = 0;
	if (!cmd[0])
		return (0);
	flag = false;
	// if (!ft_strncmp("-n", cmd[i], ft_strlen(cmd[i])))
	if (cmd[0][0] == '-' && cmd[0][1] == 'n')
	{
		flag = true;
		++i;
	}
	echo_it(cmd, i);
	if (!flag)
		printf("\n");
	return (0);
}

int	is_builtin_cmd(t_command *command)
{
	// printf("\n\n%s\n\n", command->value);
	if (ft_strcmp(command->value, "echo") == 0)
		echo(&command->args[1]);
	else if (ft_strcmp(command->value, "pwd") == 0)
		pwd();
	else if (ft_strcmp(command->value, "cd") == 0)
		cd(&command->args[1]);
	else if (ft_strcmp(command->value, "env") == 0)
		env(data->env);
	else if (ft_strcmp(command->value, "export") == 0)
		export(command, data->env);
	else if (ft_strcmp(command->value, "unset") == 0)
		unset(command->args[1], data->env);
	else if (ft_strcmp(command->value, "exit") == 0)
	{
		printf("exit\n");
		clear_list(&data->list);
		exit(0);
	}
	else
		return (0);
	return (1);
}
