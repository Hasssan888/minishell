/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utiles5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 08:32:12 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/18 14:11:19 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	what_quote(t_command *list, char *arg)
{
	int		i;
	char	quote;

	i = -1;
	quote = 0;
	while (arg && arg[++i] && ft_strchr(WHITESPACES, arg[i]))
		;
	if (arg[i] == '\'' || arg[i] == '"')
	{
		quote = arg[i];
		while (arg[i] && arg[i] != quote)
			i++;
	}
	if (arg[i])
	{
		if (arg[i] == quote && quote == '\'')
			list->quoted = 1;
		else if (arg[i] == quote && quote == '"')
			list->quoted = 2;
		else
			list->quoted = 0;
	}
}

int	exist_exp_cmd(char *cmd, char *arg)
{
	return (!ft_strncmp(cmd, "export", -1) && ft_strchr(arg, '$')
		&& ft_strchr(arg, '='));
}
