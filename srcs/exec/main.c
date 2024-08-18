/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:57:02 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/18 20:33:50 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

pid_t	fork_pipe(t_data *data, t_command *node1, char **env, t_pipex *p)
{
	if (pipe(p->end) == -1)
		ft_error_2();
	p->pid = fork();
	if (p->pid == -1)
		ft_error_2();
	else if (p->pid == 0)
	{
		handle_signals(SIG_INT_QUI_DFL);
		child_process(data, node1, env, p);
		clear_list(&data->list);
		clear_all(data);
		exit(g_exit_stat);
	}
	close(p->end[1]);
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
			return ;
		if (p->cur->next && p->cur->next->syntxerr == AMPIGOUS)
		{
			ft_putstr_fd("minishell: ambiguous redirect\n", 2);
			p->cur = p->cur->next;
			continue ;
		}
		if (p->cur->type == PIPE || p->cur->type == RED_IN
			|| p->cur->type == HER_DOC)
			skip_pipe(p);
		else if ((p->cur->type != RED_OUT || p->cur->type != APP)
			&& p->cur->type == TOKEN)
			skip_two(data, ev, p);
		if (g_exit_stat == SYNTERRR)
			break ;
		p->cur = p->cur->next;
	}
}

void	exec_cmd(t_data *data, t_pipex *pipex, t_command *list)
{
	if (list->next && list->next->syntxerr == AMPIGOUS)
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		return ;
	}
	if (pipex->count_pipe == 0 && if_is_buil(list))
	{
		exec_built_in(pipex, data, list);
		free_int_array(pipex->strs);
	}
	else
	{
		ft_pipe(data, list, data->envirenment, pipex);
		free_int_array(pipex->strs);
		waitpid(pipex->r, &pipex->status, 0);
		data->ignore_sig = check_exit_status(pipex->status);
		while (wait(NULL) != -1)
			;
	}
}

void	ft_count(t_data *data, t_command *list, t_pipex *pipex)
{
	ft_bzero(pipex, sizeof(t_pipex));
	ft_count_here_doc(list, pipex);
	ft_count_pipe(list, pipex);
	ft_count_read_out_in(list, pipex);
	if (pipex->count_here_doc > 0 && pipex->count_here_doc <= 16)
		open_here_doc(data, list, pipex);
	if (data->ignore_sig == 130)
	{
		free(pipex->line);
		return ;
	}
	else if (pipex->count_here_doc > 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		g_exit_stat = 2;
		clear_list(&data->list);
		clear_all(data);
		exit(g_exit_stat);
	}
	if (pipex->count_read_out_in > 0)
		open_file(list, pipex);
}

int	func(t_data *data, t_command *list)
{
	t_pipex	pipex;

	pipex.s = NULL;
	pipex.s2 = NULL;
	pipex.s1 = NULL;
	data->ignore_sig = 0;
	if (!list || (!list->value && !list->next))
		return (0);
	if (!list->value && list->next && list->next->type == PIPE)
		list = list->next;
	ft_count(data, list, &pipex);
	if (data->ignore_sig == 130)
	{
		free_int_array(pipex.strs);
		return (0);
	}
	if (g_exit_stat == SYNTERRR)
	{
		free_int_array(pipex.strs);
		ft_putstr_fd("minishell: syntax error near unexpected \
token `newline'\n", 2);
		return (0);
	}
	exec_cmd(data, &pipex, list);
	return (0);
}
