/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:45:04 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/04 10:27:33 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"


int check_exit_status(int status)
{
	if (WIFSIGNALED(status)) // if the child terminate with a signal
	{
		write(1,"\n",1);
		g_exit_stat = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status)) // if the child terminate normally 
		g_exit_stat = WEXITSTATUS(status);
	return (g_exit_stat);
}

void	child_process(t_data *data, t_command *node1, char **env, t_pipex *p)
{
	if (p->flag == 1)
	{
		printf("infile\n");
		infile(data, node1, env, p);
	}
	else if (p->flag == 2)
		pipe_heredoc(data, node1, env, p);

	else if (p->w == 2)
		outfile(data, node1, env, p);
	else if (node1->type == CMD && node1->next == NULL)
		excut_butlin(data, node1, env);
	else
	{
		close(p->end[0]);
		dup2(p->end[1], STDOUT_FILENO);
		close(p->end[1]);
		excut_butlin(data, node1, env);
	}

}

pid_t	fork_pipe(t_data *data, t_command *node1, char **env, t_pipex *p)
{
	if (pipe(p->end) == -1)
		ft_error_2();
	p->pid = fork();
	if (p->pid == -1)
		ft_error_2();
	else if (p->pid == 0)
	{
		handle_signals(3);
		child_process(data, node1, env, p);
	}
	close(p->end[1]);
	dup2(p->end[0], STDIN_FILENO);
	close(p->end[0]);
	// if (ft_strcmp(node1->args[0], "cat") != 0)
	// {
		// wait(&p->status);

	wait(&p->status);
	data->ignore_sig = check_exit_status(p->status);
	return (p->pid);
}
int check(t_command *node)
{
	t_command *c = node;

	while (c)
	{
		if (c->type == PIPE)
		 return (1);
		else if (c->type == RED_OUT || c->type == APP)
			return(2);
		c = c->next;
	}
	return(0);
}

void	ft_pipe(t_data *data, t_command *node1, char **ev, t_pipex *p)
{
	p->cur = node1;
	p->flag = 0;
	p->k = 0;

	p->w = check(node1);
	while (p->cur != NULL)
	{
		if (p->cur->type == PIPE || p->cur->type == RED_IN
			|| p->cur->type == HER_DOC)
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
			p->cur = p->cur->next;
		}
		else if ((p->cur->type != RED_OUT || p->cur->type != APP)
			&& p->cur->type == CMD)
		{
			t_command *c = p->cur->next;
			while (c)
			{
				if (c->type == PIPE)
					break;
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
			p->cur = p->cur->next;
		}
		else
			p->cur = p->cur->next;
	}
}

void free_int_array(int **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i] != NULL)
		free(array[i++]);
	free(array);
	array = NULL;
}


int	func(t_data *data, t_command *list)
{
	t_pipex	pipex;
	if (!list)
		return 0;

	ft_bzero(&pipex, sizeof(t_pipex));
	ft_count_here_doc(list, &pipex);
	ft_count_pipe(list, &pipex);
	ft_count_read_out(list, &pipex);
	ft_count_read_in(list, &pipex);
	if (pipex.count_read_in > 0)
		open_infile(list, &pipex);
	if (pipex.count_read_out > 0)
		open_outfile(list, &pipex);
	if (pipex.count_here_doc > 0 && pipex.count_here_doc <= 16)
		open_here_doc(data, list, &pipex);
	else if (pipex.count_here_doc > 16)
	{
		perror("minishell: maximum here-document count exceeded");
		g_exit_stat = 2;
		exit(2);
	}
	if (data->ignore_sig == 130 || data->ignore_sig == 130)
	{
		data->ignore_sig = 0;
		return (g_exit_stat);
	}
	else if (pipex.count_pipe == 0 && if_is_buil(list))
		is_builtin_cmd(data, list);
	else
		ft_pipe(data, list, data->envirenment, &pipex);
	while (pipex.i != -1)
	{
		pipex.i = waitpid(pipex.r, &pipex.status, 0);
		pipex.i = wait(NULL);
	}
	free_int_array(pipex.strs);
	// if (pipex.count_here_doc > 0)
	return (0);
}