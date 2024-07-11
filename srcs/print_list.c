/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 09:57:50 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/10 09:57:55 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// void	print_type(int type)
// {
// 	if (type == -1)
// 		printf("| type: ----------- ERROR   |\n+---------------------------+\n");
// 	else if (type == TOKEN)
// 		printf("| type: ----------- TOKEN   |\n+---------------------------+\n");
// 	else if (type == RED_OUT)
// 		printf("| type: ----------- RED_OUT |\n+---------------------------+\n");
// 	else if (type == RED_IN)
// 		printf("| type: ----------- RED_IN  |\n+---------------------------+\n");
// 	else if (type == APP)
// 		printf("| type: ----------- APP     |\n+---------------------------+\n");
// 	else if (type == HER_DOC)
// 		printf("| type: ----------- HER_DOC |\n+---------------------------+\n");
// 	else if (type == PIPE)
// 		printf("| type: ----------- PIPE    |\n+---------------------------+\n\n");
// 	else if (type == LIST)
// 		printf("| type: ----------- LIST    |\n+---------------------------+\n\n");
// 	else if (type == BACK)
// 		printf("| type: ----------- BACK    |\n+---------------------------+\n\n");
// 	else if (type == ARG)
// 		printf("| type: ----------- ARG     |\n+---------------------------+\n\n");
// 	else if (type == OR_OP)
// 		printf("| type: ----------- OR_OP   |\n+---------------------------+\n\n");
// 	else if (type == AND_OP)
// 		printf("| type: ----------- AND_OP  |\n+---------------------------+\n\n");
// 	else if (type == FLE)
// 		printf("| type: ----------- FLE     |\n+---------------------------+\n\n");
// }

// void	print_args(char **args)
// {
// 	int	i;

// 	i = 0;
// 	while (args != NULL && *args)
// 	{
// 		printf("	+---------------------------+\n");
// 		printf("	| arg[%d]: --------- [%s]\n", i++, *args++);
// 		printf("	+---------------------------+\n");
// 	}
// }

// void	print_list(t_command *table)
// {
// 	if (!table)
// 		return ;
// 	while (table != NULL)
// 	{
// 		printf("+---------------------------+\n");
// 		printf("| token: ---------- [%s]\n", table->value);
// 		print_type(table->type);
// 		print_args(table->args);
// 		table = table->next;
// 	}
// }