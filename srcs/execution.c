/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:45:27 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/09 14:45:51 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	exec_command(t_command *commands_list)
{
		pid_t pid;
		char *cmd_path;

	while (commands_list != NULL)
	{
		// if (is_builtin_cmd(commands_list) && commands_list->type == TOKEN)
		// {
		is_builtin_cmd(commands_list) && commands_list->type == TOKEN;
		return (0);
		// }
		// else if (commands_list->type == TOKEN)
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
		// else if (commands_list->type == RED_IN
			// || commands_list->type == RED_OUT)
		// {
		// 	open(commands_list->args[0], O_RDWR | O_CREAT | O_TRUNC, 0644);
		// }
		commands_list = commands_list->next;
	}
	return (0);
}
