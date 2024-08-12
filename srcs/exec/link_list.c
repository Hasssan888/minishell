/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:46:47 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/12 11:58:07 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

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

void	skip_two(t_data *data, char **ev, t_pipex *p)
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
	ft_putstr_fd("minishell: warning: here-document \
	delimited by end-of-file wanted ", 2);
	ft_putstr_fd(av[0], 2);
	write(2, "\n", 1);
	g_exit_stat = 1;
}

void	exec_built_in(t_pipex *pipex, t_data *data, t_command *list)
{
	pipex->d = 1;
	pipex->w = check(list);
	pipex->cur = list;
	while (pipex->cur)
	{
		if (pipex->cur->type == RED_IN)
			pipex->flag = 1;
		else if (pipex->cur->type == HER_DOC)
			pipex->flag = 2;
		else if (pipex->cur->type == TOKEN && pipex->cur->type != RED_OUT
			&& pipex->cur->type != APP)
		{
			pipex->c = pipex->cur->next;
			while (pipex->c)
			{
				if (pipex->c && pipex->c->type == HER_DOC)
					pipex->flag = 2;
				if (pipex->c && pipex->c->type == RED_IN)
					pipex->flag = 1;
				pipex->c = pipex->c->next;
			}
			child_process(data, pipex->cur, data->envirenment, pipex);
		}
		pipex->cur = pipex->cur->next;
	}
}
