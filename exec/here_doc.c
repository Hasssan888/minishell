/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:45:44 by hbakrim           #+#    #+#             */
/*   Updated: 2024/07/24 16:21:33 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

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
	unlink("file_here_doc.txt");
}

void	here_doc(t_command *node, t_pipex *pipex)
{
	char	*str;
	// int 	status = 0;

	// int pid = fork();
	// if (pid == 0)
	// {
	// 	signal(SIGQUIT, SIG_DFL);
	// 	signal(SIGINT, SIG_DFL);
		pipex->line = readline("> ");
		if (pipex->line == NULL)
			here_doc_error(node->args);
		else
			pipex->line = strjoin1(pipex->line, "\n");
		pipex->infile_here_doc = open("file_here_doc.txt",
				O_RDWR | O_CREAT | O_TRUNC, 0644);
		str = strjoin1(node->args[0], "\n");
		str = unquote_arg(node, str, 0, 0);
		while (pipex->line != NULL && ft_strcmp(pipex->line, str) != 0)
		{
			if (pipex->line[0] == '$' && !node->quoted)
				pipex->line = expand_vars(pipex->line, 0);
			write(pipex->infile_here_doc, pipex->line, ft_strlen(pipex->line));
			free(pipex->line);
			pipex->line = readline("> ");
			if (pipex->line == NULL)
				here_doc_error(node->args);
			else
				pipex->line = strjoin1(pipex->line, "\n");
		}
		free(pipex->line);
		free(str);
	// }
	// else
	// 	waitpid(pid , &status, 0);
	// if(WIFSIGNALED(status))
	// {
	// 	if (WTERMSIG(status) == SIGINT)
	// 		data->exit_status = 130;
	// }
	// else
	// 	data->exit_status = 2;
}

void	open_here_doc(t_command *node, t_pipex *pipex)
{
	t_command	*cur;

	cur = node;
	while (cur != NULL)
	{
		if (cur->type == HER_DOC)
			here_doc(cur, pipex);
		if (cur->type == PIPE)
			break;
		cur = cur->next;
	}
}
