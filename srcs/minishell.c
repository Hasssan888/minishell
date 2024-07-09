/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:42:13 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/09 14:45:21 by aelkheta         ###   ########.fr       */
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
	char	*prompt3;
	char	*final_prompt;

	prompt1 = BBLU "┌──(aziz㉿aelkheta)-[" COLOR_RESET;
	prompt2 = getcwd(NULL, 0);
	prompt3 = ft_strjoin(BWHT, prompt2);
	free(prompt2);
	prompt1 = ft_strjoin(prompt1, prompt3);
	free(prompt3);
	final_prompt = ft_strjoin(prompt1, BBLU "]\n└─$ " COLOR_RESET);
	free(prompt1);
	return (final_prompt);
}

void	init_minishell(int ac, char **av, char **env)
{
	data->ac = ac;
	data->env = creat_env(env);
	data->envirenment = env;
	data->av = av;
	data->syntax_error = false;
	data->prompt = get_prompt();
	data->new_command = NULL;
}

int	main(int ac, char **av, char **env)
{
	char	*command;
	t_pipex	pipex;

	data = (t_data *)malloc(sizeof(t_data));
	init_minishell(ac, av, env);
	print_minishell();
	signal(SIGQUIT, sig_handler);
	// signal(SIGINT, sig_handler);
	command = readline(data->prompt);
	while (command != NULL)
	{
		pipex.save1 = dup(STDIN_FILENO);
		add_history(command);
		parse_command(command);
		dup2(pipex.save1, STDIN_FILENO);
		close(pipex.save1);
		// free(command);
		command = readline(data->prompt);
	}
	clear_history();
	free(data->new_command);
	free(data->prompt);
	return (0);
}
