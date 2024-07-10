/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:40:09 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/10 12:28:01 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

t_command	*syntax_error(t_command *list_command, t_command *head)
{
	printf("syntax error\n");
	data->syntax_error = 1;
	free_node(list_command);
	clear_list(&head);
	return (NULL);
}

t_command	*get_command_with_args(t_command *list_command)
{
	int	i;

	i = 0;
	list_command->args = malloc((get_args_size(data->_tokens_list) + 2)
			* sizeof(char *));
	while (data->_tokens_list != NULL && data->_tokens_list->type != PIPE)
	{
		list_command->args[i++] = ft_strdup(data->_tokens_list->value);
		list_command->args[i] = NULL;
		data->_tokens_list = free_node(data->_tokens_list);
		while (data->_tokens_list != NULL && (data->_tokens_list->type >= 2
				&& data->_tokens_list->type <= 5))
		{
			data->_tokens_list = redirect_list(&data->head, list_command,
					data->_tokens_list, &data->rdrct_head);
			if (data->syntax_error)
				return (NULL);
		}
	}
	return (list_command);
}

void	init_parser_var(void)
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
			data->list_command = get_command_with_args(data->list_command);
			if (!data->list_command)
				return (NULL);
		}
		else if (data->_tokens_list->type >= 2 && data->_tokens_list->type <= 5)
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
	t_command	*tokens_list;
	t_command	*list;

	printf("line befor lexer: %s\n", line);
	data->syntax_error = 0;
	printf("\n\n");
	line = lexer_command(line, 0, 0);
	if (line != NULL && line[0])
		printf("line after lexer: %s\n", line);
	printf("\n\n");
	tokens_list = tokenzer_command(line);
	print_list(tokens_list);
	printf("\n\n");
	list = parser_command(tokens_list);
	print_list(list);
	printf("\n\n");
	list = expander_command(list);
	print_list(list);
	printf("\n\n");
	data->list = list;
	exec_command(list);
	clear_list(&list);
	return (0);
}

// func(list);