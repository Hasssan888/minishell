/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:28:02 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/18 14:41:36 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

t_command	*syntax_error(t_data *data, t_command *list_command,
		t_command *head, char *str)
{
	ft_putstr_fd(str, 2);
	data->syntax_error = SYNTERRR;
	g_exit_stat = SYNTERRR;
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
		if (list->next != NULL && list->next == TOKEN && (list->type >= 2
				&& list->type <= 5))
			i--;
		list = list->next;
	}
	return (i);
}

t_command	*redirect_list(t_data *data, t_command **redirect_head)
{
	t_command	*red_node;

	red_node = new_node(data->_tokens_list->type,
			ft_strdup(data->_tokens_list->value));
	data->_tokens_list = free_node(&data->_tokens_list);
	if (!data->_tokens_list || data->_tokens_list->type != TOKEN)
	{
		check_synt_err(red_node->value);
		clear_list(&data->rdrct_head);
		clear_list(&data->head);
		free_node(&red_node);
		free_array(data->list_cmd->args);
		free_node(&data->list_cmd);
		g_exit_stat = SYNTERRR;
		clear_list(&data->_tokens_list);
		return (NULL);
	}
	red_node->args = malloc(2 * sizeof(char *));
	if (!red_node->args)
		panic("malloc fail\n", 1);
	red_node->args[0] = ft_strdup(data->_tokens_list->value);
	red_node->args[1] = NULL;
	data->_tokens_list = free_node(&data->_tokens_list);
	add_back_list(redirect_head, red_node);
	return (data->_tokens_list);
}

void	get_redirect_node(t_data *data)
{
	data->_tokens_list = free_node(&data->_tokens_list);
	if (!data->_tokens_list || data->_tokens_list->type != TOKEN)
	{
		g_exit_stat = SYNTERRR;
		return ;
	}
	data->list_cmd->args = malloc(2 * sizeof(char *));
	if (!data->list_cmd->args)
		panic("malloc fail\n", 1);
	data->list_cmd->args[0] = ft_strdup(data->_tokens_list->value);
	data->list_cmd->args[1] = NULL;
	data->_tokens_list = free_node(&data->_tokens_list);
}

char	*duplicate_word(char *command_line, int *i, int j)
{
	char	*token_val;

	token_val = malloc((j - *i) * sizeof(char) + 1);
	if (!token_val)
		panic("malloc fail\n", 1);
	ft_strlcpy(token_val, &command_line[*i], j - *i + 1);
	*i = j;
	return (token_val);
}
