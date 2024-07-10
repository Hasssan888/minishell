/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:45:27 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/10 09:48:51 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	exec_command(t_command *commands_list)
{
	pid_t	pid;
	char	*cmd_path;

	while (commands_list != NULL)
	{
		is_builtin_cmd(commands_list);
		return (0);
		if (commands_list->type == TOKEN)
		{
			cmd_path = get_cmd_path(commands_list->value);
			if (cmd_path != NULL)
			{
				pid = fork();
				if (pid == -1)
					return (0);
				else if (pid == 0)
					execve(cmd_path, commands_list->args, data->envirenment);
				else
					wait(NULL);
			}
			else
				printf("%s: command not found\n", commands_list->value);
			free(cmd_path);
		}
		commands_list = commands_list->next;
	}
	return (0);
}
