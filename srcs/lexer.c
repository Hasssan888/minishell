/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:50:47 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/09 14:51:13 by aelkheta         ###   ########.fr       */
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

char	*lexer_command(char *line)
{
	int		i;
	int		j;
	char	*trimed_line;
	char	*unquoted_line;

	i = 0;
	j = 0;
	trimed_line = ft_strtrim(line, " \t\n\v");
	free(line);
	unquoted_line = ft_calloc(ft_strlen(trimed_line) + 1, sizeof(char));
	while (trimed_line[i])
	{
		if (trimed_line[i] && trimed_line[i] == '\'')
		{
			if (trimed_line[i + 1] == '\'')
				i += 2;
			else // if (trimed_line[i + 1] == '"')
			{
				unquoted_line[j++] = trimed_line[i++];
				while (trimed_line[i] && trimed_line[i] != '\'')
					unquoted_line[j++] = trimed_line[i++];
				if (!trimed_line[i])
					break ;
				unquoted_line[j++] = trimed_line[i++];
			}
			// else
			// 	unquoted_line[j++] = trimed_line[i++];
		}
		else if (trimed_line[i] && trimed_line[i] == '"')
		{
			if (trimed_line[i + 1] == '"')
				i += 2;
			else // if (trimed_line[i + 1] == '\'')
			{
				unquoted_line[j++] = trimed_line[i++];
				while (trimed_line[i] && trimed_line[i] != '"')
					unquoted_line[j++] = trimed_line[i++];
				if (!trimed_line[i])
					break ;
				unquoted_line[j++] = trimed_line[i++];
			}
			// else
			// 	unquoted_line[j++] = trimed_line[i++];
		}
		else
			unquoted_line[j++] = trimed_line[i++];
	}
	free(trimed_line);
	unquoted_line[j] = '\0';
	if (check_unqoted(unquoted_line))
	{
		printf("syntax error\n");
		free(unquoted_line);
		unquoted_line = NULL;
	}
	return (unquoted_line);
}
