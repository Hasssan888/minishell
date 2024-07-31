/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:51:08 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/31 13:21:40 by aelkheta         ###   ########.fr       */
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

int	get_quoted_word_index(char *cmd_line, int j)
{
	char	quote;

	quote = cmd_line[j++];
	while (cmd_line[j] && cmd_line[j] != quote)
		j++;
	if (cmd_line[j] == quote)
		j++;
	return (j);
}

int	get_word_index(char *cmd_line, int j)
{
	char	quote;

	while (cmd_line[j] && !ft_strchr(" \t\v<|>", cmd_line[j]))
	{
		if (cmd_line[j] == '\'' || cmd_line[j] == '"')
		{
			quote = cmd_line[j];
			while (cmd_line[++j] && cmd_line[j] != quote)
				;
			if (cmd_line[j] == quote)
				j++;
		}
		else if (cmd_line[j])
			j++;
	}
	return (j);
}

char	*get_token(char *cmd_line, int *i)
{
	int		j;
	char	special;

	j = 0;
	j = *i;
	while (cmd_line[*i])
	{
		while (cmd_line[j] && ft_strchr(" \t\v", cmd_line[j]))
			j++;
		*i = j;
		if (cmd_line[j] == '\'' || cmd_line[j] == '"')
			j = get_quoted_word_index(cmd_line, j);
		else if (cmd_line[j] == '<' || cmd_line[j] == '>'
			|| cmd_line[j] == '|')
		{
			special = cmd_line[j];
			while (cmd_line[j] && cmd_line[j] == special)
				j++;
			return (duplicate_word(cmd_line, i, j));
		}
		j = get_word_index(cmd_line, j);
		return (duplicate_word(cmd_line, i, j));
	}
	return (NULL);
}

t_command	*tokenzer_command(char *cmd_line)
{
	int			i;
	int			type;
	char		*token;
	t_command	*table;
	t_command	*node;

	i = 0;
	node = NULL;
	table = NULL;
	if (!cmd_line)
		return (NULL);
	else if (!cmd_line[0])
	{
		free(cmd_line);
		node = new_node(TOKEN, ft_strdup(""));
		return (node);
	}
	while (cmd_line[i])
	{
		token = get_token(cmd_line, &i);
		type = get_token_type(token);
		if (type == -1)
		{
			free(token);
			free(cmd_line);
			clear_list(&table);
			g_data->exit_status = 2;
			ft_perror("syntax error\n");
			return (NULL);
		}
		node = new_node(type, token);
		add_back_list(&table, node);
	}
	free(cmd_line);
	return (table);
}
