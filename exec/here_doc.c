/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:45:44 by hbakrim           #+#    #+#             */
/*   Updated: 2024/07/27 15:21:16 by aelkheta         ###   ########.fr       */
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
	unlink("file_here_doc.txt");
}

void	here_doc(t_command *node, t_pipex *pipex)
{
	printf("d5al here_doc\n");
	printf("j = %d\n", pipex->j);
	char	*str;
	int 	status = 0;

	int pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		
			
			pipex->line = readline("> ");
			printf("pipex->line = %s\n", pipex->line);
			if (pipex->line == NULL)
				here_doc_error(node->args);
			else
				pipex->line = strjoin1(pipex->line, "\n");
			printf("node->args[1] = %s\n", node->args[0]);
			// pipex->strs[pipex->j] = malloc(sizeof(char) * (ft_strlen(node->args[0]) + 1));
			// ft_strcpy(pipex->strs[pipex->j], node->args[0]);
			pipex->strs[pipex->j] = ft_strdup(node->args[0]);
			printf("pipex->strs[%d] = %s\n", pipex->j ,pipex->strs[pipex->j]);
			pipex->tb[pipex->j] = open(pipex->strs[pipex->j],
					O_RDWR | O_CREAT | O_TRUNC, 0644);
			printf("pipex->tb[pipex->j] = %d\n", pipex->tb[pipex->j]);
			str = strjoin1(node->args[0], "\n");
			str = unquote_arg(node, str, 0, 0);
			while (pipex->line != NULL && ft_strcmp(pipex->line, str) != 0)
			{
				if (pipex->line[0] == '$' && !node->quoted)
					pipex->line = expand_vars(pipex->line, 0);
				write(pipex->tb[pipex->j], pipex->line, ft_strlen(pipex->line));
	
				free(pipex->line);
				pipex->line = readline("> ");
				if (pipex->line == NULL)
					here_doc_error(node->args);
				else
					pipex->line = strjoin1(pipex->line, "\n");
			}
			free(pipex->line);
			free(str);

	}
	else
		waitpid(pid , &status, 0);
	if(WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			data->exit_status = 130;
	}
	else
		data->exit_status = 2;
}

void	open_here_doc(t_command *node, t_pipex *pipex)
{
	t_command	*cur;

	pipex->j = 0;
	pipex->q = 0;
	printf("d5al openheredoc\n");
	cur = node;
	pipex->tb = malloc(sizeof(int) * pipex->count_here_doc);
	pipex->strs = malloc(sizeof(char *) * (pipex->count_here_doc + 1));
	while (cur != NULL)
	{
		if (cur->type == HER_DOC)
		{
			printf("9balma yd5ol here_doc\n");
			here_doc(cur, pipex);
			pipex->arr[pipex->q] = pipex->j;
			pipex->j++;
		}
		if (cur->type == PIPE)
			pipex->q++;
		printf("pipex->arr[%d] = %d\n", pipex->q,pipex->arr[pipex->q]);
		cur = cur->next;
	}
	pipex->strs[pipex->j] = NULL;
	// pipex->j = 0;
	// while (pipex->strs[pipex->j])
	// {
	// 	printf("pipex->strs[%d] = %s\n", pipex->j ,pipex->strs[pipex->j]);
	// 	pipex->j++;
	// }
	
}