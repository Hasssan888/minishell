#include "../libraries/minishell.h"

// void    ft_pipe(t_command *node1, char **ev, t_pipex *p)
// {
//     //pid_t pid;
//     //int status;
//     t_command *cur = node1;
//     p->flag = 0;

//     while (cur != NULL)
//     {
//         if (cur->type == PIPE || cur->type == RED_IN)
//         {
//             if (cur->type == RED_IN)
//                 p->flag = 1;
//             cur = cur->next;
//         }
//         else if (cur->type != RED_OUT && cur->type == CMD)
//         {
            
//             fork_pipe(cur, ev, p);
//             //waitpid(pid, &status, 0);
//             p->flag = 0;
//             cur = cur->next;
//         }
//         else
//             cur = cur->next;
//     }
// }

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
                p->flag = 1;
            cur = cur->next;
        }
        else if ((cur->type != RED_OUT  || cur->type != APP) && cur->type == CMD)
        {
            if (ft_strcmp(cur->args[0], "ls") != 0 && cur->next && cur->next->type == HER_DOC )
            {
                printf("flag == 2\n");
                p->flag = 2;
            }
            fork_pipe(cur, ev, p);
            //waitpid(pid, &status, 0);
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
    if (list->type  == HER_DOC && list->next == NULL)
    {
        printf("one here_doc\n");
        open_here_doc(list, &pipex);
        exit(1);
    }
    ft_count_pipe(list, &pipex);
    printf("count_pipe = %d\n",pipex.count_pipe);
    ft_count_read_out(list, &pipex);
    ft_count_read_in(list, &pipex);
    if (pipex.count_read_in)
        open_infile(list, &pipex);
    if (pipex.count_read_out)
        open_outfile(list, &pipex);
    //open_herdoc(node1, &pipex);
    if (pipex.count_pipe == 0 && pipex.count_read_out == 0)
    {
        if (pipex.count_here_doc == 0 && pipex.count_read_in == 0)
        {
            int pp = fork();
            if (pp == 0)
                ft_excute(list->args, data->envirenment);
            else if (pp > 0)
                wait(NULL);
        }
        else if (pipex.count_here_doc == 0 && pipex.count_read_in != 0)
        {
            printf("rin\n");
            while (list)
            {
                if (list->type == RED_IN)
                    list = list->next;
                else
                    break;
            }
            int pp = fork();
            if (pp == 0)
            {
                dup2(pipex.infile, 0);
                close(pipex.infile);
                ft_excute(list->args, data->envirenment);
            }
            else
                wait(NULL);
        }
        else
        {
            printf("list->args[0] == %s\n", list->args[0]);
            printf("not\n");
            open_here_doc(list, &pipex);
            pipex.infile_here_doc = open("file_here_doc.txt", O_RDONLY, 0644);
            dup2(pipex.infile_here_doc, 0);
            close(pipex.infile_here_doc);
            pid_t pid = fork();
            if (pid == 0)
                ft_excute(list->args, data->envirenment);
            else
                wait(NULL);
            unlink("file_here_doc.txt");
            //wait(NULL);
        }

    }
    else if (pipex.count_pipe == 0 && pipex.count_read_out != 0)
    {
        if (pipex.count_here_doc != 0)
        {
            open_here_doc(list, &pipex);
            ft_onecmd(list, data->envirenment, &pipex);
           // wait(NULL);
            unlink("file_here_doc.txt");
        }
        else
        {
            int pid = fork();
            if (pid == 0)
                ft_onecmd(list, data->envirenment, &pipex);
            else
                wait(NULL);
        }
    }
    else //if have pipe
    {
        printf("existing pipe\n");
        ft_pipe(list, data->envirenment, &pipex);
        unlink("file_here_doc.txt");
    }
    return (0);
}
