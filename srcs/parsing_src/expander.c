/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:52:20 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/31 16:23:18 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	*expand_digits(char *argument, int *i)
{
	int	j;

	if (argument[*i + 1] && ft_isdigit(argument[*i + 1]))
	{
		*i += 2;
		j = *i;
		if (!argument[j])
			return (g_data->expanded);
		while (argument[++j] && argument[j] != '$')
			;
		g_data->expanded = ft_strjoin(g_data->expanded, duplicate_word(argument,
					i, j));
		*i = j;
	}
	return (g_data->expanded);
}
void	expand_(char *argument, int *i)
{
	if (!argument[*i + 1])
	{
		(*i)++;
		g_data->expanded = ft_strjoin(g_data->expanded, ft_strdup("$"));
	}
	else if (argument[*i + 1] == '?')
	{
		i += 2;
		g_data->expanded = ft_strjoin(g_data->expanded,
				ft_itoa(g_data->exit_status));
	}
	else if (!ft_isdigit(argument[*i + 1]))
	{
		get_expanded(argument, i);
	}
	else if (ft_isdigit(argument[*i + 1]))
		g_data->expanded = expand_digits(argument, i);
}

char	*expand_vars__(char *argument)
{
	int	i;

	i = 0;
	while (argument[i])
	{
		if (argument[i] == '$' && (ft_isalnum(argument[i + 1])
				|| ft_strchr("?_", argument[i + 1])))
			expand_(argument, &i);
		else
		{
			g_data->str1 = get_word(argument, &i);
			g_data->expanded = ft_strjoin(g_data->expanded, g_data->str1);
		}
	}
	if (!g_data->expanded[0])
	{
		free(g_data->expanded);
		g_data->expanded = NULL;
	}
	return (g_data->expanded);
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

char	*expand_vars(char *argument, int i)
{
	char	*word;

	g_data->expanded = ft_strdup("");
	while (argument[i])
	{
		word = _get_quoted___word(argument, &i);
		if (word != NULL && word[0] != '\'')
		{
			expand_vars__(word);
			free(word);
		}
		else
			g_data->expanded = ft_strjoin(g_data->expanded, word);
	}
	free(argument);
	return (g_data->expanded);
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

int	is_ambigous(t_command *list)
{
	if (!list->quoted && (list->type == RED_OUT || list->type == RED_IN))
	{
		if (ambigous_red(list->args[0]))
		{
			clear_list(&g_data->head);
			ft_perror("ambiguous redirect\n");
			g_data->syntax_error = AMPIGOUS;
			g_data->exit_status = 1;
			return (0);
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

// char	**split_args_(char **exp_args, int i)
// {
// 	char	**args;
// 	char	**splited;

// 	args = NULL;
// 	splited = NULL;
// 	if (!exp_args)
// 		return (NULL);
// 	while (exp_args[i] != NULL)
// 	{
// 		if (is_empty(exp_args[i]))
// 		{
// 			i++;
// 			continue ;
// 		}
// 		splited = ft_split_str(exp_args[i], WHITESPACES);
// 		if (splited != NULL && splited[0] != NULL && splited[1] != NULL)
// 			args = ft_arr_join(args, splited);
// 		else
// 			args = ft_arr_join(args, splited);
// 		i++;
// 	}
// 	free_array(exp_args);
// 	return (args);
// }


void	set_error(int err_num, char *str, t_command **cmd)
{
	ft_perror(str);
	g_data->list->syntxerr = err_num;
	g_data->exit_status = err_num;
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
	// char **splited;

	i = 0;
	// splited = NULL;
	if (list->type == TOKEN && is_empty(list->value))
	{
		while (list->args[i] != NULL && is_empty(list->args[i]))
			i++;
		if (!list->args[i])
			return (0);
		free(list->value);
		list->value = ft_strdup(list->args[0]);
	}
	else
	{
		// splited = ft_split_str(list->value, WHITESPACES);
		// if (!splited)
		// 	return (0);
		// free(list->value);
		// list->value = ft_strdup(splited[0]);
		// free_array(splited);
	}
	return (1);
}

char **split_and_join(char **args, char *exp_args)
{
	char **splited = NULL;
	splited = ft_split_str(exp_args, WHITESPACES);
	if (splited != NULL && splited[0] != NULL && splited[1] != NULL)
		args = ft_arr_join(args, splited);
	else
		args = ft_arr_join(args, splited);
	return args;
}

char **ft_arrcpy(char **arr, int arr_len)
{
	int i = -1;
	if (arr_len <= 0 || !arr)
		return NULL;
	char **arr_cpy = malloc((arr_len + 1)* sizeof(char *));
	if (!arr_cpy)
		return NULL;
	while(++i < arr_len)
		arr_cpy[i] = ft_strdup(arr[i]);
	arr_cpy[i] = NULL;
	return arr_cpy;
}

int	expander_extended(t_command *list)
{
	int i = -1;
	int SPLIT = 0;
	char **args = NULL;
	while (list->value != NULL && list->value[0] && list->args != NULL
		&& list->args[++i] != NULL)
	{
		if (is_empty(list->args[i]))
			continue;
		if (ft_strchr(list->args[i], '$') && list->quoted != 1
			&& list->type != HER_DOC)
		{
			list->args[i] = expand_vars(list->args[i], 0);
			SPLIT = 1;
		}
		if (list->type != HER_DOC)
			list->args[i] = unquote_arg(list, list->args[i], 0, 0);
		if (SPLIT)
		{
			args = ft_arrcpy(list->args, i);
			args = split_and_join(args, list->args[i]);
			SPLIT = 0;
		}
		if (g_data->syntax_error)
		{
			set_error(SYNTERRR, "syntax error\n", &g_data->head);
			return (0);
		}
	}
	if (args)
	{
		free_array(list->args);
		list->args = args;
	}
	if (list->value && list->value[0] && ft_strchr(list->value, '$'))
		list->value = expand_vars(list->value, 0);
	list->value = unquote_arg(list, list->value, 0, 0);
	if (!get_cmd_if_empty(list))
	{
		clear_list(&list);
		return (0);
	}
	if (!is_ambigous(list))
		return (0);
	return (1);
}

t_command	*expander_command(t_command *list)
{
	g_data->head = list;
	if (!list)
		return (NULL);
	if (list != NULL && list->type == PIPE)
	{
		set_error(SYNTERRR, "syntax error\n", &g_data->head);
		return (NULL);
	}
	while (list != NULL)
	{
		list->quoted = 0;
		if (list->type == -1)
		{
			set_error(SYNTERRR, "syntax error\n", &g_data->head);
			return (NULL);
		}
		if (!expander_extended(list))
			return (NULL);
		list = list->next;
	}
	return (g_data->head);
}
