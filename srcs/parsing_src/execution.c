/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:45:27 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/31 12:01:01 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	sig_child_hanler(int signo)
{
	g_data->ignore_sig = 1;
	if (signo == SIGINT)
	{
		// printf("\n%s", g_data->prompt);
		g_data->exit_status = 130;
		exit(0);
	}
	else if (signo == SIGQUIT)
		g_data->exit_status = 131;
	else if (signo == SIGTSTP)
		g_data->exit_status = 132;
	exit(0);
}

void	sig_dflt(void)
{
	signal(SIGTSTP, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
int	exec_command(t_command *commands_list)
{
	pid_t	pid;
	int		fd[2];
	int		prev_fd;
	char	*cmd_path;
	int		fd_out;

	prev_fd = -1;
	while (commands_list != NULL)
	{
		fd_out = 1;
		if (is_builtin_cmd(commands_list))
		{
			commands_list = commands_list->next;
			continue ;
		}
		else
		{
			cmd_path = get_cmd_path(commands_list->value);
			if (commands_list->type == TOKEN && cmd_path != NULL)
			{
				if (commands_list->next != NULL
					&& commands_list->next->type == PIPE)
					pipe(fd);
				// else if (commands_list->next != NULL
				// && (commands_list->next->type == RED_OUT
				// || commands_list->next->type == RED_IN))
				// {
				// 	t_command *ptr = commands_list->next;
				// 	while(ptr != NULL && (ptr->type == RED_OUT
				// || ptr->type == RED_IN))
				// {
				// 		if (ptr->type == RED_OUT)
				// 			fd_out = open(ptr->args[0],
				// O_TRUNC | O_CREAT | O_RDWR, 0744);
				// 		if (ptr->type == RED_IN)
				// 			prev_fd = open(ptr->args[0], O_RDONLY);
				// 		ptr = ptr->next;
				// 	}
				// }
				pid = fork();
				if (pid == -1)
				{
					perror("fork");
					return (0);
				}
				else if (pid == 0)
				{
					g_data->ignore_sig = 0;
					sig_dflt();
					dup2(fd_out, STDOUT_FILENO);
					if (prev_fd != -1)
					{
						dup2(prev_fd, STDIN_FILENO);
						close(prev_fd);
					}
					// else if (commands_list->next != NULL
					// 	&& (commands_list->next->type == RED_OUT))
					if (commands_list->next != NULL
						&& (commands_list->next->type == PIPE))
					{
						close(fd[0]);
						dup2(fd[1], STDOUT_FILENO);
						close(fd[1]);
					}
					// else if (commands_list->next != NULL
					// 	&& (commands_list->next->type == RED_IN))
					// 	dup2(fd_out, STDIN_FILENO);
					execve(cmd_path, commands_list->args, g_data->envirenment);
					ft_perror("execve");
					exit(1);
				}
				else
				{
					if (prev_fd != -1)
						close(prev_fd);
					if (commands_list->next != NULL
						&& commands_list->next->type == PIPE)
					{
						close(fd[1]);
						prev_fd = fd[0];
					}
					wait(&g_data->exit_status);
				}
			}
			else if (commands_list->type >= 2 && commands_list->type <= 5)
			{
				// open(commands_list->args[0], O_CREAT | O_RDWR, 0644);
				// while(commands_list != NULL
				// && commands_list->type == RED_OUT)
				// {
				// 	open(commands_list->args[0], O_CREAT | O_RDWR, 0744);
				// 	commands_list = commands_list->next;
				// }
				commands_list = commands_list->next;
				continue ;
			}
			else
			{
				printf("%s: command not found\n", commands_list->value);
				g_data->exit_status = 127;
			}
			free(cmd_path);
		}
		commands_list = commands_list->next;
		if (commands_list != NULL && commands_list->type == PIPE)
			commands_list = commands_list->next;
	}
	return (0);
}
