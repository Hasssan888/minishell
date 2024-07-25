/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:40:09 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/24 14:07:05 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

t_command	*syntax_error(t_command *list_command, t_command *head)
{
	printf("syntax error\n");
	data->syntax_error = 1;
	free_node(&list_command);
	clear_list(&head);
	return (NULL);
}

void free_command(t_command *cmd)
{
	free(cmd->value);
	free_array(cmd->args);
	free(cmd);
}

t_command	*get_command_with_args()
{
	int	i;

	i = 0;
	int args_size = get_args_size(data->_tokens_list);
	data->list_command->args = malloc((args_size + 2) * sizeof(char *));
	if (!data->list_command)
		return (NULL);
	while (data->_tokens_list != NULL && data->_tokens_list->type != PIPE)
	{
		data->list_command->args[i++] = ft_strdup(data->_tokens_list->value);
		data->list_command->args[i] = NULL;
		data->_tokens_list = free_node(&data->_tokens_list);
		while (data->_tokens_list != NULL && (data->_tokens_list->type >= 2
				&& data->_tokens_list->type <= 5))
		{
			data->_tokens_list = redirect_list(&data->head, &data->rdrct_head);
			if (data->syntax_error)
			{
				// free(data->_tokens_list);
				// syntax_error(data->_tokens_list, data->head);
				// free_command(data->_tokens_list);
				clear_list(&data->rdrct_head);
				clear_list(&data->head);
				return (NULL);
			}
		}
	}
	return (data->list_command);
}

void	init_parser_var(void)
{
	data->list_command = new_node(data->_tokens_list->type,
			ft_strdup(data->_tokens_list->value));
	if (!data->list_command)
		return ;
	data->list_command->args = NULL;
	data->rdrct_head = NULL;
}

t_command	*parser_command(t_command *_tokens_list)
{
	data->head = NULL;
	data->_tokens_list = _tokens_list;
	while (data->_tokens_list != NULL)
	{
		if (data->syntax_error)
			return (NULL);
		init_parser_var();
		if (!data->list_command)
		{
			clear_list(&data->head);
			return (NULL);
		}
		if (data->_tokens_list->type == TOKEN)
		{
			data->list_command = get_command_with_args();
			if (!data->list_command)
				return (NULL);
		}
		else if (data->_tokens_list->type >= 2 && data->_tokens_list->type <= 5)
			get_redirect_node(data->list_command);
		else
		{
			data->_tokens_list = free_node(&data->_tokens_list);
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
	// t_command	*list;
	// printf("\n++++++++++++++++++ parsing is started +++++++++++++++++\n");
	data->list = NULL;
	// printf("line befor lexer: %s\n", line);
	data->ignore_sig = 0;
	data->syntax_error = 0;
	// printf("\n\n");
	line = lexer_command(line, 0, 0);
	// if (line != NULL && line[0])
	// 	printf("line after lexer: %s\n", line);
	// printf("\n\n");
	tokens_list = tokenzer_command(line);
	// print_list(tokens_list);
	// printf("\n\n");
	data->list = parser_command(tokens_list);
	// print_list(data->list);
	// printf("\n\n");
	data->list = expander_command(data->list);
	print_list(data->list);
	// printf("\n\n");
	// printf("\n\n");
	// printf("\n++++++++++++++++++ parsing is done +++++++++++++++++\n");
	// printf("\n++++++++++++++++++ execution is started +++++++++++++++++\n");

	// is_builtin_cmd(data->list);
	// exec_command(data->list);
	func(data->list);
	// printf("\n++++++++++++++++++ execution is done +++++++++++++++++\n");
	clear_list(&data->list);
	return (0);
}
