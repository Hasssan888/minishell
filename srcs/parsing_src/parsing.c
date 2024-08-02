/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:40:09 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/31 18:14:40 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

t_command	*syntax_error(t_command *list_command, t_command *head)
{
	ft_perror("syntax error\n");
	g_data->syntax_error = SYNTERRR;
	g_data->exit_status = 2;
	free_node(&list_command);
	clear_list(&head);
	return (NULL);
}

void	free_command(t_command *cmd)
{
	free(cmd->value);
	free_array(cmd->args);
	free(cmd);
}

t_command	*get_command_with_args(void)
{
	int	i;
	int	args_size;

	i = 0;
	args_size = get_args_size(g_data->_tokens_list);
	g_data->list_command->args = malloc((args_size + 2) * sizeof(char *));
	if (!g_data->list_command)
		return (NULL);
	while (g_data->_tokens_list != NULL && g_data->_tokens_list->type != PIPE)
	{
		g_data->list_command->args[i++] = ft_strdup(g_data->_tokens_list->value);
		g_data->list_command->args[i] = NULL;
		g_data->_tokens_list = free_node(&g_data->_tokens_list);
		while (g_data->_tokens_list != NULL && (g_data->_tokens_list->type >= 2
				&& g_data->_tokens_list->type <= 5))
		{
			g_data->_tokens_list = redirect_list(&g_data->head,
					&g_data->rdrct_head);
			if (g_data->syntax_error)
			{
				clear_list(&g_data->rdrct_head);
				clear_list(&g_data->head);
				return (NULL);
			}
		}
	}
	return (g_data->list_command);
}

void	init_parser_var(void)
{
	g_data->list_command = new_node(g_data->_tokens_list->type,
			ft_strdup(g_data->_tokens_list->value));
	if (!g_data->list_command)
		return ;
	g_data->list_command->args = NULL;
	g_data->rdrct_head = NULL;
}

t_command	*parser_command(t_command *_tokens_list)
{
	g_data->head = NULL;
	g_data->_tokens_list = _tokens_list;
	while (g_data->_tokens_list != NULL)
	{
		g_data->_tokens_list->syntxerr = 0;
		if (g_data->syntax_error)
			return (NULL);
		init_parser_var();
		if (!g_data->list_command)
		{
			clear_list(&g_data->head);
			return (NULL);
		}
		if (g_data->_tokens_list->type == TOKEN)
		{
			g_data->list_command = get_command_with_args();
			if (!g_data->list_command)
				return (NULL);
		}
		else if (g_data->_tokens_list->type >= 2
			&& g_data->_tokens_list->type <= 5)
			get_redirect_node(g_data->list_command);
		else
		{
			g_data->_tokens_list = free_node(&g_data->_tokens_list);
			if (!g_data->_tokens_list)
				return (syntax_error(g_data->list_command, g_data->head));
		}
		add_back_list(&g_data->head, g_data->list_command);
		add_back_list(&g_data->head, g_data->rdrct_head);
	}
	return (g_data->head);
}

int	parse_command(char *line)
{
	t_command	*tokens_list;

	// printf("\n++++++++++++++++++ parsing is started +++++++++++++++++\n");
	g_data->list = NULL;
	g_data->ignore_sig = 0;
	g_data->syntax_error = 0;
	if (line != NULL && !line[0])
		return (0);
	line = lexer_command(line);
	if (!line)
		return (0);
	tokens_list = tokenzer_command(line);
	g_data->list = parser_command(tokens_list);
	print_list(g_data->list);
	printf("\n\n");
	g_data->list = expander_command(g_data->list);
	print_list(g_data->list);
	printf("\n\n");
	// delete_empty_nodes(g_data->list);
	// printf("\n++++++++++++++++++ parsing is done +++++++++++++++++\n");
	// printf("\n++++++++++++++++++ execution is started +++++++++++++++++\n");
	// exec_command(g_data->list);
	g_data->ignore_sig = 1;
	// is_builtin_cmd(g_data->list);
	func(g_data->list);
	// printf("\n++++++++++++++++++ execution is done +++++++++++++++++\n");
	clear_list(&g_data->list);
	return (0);
}
