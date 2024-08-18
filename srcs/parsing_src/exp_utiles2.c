/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utiles2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:12:16 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/18 17:59:33 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	*expand_digits(t_data *data, char *argument, int *i)
{
	int		j;
	char	*dup;

	if (argument[*i + 1] && ft_isdigit(argument[*i + 1]))
	{
		*i += 2;
		j = *i;
		if (!argument[j])
			return (data->expanded);
		while (argument[++j] && argument[j] != '$')
			;
		dup = duplicate_word(argument, i, j);
		data->expanded = ft_strjoin(data->expanded, dup);
		*i = j;
	}
	return (data->expanded);
}

void	expand_(t_data *data, char *argument, int *i)
{
	if (!argument[*i + 1])
	{
		(*i)++;
		data->expanded = ft_strjoin(data->expanded, ft_strdup("$"));
	}
	else if (argument[*i + 1] == '?')
	{
		*i += 2;
		data->expanded = ft_strjoin(data->expanded, ft_itoa(g_exit_stat));
	}
	else if (!ft_isdigit(argument[*i + 1]))
	{
		get_expanded(data, argument, i);
	}
	else if (ft_isdigit(argument[*i + 1]))
		data->expanded = expand_digits(data, argument, i);
}

char	*expand_vars__(t_data *data, char *argument)
{
	int	i;

	i = 0;
	while (argument[i])
	{
		if (argument[i] == '$' && (ft_isalnum(argument[i + 1])
				|| ft_strchr("?_", argument[i + 1])))
			expand_(data, argument, &i);
		else
		{
			data->str1 = get_word(argument, &i);
			data->expanded = ft_strjoin(data->expanded, data->str1);
		}
	}
	if (!data->expanded[0])
	{
		free(data->expanded);
		data->expanded = NULL;
	}
	free(argument);
	return (data->expanded);
}

char	*_get_quoted___word(char *arg, int *i)
{
	int		j;
	char	*_quoted_word;
	char	quote;

	j = *i;
	_quoted_word = NULL;
	quote = 0;
	if (arg[j] == '$' && (arg[j + 1] == '\'' || arg[j + 1] == '"'))
		*i += 1;
	if (arg[j] == '\'' || arg[j] == '"')
		quote = arg[j++];
	while (arg[j] && arg[j] != quote)
	{
		if (quote == 0 && (arg[j] == '\'' || arg[j] == '"'))
			break ;
		j++;
	}
	if (arg[j] && arg[j] == quote)
		j++;
	_quoted_word = ft_calloc(j - *i + 1, sizeof(char));
	ft_strlcpy(_quoted_word, &arg[*i], j - *i + 1);
	*i = j;
	return (_quoted_word);
}

char	*expand_vars(t_data *data, t_command *list, char *argument)
{
	int		i;
	char	*word;

	i = 0;
	data->expanded = ft_strdup("");
	while (argument[i])
	{
		word = _get_quoted___word(argument, &i);
		word = unquote_arg(list, word, 0, 0);
		if (word && ft_strchr(word, '$') && list->quoted != 1)
			expand_vars__(data, word);
		else if (word)
		{
			data->expanded = ft_strjoin(data->expanded, word);
		}
	}
	free(argument);
	return (data->expanded);
}
