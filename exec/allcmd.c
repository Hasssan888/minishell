#include "../libraries/minishell.h"

int if_is_buil(t_command *command)
{	
    if (ft_strcmp(command->value, "echo") == 0)
		return(1);
	else if (ft_strcmp(command->value, "pwd") == 0)
		return(1);
	else if (ft_strcmp(command->value, "cd") == 0)
		return(1);
	else if (ft_strcmp(command->value, "env") == 0)
		return(1);
	else if (ft_strcmp(command->value, "export") == 0)
		return(1);
	else if (ft_strcmp(command->value, "unset") == 0)
		return(1);
	else if (ft_strcmp(command->value, "exit") == 0)
        return(1);
    return (0);
}

pid_t	fork_pipe(t_command *node1, char **env, t_pipex *p)
{
	if (pipe(p->end) == -1)
		ft_error_2();
	p->pid = fork();
	if (p->pid == -1)
		ft_error_2();
	else if (p->pid == 0)
    {
        if (p->flag == 1)
        {
            printf("flag = 1\n");
            close(p->end[0]);
            dup2(p->infile, STDIN_FILENO);
            close(p->infile);
            printf("read_in\n");
            if (node1->next && node1->next->type == PIPE)
            {
                dup2(p->end[1], STDOUT_FILENO);
                close(p->end[1]);
            }
            else if (node1->next && (node1->next->type == RED_OUT || node1->next->type == APP))
            {
                if (node1->next->next && (node1->next->next->type == RED_OUT
                        || node1->next->next->type == APP))
                {
                    printf("1+outfile\n");
                    open_outfile(node1, p);
                }
                else
                {
                    printf("one outfile\n");
                    if (node1->next->type == APP)
                        p->outfile = open(node1->next->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
                    else
                        p->outfile = open(node1->next->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }
                dup2(p->outfile, STDOUT_FILENO);
                close(p->outfile);
            }
            if (if_is_buil(node1))
                is_builtin_cmd(node1);
            else
                ft_excute(node1->args, env);
        }
        else if (p->flag == 2 && node1->next->type == HER_DOC && node1->next->next == NULL)
        {
            printf("node1->type == %s\n", node1->args[0]);
            printf("HEREDOC\n");
            close(p->end[0]);
            p->infile_here_doc = open("file_here_doc.txt", O_RDONLY, 0644);
            dup2(p->infile_here_doc, 0);
            close(p->infile_here_doc);
            // dup2(p->end[1], 1);
            // close(p->end[1]);
            if (if_is_buil(node1))
                is_builtin_cmd(node1);
            else
                ft_excute(node1->args, env);
        }
        else if (p->flag == 2 && node1->next->type == HER_DOC
                    && (node1->next->next->type == RED_OUT || node1->next->next->type == APP))
        {
            printf("node1->type == %d\n", node1->type);
            printf("HEREDOC + RED_OUT\n");
            close(p->end[0]);
            if (node1->next->next &&
                (node1->next->next->type == RED_OUT || node1->next->next->type == APP))
            {
                printf("1+outfile\n");
                open_outfile(node1, p);
            }
            else
            {

                printf("one outfile\n");
                if (node1->next->type == APP)
                    p->outfile = open(node1->next->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    p->outfile = open(node1->next->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            p->infile_here_doc = open("file_here_doc.txt", O_RDONLY, 0644);
            dup2(p->infile_here_doc, 0);
            close(p->infile_here_doc);
            dup2(p->outfile, 1);
            close(p->outfile);
            if (if_is_buil(node1))
                is_builtin_cmd(node1);
            else
                ft_excute(node1->args, env);
        }

        else if (p->flag == 2)
        {
            printf("node1->type == %s\n", node1->args[0]);
            printf("HEREDOC--3--\n");
            close(p->end[0]);
            p->infile_here_doc = open("file_here_doc.txt", O_RDONLY, 0644);
            dup2(p->infile_here_doc, 0);
            close(p->infile_here_doc);
            dup2(p->end[1], 1);
            close(p->end[1]);
            if (if_is_buil(node1))
                is_builtin_cmd(node1);
            else
                ft_excute(node1->args, env);
        }
        else if (node1->type == CMD && node1->next != NULL && 
                (node1->next->type == RED_OUT || node1->next->type == APP))
        {
            printf("read_out\n");
            if (node1->next->next && (node1->next->next->type == RED_OUT || node1->next->next->type == APP))
            {
                printf("1+outfile\n");
                open_outfile(node1, p);
            }
            else
            {

                printf("one outfile\n");
                if (node1->next->type == APP)
                    p->outfile = open(node1->next->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    p->outfile = open(node1->next->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            close(p->end[0]);
            close(p->end[1]);
            dup2(p->outfile, STDOUT_FILENO);
            close(p->outfile);
            if (if_is_buil(node1))
                is_builtin_cmd(node1);
            else
                ft_excute(node1->args, env);
        }
        else if (node1->type == CMD && node1->next == NULL)
        {
            printf("STIN_OUT\n");
            if (if_is_buil(node1))
            {
                 printf("is_builtin\n");
                is_builtin_cmd(node1);
            }
            else
            {
                 printf("is_not_builtin\n");
                ft_excute(node1->args, env);
            }
        }
        else
        {
            printf("CMD\n");
            close(p->end[0]);
            dup2(p->end[1], STDOUT_FILENO);
            close(p->end[1]);
            if (if_is_buil(node1))
                is_builtin_cmd(node1);
            else
                ft_excute(node1->args, env);
        }
    }

	dup2(p->end[0], STDIN_FILENO);
	close(p->end[1]);
	close(p->end[0]);
    // waitpid(p->pid, &p->status, 0);
    return (p->pid);
}