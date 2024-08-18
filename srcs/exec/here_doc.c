/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:45:44 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/18 20:34:53 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	w_loop(t_data *data, t_command *node, t_pipex *pipex, char *str)
{
	while (true)
	{
		pipex->line = readline("> ");
		if (!pipex->line)
		{
			here_doc_error(node->args);
			if (pipex->line)
				free(pipex->line);
			break ;
		}
		else
			pipex->line = ft_strjoin(pipex->line, ft_strdup("\n"));
		if (pipex->line && ft_strcmp(pipex->line, str) == 0)
			break ;
		if (pipex->line && pipex->line[0] == '$' && !pipex->quoted)
			pipex->line = expand_vars(data, node, pipex->line);
		write(pipex->strs[pipex->j][1], pipex->line, ft_strlen(pipex->line));
		free(pipex->line);
		pipex->line = NULL;
	}
}

void	here_doc_child_pro(t_data *data, t_command *node, t_pipex *pipex)
{
	char	*str;

	str = NULL;
	pipex->quoted = 0;
	handle_signals(SIG_QUI_IGN);
	close(pipex->strs[pipex->j][0]);
	if (!node->args || !node->args[0])
		exit(g_exit_stat);
	str = strjoin1(node->args[0], "\n");
	if (ft_strchr(str, '\'') || ft_strchr(str, '"'))
		pipex->quoted = 1;
	str = unquote_arg(node, str, 0, 0);
	w_loop(data, node, pipex, str);
	close(pipex->strs[pipex->j][1]);
	free_int_array(pipex->strs);
	clear_list(&data->list);
	clear_all(data);
	free(pipex->line);
	free(str);
	exit(g_exit_stat);
}

void	here_doc(t_data *data, t_command *node, t_pipex *pipex)
{
	int	pid;

	pid = fork();
	if (pid == 0)
		here_doc_child_pro(data, node, pipex);
	else if (pid > 0)
		close(pipex->strs[pipex->j][1]);
	else
		panic("fork fail\n", 1);
	pipex->r = pid;
	wait(&pipex->status);
	if (g_exit_stat != 130)
		data->ignore_sig = check_exit_status(pipex->status);
}

void	type_here_doc(t_data *data, t_command *cur, t_pipex *pipex)
{
	pipex->pipe_t = ft_calloc(2, sizeof(int));
	pipe(pipex->pipe_t);
	pipex->strs[pipex->j] = pipex->pipe_t;
	here_doc(data, cur, pipex);
	pipex->arr[pipex->q] = pipex->j;
	pipex->j++;
	pipex->strs[pipex->j] = NULL;
}

void	open_here_doc(t_data *data, t_command *node, t_pipex *pipex)
{
	t_command	*cur;

	pipex->strs = ft_calloc((pipex->count_here_doc + 1), sizeof(int *));
	if (!pipex->strs)
		exit(1);
	pipex->j = 0;
	pipex->q = 0;
	cur = node;
	while (cur != NULL)
	{
		if (g_exit_stat == 130 && data->ignore_sig == 130)
			break ;
		if (cur->type == HER_DOC)
			type_here_doc(data, cur, pipex);
		if (g_exit_stat == SYNTERRR)
			break ;
		if (cur->type == PIPE)
			pipex->q++;
		cur = cur->next;
	}
	pipex->strs[pipex->j] = NULL;
}
