/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:28:02 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/07 19:08:37 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

t_command	*syntax_error(t_data *data, t_command *list_command,
		t_command *head)
{
	ft_perror("syntax error\n");
	data->syntax_error = SYNTERRR;
	g_exit_stat = 2;
	free_node(&list_command);
	clear_list(&head);
	return (NULL);
}

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

t_command	*redirect_list(t_data *data, t_command **redirect_head)
{
	t_command	*redirection_node;

	redirection_node = new_node(data->_tokens_list->type,
			ft_strdup(data->_tokens_list->value));
	data->_tokens_list = free_node(&data->_tokens_list);
	if (!data->_tokens_list || data->_tokens_list->type != TOKEN)
	{
		g_exit_stat = SYNTERRR;
		ft_perror("syntax error\n");
		free_node(&redirection_node);
		free_array(data->list_command->args);
		free_node(&data->list_command);
		clear_list(&data->_tokens_list);
		return (NULL);
	}
	redirection_node->args = malloc(2 * sizeof(char *));
	redirection_node->args[0] = ft_strdup(data->_tokens_list->value);
	redirection_node->args[1] = NULL;
	data->_tokens_list = free_node(&data->_tokens_list);
	add_back_list(redirect_head, redirection_node);
	return (data->_tokens_list);
}

void	get_redirect_node(t_data *data)
{
	int	i;

	i = 0;
	data->_tokens_list = free_node(&data->_tokens_list);
	if (!data->_tokens_list || data->_tokens_list->type != TOKEN)
	{
		ft_perror("syntax error\n");
		free_array(data->list_command->args);
		free_node(&data->list_command);
		clear_list(&data->_tokens_list);
		g_exit_stat = SYNTERRR;
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
