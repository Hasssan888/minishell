/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:52:20 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/18 20:26:53 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	**ft_arr_add_back(char **arr, char *str)
{
	int		i;
	int		len;
	char	**arr_cpy;

	i = -1;
	len = get_real_len(arr);
	arr_cpy = malloc((len + 2) * sizeof(char *));
	if (!arr_cpy)
		panic("malloc fail\n", 1);
	if (!arr || !len)
	{
		arr_cpy[0] = ft_strdup(str);
		arr_cpy[1] = NULL;
		return (arr_cpy);
	}
	while (++i < len)
		arr_cpy[i] = ft_strdup(arr[i]);
	arr_cpy[i] = ft_strdup(str);
	arr_cpy[i + 1] = NULL;
	free_array(arr);
	return (arr_cpy);
}

int	is_all_quotes(char *str)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	if (str && (str[i] == '\'' || str[i] == '"'))
		quote = str[i++];
	else
		return (0);
	while (str && str[i++] && str[i] == quote)
		;
	if (!str[i] && ft_strlen(str) > 2)
		return (1);
	return (0);
}

char	**exp___(t_data *data, t_command *list, char **args, int i)
{
	if (ft_strchr(list->args[i], '$') && list->type != HER_DOC)
	{
		list->args[i] = expand_vars(data, list, list->args[i]);
		data->flag = 1;
	}
	else if (!is_empty(list->args[i]) && list->type != HER_DOC)
		list->args[i] = unquote_arg(list, list->args[i], 0, 0);
	if (!is_empty(list->args[i]) || !data->flag)
	{
		if (data->flag && !list->quoted && !is_empty(list->args[i]))
		{
			args = split_and_join(args, list->args[i]);
			data->flag = 0;
		}
		else
			args = ft_arr_add_back(args, list->args[i]);
		if (data->syntax_error)
		{
			set_error(data, SYNTERRR, "minishell: syntax error\n", &data->head);
			return (NULL);
		}
	}
	if (is_empty(list->args[0]))
		free(list->args[i]);
	return (args);
}

int	expander_extended(t_data *data, t_command *list)
{
	int		i;
	char	**args;

	i = -1;
	args = NULL;
	while (list->value != NULL && list->value[0] && list->args != NULL
		&& list->args[++i] != NULL)
	{
		data->flag = 0;
		args = exp___(data, list, args, i);
		if (data->syntax_error)
			return (0);
	}
	if (args != NULL)
	{
		free_array(list->args);
		list->args = args;
	}
	if (list->value && list->value[0] && ft_strchr(list->value, '$'))
		list->value = expand_vars(data, list, list->value);
	get_cmd_if_empty(list);
	is_ambiguous(list);
	return (1);
}

t_command	*expander_command(t_data *data, t_command *list)
{
	data->head = list;
	if (!list)
		return (NULL);
	if (list->type == PIPE)
	{
		set_error(data, SYNTERRR, "minishell: syntax error\n", &data->head);
		return (NULL);
	}
	while (list != NULL)
	{
		list->quoted = 0;
		list->syntxerr = 0;
		if (list->type == -1)
		{
			set_error(data, SYNTERRR, "minishell: syntax error\n", &data->head);
			return (NULL);
		}
		if (!expander_extended(data, list))
			return (NULL);
		list = list->next;
	}
	return (data->head);
}
