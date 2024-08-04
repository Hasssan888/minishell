/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:52:20 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/04 15:38:15 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	*expand_digits(t_data *data, char *argument, int *i)
{
	int	j;

	if (argument[*i + 1] && ft_isdigit(argument[*i + 1]))
	{
		*i += 2;
		j = *i;
		if (!argument[j])
			return (data->expanded);
		while (argument[++j] && argument[j] != '$')
			;
		data->expanded = ft_strjoin(data->expanded, duplicate_word(argument,
					i, j));
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
		data->expanded = ft_strjoin(data->expanded,
				ft_itoa(g_exit_stat));
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
		j++;
	if (arg[j] && arg[j] == quote)
		j++;
	_quoted_word = ft_calloc(j - *i + 1, sizeof(char));
	ft_strlcpy(_quoted_word, &arg[*i], j - *i + 1);
	*i = j;
	return (_quoted_word);
}

char	*expand_vars(t_data *data, char *argument, int i)
{
	char	*word;

	data->expanded = ft_strdup("");
	while (argument[i])
	{
		word = _get_quoted___word(argument, &i);
		if (word != NULL && word[0] != '\'')
		{
			expand_vars__(data, word);
			free(word);
		}
		else
			data->expanded = ft_strjoin(data->expanded, word);
	}
	free(argument);
	return (data->expanded);
}

size_t	ft_len_arr(char **arr)
{
	size_t	len;

	len = -1;
	if (!arr)
		return (0);
	while (arr[++len] != NULL)
		;
	return (len);
}

char	**ft_arr_join(char **arr1, char **arr2)
{
	int		i;
	int		j;
	int		len1;
	int		len2;
	char	**joined;

	i = 0;
	j = 0;
	if (!arr1 && !arr2)
		return (NULL);
	len1 = ft_len_arr(arr1);
	len2 = ft_len_arr(arr2);
	joined = malloc((len1 + len2 + 1) * sizeof(char *));
	while (arr1 != NULL && arr1[j] != NULL)
		joined[i++] = ft_strdup(arr1[j++]);
	j = 0;
	while (arr2 != NULL && arr2[j] != NULL)
		joined[i++] = ft_strdup(arr2[j++]);
	joined[i] = NULL;
	free_array(arr1);
	free_array(arr2);
	return (joined);
}

int	ambigous_red(char *red_file)
{
	int		i;
	char	*str;

	i = 0;
	if (!red_file)
		return (1);
	while (red_file != NULL && red_file[i] && ft_strchr(" \t", red_file[i]))
		i++;
	while (red_file != NULL && red_file[i])
	{
		str = ft_strchr(" \t", red_file[i]);
		if (str != NULL)
		{
			while (red_file[i] && ft_strchr(" \t", red_file[i]))
				i++;
			if (red_file[i] && !ft_strchr(" \t", red_file[i]))
				return (1);
		}
		i++;
	}
	return (0);
}

int	is_ambiguous(t_command *list)
{
	if (!list->quoted && (list->type == RED_OUT || list->type == RED_IN))
	{
		if (ambigous_red(list->args[0]))
		{
			ft_perror("ambiguous redirect\n");
			list->syntxerr = AMPIGOUS;
			g_exit_stat = 1;
		}
	}
	return (1);
}

int	is_empty(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	if (!str[i])
		return (0);
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (!str[i])
		return (1);
	return (0);
}

void	set_error(t_data *data, int err_num, char *str, t_command **cmd)
{
	ft_perror(str);
	data->list->syntxerr = err_num;
	g_exit_stat = err_num;
	clear_list(cmd);
}

char	**split_argument(t_command *list, int i)
{
	char	**splited;

	splited = NULL;
	splited = ft_split_str(list->args[i], WHITESPACES);
	if (!splited)
		return (splited);
	free(list->value);
	list->value = ft_strdup(splited[0]);
	free_array(splited);
	return (splited);
}

int	get_cmd_if_empty(t_command *list)
{
	int	i;

	i = 0;
	if (!list->args || !list->args[0])
		return 0;
	if (list->type == TOKEN && is_empty(list->value))
	{
		while (list->args[i] != NULL && is_empty(list->args[i]))
			i++;
		if (!list->args[i])
			return (0);
		free(list->value);
		list->value = ft_strdup(list->args[0]);
	}
	return (1);
}

char **split_and_join(char **args, char *exp_args)
{
	char **splited = NULL;
	splited = ft_split_str(exp_args, WHITESPACES);
	args = ft_arr_join(args, splited);
	return args;
}

int get_real_len(char **arr)
{
	int i = -1;
	int len = 0;

	while(arr && arr[++i])
	{
		if (!is_empty(arr[i]))
			len++;
	}
	return (len);
}

char **ft_arr_add_back(char **arr, char *str)
{
	int i = -1;
	int len = get_real_len(arr);
	char **arr_cpy = malloc((len + 2) * sizeof(char *));
	if (!arr_cpy)
		return NULL;
	if (!arr || !len)
	{
		arr_cpy[0] = ft_strdup(str);
		arr_cpy[1] = NULL;
		return arr_cpy;
	}
	if (!arr_cpy)
		return NULL;
	while(++i < len)
		arr_cpy[i] = ft_strdup(arr[i]);
	arr_cpy[i] = ft_strdup(str);
	arr_cpy[i + 1] = NULL;
	free_array(arr);
	return arr_cpy;
}

char **exp___(t_data *data, t_command *list, char **args, int i)
{
	int SPLIT = 0;
	if (ft_strchr(list->args[i], '$') && list->quoted != 1
			&& list->type != HER_DOC)
	{
		list->args[i] = expand_vars(data, list->args[i], 0);
		SPLIT = 1;
	}
	if (!is_empty(list->args[i]))
	{
		if (list->type != HER_DOC)
			list->args[i] = unquote_arg(list, list->args[i], 0, 0);
		if (SPLIT && !list->quoted)
		{
			args = split_and_join(args, list->args[i]);
			SPLIT = 0;
		}
		else
			args = ft_arr_add_back(args, list->args[i]);
		if (data->syntax_error)
		{
			set_error(data, SYNTERRR, "syntax error\n", &data->head);
			return (NULL);
		}
	}
	return (args);
}

int	expander_extended(t_data *data, t_command *list)
{
	int i = -1;
	char **args = NULL;

	while (list->value != NULL && list->value[0] && list->args != NULL
		&& list->args[++i] != NULL)
	{
		args = exp___(data, list, args, i);
		if (!args)
			return 0;
	}
	free_array(list->args);
	list->args = args;
	if (list->value && list->value[0] && ft_strchr(list->value, '$'))
		list->value = expand_vars(data, list->value, 0);
	list->value = unquote_arg(list, list->value, 0, 0);
	if (!get_cmd_if_empty(list))
	{
		clear_list(&list);
		return (0);
	}
	is_ambiguous(list);
	return (1);
}

t_command	*expander_command(t_data *data, t_command *list)
{
	data->head = list;
	if (!list)
		return (NULL);
	if (list != NULL && list->type == PIPE)
	{
		set_error(data, SYNTERRR, "syntax error\n", &data->head);
		return (NULL);
	}
	while (list != NULL)
	{
		list->quoted = 0;
		list->syntxerr = 0;
		if (list->type == -1)
		{
			set_error(data, SYNTERRR, "syntax error\n", &data->head);
			return (NULL);
		}
		if (!expander_extended(data, list))
			return (NULL);
		list = list->next;
	}
	return (data->head);
}
