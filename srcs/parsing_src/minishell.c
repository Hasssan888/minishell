/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:42:13 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/17 10:46:35 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

int		g_exit_stat = 0;

void	sig_handler(int signal)
{
	(void)signal;
	write(0, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_stat = 130;
}

void	handle_signals(int mode)
{
	if (mode == SIG_INT_HANDL)
	{
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == SIG_INT_QUI_IGN)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == SIG_INT_QUI_DFL)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (mode == SIG_QUI_IGN)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
	}
}

void	shell_loop(t_data *data)
{
	char	*command;
	t_pipex	pipex;

	command = NULL;
	handle_signals(SIG_INT_HANDL);
	command = readline("minishell$-3.0 ");
	handle_signals(SIG_INT_QUI_IGN);
	while (command != NULL)
	{
		pipex.save1 = dup(STDIN_FILENO);
		pipex.save2 = dup(STDOUT_FILENO);
		if (command[0])
			add_history(command);
		parse_command(data, command);
		dup2(pipex.save1, STDIN_FILENO);
		close(pipex.save1);
		dup2(pipex.save2, STDOUT_FILENO);
		close(pipex.save2);
		handle_signals(SIG_INT_HANDL);
		command = readline("minishell$-3.0 ");
		handle_signals(SIG_INT_QUI_IGN);
	}
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	print_minishell();
	init_minishell(&data, ac, av, env);
	shell_loop(&data);
	printf("exit\n");
	clear_history();
	clear_all(&data);
	return (g_exit_stat);
}
