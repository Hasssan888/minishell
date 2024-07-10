/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:43:58 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/10 09:55:26 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	change_dir(char *path)
{
	if (chdir(path))
		return (0);
	free(data->prompt);
	data->prompt = get_prompt();
	return (1);
}

void check_errors()
{
	if (errno == EACCES)
		ft_perror("minishell: cd: Permission denied\n");
	else if (errno == ENOENT)
		ft_perror("minishell: cd: No such file or directory\n");
	else if (errno == ENOTDIR)
		ft_perror("minishell: cd: No such file or directory\n");
}

int	cd(char **args)
{
	if (args[0] != NULL && args[1] != NULL)
	{
		ft_perror("cd: string not in pwd\n");
		return (0);
	}
	else if (args[0] != NULL && args[0][0] == '-' && args[0][1] == '\0')
	{
		args[0] = getenv("OLDPWD");
		printf("%s\n", args[0]);
		change_dir(args[0]);
		return (1);
	}
	else if (args[0] == NULL || (args[0][0] == '~' && args[0][1] == '\0'))
	{
		args[0] = getenv("HOME");
		change_dir(args[0]);
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
	char	*path;

	i = 0;
	if (!cmd[0])
		return (0);
	flag = false;
	if (!ft_strncmp("-n", cmd[i], ft_strlen(cmd[i])))
	{
		flag = true;
		++i;
	}
	if (cmd[i][0] == '$' && cmd[i][1] != ' ')
	{
		path = getenv(&cmd[i][1]);
		if (!path)
		{
			printf("\n");
			return (0);
		}
		printf("%s\n", path);
		return (0);
	}
	echo_it(cmd, i);
	if (!flag)
		printf("\n");
	return (0);
}

int	is_builtin_cmd(t_command *command)
{
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
	return (1);
}
