/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbakrim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:46:01 by hbakrim           #+#    #+#             */
/*   Updated: 2024/07/16 12:46:03 by hbakrim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	if_is_buil(t_command *command)
{
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

void	excut_butlin(t_command *node1, char **env)
{
	if (if_is_buil(node1))
	{
		is_builtin_cmd(node1);
		exit(0);
	}
	else
		ft_excute(node1->args, env);
}

void	readout_append(t_command *node1, t_pipex *p)
{
	if (node1->next->next && (node1->next->next->type == RED_OUT
			|| node1->next->next->type == APP))
		open_outfile(node1, p);
	else
	{
		if (node1->next->type == APP)
			p->outfile = open(node1->next->args[0],
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			p->outfile = open(node1->next->args[0],
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (p->outfile == -1)
		{
			printf("%s: Permission denied\n", node1->next->args[0]);
			exit(1);
		}
	}
}

void	readout_append_2(t_command *node1, t_pipex *p)
{
	t_command * c = node1;
	while (c)
	{
		if (c->type == RED_OUT)
			break;
		c = c->next;
	}
	printf("c->arg[0] = %s\n", c->args[0]);
	if (c->next && (c->next->type == RED_OUT
			|| c->next->type == APP))
		open_outfile(c, p);
	else
	{
		printf("outfile\n");
		if (c->type == APP)
			p->outfile = open(c->args[0],
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			p->outfile = open(c->args[0],
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		printf("p->outfile = %d\n", p->outfile);
		if (p->outfile == -1)
		{
			printf("%s: Permission denied\n", c->args[0]);
			exit(1);
		}

	}
}
