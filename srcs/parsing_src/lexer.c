/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:50:47 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/31 11:50:29 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

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

void	get_quoted_index(int *i, int *j, int quote)
{
	g_data->unquoted_line[(*j)++] = g_data->trimed_line[(*i)++];
	while (g_data->trimed_line[(*i)] && g_data->trimed_line[(*i)] != quote)
		g_data->unquoted_line[(*j)++] = g_data->trimed_line[(*i)++];
	if (!g_data->trimed_line[(*i)])
		return ;
	g_data->unquoted_line[(*j)++] = g_data->trimed_line[(*i)++];
}

char	*lexer_command(char *line)
{
	g_data->trimed_line = ft_strtrim(line, " \t\n\v");
	free(line);
	if (check_unqoted(g_data->trimed_line))
	{
		g_data->exit_status = 2;
		ft_perror("syntax error\n");
		free(g_data->trimed_line);
		g_data->trimed_line = NULL;
	}
	return (g_data->trimed_line);
}
