/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:42:13 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/03 13:26:40 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

int	g_exit_stat = 0;

void	sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		// printf("\n%s", data->prompt);
		rl_replace_line("", 0);
		rl_on_new_line();
        rl_redisplay();
		g_exit_stat = 130;
	}
	else if (signal == SIGQUIT)
		g_exit_stat = 131;
}

char	*get_prompt()
{
	char	*prompt1;
	char	*final_prompt;

	prompt1 = ft_strjoin(ft_strdup("┌──(aziz㉿aelkheta)-["), getcwd(NULL, 0));
	final_prompt = ft_strjoin(prompt1, ft_strdup("]\n└─$ "));
	return (final_prompt);
}

void	init_minishell(t_data *data, int ac, char **av, char **env)
{
	ft_bzero(data, sizeof(t_data));
	data->av = av;
	data->ac = ac;
	data->redirect = 0;
	data->ignore_sig = 1;
	data->list = NULL;
	// data->expanded = NULL;
	// data->old_pwd = NULL;
	// data->current_pwd = NULL;
	// data->envirenment = NULL;
	// data->exit_status = 0;
	g_exit_stat = 0;
	data->env = creat_env(data, env);
	data->envirenment = env_to_array_(data->env);
	data->syntax_error = false;
	data->prompt = get_prompt();
	// data->new_command = NULL;
}

void	clear_env(t_env **env)
{
	t_env	*node;
	t_env	*ptr;

	if (!env)
		return ;
	node = *env;
	while (node != NULL)
	{
		ptr = node->next;
		if (node->env_value != NULL)
			free(node->env_value);
		if (node->env_key != NULL)
			free(node->env_key);
		free(node);
		node = ptr;
	}
	*env = NULL;
}

void	clear_all(t_data *data)
{
	free(data->prompt);
	free(data->new_command);
	if (data->envirenment != NULL)
		free_array(data->envirenment);
	clear_env(&data->env);
	// free(data);
}

void	ignr_signals(void)
{
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	// signal(SIGTSTP, SIG_IGN);
	// signal(SIGTSTP, SIG_IGN);
}

int	main(int ac, char **av, char **env)
{
	char	*command;
	t_data 	data;
	t_pipex	pipex;
	// data = (t_data *)malloc(sizeof(t_data));
	init_minishell(&data, ac, av, env);
	// print_minishell();
	ignr_signals();
	command = readline(data.prompt);
	signal(SIGINT, SIG_IGN);
	while (command != NULL)
	{
		pipex.save1 = dup(STDIN_FILENO);
		add_history(command);
		parse_command(&data, command);
		dup2(pipex.save1, STDIN_FILENO);
		close(pipex.save1);
		ignr_signals();
		command = readline(data.prompt);
		signal(SIGINT, SIG_IGN);
	}
	printf("exit\n");
	clear_history();
	clear_all(&data);
	return (g_exit_stat);
}
