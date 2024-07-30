/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:45:44 by hbakrim           #+#    #+#             */
/*   Updated: 2024/07/30 11:24:32 by aelkheta         ###   ########.fr       */
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
	// printf("d5al here_doc\n");
	// printf("j = %d\n", pipex->j);
	char	*str;
	// int 	status = 0;

			
	// int pid = fork();
	// if (pid == 0)
	// {

			// signal(SIGQUIT, SIG_DFL);
			// signal(SIGINT, SIG_DFL);
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
					pipex->line = expand_vars(pipex->line, 0);
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
	// }
	// else
	// 	wait(&status);
	// signal(SIGQUIT, sig_handler);
	// signal(SIGINT, SIG_IGN);
	// if(WIFSIGNALED(status))
	// {
	// 	if (WTERMSIG(status) == SIGINT)
	// 		data->exit_status = 130;
	// }
	// else
	// 	data->exit_status = 2;
}

// }

void	open_here_doc(t_command *node, t_pipex *pipex)
{

	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		pipex->strs = malloc(sizeof(int *) * (pipex->count_here_doc + 1));
		t_command	*cur;
		pipex->j = 0;
		pipex->q = 0;
		cur = node;
		int her = 0;
		while (cur != NULL)
		{
			if (cur->type == HER_DOC)
			{
				int end[2];
				pipe(end);
				pipex->strs[pipex->j] = end;
				// if (!pipex->strs[pipex->j])
				// 	exit(0);
				here_doc(cur, pipex);
				pipex->arr[pipex->q] = pipex->j;
					pipex->j++;
			}
			if (cur->type == PIPE)
				pipex->q++;
			her++;
			cur = cur->next;
		}
		pipex->strs[pipex->j] = NULL;
		// pipex->strs;
		// exit(0);
	}
	pipex->r = pid;
	wait(NULL);

}

// char* random_file(char *r, int i)
// {
//     char *str = ttyname(0);
// 	char *c = ft_itoa(((ft_strlen(r) / 2 + i) * 1000) % 1026 );
// 	char * a = strjoin1("/tmp/XFILE", c);
// 	free(c);
// 	char *d = ft_substr(str, ft_strlen(str) - 1, ft_strlen(str) + 1);
// 	char *b = strjoin1(r, d);
// 	free(d);
// 	char *t = strjoin1(a, b);
// 	free(a);
// 	free(b);
// 	return t;
// }