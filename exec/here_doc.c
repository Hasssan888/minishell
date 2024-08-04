/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:45:44 by hbakrim           #+#    #+#             */
/*   Updated: 2024/08/04 10:27:18 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
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
	ft_putstr_fd("minishell: warning: here-document\
	delimited by end-of-file wanted ",
		2);
	ft_putstr_fd(av[0], 2);
	write(2, "\n", 1);
	exit(1);
}

void	here_doc(t_data *data, t_command *node, t_pipex *pipex)
{
	char	*str;

	pipex->line = readline("> ");
	if (pipex->line == NULL)
		here_doc_error(node->args);
	else
		pipex->line = strjoin1(pipex->line, "\n");
	str = strjoin1(node->args[0], "\n");
	str = unquote_arg(node, str, 0, 0);
	while (pipex->line != NULL && ft_strcmp(pipex->line, str) != 0)
	{
		if (pipex->line[0] == '$' && !node->quoted)
			pipex->line = expand_vars(data, pipex->line, 0);
		write(pipex->strs[pipex->j][1], pipex->line, ft_strlen(pipex->line));
		free(pipex->line);
		pipex->line = readline("> ");
		if (pipex->line == NULL)
			here_doc_error(node->args);
		else
			pipex->line = strjoin1(pipex->line, "\n");
	}
	close(pipex->strs[pipex->j][1]);
	free(pipex->line);
	free(str);
}

void	open_here_doc(t_data *data, t_command *node, t_pipex *pipex)
{

	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		handle_signals(&data->ignore_sig, 4);
		pipex->strs = malloc(sizeof(int *) * (pipex->count_here_doc + 1));
		t_command	*cur;
		pipex->j = 0;
		pipex->q = 0;
		cur = node;
		while (cur != NULL)
		{
			if (cur->type == HER_DOC)
			{
				pipex->pipe_t = malloc(sizeof(int) * 2);
				pipe(pipex->pipe_t);
				pipex->strs[pipex->j] = pipex->pipe_t;
				here_doc(data, cur, pipex);
				pipex->arr[pipex->q] = pipex->j;
				pipex->j++;
					
			}
			if (cur->type == PIPE)
				pipex->q++;
				
			cur = cur->next;
		}
		pipex->strs[pipex->j] = NULL;

	}
	pipex->r = pid;
	wait(&pipex->status);
	data->ignore_sig = check_exit_status(pipex->status);

}
