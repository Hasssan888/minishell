/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:40:09 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/18 20:12:49 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

t_command	*get_command_with_args(t_data *data)
{
	int	i;
	int	args_size;

	i = 0;
	args_size = get_args_size(data->_tokens_list);
	data->list_cmd->args = malloc((args_size + 2) * sizeof(char *));
	if (!data->list_cmd)
		panic("malloc fail\n", 1);
	while (data->_tokens_list != NULL && data->_tokens_list->type != PIPE)
	{
		data->list_cmd->args[i++] = ft_strdup(data->_tokens_list->value);
		data->list_cmd->args[i] = NULL;
		data->_tokens_list = free_node(&data->_tokens_list);
		while (data->_tokens_list != NULL && (data->_tokens_list->type >= 2
				&& data->_tokens_list->type <= 5))
		{
			data->_tokens_list = redirect_list(data, &data->rdrct_head);
			if (data->syntax_error)
			{
				return (NULL);
			}
		}
	}
	return (data->list_cmd);
}

void	init_parser_var(t_data *data)
{
	data->list_cmd = new_node(data->_tokens_list->type,
			ft_strdup(data->_tokens_list->value));
	if (!data->list_cmd)
		return ;
	data->list_cmd->args = NULL;
	data->rdrct_head = NULL;
}

int	parser_continue(t_data *data)
{
	if (data->_tokens_list->type == TOKEN)
	{
		data->list_cmd = get_command_with_args(data);
		if (!data->list_cmd)
			return (0);
	}
	else if (data->_tokens_list->type >= 2 && data->_tokens_list->type <= 5)
		get_redirect_node(data);
	else
	{
		data->_tokens_list = free_node(&data->_tokens_list);
		if (!data->_tokens_list)
		{
			syntax_error(data, data->list_cmd, data->head,
				"minishell: syntax error near unexpected token `|'\n");
			return (0);
		}
	}
	return (1);
}

t_command	*parser_command(t_data *data, t_command *_tokens_list)
{
	data->head = NULL;
	data->_tokens_list = _tokens_list;
	while (data->_tokens_list != NULL)
	{
		data->_tokens_list->syntxerr = 0;
		if (data->syntax_error)
			return (NULL);
		init_parser_var(data);
		if (!data->list_cmd)
		{
			clear_list(&data->head);
			return (NULL);
		}
		if (!parser_continue(data))
			return (NULL);
		add_back_list(&data->head, data->list_cmd);
		add_back_list(&data->head, data->rdrct_head);
	}
	return (data->head);
}

int	parse_command(t_data *data, char *line)
{
	t_command	*tokens_list;

	data->ignore_sig = 0;
	data->syntax_error = 0;
	if (line != NULL && !line[0])
		return (0);
	if (is_empty(line))
	{
		free(line);
		return (0);
	}
	line = lexer_command(data, line);
	if (!line)
		return (0);
	tokens_list = tokenzer_command(line);
	data->list = parser_command(data, tokens_list);
	data->list = expander_command(data, data->list);
	func(data, data->list);
	clear_list(&data->list);
	return (0);
}
