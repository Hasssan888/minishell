/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:51:08 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/09 14:48:55 by aelkheta         ###   ########.fr       */
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

char	*get_token(char *command_line, int *i)
{
	int		j;
	char	*token_val;
	char	quote;
	char	special;

	j = 0;
	token_val = NULL;
	j = *i;
	while (command_line[*i])
	{
		while (command_line[j] && ft_strchr(" \t\v", command_line[j]))
			j++;
		*i = j;
		if (command_line[j] == '\'' || command_line[j] == '"')
		{
			quote = command_line[j++];
			while (command_line[j] && command_line[j] != quote)
				j++;
			if (command_line[j] == quote)
				j++;
		}
		else if (command_line[j] == '<' || command_line[j] == '>'
			|| command_line[j] == '|')
		{
			special = command_line[j];
			while (command_line[j] && command_line[j] == special)
				j++;
			token_val = malloc((j - *i) * sizeof(char) + 1);
			ft_strlcpy(token_val, &command_line[*i], j - *i + 1);
			*i = j;
			return (token_val);
		}
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
		token_val = malloc((j - *i) * sizeof(char) + 1);
		ft_strlcpy(token_val, &command_line[*i], j - *i + 1);
		*i = j;
		return (token_val);
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
