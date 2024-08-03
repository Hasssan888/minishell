/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:44:49 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/03 13:16:23 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	infile(t_data *data, t_command *node1, char **env, t_pipex *p)
{
	if (p->indixe == 1)
	{
		p->indixe = 0;
		exit(1);
	}
	close(p->end[0]);
	dup2(p->infile, STDIN_FILENO);
	close(p->infile);
	if (p->w == 1)
	{
		dup2(p->end[1], STDOUT_FILENO);
		close(p->end[1]);
	}
	else if (p->w == 2)
	{
		dup2(p->outfile, STDOUT_FILENO);
		close(p->outfile);
	}
	excut_butlin(data, node1, env);
}

void	outfile(t_data *data, t_command *node1, char **env, t_pipex *p)
{
	if (p->indixe == 1)
	{
		p->indixe = 0;
		exit(1);
	}
	dup2(p->outfile, STDOUT_FILENO);
	close(p->outfile);
	excut_butlin(data, node1, env);
}

int	check_redout(t_command *node1)
{
	t_command *cur = node1;

	while (cur)
	{
		if (cur->type == PIPE)
			break;
		else if (cur->type == RED_OUT)
			return(1);
		cur = cur->next;
	}
	return(0);
}

void	pipe_heredoc(t_data *data, t_command *node1, char **env, t_pipex *p)
{
	close(p->end[0]);
	if (p->strs != NULL)
	{
		close(p->strs[p->arr[p->k]][1]);
		dup2(p->strs[p->arr[p->k]][0], 0);
		close(p->strs[p->arr[p->k]][0]);
	}
	if (p->w  == 1)
	{
		dup2(p->end[1], 1);
		close(p->end[1]);
	}
	else if (p->w == 2)
	{

		if (p->indixe == 1)
		{
			p->indixe = 0;
			exit(1);
		}
		if (check_redout(node1) == 1)
			open_outfile(node1, p);
		if (p->b == 1)
			p->outfile = open(p->s ,O_WRONLY | O_CREAT | O_TRUNC,
						0644);
		else if (p->b == 2)
			p->outfile = open(p->s ,O_WRONLY | O_CREAT | O_APPEND,
						0644);
		free(p->s);
		dup2(p->outfile, 1);
		close(p->outfile);
	}

	excut_butlin(data, node1, env);
}
