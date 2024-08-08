/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:44:30 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/08 10:24:32 by aelkheta         ###   ########.fr       */
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
		free_env_ptr(&old_pwd);
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
	data->prompt = get_prompt();
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
			panic("malloc fail\n");
		arr[0] = ft_substr(str, 0, ft_strlen(str) - ft_strlen(ch_str));
		arr[1] = ft_substr(str, ft_strlen(str) - ft_strlen(ch_str) + 1,
				ft_strlen(str));
		arr[2] = NULL;
	}
	else
	{
		arr = ft_calloc(2, sizeof(char *));
		if (!arr)
			panic("malloc fail\n");
		arr[0] = ft_strdup(str);
		arr[1] = NULL;
	}
	return (arr);
}

// void	check_cd_errors(void)
// {
// 	if (errno == EACCES)
// 		perror("minishell: cd: Permission denied\n");
// 	else if (errno == ENOENT)
// 		perror("minishell: cd: No such file or directory\n");
// 	else if (errno == ENOTDIR)
// 		perror("minishell: cd: No such file or directory\n");
// 	g_exit_stat = 1;
// }

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

bool	check_echo_options(char *cmd)
{
	int		j;
	bool	flag;

	j = 0;
	flag = false;
	if (!cmd || !cmd[0])
		return (flag);
	if (cmd[j] == '-')
		while (cmd[++j] == 'n')
			;
	if (!cmd[j])
		flag = true;
	return (flag);
}
