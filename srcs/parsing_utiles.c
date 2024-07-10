/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:28:02 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/10 09:14:01 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

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

t_command	*redirect_list(t_command **head, t_command *list_command,
		t_command *_tokens_list, t_command **redirect_head)
{
	t_command	*redirection_node;

	redirection_node = new_node(_tokens_list->type,
			ft_strdup(_tokens_list->value));
	_tokens_list = free_node(_tokens_list);
	if (!_tokens_list || _tokens_list->type != TOKEN)
	{
		printf("syntax error parser\n");
		free_node(redirection_node);
		free_array(list_command->args);
		free_node(list_command);
		free_node(_tokens_list);
		clear_list(head);
		data->syntax_error = 1;
		return (NULL);
	}
	redirection_node->args = malloc(2 * sizeof(char *));
	redirection_node->args[0] = ft_strdup(_tokens_list->value);
	redirection_node->args[1] = NULL;
	_tokens_list = free_node(_tokens_list);
	add_back_list(redirect_head, redirection_node);
	return (_tokens_list);
}

void	get_redirect_node(t_command *_tokens_list, t_command *list_command)
{
	_tokens_list = free_node(_tokens_list);
	if (!_tokens_list || _tokens_list->type != TOKEN)
	{
		printf("syntax error parser\n");
		free_array(list_command->args);
		free_node(list_command);
		free_node(_tokens_list);
		data->syntax_error = 1;
	}
	list_command->args = malloc(2 * sizeof(char *));
	list_command->args[0] = ft_strdup(_tokens_list->value);
	list_command->args[1] = NULL;
	_tokens_list = free_node(_tokens_list);
}

char	*duplicate_word(char *command_line, int *i, int j)
{
	char	*token_val;

	token_val = malloc((j - *i) * sizeof(char) + 1);
	ft_strlcpy(token_val, &command_line[*i], j - *i + 1);
	*i = j;
	return (token_val);
}
