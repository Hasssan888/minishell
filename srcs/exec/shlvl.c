/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 11:49:23 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/13 09:58:37 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	child_process(t_data *data, t_command *node1, char **env, t_pipex *p)
{
	shlvl_update(data);
	if (p->flag == 1)
		infile(data, node1, env, p);
	else if (p->flag == 2)
		pipe_heredoc(data, node1, env, p);
	else if (p->w == 2)
		outfile(data, node1, env, p);
	else if (node1->type == TOKEN && node1->next == NULL)
		excut_butlin(data, node1, env, p);
	else
	{
		close(p->end[0]);
		dup2(p->end[1], STDOUT_FILENO);
		close(p->end[1]);
		excut_butlin(data, node1, env, p);
	}
}

char	**get_env_add_ele(char **env)
{
	int	i;

	i = -1;
	while (env && env[++i])
	{
		if (!ft_strncmp(env[i], "SHLVL", ft_strlen("SHLVL")))
			return (&env[i]);
	}
	return (NULL);
}

void	shlvl_update(t_data *data)
{
	int		a;
	char	**ptr;
	t_env	*env_ptr;

	ptr = NULL;
	env_ptr = NULL;
	env_ptr = get_env_ele_ptr(data->env, "SHLVL");
	if (env_ptr && env_ptr->env_key)
	{
		a = ft_atoi(env_ptr->env_key) + 1;
		ptr = get_env_add_ele(data->envirenment);
		if (ptr && *ptr)
		{
			free(*ptr);
			*ptr = ft_strjoin(ft_strdup("SHLVL="), ft_itoa(a));
		}
	}
}
