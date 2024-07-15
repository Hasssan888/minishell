/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:52:20 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/15 11:46:54 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*aziz(char *argument, int *i)
{
	if (argument[1] && ft_isdigit(argument[1]) && ft_strlen(&argument[1]) > 2)
	{
		*i += 2;
		int j = *i;
		while(argument[++j] && argument[j] != '$');
		data->expanded = ft_strjoin(data->expanded, duplicate_word(argument, i, j));
		// *i += ft_strlen(&argument[2]);
	}
	// free(argument);
	return (data->expanded);
}

char	*expand_vars(char *argument, int i)
{
	data->expanded = ft_strdup("");
	while (argument[i])
	{
		if (argument[i] == '$' && (ft_isalnum(argument[i + 1]) || argument[i + 1] == '?'))
		{
			if (!argument[i + 1])
				return (free(argument), ft_strdup("$"));
			else if (argument[i + 1] == '?')
			{
				i += 2;
				data->expanded = ft_strjoin(data->expanded,
						ft_itoa(data->exit_status));
			}
			else if (!ft_isdigit(argument[i + 1]))
			{
				get_expanded(argument, &i);
				// {
					// free(data->expanded);
					// data->expanded = NULL;
				// }
			}
			else if (ft_isdigit(argument[i + 1]))
				aziz(argument, &i);
		}
		// else if ()
		else
		{
			data->str1 = get_word(argument, &i);
			data->expanded = ft_strjoin(data->expanded, data->str1);
		}
		// printf("%s\n", data->expanded);
	}
	free(argument);
	return (data->expanded);
}

size_t ft_len_arr(char **arr)
{
	size_t len = -1;
	if (!arr)
		return (0);
	while(arr[++len] != NULL);
	return (len);
}

char **ft_arr_join(char **arr1, char **arr2)
{
	int i = 0;
	int j = 0;

	if (!arr1 && !arr2)
		return (NULL);
	int len1 = ft_len_arr(arr1);
	int len2 = ft_len_arr(arr2);
	char **joined = malloc((len1 + len2 + 1) * sizeof(char *));

	while(arr1 != NULL && arr1[j] != NULL)
		joined[i++] = ft_strdup(arr1[j++]);
	j = 0;
	while(arr2 != NULL && arr2[j] != NULL)
		joined[i++] = ft_strdup(arr2[j++]);
	joined[i] = NULL;
	// free_array(arr1);
	// free_array(arr2);
	return (joined);
}

int	expander_extended(t_command *list)
{
	char **args = NULL;
	char **splited = NULL;
	list->value = unquote_arg(list, list->value, 0, 0);
	while (list->value != NULL && list->args != NULL && list->args[data->i] != NULL)
	{
		list->args[data->i] = unquote_arg(list, list->args[data->i], 0, 0);
		if (list->quoted != 1 && list->type != HER_DOC)
			list->args[data->i] = expand_vars(list->args[data->i], 0);
		if (data->syntax_error)
		{
			clear_list(&data->head);
			ft_perror("syntax error\n");
			data->syntax_error = 0;
			return (0);
		}
		data->i++;
	}
	// if (!list->value)
	// 	return (0);
	if (list->quoted != 1 && ft_strchr(list->value, '$'))
	{
		list->value = expand_vars(list->value, 0);
		if (!list->quoted && list->value != NULL)
		{
			splited = ft_split_str(list->value, " \t\v");
			list->value = splited[0];
			if (splited != NULL && splited[0] != NULL && splited[1] != NULL)
			{
				args = ft_arr_join(splited, &list->args[1]);	
				list->args = args;
			}
		}	
	}
	// list = list->next;
	return (1);
}

t_command	*expander_command(t_command *list)
{
	data->head = list;
	if (list != NULL && list->type == PIPE)
	{
		ft_perror("syntax error\n");
		clear_list(&data->head);
		return (NULL);
	}
	while (list != NULL)
	{
		data->i = 0;
		list->quoted = 0;
		if (list->type == -1)
		{
			ft_perror("syntax error\n");
			clear_list(&data->head);
			return (NULL);
		}
		if (!expander_extended(list))
			return (NULL);
		list = list->next;
	}
	return (data->head);
}
