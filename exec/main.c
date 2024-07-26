/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbakrim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:45:04 by hbakrim           #+#    #+#             */
/*   Updated: 2024/07/16 12:45:06 by hbakrim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	child_process(t_command *node1, char **env, t_pipex *p)
{
	if (p->flag == 1)
		infile(node1, env, p);
	else if (p->flag == 2 && node1->next == NULL)
		one_here_doc(node1, env, p);
	else if (p->flag == 2 && node1->next && 
			(node1->next->type == RED_OUT || node1->next->type == APP))
	{
		p->b = 0;
		heredoc_readout_app(node1, env, p);
	}
	else if (p->flag == 2 && node1->next->type == HER_DOC
		&& node1->next->next == NULL)
		one_here_doc(node1, env, p);
	else if (p->flag == 2 && node1->next->type == HER_DOC
		&& (node1->next->next->type == RED_OUT
			|| node1->next->next->type == APP))
	{
		p->b = 1;
		heredoc_readout_app(node1, env, p);
	}
	else if (p->flag == 2)
		pipe_heredoc(node1, env, p);
	else if (node1->type == CMD && node1->next != NULL
		&& (node1->next->type == RED_OUT || node1->next->type == APP))
		outfile(node1, env, p);
	else if (node1->type == CMD && node1->next == NULL)
		excut_butlin(node1, env);
	else
	{
		close(p->end[0]);
		dup2(p->end[1], STDOUT_FILENO);
		close(p->end[1]);
		excut_butlin(node1, env);
	}

}

pid_t	fork_pipe(t_command *node1, char **env, t_pipex *p)
{
	printf("fork_pipe\n");
	// int last;
	// int temp;
	// pid_t last_pid = 0;
	if (pipe(p->end) == -1)
		ft_error_2();
	p->pid = fork();
	// if (p->pid != 0)
	// last_pid = p->pid; 
	if (p->pid == -1)
		ft_error_2();
	else if (p->pid == 0)
		child_process(node1, env, p);
	close(p->end[1]);
	dup2(p->end[0], STDIN_FILENO);
	close(p->end[0]);
	if (ft_strcmp(node1->args[0], "cat") != 0 || (ft_strcmp(node1->args[0],
				"cat") == 0 && node1->next == NULL))
	{
		wait(&p->status);
		data->exit_status = WEXITSTATUS(p->status);
	}


		// while (1)
		// {
		// 	last = wait(&temp);
		// 	if (last == last_pid)
		// 	{
		// 		data->exit_status  = WEXITSTATUS(temp);
		// 	}
		// }
	
	return (p->pid);
}

// void	skip_prh(t_pipex *p)
// {
// 	if (p->cur->type == RED_IN)
// 	{
// 		open_infile(p->cur, p);
		// p->infile = open(p->cur->args[0], O_RDONLY, 0644);
		// if (p->infile == -1)
		// {
		// 	printf("%s: Permission denied\n", p->cur->args[0]);
		// 	p->indixe = 1;
		// }
// 		p->flag = 1;
// 	}
// 	if (p->cur->type == PIPE)
// 	{
// 		p->indixe = 0;
// 		if (p->cur->next && p->cur->next->type == HER_DOC)
// 			p->k++;
// 	}
// 	if (p->cur->type == HER_DOC)
// 		p->flag = 2;
// 	p->cur = p->cur->next;
// }

void	ft_pipe(t_command *node1, char **ev, t_pipex *p)
{
	p->cur = node1;
	p->flag = 0;
	p->k = 0;
	while (p->cur != NULL)
	{
		if (p->cur->type == PIPE || p->cur->type == RED_IN
			|| p->cur->type == HER_DOC)
		{
			if (p->cur->type == RED_IN)
			{
				// p->infile = open(p->cur->args[0], O_RDONLY, 0644);
				// if (p->infile == -1)
				// {
				// 	printf("%s: Permission denied\n", p->cur->args[0]);
				// 	p->indixe = 1;
				// }
				// open_infile(p->cur, p);
				p->flag = 1;
			}
			if (p->cur->type == PIPE)
			{
				ft_count_read_in(p->cur, p);
				if (p->count_read_in > 0)
					open_infile(p->cur->next, p);
				// p->indixe = 0;
				// if (p->cur->next && p->cur->next->type == HER_DOC)
				// 	p->k++;
			}
			// if (p->cur->type == HER_DOC)
			// 	p->flag = 2;
			p->cur = p->cur->next;

			// skip_prh(p);
		}
		else if ((p->cur->type != RED_OUT || p->cur->type != APP)
			&& p->cur->type == CMD)
		{
			printf("cmd\n");
			if (p->cur->next && p->cur->next->type == HER_DOC)
				p->flag = 2;
			p->r = fork_pipe(p->cur, ev, p);
			p->flag = 0;
			p->cur = p->cur->next;
		}
		else
			p->cur = p->cur->next;
	}
}

int	func(t_command *list)
{
	t_pipex	pipex;

	ft_count_here_doc(list, &pipex);
	ft_count_pipe(list, &pipex);
	ft_count_read_out(list, &pipex);
	ft_count_read_in(list, &pipex);
	printf("pipex.count_in = %d\n", pipex.count_read_in);
	if (pipex.count_read_in > 0)
		open_infile(list, &pipex);
	printf("pipex.count_here_doc = %d\n", pipex.count_here_doc );
	
	if (pipex.count_here_doc > 0)
		open_here_doc(list, &pipex);
	ft_pipe(list, data->envirenment, &pipex);
	while (pipex.i != -1)
	{
		pipex.i = waitpid(pipex.r, &pipex.status, 0);
		pipex.i = wait(NULL);
	}
	if (pipex.count_here_doc > 0)
	{
		pipex.i = 0;
		while (pipex.strs[pipex.i])
		{
			unlink(pipex.strs[pipex.i]);
			// free(pipex.strs[pipex.i]);
			pipex.i++;
		}
		free(pipex.strs);
	}
	
	// unlink("file_here_doc.txt");
	return (0);
}
