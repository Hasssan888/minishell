/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:40:09 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/09 19:11:52 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*skip_white_spaces(char *command)
{
	while (*command && (*command == ' ' || *command == '\t' || *command == '\r'
			|| *command == '\v'))
		command++;
	return (command);
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

void get_redirect_node(t_command *_tokens_list, t_command *list_command)
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

t_command *syntax_error(t_command *list_command, t_command *head)
{
	printf("syntax error\n");
	data->syntax_error = 1;
	free_node(list_command);
	clear_list(&head);
	return (NULL);
}

t_command *get_command_with_args(t_command *list_command, t_command *head, t_command *rdrct_head)
{
	int i = 0;
	
	list_command->args = malloc((get_args_size(data->_tokens_list) + 2)
			* sizeof(char *));
	while (data->_tokens_list != NULL && data->_tokens_list->type != PIPE)
	{
		list_command->args[i++] = ft_strdup(data->_tokens_list->value);
		list_command->args[i] = NULL;
		data->_tokens_list = free_node(data->_tokens_list);
		while (data->_tokens_list != NULL && (data->_tokens_list->type == RED_IN
				|| data->_tokens_list->type == RED_OUT
				|| data->_tokens_list->type == APP
				|| data->_tokens_list->type == HER_DOC))
		{
			data->_tokens_list = redirect_list(&head, list_command,
					data->_tokens_list, &rdrct_head);
			if (data->syntax_error)
				return (NULL);
		}
	}
	return (list_command);
}

void init_parser_var()
{
	data->list_command = new_node(data->_tokens_list->type,
				ft_strdup(data->_tokens_list->value));
	data->list_command->args = NULL;
	data->rdrct_head = NULL;
}

t_command	*parser_command(t_command *_tokens_list)
{
	data->_tokens_list = _tokens_list;
	data->head = NULL;
	while (data->_tokens_list != NULL)
	{
		init_parser_var();
		if (data->_tokens_list->type == TOKEN)
		{
			data->list_command = get_command_with_args(data->list_command, data->head, data->rdrct_head);
			if (!data->list_command)
				return (NULL);
		}
		else if (data->_tokens_list->type == RED_IN || data->_tokens_list->type == RED_OUT
			|| data->_tokens_list->type == APP || data->_tokens_list->type == HER_DOC)
			get_redirect_node(data->_tokens_list, data->list_command);
		else
		{
			data->_tokens_list = free_node(data->_tokens_list);
			if (!data->_tokens_list)
				return (syntax_error(data->list_command, data->head));
		}
		add_back_list(&data->head, data->list_command);
		add_back_list(&data->head, data->rdrct_head);
	}
	return (data->head);
}

int	parse_command(char *line)
{
	// printf("line befor lexer: %s\n", line);
	data->syntax_error = 0;
	// printf("\n\n");
	line = lexer_command(line);
	// if (line != NULL && line[0])
	// 	printf("line after lexer: %s\n", line);
	// printf("\n\n");
	t_command *tokens_list = tokenzer_command(line);
	print_list(tokens_list);
	printf("\n\n");
	t_command *list = parser_command(tokens_list);
	print_list(list);
	printf("\n\n");
	list = expander_command(list);
	print_list(list);
	printf("\n\n");
	data->list = list;

	// for execute commands
	exec_command(list);

	// func(list);
	clear_list(&list);
	return (0);
}
