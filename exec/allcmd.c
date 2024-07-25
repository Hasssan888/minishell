/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbakrim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:44:49 by hbakrim           #+#    #+#             */
/*   Updated: 2024/07/16 12:44:51 by hbakrim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	infile(t_command *node1, char **env, t_pipex *p)
{
	if (p->indixe == 1 && p->count_pipe == 0)
		exit(1);
	close(p->end[0]);
	dup2(p->infile, STDIN_FILENO);
	close(p->infile);
	if (node1->next && node1->next->type == PIPE)
	{
		dup2(p->end[1], STDOUT_FILENO);
		close(p->end[1]);
	}
	else if (node1->next && (node1->next->type == RED_OUT
			|| node1->next->type == APP))
	{
		readout_append(node1, p);
		dup2(p->outfile, STDOUT_FILENO);
		close(p->outfile);
	}
	excut_butlin(node1, env);
}

void	outfile(t_command *node1, char **env, t_pipex *p)
{
	readout_append(node1, p);
	close(p->end[0]);
	close(p->end[1]);
	dup2(p->outfile, STDOUT_FILENO);
	close(p->outfile);
	excut_butlin(node1, env);
}

void	one_here_doc(t_command *node1, char **env, t_pipex *p)
{
	close(p->end[0]);
	p->tb[p->k] = open(p->strs[p->k], O_RDONLY, 0644);
	if (p->tb[p->k] == -1)
	{
		perror("infile_here_doc");
		exit(1);
	}
	dup2(p->tb[p->k], 0);
	close(p->tb[p->k]);
	excut_butlin(node1, env);
}

void	heredoc_readout_app(t_command *node1, char **env, t_pipex *p)
{
	close(p->end[0]);
	if (p->b == 0)
		readout_append_2(node1, p);
	else if (p->b == 1)
		readout_append(node1, p);
	p->tb[p->k] = open(p->strs[p->k], O_RDONLY, 0644);
	if (p->tb[p->k] == -1)
	{
		perror("infile_here_doc");
		exit(1);
	}
	dup2(p->tb[p->k], 0);
	close(p->tb[p->k]);
	dup2(p->outfile, 1);
	close(p->outfile);
	excut_butlin(node1, env);
}

void	pipe_heredoc(t_command *node1, char **env, t_pipex *p)
{
	close(p->end[0]);
	p->tb[p->k] = open(p->strs[p->k], O_RDONLY, 0644);
	if (p->tb[p->k] == -1)
	{
		perror("infile_here_doc");
		exit(1);
	}
	dup2(p->tb[p->k], 0);
	close(p->tb[p->k]);
	dup2(p->end[1], 1);
	close(p->end[1]);
	excut_butlin(node1, env);
}
