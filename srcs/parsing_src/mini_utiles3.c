/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utiles3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:10:40 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/09 11:36:49 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	print_list(t_command *table)
{
	if (!table)
		return ;
	while (table != NULL)
	{
		if (table->value != NULL)
		{
			printf("┌───────────────────────────┐\n");
			printf("│ token: ---------- [%s]    \n", table->value);
			print_type(table->type);
			print_args(table->args);
		}
		table = table->next;
	}
}

void	init_minishell(t_data *data, int ac, char **av, char **env)
{
	ft_bzero(data, sizeof(t_data));
	data->av = av;
	data->ac = ac;
	data->env = creat_env(data, env);
	data->envirenment = env_to_array_(data->env);
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

int	get_token_type(char *token)
{
	if (token[0] == '|' && !token[1])
		return (PIPE);
	else if (token[0] == '>' && !token[1])
		return (RED_OUT);
	else if (token[0] == '<' && !token[1])
		return (RED_IN);
	else if (token[0] == '>' && token[1] == '>' && !token[2])
		return (APP);
	else if (token[0] == '<' && token[1] == '<' && !token[2])
		return (HER_DOC);
	else if ((ft_strchr("|&", token[0]) && ft_strlen(token) > 1)
		|| (ft_strchr("<>", token[0]) && ft_strlen(token) > 2))
		return (-1);
	else
		return (TOKEN);
}
