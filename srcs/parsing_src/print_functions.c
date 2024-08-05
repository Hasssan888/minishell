/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:37:47 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/05 12:10:19 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	print_minishell(void)
{
	printf("\n");
	printf("███╗   ███╗██╗███╗   ██╗██╗");
	printf(COLOR "███████╗██╗  ██╗███████╗██╗     ██╗\n" reset);
	printf("████╗ ████║██║████╗  ██║██║");
	printf(COLOR "██╔════╝██║  ██║██╔════╝██║     ██║\n" reset);
	printf("██╔████╔██║██║██╔██╗ ██║██║");
	printf(COLOR "███████╗███████║█████╗  ██║     ██║\n" reset);
	printf("██║╚██╔╝██║██║██║╚██╗██║██║");
	printf(COLOR "╚════██║██╔══██║██╔══╝  ██║     ██║\n" reset);
	printf("██║ ╚═╝ ██║██║██║ ╚████║██║");
	printf(COLOR "███████║██║  ██║███████╗███████╗███");
	printf(COLOR "████╗\n" reset);
	printf("╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝");
	printf(COLOR "╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══");
	printf("════╝\n" reset);
	printf("\n");
}

void	print_prompt(void)
{
	char	*prompt;
	char	*output_color;
	char	*reset_color;

	prompt = "┌──(aziz㉿hostname)-[~/Desktop/minishell]\n└─$ ";
	output_color = GRN;
	reset_color = CRESET;
	write(1, output_color, ft_strlen(output_color));
	write(1, prompt, ft_strlen(prompt));
	write(1, reset_color, ft_strlen(reset_color));
}

void	print_str2(char *str1, char *str2)
{
	printf("│ %s: ----------- %s   	", str1, str2);
	printf("\n└───────────────────────────┘\n");
}

void	print_type(int type)
{
	if (type == -1)
		print_str2("type", "ERROR");
	else if (type == TOKEN)
		print_str2("type", "TOKEN");
	else if (type == RED_OUT)
		print_str2("type", "RED_OUT");
	else if (type == RED_IN)
		print_str2("type", "RED_IN");
	else if (type == APP)
		print_str2("type", "APP");
	else if (type == HER_DOC)
		print_str2("type", "HER_DOC");
	else if (type == PIPE)
		print_str2("type", "PIPE");
}

void	print_args(char **args)
{
	int	i;

	i = 0;
	while (args != NULL && *args)
	{
		printf("	┌───────────────────────────┐\n");
		printf("  ─────>│ arg[%d]: --------- [%s]\n", i++, *args++);
		printf("	└───────────────────────────┘\n");
	}
}
