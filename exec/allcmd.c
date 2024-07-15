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

void    excut_butlin(t_command *node1, char **env)
{
    if (if_is_buil(node1))
    {
        printf("is_bul\n");
        is_builtin_cmd(node1);
    }
    else
        ft_excute(node1->args, env);
}

void    readout_append(t_command *node1, t_pipex *p)
{
    if (node1->next->next && (node1->next->next->type == RED_OUT || node1->next->next->type == APP))
            open_outfile(node1, p);
    else
    {
        if (node1->next->type == APP)
            p->outfile = open(node1->next->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            p->outfile = open(node1->next->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (p->outfile == -1)
        {
            printf("%s: Permission denied\n", node1->next->args[0]);
            exit(1);
        }
    }
}

void    infile(t_command *node1, char **env,t_pipex *p)
{
    if (p->infile == -1)
    {
        perror("file infile");
        exit(1);
    }
    close(p->end[0]);
    dup2(p->infile, STDIN_FILENO);
    close(p->infile);
    if (node1->next && node1->next->type == PIPE)
    {
        dup2(p->end[1], STDOUT_FILENO);
        close(p->end[1]);
    }
    else if (node1->next && (node1->next->type == RED_OUT || node1->next->type == APP))
    {
        readout_append(node1, p);
        dup2(p->outfile, STDOUT_FILENO);
        close(p->outfile);
    }
    excut_butlin(node1, env);
}

void    outfile(t_command *node1, char **env,t_pipex *p)
{
    readout_append(node1, p);
    close(p->end[0]);
    close(p->end[1]);
    dup2(p->outfile, STDOUT_FILENO);
    close(p->outfile);
    excut_butlin(node1, env);
}

void    one_here_doc(t_command *node1, char **env, t_pipex *p)
{
    close(p->end[0]);
    p->infile_here_doc = open("file_here_doc.txt", O_RDONLY, 0644);
    if (p->infile_here_doc == -1)
    {
        perror("infile_here_doc");
        exit(1);
    }
    dup2(p->infile_here_doc, 0);
    close(p->infile_here_doc);
    excut_butlin(node1, env);
}

void    heredoc_readout_app(t_command *node1, char **env,t_pipex *p)
{
    close(p->end[0]);
    readout_append(node1, p);
    p->infile_here_doc = open("file_here_doc.txt", O_RDONLY, 0644);
    if (p->infile_here_doc == -1)
    {
        perror("infile_here_doc");
        exit(1);
    }
    dup2(p->infile_here_doc, 0);
    close(p->infile_here_doc);
    dup2(p->outfile, 1);
    close(p->outfile);
    excut_butlin(node1, env);
}

void    pipe_heredoc(t_command *node1, char **env, t_pipex *p)
{
    close(p->end[0]);
    p->infile_here_doc = open("file_here_doc.txt", O_RDONLY, 0644);
    if (p->infile_here_doc == -1)
    {
        perror("infile_here_doc");
        exit(1);
    }
    dup2(p->infile_here_doc, 0);
    close(p->infile_here_doc);
    dup2(p->end[1], 1);
    close(p->end[1]);
    excut_butlin(node1, env);
}

void    child_process(t_command *node1, char **env,t_pipex *p)
{
    if (p->flag == 1)
        infile(node1, env, p);
    else if (p->flag == 2 && node1->next->type == HER_DOC && node1->next->next == NULL)
        one_here_doc(node1, env, p);
    else if (p->flag == 2 && node1->next->type == HER_DOC
                && (node1->next->next->type == RED_OUT || node1->next->next->type == APP))
        heredoc_readout_app(node1, env, p);
    else if (p->flag == 2)
        pipe_heredoc(node1 ,env, p);
    else if (node1->type == CMD && node1->next != NULL && 
            (node1->next->type == RED_OUT || node1->next->type == APP))
        outfile(node1, env, p);
    else if (node1->type == CMD && node1->next == NULL)
        excut_butlin(node1, env);
    else
    {
        close(p->end[0]);
        dup2(p->end[1], STDOUT_FILENO);
        close(p->end[1]);
        excut_butlin(node1, env);
    }
}

pid_t	fork_pipe(t_command *node1, char **env, t_pipex *p)
{
	if (pipe(p->end) == -1)
		ft_error_2();
	p->pid = fork();
	if (p->pid == -1)
		ft_error_2();
	else if (p->pid == 0)
        child_process(node1, env, p);
	dup2(p->end[0], STDIN_FILENO);
	close(p->end[1]);
	close(p->end[0]);
    if (ft_strcmp(node1->args[0] , "cat") != 0)
    {
        wait(&p->status);
        data->exit_status  = WEXITSTATUS(p->status);
    }
    return (p->pid);
}