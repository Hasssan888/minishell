/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:57:02 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/10 15:52:20 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	child_process(t_data *data, t_command *node1, char **env, t_pipex *p)
{
	if (p->flag == 1)
		infile(data, node1, env, p);
	else if (p->flag == 2)
		pipe_heredoc(data, node1, env, p);
	else if (p->w == 2)
		outfile(data, node1, env, p);
	else if (node1->type == TOKEN && node1->next == NULL)
		excut_butlin(data, node1, env, p);
	else
	{
		close(p->end[0]);
		dup2(p->end[1], STDOUT_FILENO);
		close(p->end[1]);
		excut_butlin(data, node1, env, p);
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
		clear_list(&data->list);
		clear_all(data);
		exit(g_exit_stat);
	}
	dup2(p->end[0], STDIN_FILENO);
	close(p->end[0]);
	return (p->pid);
}

void	ft_pipe(t_data *data, t_command *node1, char **ev, t_pipex *p)
{
	p->cur = node1;
	p->flag = 0;
	p->k = 0;
	p->w = check(node1);
	while (p->cur != NULL)
	{
		if (data->ignore_sig == 130 || data->ignore_sig == 131)
		{
			data->ignore_sig = 0;
			return ;
		}
		if (p->cur->next && p->cur->next->syntxerr == AMPIGOUS)
		{
			p->cur = p->cur->next;
			continue ;
		}
		if (p->cur->type == PIPE || p->cur->type == RED_IN
			|| p->cur->type == HER_DOC)
			skip_pipe(p);
		else if ((p->cur->type != RED_OUT || p->cur->type != APP)
			&& p->cur->type == TOKEN)
			skip_two(data, ev, p);
		p->cur = p->cur->next;
	}
}

void	ft_count(t_data *data, t_command *list, t_pipex *pipex)
{
	ft_bzero(pipex, sizeof(t_pipex));
	ft_count_here_doc(list, pipex);
	ft_count_pipe(list, pipex);
	ft_count_read_out(list, pipex);
	ft_count_read_in(list, pipex);
	if (pipex->count_read_in > 0)
		open_infile(list, pipex);
	if (pipex->count_read_out > 0)
		open_outfile(list, pipex);
	if (pipex->count_here_doc > 0 && pipex->count_here_doc <= 16)
		open_here_doc(data, list, pipex);
	else if (pipex->count_here_doc > 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		g_exit_stat = 2;
		clear_list(&data->list);
		clear_all(data);
		exit(g_exit_stat);
	}
}

int	func(t_data *data, t_command *list)
{
	t_pipex	pipex;

	pipex.s = NULL;
	if (!list || (!list->value && !list->next))
		return (0);
	if (!list->value && list->next && list->next->type == PIPE)
		list = list->next;
	ft_count(data, list, &pipex);

	if (pipex.count_pipe == 0 && if_is_buil(list) && list->syntxerr == AMPIGOUS)
	{
		exec_built_in(&pipex, data, list);
		free_int_array(pipex.strs);
	}
	else
	{
		ft_pipe(data, list, data->envirenment, &pipex);
		free_int_array(pipex.strs);
		waitpid(pipex.r, &pipex.status, 0);
		data->ignore_sig = check_exit_status(pipex.status);
		while (wait(NULL) != -1)
			;
	}
	return (0);
}
