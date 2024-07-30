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
	if (p->indixe == 1)
	{
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
	excut_butlin(node1, env);
}

void	outfile(t_command *node1, char **env, t_pipex *p)
{
	// readout_append(node1, p);
	// close(p->end[0]);
	// close(p->end[1]);
	if (p->indixe == 1)
	{
		exit(1);
	}
	dup2(p->outfile, STDOUT_FILENO);
	close(p->outfile);
	excut_butlin(node1, env);
}


void	pipe_heredoc(t_command *node1, char **env, t_pipex *p)
{
	printf("pipe_heredoc\n");
	close(p->end[0]);
	close(p->strs[p->arr[p->k]][1]);
	dup2(p->strs[p->arr[p->k]][0], 0);
	close(p->strs[p->arr[p->k]][0]);
	printf("p->w = %d\n", p->w);
	if (p->w  == 1)
	{
		printf("here\n");
		dup2(p->end[1], 1);
		close(p->end[1]);
	}
	else if (p->w == 2)
	{

		if (p->indixe == 1)
			exit(1);
		open_outfile(node1, p);;
		if (p->b == 1)
			p->outfile = open(p->s ,O_WRONLY | O_CREAT | O_TRUNC,
						0644);
		else if (p->b == 2)
			p->outfile = open(p->s ,O_WRONLY | O_CREAT | O_APPEND,
						0644);
		dup2(p->outfile, 1);
		close(p->outfile);
	}

	excut_butlin(node1, env);
}
