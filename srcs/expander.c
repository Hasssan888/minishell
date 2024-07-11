/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:52:20 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/10 17:09:59 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*get_exit_code(char *argument)
{
	free(argument);
	return (ft_strdup("$"));
}

char	*expand_vars(char *argument, int i)
{
	data->expanded = ft_strdup("");
	while (argument[i])
	{
		if (argument[i] == '$')
		{
			if (!argument[i + 1])
				return (get_exit_code(argument));
			else if (argument[i + 1] == '?')
			{
				i += 2;
				data->expanded = ft_strjoin(data->expanded, ft_strdup("$?"));
				continue ;
			}
			else if (!get_expanded(argument, &i))
				return (NULL);
		}
		else
		{
			data->str1 = get_word(argument, &i);
			data->expanded = ft_strjoin(data->expanded, data->str1);
		}
	}
	free(argument);
	return (data->expanded);
}

int	expander_extended(t_command *list)
{
	while (list->args != NULL && list->args[data->i] != NULL)
	{
		list->value = unquote_arg(list, list->value, 0, 0);
		list->args[data->i] = unquote_arg(list, list->args[data->i], 0, 0);
		if (list->quoted != 1)
			list->args[data->i] = expand_vars(list->args[data->i], 0);
		if (data->syntax_error)
		{
			clear_list(&data->head);
			printf("syntax error\n");
			data->syntax_error = 0;
			return (0);
		}
		data->i++;
	}
	list = list->next;
	return (1);
}

t_command	*expander_command(t_command *list)
{
	data->head = list;
	if (list != NULL && list->type == PIPE)
	{
		printf("syntax error\n");
		clear_list(&data->head);
		return (NULL);
	}
	while (list != NULL)
	{
		data->i = 0;
		list->quoted = 0;
		if (list->type == -1)
		{
			printf("syntax error\n");
			clear_list(&data->head);
			return (NULL);
		}
		if (!expander_extended(list))
			return (NULL);
		list = list->next;
	}
	return (data->head);
}
