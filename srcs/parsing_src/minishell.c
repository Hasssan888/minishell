/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:42:13 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/08 14:39:17 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

int		g_exit_stat = 0;

void	panic(char *error_str)
{
	perror(error_str);
	exit(EXIT_FAILURE);
}

void	sig_handler(int signal)
{
	(void)signal;
	rl_replace_line("", 0);
	printf("\n%s", "minishell$ ");
	g_exit_stat = 130;
}

void	handle_signals(int mode)
{
	if (mode == 1)
	{
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == 2)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == 3)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (mode == 4)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
	}
}

int	main(int ac, char **av, char **env)
{
	t_data	data;
	t_pipex	pipex;
	char	*command;

	print_minishell();
	init_minishell(&data, ac, av, env);
	handle_signals(1);
	command = readline("minishell$ ");
	handle_signals(2);
	while (command != NULL)
	{
		pipex.save1 = dup(STDIN_FILENO);
		add_history(command);
		parse_command(&data, command);
		dup2(pipex.save1, STDIN_FILENO);
		close(pipex.save1);
		handle_signals(1);
		command = readline("minishell$ ");
		handle_signals(2);
	}
	printf("exit\n");
	clear_history();
	clear_all(&data);
	return (g_exit_stat);
}
