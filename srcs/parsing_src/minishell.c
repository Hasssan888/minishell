/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:42:13 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/04 20:15:34 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

int		g_exit_stat = 0;

void	sig_handler(int signal)
{
	char	*prompt;

	prompt = get_prompt();
	if (signal == SIGINT)
	{
		rl_replace_line("", 0); // Clear the current input line
		printf("\n%s", prompt);
		g_exit_stat = 130;
	}
	else if (signal == SIGQUIT)
		g_exit_stat = 131;
	free(prompt);
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

char	*get_prompt(void)
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
	data->env = creat_env(data, env);
	data->envirenment = env_to_array_(data->env);
	data->syntax_error = false;
	data->prompt = get_prompt();
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

int	main(int ac, char **av, char **env)
{
	t_data	data;
	t_pipex	pipex;
	char	*command;

	print_minishell();
	init_minishell(&data, ac, av, env);
	handle_signals(1);
	command = readline(data.prompt);
	handle_signals(2);
	while (command != NULL)
	{
		pipex.save1 = dup(STDIN_FILENO);
		add_history(command);
		parse_command(&data, command);
		dup2(pipex.save1, STDIN_FILENO);
		close(pipex.save1);
		handle_signals(1);
		command = readline(data.prompt);
		handle_signals(2);
	}
	printf("exit\n");
	clear_history();
	clear_all(&data);
	return (g_exit_stat);
}
