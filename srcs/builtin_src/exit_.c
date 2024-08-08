/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:01:31 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/08 22:21:05 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	panic(char *error_str, int exit_stat)
{
	perror(error_str);
	exit(exit_stat);
}

int	check_exit_overflow(char *str, int exit_num)
{
	return (ft_strlen(str) > ft_strlen(MAX_LONG) + 1 || ((exit_num < 0
				&& str[0] != '-') || (exit_num > 0 && str[0] == '-')));
}

void	check_exit(t_data *data, t_command *command)
{
	data->exit_status = ft_atoi(command->args[1]);
	if (check_exit_overflow(command->args[1], data->exit_status))
	{
		clear_all(data);
		clear_list(&data->list);
		panic("minishell: exit: numeric argument required\n", 2);
	}
	g_exit_stat = data->exit_status;
}

void	exit_(t_data *data, t_command *command)
{
	printf("exit\n");
	if (command->args[1] != NULL && !ft_is_str_digit(command->args[1]))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		g_exit_stat = 2;
	}
	else if (command->args[1] != NULL && command->args[2] != NULL)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_exit_stat = 1;
		return ;
	}
	else
		check_exit(data, command);
	clear_all(data);
	clear_list(&data->list);
	exit(g_exit_stat);
}
