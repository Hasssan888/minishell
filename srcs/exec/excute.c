/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:17:43 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/09 16:55:13 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	*function(char **env)
{
	int		i;
	char	*path_from_envp;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH", ft_strlen("PATH")) == 0)
			break ;
		i++;
	}
	if (!env[i])
		return (NULL);
	path_from_envp = ft_substr(env[i], ft_strlen("PATH="), ft_strlen(env[i]));
	return (path_from_envp);
}

char	*slash(char *mycmdargs)
{
	if (access(mycmdargs, F_OK) == 0)
		return (mycmdargs);
	else
	{
		ft_putstr_fd(mycmdargs, 2);
		return (NULL);
	}
}

char	*without_slash(t_data *data, char **env, char *mycmdargs)
{
	t_path	path;

	path.path_from_envp = function(env);
	if (!(path.path_from_envp))
		return (ft_error(data, &mycmdargs), NULL);
	path.mypaths = ft_split(path.path_from_envp, ':');
	free(path.path_from_envp);
	path.i = 0;
	while (path.mypaths[path.i])
	{
		path.part_path = strjoin1(path.mypaths[path.i], "/");
		path.path = strjoin1(path.part_path, mycmdargs);
		free(path.part_path);
		if (access(path.path, F_OK) == 0)
			return (path.path);
		free(path.path);
		path.i++;
	}
	path.i = -1;
	while (path.mypaths[++path.i])
		free(path.mypaths[path.i]);
	free(path.mypaths);
	return (0);
}

char	*search_path(t_data *data, char *mycmdargs, char **env)
{
	if (ft_strchr(mycmdargs, '/') != 0)
		return (slash(mycmdargs));
	else
		return (without_slash(data, env, mycmdargs));
}

void	ft_excute(t_data *data, char **av, char **env)
{
	char	**mycmdargs;
	char	*path;

	if (av[0] && (ft_strcmp(av[0], "") == 0 || ft_strcmp(av[0], ".") == 0
			|| ft_strcmp(av[0], "..") == 0 || ft_strcmp(av[0], " ") == 0))
		ft_error(data, av);
	else
	{
		mycmdargs = av;
		if (!handle_direct(av))
			return ;
		path = search_path(data, mycmdargs[0], env);
		if (!path)
			ft_error(data, av);
		else
			execve(path, mycmdargs, env);
	}
}
