/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:46:17 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/11 22:27:46 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	out_app(t_command *cur, t_pipex *p)
{
	if (cur->args[0] && ft_strcmp(cur->args[0], "/dev/stdout") != 0)
	{
		if (cur->type == RED_OUT && cur->args[0] != NULL)
		{
			p->fd[p->i] = open(cur->args[0], O_WRONLY | O_CREAT | O_TRUNC,
					0644);
			p->b = 1;
		}
		else if (cur->type == APP && cur->args[0] != NULL)
		{
			p->fd[p->i] = open(cur->args[0], O_WRONLY | O_CREAT | O_APPEND,
					0644);
			p->b = 2;
		}
		if (p->fd[p->i] == -1)
		{
			perror(cur->args[0]);
			p->indixe = 1;
		}
		p->s = cur->args[0];
		p->i++;
	}
}

void	ft_loop(t_command *cur, t_pipex *p)
{
	while (cur && p->i < p->count_read_out)
	{
		if (cur->type == PIPE)
			break ;
		else if (cur->syntxerr != AMPIGOUS && (cur->type == RED_OUT
				|| cur->type == APP))
			out_app(cur, p);
		cur = cur->next;
	}
}

void	open_outfile(t_command *node, t_pipex *p)
{
	t_command	*cur;

	p->b = 0;
	p->indixe = 0;
	p->fd = malloc(sizeof(int) * p->count_read_out);
	if (p->fd == NULL)
		panic("malloc fail\n", 1);
	p->i = 0;
	cur = node;
	ft_loop(cur, p);
	if (p->i > 0)
		p->outfile = p->fd[p->i - 1];
	free(p->fd);
	p->i = -1;
}

void	red_in(t_command *cur, t_pipex *p)
{
	if (cur->args[0] && ft_strcmp(cur->args[0], "/dev/stdin") != 0)
	{
		p->fd[p->i] = open(cur->args[0], O_RDONLY, 0644);
		if (p->fd[p->i] == -1)
		{
			perror("minishell");
			p->indixe = 1;
		}
		p->i++;
	}
	else
		p->save_in = open(cur->args[0], O_RDONLY, 0644);
}

void	open_infile(t_command *node, t_pipex *p)
{
	t_command	*cur;

	p->save_in = 0;
	p->indixe = 0;
	p->fd = malloc(sizeof(int) * p->count_read_in);
	if (p->fd == NULL)
		panic("malloc fail\n", 1);
	p->i = 0;
	cur = node;
	while (cur && p->i < p->count_read_in)
	{
		if (cur->type == PIPE)
			break ;
		if (cur->type == RED_IN)
			red_in(cur, p);
		cur = cur->next;
	}
	if (p->i > 0)
		p->infile = p->fd[p->i - 1];
	else
		p->infile = p->save_in;
	free(p->fd);
}
