#include "../libraries/minishell.h"

// void	fork_pipe(t_command *node1, char **env, t_pipex *p)
// {
// 	pid_t	pid;
// 	if (pipe(p->end) == -1)
// 		ft_error_2();
// 	pid = fork();
// 	if (pid == -1)
// 		ft_error_2();
// 	else if (pid == 0)
//     {
//         if (p->flag == 1)
//         {
//             printf("read_in\n");
//             close(p->end[0]);
//             dup2(p->infile, STDIN_FILENO);
//             close(p->infile);
//             dup2(p->end[1], STDOUT_FILENO);
//             close(p->end[1]);
//             ft_excute(node1->args, env);
//         }
//         // else if (p->flag == 2 && node1->next->type == HER_DOC && node1->next->next == NULL)
//         // {
//         //     printf("node1->type == %s\n", node1->args[0]);
//         //     printf("HEREDOC\n");
//         //     close(p->end[0]);
//         //     p->infile_here_doc = open("file_here_doc.txt", O_RDONLY, 0644);
//         //     dup2(p->infile_here_doc, 0);
//         //     close(p->infile_here_doc);
//         //     // dup2(p->end[1], 1);
//         //     // close(p->end[1]);
//         //     ft_excute(node1->args, env);
//         // }
//         // else if (p->flag == 2 && node1->next->type == HER_DOC && node1->next->next->type == RED_OUT)
//         // {
//         //     printf("node1->type == %d\n", node1->type);
//         //     printf("HEREDOC + RED_OUT\n");
//         //     close(p->end[0]);
//         //     p->infile_here_doc = open("file_here_doc.txt", O_RDONLY, 0644);
//         //     dup2(p->infile_here_doc, 0);
//         //     close(p->infile_here_doc);
//         //     dup2(p->outfile, 1);
//         //     close(p->outfile);
//         //     ft_excute(node1->args, env);
//         // }
//         // else if (p->flag == 2)
//         // {
//         //     printf("node1->type == %s\n", node1->args[0]);
//         //     printf("HEREDOC-3\n");
//         //     close(p->end[0]);
//         //     p->infile_here_doc = open("file_here_doc.txt", O_RDONLY, 0644);
//         //     dup2(p->infile_here_doc, 0);
//         //     close(p->infile_here_doc);
//         //     dup2(p->end[1], 1);
//         //     close(p->end[1]);
//         //     ft_excute(node1->args, env);
//         // }
//         else if (node1->type == CMD && node1->next != NULL && node1->next->type == RED_OUT)
//         {
//             printf("read_out\n");
//             close(p->end[0]);
//             close(p->end[1]);
//             dup2(p->outfile, STDOUT_FILENO);
//             close(p->outfile);
//             ft_excute(node1->args, env);
//         }
//         else if (node1->type == CMD && node1->next == NULL)
//         {
//             printf("STIN_OUT\n");
//             ft_excute(node1->args, env);
//         }
//         else
//         {
//             printf("CMD\n");
//             close(p->end[0]);
//             dup2(p->end[1], STDOUT_FILENO);
//             close(p->end[1]);
//             ft_excute(node1->args, env);
//         }
//     }
//         // all_cmd(node1, env, pipex);
// 	close(p->end[1]);
// 	dup2(p->end[0], STDIN_FILENO);
// 	close(p->end[0]);
//     wait(NULL);
// 	//return (pid);
// }

void	fork_pipe(t_command *node1, char **env, t_pipex *p)
{
	pid_t	pid;
	if (pipe(p->end) == -1)
		ft_error_2();
	pid = fork();
	if (pid == -1)
		ft_error_2();
	else if (pid == 0)
    {
        if (p->flag == 1)
        {
            printf("read_in\n");
            close(p->end[0]);
            dup2(p->infile, STDIN_FILENO);
            close(p->infile);
            dup2(p->end[1], STDOUT_FILENO);
            close(p->end[1]);
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
            ft_excute(node1->args, env);
        }
        else if (p->flag == 2 && node1->next->type == HER_DOC
                    && (node1->next->next->type == RED_OUT || node1->next->next->type == APP))
        {
            printf("node1->type == %d\n", node1->type);
            printf("HEREDOC + RED_OUT\n");
            close(p->end[0]);
            p->infile_here_doc = open("file_here_doc.txt", O_RDONLY, 0644);
            dup2(p->infile_here_doc, 0);
            close(p->infile_here_doc);
            dup2(p->outfile, 1);
            close(p->outfile);
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
            ft_excute(node1->args, env);
        }
        else if (node1->type == CMD && node1->next != NULL && 
                (node1->next->type == RED_OUT || node1->next->type == APP))
        {
            printf("read_out\n");
            close(p->end[0]);
            close(p->end[1]);
            dup2(p->outfile, STDOUT_FILENO);
            close(p->outfile);
            ft_excute(node1->args, env);
        }
        else if (node1->type == CMD && node1->next == NULL)
        {
            printf("STIN_OUT\n");
            ft_excute(node1->args, env);
        }
        else
        {
            printf("CMD\n");
            close(p->end[0]);
            dup2(p->end[1], STDOUT_FILENO);
            close(p->end[1]);
            ft_excute(node1->args, env);
        }
    }
        // all_cmd(node1, env, pipex);
	close(p->end[1]);
	dup2(p->end[0], STDIN_FILENO);
	close(p->end[0]);
    wait(NULL);
	//return (pid);
}