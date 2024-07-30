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
		if (cur->type == PIPE)
			break;
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
			break;
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
			// printf("cur->args[0] == %s\n", cur->args[0]);
			if (cur->type == RED_OUT)
			{
				p->fd[p->i] = open(cur->args[0], O_WRONLY | O_CREAT | O_TRUNC,
						0644);
				p->b = 1;
			}
			else
			{
				p->fd[p->i] = open(cur->args[0], O_WRONLY | O_CREAT | O_APPEND,
						0644);
				p->b = 2;
			}
			if (p->fd[p->i] == -1)
			{
				// printf("%s: Permission denied\n", cur->args[0]);
				perror(cur->args[0]);
				p->indixe = 1;
			}
			p->name_file[p->i] = ft_strdup(cur->args[0]);
			p->i++;
		}
		cur = cur->next;
	}
}

void	open_outfile(t_command *node, t_pipex *p)
{
	t_command	*cur;

	p->b = 0;
	p->fd = malloc(sizeof(int) * p->count_read_out);
	p->name_file = malloc(sizeof(char *) * (p->count_read_out + 1));
	if (p->fd == NULL)
	{
		perror("malloc");
		exit(1);
	}
	p->i = 0;
	cur = node;
	ft_loop(cur, p);
	p->name_file[p->i] = NULL;
	p->outfile = p->fd[p->i - 1];
	printf("outfile = %d\n", p->outfile);
	p->s = ft_strdup(p->name_file[p->i - 1]);
	free(p->fd);
	p->i = -1;
	while (p->name_file[++p->i])
		free(p->name_file[p->i]);
	free(p->name_file);

}

void	open_infile(t_command *node, t_pipex *p)
{
	// printf("d5al openfile\n");
	t_command	*cur;

	p->fd = malloc(sizeof(int) * p->count_read_in);
	if (p->fd == NULL)
	{
		perror("malloc");
		exit(1);
	}
	p->i = 0;
	cur = node;
	while (cur && p->i < p->count_read_in)
	{
		if (cur->type == PIPE)
			break ;
		if (cur->type == RED_IN)
		{
			p->fd[p->i] = open(cur->args[0], O_RDONLY, 0644);
			// printf("p->fd[%d] = %d\n", p->i,p->fd[p->i]);
			if 	(p->fd[p->i] == -1)
			{
				// printf("%s: Permission denied\n", cur->args[0]);
				perror(cur->args[0]);
				p->indixe = 1;
			}
			p->i++;
		}
		cur = cur->next;
	}
	// printf("p->fd[%d] = %d\n", p->i,p->fd[p->i - 1]);
	p->infile = p->fd[p->i - 1];
	free(p->fd);;
}