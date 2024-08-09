/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:46:01 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/09 12:52:52 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

int	if_is_buil(t_command *command)
{
	if (!command || !command->value)
		return (0);
	if (ft_strcmp(command->value, "echo") == 0)
		return (1);
	else if (ft_strcmp(command->value, "pwd") == 0)
		return (1);
	else if (ft_strcmp(command->value, "cd") == 0)
		return (1);
	else if (ft_strcmp(command->value, "env") == 0)
		return (1);
	else if (ft_strcmp(command->value, "export") == 0)
		return (1);
	else if (ft_strcmp(command->value, "unset") == 0)
		return (1);
	else if (ft_strcmp(command->value, "exit") == 0)
		return (1);
	return (0);
}

void	excut_butlin(t_data *data, t_command *node1, char **env, t_pipex *p)
{
	if (p->d == 1)
		is_builtin_cmd(data, node1);
	else
	{
		if (if_is_buil(node1))
		{
			is_builtin_cmd(data, node1);
			clear_list(&data->list);
			clear_all(data);
			exit(0);
		}
		else
			ft_excute(node1->args, env);
	}
}

void	ft_count_pipe(t_command *list, t_pipex *p)
{
	t_command	*cur;

	cur = list;
	p->count_pipe = 0;
	while (cur)
	{
		if (cur->type == PIPE)
			p->count_pipe++;
		cur = cur->next;
	}
}

void	ft_count_read_out(t_command *node, t_pipex *p)
{
	t_command	*cur;

	cur = node;
	p->count_read_out = 0;
	while (cur)
	{
		if (cur->type == PIPE)
			break ;
		if (cur->type == RED_OUT || cur->type == APP)
			p->count_read_out++;
		cur = cur->next;
	}
}

void	ft_count_read_in(t_command *node, t_pipex *p)
{
	t_command	*cur;

	cur = node;
	p->count_read_in = 0;
	while (cur)
	{
		if (cur->type == PIPE)
			break ;
		if (cur->type == RED_IN)
			p->count_read_in++;
		cur = cur->next;
	}
}
