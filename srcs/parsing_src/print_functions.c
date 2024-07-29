/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:37:47 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/29 09:27:42 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	print_minishell(void)
{
	printf("\n");
	printf("███╗   ███╗██╗███╗   ██╗██╗");printf(COLOR"███████╗██╗  ██╗███████╗██╗     ██╗\n"reset);
	printf("████╗ ████║██║████╗  ██║██║");printf(COLOR"██╔════╝██║  ██║██╔════╝██║     ██║\n"reset);
	printf("██╔████╔██║██║██╔██╗ ██║██║");printf(COLOR"███████╗███████║█████╗  ██║     ██║\n"reset);
	printf("██║╚██╔╝██║██║██║╚██╗██║██║");printf(COLOR"╚════██║██╔══██║██╔══╝  ██║     ██║\n"reset);
	printf("██║ ╚═╝ ██║██║██║ ╚████║██║");printf(COLOR"███████║██║  ██║███████╗███████╗███");
	printf(COLOR"████╗\n"reset);
	printf("╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝");printf(COLOR"╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══");
	printf("════╝\n"reset);
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

void	print_type(int type)
{
	if (type == -1)
		printf("│ type: ----------- ERROR   │\n└───────────────────────────┘\n");
	else if (type == TOKEN)
		printf("│ type: ----------- TOKEN   │\n└───────────────────────────┘\n");
	else if (type == RED_OUT)
		printf("│ type: ----------- RED_OUT │\n└───────────────────────────┘\n");
	else if (type == RED_IN)
		printf("│ type: ----------- RED_IN  │\n└───────────────────────────┘\n");
	else if (type == APP)
		printf("│ type: ----------- APP     │\n└───────────────────────────┘\n");
	else if (type == HER_DOC)
		printf("│ type: ----------- HER_DOC │\n└───────────────────────────┘\n");
	else if (type == PIPE)
		printf("│ type: ----------- PIPE    │\n└───────────────────────────┘\n\n");
}

void	print_args(char **args)
{
	int	i;

	i = 0;
	while (args != NULL && *args)
	{
		printf("	┌───────────────────────────┐\n");
		printf("	│ arg[%d]: --------- [%s]   \n", i++, *args++);
		printf("	└───────────────────────────┘\n");
	}
}

void	print_list(t_command *table)
{
	if (!table)
		return ;
	while (table != NULL)
	{
		printf("┌───────────────────────────┐\n");
		if (table->value != NULL)
		{
			printf("│ token: ---------- [%s]    \n", table->value);
			print_type(table->type);
			print_args(table->args);
		}
		table = table->next;
	}
}
