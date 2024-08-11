/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:02:02 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/08 22:10:17 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	echo_it(char **cmd, int i)
{
	while (cmd[i] != NULL)
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1] != NULL)
			printf(" ");
		i++;
	}
}

bool	check_echo_options(char *cmd)
{
	int		j;
	bool	flag;

	j = 0;
	flag = false;
	if (!cmd || !cmd[0])
		return (flag);
	if (cmd[j] == '-')
		while (cmd[++j] == 'n')
			;
	if (!cmd[j])
		flag = true;
	return (flag);
}

int	echo(char **cmd)
{
	int		i;
	bool	flag;

	i = 0;
	flag = false;
	if (check_echo_options(cmd[i]))
	{
		flag = true;
		while (check_echo_options(cmd[++i]))
			;
	}
	echo_it(cmd, i);
	if (!flag)
		printf("\n");
	g_exit_stat = 0;
	return (0);
}
