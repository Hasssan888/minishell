#include "../libraries/minishell.h"

void	handle_child_exit_status(int status)
{
	int	exit_status;

	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		if (exit_status != 0)
			exit(exit_status);
	}
	else
		exit(EXIT_SUCCESS);
}

void    ft_pipe(t_command *node1, char **ev, t_pipex *p)
{
    //pid_t pid;
    //int status;
    t_command *cur = node1;
    p->flag = 0;
    if (p->count_here_doc > 0)
        open_here_doc(node1, p);
    while (cur != NULL)
    {
        if (cur->type == PIPE || cur->type == RED_IN || cur->type == HER_DOC)
        {
            if (cur->type == RED_IN)
            {
                if (cur->next->type == RED_IN )
                    open_infile(cur, p);
                else
                    p->infile = open(cur->args[0], O_RDONLY, 0644);
                p->flag = 1;
            }
            cur = cur->next;
        }
        else if ((cur->type != RED_OUT  || cur->type != APP) && cur->type == CMD)
        {
            if (cur->next && cur->next->type == HER_DOC )
            {
                printf("flag == 2\n");
                p->flag = 2;
            }
            p->r = fork_pipe(cur, ev, p);
            if (ft_strcmp(cur->args[0], "cat") != 0)
                waitpid(p->r, &p->status, 0);
            p->flag = 0;
            cur = cur->next;
        }
        else
            cur = cur->next;
    }
}

int func(t_command *list)
{
    t_pipex	pipex;

    ft_count_here_doc(list, &pipex);
    ft_count_pipe(list, &pipex);
    printf("count_pipe = %d\n",pipex.count_pipe);
    ft_count_read_out(list, &pipex);
    ft_count_read_in(list, &pipex);
    ft_pipe(list, data->envirenment, &pipex);
    while (pipex.i != -1)
	{
		pipex.i = waitpid(pipex.r, &pipex.status, 0);
        pipex.i = wait(NULL);
	}
    handle_child_exit_status(pipex.status_1);
    unlink("file_here_doc.txt");
    return (0);
}
