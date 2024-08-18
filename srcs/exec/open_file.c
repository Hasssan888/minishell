/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:46:17 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/18 20:30:10 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	red_in(t_command *cur, t_pipex *p)
{
	if (cur->type == RED_OUT && cur->args[0] != NULL && p->indixe == 0)
	{
		p->outfile = open(cur->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		p->b = 1;
		p->s2 = cur->args[0];
	}
	else if (cur->type == APP && cur->args[0] != NULL && p->indixe == 0)
	{
		p->outfile = open(cur->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
		p->s2 = cur->args[0];
		p->b = 2;
	}
}

void	out_app_in_(t_command *cur, t_pipex *p)
{
	if (ft_strncmp(cur->args[0], "/dev/stdout", -1) != 0)
		red_in(cur, p);
	if (ft_strncmp(cur->args[0], "/dev/stdin", -1) != 0)
	{
		if (cur->type == RED_IN && cur->args[0] != NULL && p->indixe != 1)
		{
			p->infile = open(cur->args[0], O_RDONLY, 0644);
			p->s1 = cur->args[0];
		}
	}
	if (p->outfile == -1)
		p->indixe = 1;
	if (p->infile == -1)
		p->indixe = 2;
	p->s = cur->args[0];
}

void	open_file(t_command *node, t_pipex *p)
{
	t_command	*cur;

	p->b = 0;
	p->indixe = 0;
	p->outfile = 1;
	p->infile = 0;
	cur = node;
	while (cur)
	{
		if (g_exit_stat == SYNTERRR)
			break ;
		if (cur->type == PIPE)
			break ;
		else if (cur->syntxerr != AMPIGOUS && (cur->type == RED_OUT
				|| cur->type == APP || cur->type == RED_IN))
			out_app_in_(cur, p);
		cur = cur->next;
	}
}
