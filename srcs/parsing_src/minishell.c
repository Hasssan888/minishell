/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:42:13 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/03 09:13:44 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

t_data	*g_data = NULL;

void	sig_handler(int signal)
{
	if (!g_data->ignore_sig)
		printf("\n");
	if (signal == SIGINT && g_data->ignore_sig)
	{
		printf("\n%s", g_data->prompt);
		g_data->exit_status = 130;
	}
	else if (signal == SIGQUIT)
		g_data->exit_status = 131;
}

char	*get_prompt(void)
{
	char	*prompt1;
	char	*final_prompt;

	prompt1 = ft_strjoin(ft_strdup("┌──(aziz㉿aelkheta)-["), getcwd(NULL, 0));
	final_prompt = ft_strjoin(prompt1, ft_strdup("]\n└─$ "));
	return (final_prompt);
}

void	init_minishell(int ac, char **av, char **env)
{
	ft_bzero(g_data, sizeof(t_data));
	g_data->av = av;
	g_data->ac = ac;
	g_data->redirect = 0;
	g_data->ignore_sig = 1;
	g_data->list = NULL;
	// g_data->expanded = NULL;
	// g_data->old_pwd = NULL;
	// g_data->current_pwd = NULL;
	// g_data->envirenment = NULL;
	g_data->exit_status = 0;
	g_data->env = creat_env(env);
	g_data->envirenment = env_to_array_(g_data->env);
	g_data->syntax_error = false;
	g_data->prompt = get_prompt();
	// g_data->new_command = NULL;
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

void	clear_all(void)
{
	free(g_data->prompt);
	free(g_data->new_command);
	if (g_data->envirenment != NULL)
		free_array(g_data->envirenment);
	clear_env(&g_data->env);
	free(g_data);
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
	t_pipex	pipex;

	g_data = (t_data *)malloc(sizeof(t_data));
	init_minishell(ac, av, env);
	print_minishell();
	ignr_signals();
	command = readline(g_data->prompt);
	signal(SIGINT, SIG_IGN);
	while (command != NULL)
	{
		pipex.save1 = dup(STDIN_FILENO);
		add_history(command);
		parse_command(command);
		dup2(pipex.save1, STDIN_FILENO);
		close(pipex.save1);
		ignr_signals();
		command = readline(g_data->prompt);
		signal(SIGINT, SIG_IGN);
	}
	printf("exit\n");
	clear_history();
	clear_all();
	return (0);
}
