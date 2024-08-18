/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:50:47 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/13 18:03:52 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	*get_prompt(void)
{
	char	*prompt;
	char	*cwd;

	prompt = NULL;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		prompt = ft_strjoin(ft_strdup("┌──(aelkheta㉿1337)-["), cwd);
		prompt = ft_strjoin(prompt, ft_strdup("]\n└─$ "));
	}
	return (prompt);
}

char	*read_input(void)
{
	char	*prompt;
	char	*command;

	prompt = get_prompt();
	command = readline(prompt);
	free(prompt);
	return (command);
}

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

void	get_quoted_index(t_data *data, int *i, int *j, int quote)
{
	data->unquoted_line[(*j)++] = data->trimed_line[(*i)++];
	while (data->trimed_line[(*i)] && data->trimed_line[(*i)] != quote)
		data->unquoted_line[(*j)++] = data->trimed_line[(*i)++];
	if (!data->trimed_line[(*i)])
		return ;
	data->unquoted_line[(*j)++] = data->trimed_line[(*i)++];
}

char	*lexer_command(t_data *data, char *line)
{
	data->trimed_line = ft_strtrim(line, WHITESPACES);
	free(line);
	if (check_unqoted(data->trimed_line))
	{
		g_exit_stat = 2;
		ft_putstr_fd("minishell: syntax error unclosed quotes\n", 2);
		free(data->trimed_line);
		data->trimed_line = NULL;
	}
	return (data->trimed_line);
}
