/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:50:47 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/22 15:26:26 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	check_unqoted(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'')
		{
			while (line[++i] && line[i] != '\'')
				;
			if (!line[i])
				return (1);
			i++;
		}
		else if (line[i] == '"')
		{
			while (line[++i] && line[i] != '"')
				;
			if (!line[i])
				return (1);
			i++;
		}
		else
			i++;
	}
	return (0);
}

void	check_unqoted_line(int j)
{
	free(data->trimed_line);
	data->unquoted_line[j] = '\0';
	if (check_unqoted(data->unquoted_line))
	{
		printf("syntax error\n");
		free(data->unquoted_line);
		data->unquoted_line = NULL;
	}
}

void	get_quoted_index(int *i, int *j, int quote)
{
	data->unquoted_line[(*j)++] = data->trimed_line[(*i)++];
	while (data->trimed_line[(*i)] && data->trimed_line[(*i)] != quote)
		data->unquoted_line[(*j)++] = data->trimed_line[(*i)++];
	if (!data->trimed_line[(*i)])
		return ;
	data->unquoted_line[(*j)++] = data->trimed_line[(*i)++];
}

char	*lexer_command(char *line, int i, int j)
{
	data->trimed_line = ft_strtrim(line, " \t\n\v");
	free(line);
	data->unquoted_line = ft_calloc(ft_strlen(data->trimed_line) + 1,
			sizeof(char));
	while (data->trimed_line[i])
	{
		if (data->trimed_line[i] == '$' && (data->trimed_line[i + 1] == '"' || data->trimed_line[i + 1] == '\''))
			i++;
		else if (data->trimed_line[i] && data->trimed_line[i] == '\'')
		{
			if (data->trimed_line[i + 1] == '\'')
				i += 2;
			else
				get_quoted_index(&i, &j, '\'');
		}
		else if (data->trimed_line[i] && data->trimed_line[i] == '"')
		{
			if (data->trimed_line[i + 1] == '"')
				i += 2;
			else
				get_quoted_index(&i, &j, '"');
		}
		else
			data->unquoted_line[j++] = data->trimed_line[i++];
	}
	check_unqoted_line(j);
	return (data->unquoted_line);
}
