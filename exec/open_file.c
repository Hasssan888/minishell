/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbakrim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:46:17 by hbakrim           #+#    #+#             */
/*   Updated: 2024/07/16 14:26:03 by hbakrim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

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
		if (cur->type == RED_IN)
			p->count_read_in++;
		cur = cur->next;
	}
}

void	ft_loop(t_command *cur, t_pipex *p)
{
	while (cur && p->i < p->count_read_out)
	{
		if (cur->type == PIPE)
			break ;
		else if (cur->type == RED_OUT || cur->type == APP)
		{
			if (cur->type == RED_OUT)
				p->fd[p->i] = open(cur->args[0], O_WRONLY | O_CREAT | O_TRUNC,
						0644);
			else
				p->fd[p->i] = open(cur->args[0], O_WRONLY | O_CREAT | O_APPEND,
						0644);
			if (p->fd[p->i] == -1)
			{
				printf("%s: Permission denied\n", cur->args[0]);
				exit(1);
			}
			p->i++;
		}
		cur = cur->next;
	}
}

void	open_outfile(t_command *node, t_pipex *p)
{
	t_command	*cur;

	p->fd = malloc(sizeof(int) * p->count_read_out);
	if (p->fd == NULL)
	{
		perror("malloc");
		exit(1);
	}
	p->i = 0;
	cur = node;
	ft_loop(cur, p);
	p->outfile = p->fd[p->i - 1];
	free(p->fd);
}
