/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:28:02 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/29 09:27:16 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

int	get_args_size(t_command *list)
{
	int	i;

	i = 0;
	while (list != NULL)
	{
		if (list->type == PIPE)
			return (i);
		if (list->type == TOKEN)
			i++;
		if (list->next != NULL && list->next == TOKEN && (list->type == RED_IN
				|| list->type == RED_OUT || list->type == HER_DOC
				|| list->type == APP))
			i--;
		list = list->next;
	}
	return (i);
}

t_command	*redirect_list(t_command **head, t_command **redirect_head)
{
	(void) head;
	t_command	*redirection_node;

	redirection_node = new_node(data->_tokens_list->type,
			ft_strdup(data->_tokens_list->value));
	data->_tokens_list = free_node(&data->_tokens_list);
	if (!data->_tokens_list || data->_tokens_list->type != TOKEN)
	{
		ft_perror("syntax error\n");
		free_node(&redirection_node);
		free_array(data->list_command->args);
		free_node(&data->list_command);
		clear_list(&data->_tokens_list);
		data->syntax_error = 1;
		return (NULL);
	}
	redirection_node->args = malloc(2 * sizeof(char *));
	redirection_node->args[0] = ft_strdup(data->_tokens_list->value);
	redirection_node->args[1] = NULL;
	data->_tokens_list = free_node(&data->_tokens_list);
	add_back_list(redirect_head, redirection_node);
	return (data->_tokens_list);
}

void fake_here_doc__()
{
	int pid;
	int status;
	
	status = 0;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		data->str1 = readline("> ");
		while(data->str1 != NULL)
		{
			int len = ft_strlen(data->str1) > ft_strlen(data->str2) ? ft_strlen(data->str1): ft_strlen(data->str2); 
			if(ft_strncmp(data->str2, data->str1, len) == 0)
				break;
			free(data->str1);
			data->str1 = readline("> ");
		}
		clear_list(&data->list);
		free(data->str1);
		exit(0);
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

void	get_redirect_node(void)
{
	int i = 0;
	data->_tokens_list = free_node(&data->_tokens_list);
	if (!data->_tokens_list || data->_tokens_list->type != TOKEN)
	{
		data->ignore_sig = 1;
		if (data->redirect)
			fake_here_doc__();
		ft_perror("syntax error\n");
		free_array(data->list_command->args);
		free_node(&data->list_command);
		clear_list(&data->_tokens_list);
		data->syntax_error = 1;
		data->list = NULL;
		return ;
	}
	data->list_command->args = malloc(2 * sizeof(char *));
	data->list_command->args[0] = ft_strdup(data->_tokens_list->value);
	data->list_command->args[1] = NULL;
	if (!i)
	{
		data->redirect = 1;
		data->str2 = data->list_command->args[0];
	}
	data->_tokens_list = free_node(&data->_tokens_list);
}

char	*duplicate_word(char *command_line, int *i, int j)
{
	char	*token_val;

	token_val = malloc((j - *i) * sizeof(char) + 1);
	ft_strlcpy(token_val, &command_line[*i], j - *i + 1);
	*i = j;
	return (token_val);
}
