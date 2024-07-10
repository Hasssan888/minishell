/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:51:08 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/10 09:13:40 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	get_token_type(char *token)
{
	if (token[0] == '|' && !token[1])
		return (PIPE);
	else if (token[0] == '|' && token[1] == '|' && !token[2])
		return (OR_OP);
	else if (token[0] == '>' && !token[1])
		return (RED_OUT);
	else if (token[0] == '<' && !token[1])
		return (RED_IN);
	else if (token[0] == '>' && token[1] == '>' && !token[2])
		return (APP);
	else if (token[0] == '<' && token[1] == '<' && !token[2])
		return (HER_DOC);
	else if (token[0] == '&' && !token[1])
		return (BACK);
	else if (token[0] == '&' && token[1] == '&' && !token[2])
		return (AND_OP);
	else if (ft_strchr("<|&>", token[0]) && ft_strlen(token) > 2)
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
	t_command	*table;
	char		*token;
	int			type;
	t_command	*node;

	i = 0;
	table = NULL;
	if (!command_line)
		return (NULL);
	while (command_line[i])
	{
		token = get_token(command_line, &i);
		type = get_token_type(token);
		node = new_node(type, token);
		node->args = NULL;
		add_back_list(&table, node);
	}
	free(command_line);
	return (table);
}
