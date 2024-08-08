/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utiles4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:13:28 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/08 10:19:38 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	set_error(t_data *data, int err_num, char *str, t_command **cmd)
{
	data->list->syntxerr = err_num;
	data->syntax_error = err_num;
	g_exit_stat = err_num;
	clear_list(cmd);
	perror(str);
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
		return (0);
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

char	**split_and_join(char **args, char *exp_args)
{
	char	**splited;

	splited = NULL;
	splited = ft_split_str(exp_args, WHITESPACES);
	args = ft_arr_join(args, splited);
	return (args);
}

int	get_real_len(char **arr)
{
	int	i;
	int	len;

	i = -1;
	len = 0;
	while (arr && arr[++i])
	{
		if (!is_empty(arr[i]))
			len++;
	}
	return (len);
}
