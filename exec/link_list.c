/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbakrim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:46:47 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/04 14:38:50 by hbakrim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	skip_pipe(t_pipex *p)
{
	if (p->cur->type == RED_IN)
		p->flag = 1;
	if (p->cur->type == PIPE)
	{
		p->indixe = 0;
		ft_count_read_in(p->cur->next, p);
		if (p->count_read_in > 0)
			open_infile(p->cur->next, p);
		ft_count_read_out(p->cur->next, p);
		if (p->count_read_out > 0)
			open_outfile(p->cur->next, p);
		p->k++;
		p->w = check(p->cur->next);
	}
	if (p->cur->type == HER_DOC)
		p->flag = 2;
}

void	skip_pipe_2(t_data *data, char **ev, t_pipex *p)
{
	t_command	*c;

	c = p->cur->next;
	while (c)
	{
		if (c->type == PIPE)
			break ;
		if (c && c->type == HER_DOC)
			p->flag = 2;
		if (c && c->type == RED_IN)
			p->flag = 1;
		c = c->next;
	}
	p->r = fork_pipe(data, p->cur, ev, p);
	p->flag = 0;
	while (p->cur->next != NULL && (p->cur->next->type == RED_IN
			|| p->cur->next->type == HER_DOC))
		p->cur = p->cur->next;
}

void	ft_count_here_doc(t_command *node, t_pipex *p)
{
	t_command	*cur;

	cur = node;
	p->count_here_doc = 0;
	while (cur != NULL)
	{
		if (cur->type == HER_DOC)
			p->count_here_doc++;
		cur = cur->next;
	}
}

void	here_doc_error(char **av)
{
	ft_putstr_fd("minishell: warning: here-document\
	delimited by end-of-file wanted ",
		2);
	ft_putstr_fd(av[0], 2);
	write(2, "\n", 1);
	exit(1);
}
