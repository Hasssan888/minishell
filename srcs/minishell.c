/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:42:13 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/22 16:08:31 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

t_data	*data = NULL;

void	sig_handler(int signal)
{
	char	*prompt;

	prompt = get_prompt();
	if (signal == SIGQUIT)
		return ;
	if (signal == SIGINT)
		printf("\n%s", prompt);
	free(prompt);
}

char	*get_prompt(void)
{
	char	*prompt1;
	char	*prompt2;
	char	*final_prompt;

	prompt1 = ft_strjoin(ft_strdup(BWHT), getcwd(NULL, 0));
	prompt2 = ft_strjoin(ft_strdup(BBLU "┌──(aziz㉿aelkheta)-[" COLOR_RESET),
			prompt1);
	final_prompt = ft_strjoin(prompt2, ft_strdup(BBLU "]\n└─$ " COLOR_RESET));
	return (final_prompt);
}

void	init_minishell(int ac, char **av, char **env)
{
	data->redirect = 0;
	data->expanded = NULL;
	data->list = NULL;
	data->old_pwd = NULL;
	data->current_pwd = NULL;
	data->av = av;
	data->ac = ac;
	data->exit_status = 0;
	data->env = creat_env(env);
	data->envirenment = env;
	data->syntax_error = false;
	data->prompt = get_prompt();
	data->new_command = NULL;
}

void clear_env(t_env **env)
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
void clear_all()
{
	clear_env(&data->env);
	free(data->prompt);
	free(data->new_command);
	free(data);
}
int	main(int ac, char **av, char **env)
{
	char	*command;
	t_pipex	pipex;

	data = (t_data *)malloc(sizeof(t_data));
	init_minishell(ac, av, env);
	// print_minishell();
	signal(SIGQUIT, sig_handler);
	signal(SIGINT, sig_handler);
	command = readline(data->prompt);
	while (command != NULL)
	{
		pipex.save1 = dup(STDIN_FILENO);
		add_history(command);
		parse_command(command);
		dup2(pipex.save1, STDIN_FILENO);
		close(pipex.save1);
		command = readline(data->prompt);
	}
	clear_history();
	clear_all();
	return (0);
}
