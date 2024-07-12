/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:45:27 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/12 15:28:41 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"


int exec_command(t_command *commands_list) 
{
    pid_t pid;
    int fd[2];
    int prev_fd = -1;
    char *cmd_path;

    while (commands_list != NULL) 
    {
        cmd_path = get_cmd_path(commands_list->value);
        if (commands_list->type == TOKEN && cmd_path != NULL)
        {
            if (commands_list->next != NULL && commands_list->next->type == PIPE) 
                pipe(fd);
            pid = fork();
            if (pid == -1) 
            {
                perror("fork");
                return (0);
            } 
            else if (pid == 0) 
            {
                if (prev_fd != -1) 
                {
                    dup2(prev_fd, STDIN_FILENO);
                    close(prev_fd);
                }
                if (commands_list->next != NULL && commands_list->next->type == PIPE) 
                {
                    close(fd[0]);
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);
                }
                execve(cmd_path, commands_list->args, data->envirenment);
                perror("execve");
                exit(1);
            } 
            else 
            {
                if (prev_fd != -1) 
                    close(prev_fd);
                if (commands_list->next != NULL && commands_list->next->type == PIPE) 
                {
                    close(fd[1]);
                    prev_fd = fd[0];
                }
                wait(NULL); // Wait for the child process to finish
            }
        }
        else if (commands_list->type == RED_OUT)
        {
            // while(commands_list != NULL && commands_list->type == RED_OUT)
                open(commands_list->args[0], O_CREAT | O_RDWR, 0644);
            // open(commands_list->args[0], O_CREAT | O_RDWR, 0744);
        }
        else 
        {
            printf("%s: command not found\n", commands_list->value);
            data->exit_status = 127;
        }
        free(cmd_path);
        
        commands_list = commands_list->next;
        
        if (commands_list != NULL && commands_list->type == PIPE) 
            commands_list = commands_list->next;
    }

    return (0);
}
