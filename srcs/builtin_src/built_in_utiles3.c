/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:44:30 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/05 11:45:18 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

int is_dir_exist(char *path)
{
    struct stat statbuf;

    if (stat(path, &statbuf) != 0) {
        perror("stat");
        return 1;
    }

    if (S_ISDIR(statbuf.st_mode)) {
        printf("'%s' is a directory.\n", path);
		return 0;
    } else {
        printf("'%s' is not a directory.\n", path);
		return 1;
    
	}
    return 0;
}

void	get_old_current_pwd(t_data *data)
{
	t_env	*old_pwd;
	t_env	*pwd;

	old_pwd = get_env_ele_ptr(data, "OLDPWD");
	pwd = get_env_ele_ptr(data, "PWD");
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

void	check_cd_errors(void)
{
	if (errno == EACCES)
		ft_perror("minishell: cd: Permission denied\n");
	else if (errno == ENOENT)
		ft_perror("minishell: cd: No such file or directory\n");
	else if (errno == ENOTDIR)
		ft_perror("minishell: cd: No such file or directory\n");
	g_exit_stat = 1;
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

bool	check_echo_options(char *cmd)
{
	int		j;
	bool	flag;

	j = 0;
	flag = false;
	if (!cmd)
		return (flag);
	if (cmd[j] == '-')
		while (cmd[++j] == 'n')
			;
	if (!cmd[j])
		flag = true;
	return (flag);
}
