/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:43:58 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/11 23:39:56 by aelkheta         ###   ########.fr       */
/*                                                                           */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	put_error(char *str)
{
	ft_putstr_fd("minishell: export: not a valid identifier `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'\n", 2);
}

int	is_builtin_cmd(t_data *data, t_command *command)
{
	if (!command)
		return (0);
	if (ft_strcmp(command->value, "echo") == 0)
		echo(&command->args[1]);
	else if (ft_strcmp(command->value, "pwd") == 0)
		pwd(data);
	else if (ft_strcmp(command->value, "cd") == 0)
		cd(data, &command->args[1]);
	else if (ft_strcmp(command->value, "env") == 0)
		env(data->env);
	else if (ft_strcmp(command->value, "export") == 0)
		export(data, command, data->env);
	else if (ft_strcmp(command->value, "unset") == 0)
		unset(data, &command->args[1]);
	else if (ft_strcmp(command->value, "exit") == 0)
		exit_(data, command);
	else
		return (0);
	return (1);
}
