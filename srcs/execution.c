/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:45:27 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/27 16:42:26 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void parent_sigint_handler(int signo) 
{
	if (signo == SIGINT)
	    printf("Parent caught SIGINT (Ctrl+C)\n");
	else if (signo == SIGQUIT)
	    printf("Parent caught SIGINT (Ctrl+\\)\n");
}

void child_sigint_handler(int signo) 
{
	(void) signo;
	exit(0);
}

int	exec_command(t_command *commands_list)
{
	pid_t	pid;
	int		fd[2];
	int		prev_fd;
	char	*cmd_path;
	
	prev_fd = -1;
	struct sigaction sa_parent, sa_child;
	sa_parent.sa_handler = parent_sigint_handler;
    sigemptyset(&sa_parent.sa_mask);
    sa_parent.sa_flags = 0;
	
	while (commands_list != NULL)
	{
		int		fd_out = 1;
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
				else if (commands_list->next != NULL && (commands_list->next->type == RED_OUT || commands_list->next->type == RED_IN))
				{
					t_command *ptr = commands_list->next;
					while(ptr != NULL && (ptr->type == RED_OUT || ptr->type == RED_IN))
					{
						if (ptr->type == RED_OUT)
							fd_out = open(ptr->args[0], O_TRUNC | O_CREAT | O_RDWR, 0744);
						if (ptr->type == RED_IN)
							prev_fd = open(ptr->args[0], O_RDONLY);
						ptr = ptr->next;
					}
				}
				pid = fork();
				if (pid == -1)
				{
					perror("fork");
					return (0);
				}
				else if (pid == 0)
				{
					sa_child.sa_handler = child_sigint_handler;
					sigemptyset(&sa_child.sa_mask);
					sa_child.sa_flags = 0;
					sigaction(SIGINT, &sa_child, NULL);
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
					execve(cmd_path, commands_list->args, data->envirenment);
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
					wait(&data->exit_status);
				}
			}
			// else if (commands_list->type == RED_OUT)
			// {
			// 	// open(commands_list->args[0], O_CREAT | O_RDWR, 0644);
			// 	while(commands_list != NULL && commands_list->type == RED_OUT)
			// 	{
			// 		open(commands_list->args[0], O_CREAT | O_RDWR, 0744);
			// 		commands_list = commands_list->next;
			// 	}
			// 	continue;
			// }
			else
			{
				printf("%s: command not found\n", commands_list->value);
				data->exit_status = 127;
			}
			free(cmd_path);
		}
		commands_list = commands_list->next;
		if (commands_list != NULL && commands_list->type == PIPE)
			commands_list = commands_list->next;
	}
	return (0);
}
