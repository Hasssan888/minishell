/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:51:08 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/31 09:40:08 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

int	get_token_type(char *token)
{
	if (token[0] == '|' && !token[1])
		return (PIPE);
	else if (token[0] == '>' && !token[1])
		return (RED_OUT);
	else if (token[0] == '<' && !token[1])
		return (RED_IN);
	else if (token[0] == '>' && token[1] == '>' && !token[2])
		return (APP);
	else if (token[0] == '<' && token[1] == '<' && !token[2])
		return (HER_DOC);
	else if ((ft_strchr("|&", token[0]) && ft_strlen(token) > 1)
		|| (ft_strchr("<>", token[0]) && ft_strlen(token) > 2))
		return (-1);
	else
		return (TOKEN);
}

int	get_quoted_word_index(char *command_line, int j)
{
	char	quote;

	quote = command_line[j++];
	while (command_line[j] && command_line[j] != quote)
		j++;
	if (command_line[j] == quote)
		j++;
	return (j);
}

int	get_word_index(char *command_line, int j)
{
	char	quote;

	while (command_line[j] && !ft_strchr(" \t\v<|>", command_line[j]))
	{
		if (command_line[j] == '\'' || command_line[j] == '"')
		{
			quote = command_line[j];
			while (command_line[++j] && command_line[j] != quote)
				;
			if (command_line[j] == quote)
				j++;
		}
		else if (command_line[j])
			j++;
	}
	return (j);
}

char	*get_token(char *command_line, int *i)
{
	int		j;
	char	special;

	j = 0;
	j = *i;
	while (command_line[*i])
	{
		while (command_line[j] && ft_strchr(" \t\v", command_line[j]))
			j++;
		*i = j;
		if (command_line[j] == '\'' || command_line[j] == '"')
			j = get_quoted_word_index(command_line, j);
		else if (command_line[j] == '<' || command_line[j] == '>'
			|| command_line[j] == '|')
		{
			special = command_line[j];
			while (command_line[j] && command_line[j] == special)
				j++;
			return (duplicate_word(command_line, i, j));
		}
		j = get_word_index(command_line, j);
		return (duplicate_word(command_line, i, j));
	}
	return (NULL);
}

t_command	*tokenzer_command(char *command_line)
{
	int			i;
	int			type;
	char		*token;
	t_command	*table;
	t_command	*node;

	i = 0;
	node = NULL;
	table = NULL;
	if (!command_line)
		return (NULL);
	else if (!command_line[0])
	{
		free(command_line);
		node = new_node(TOKEN, ft_strdup(""));
		return (node);
	}
	while (command_line[i])
	{
		token = get_token(command_line, &i);
		type = get_token_type(token);
		if (type == -1)
		{
			free(token);
			free(command_line);
			clear_list(&table);
			data->exit_status = 2;
			ft_perror("syntax error\n");
			return (NULL);
		}
		node = new_node(type, token);
		add_back_list(&table, node);
	}
	free(command_line);
	return (table);
}
