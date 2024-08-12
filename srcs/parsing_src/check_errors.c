/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 11:56:30 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/12 12:02:44 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	check_synt_err(char *str)
{
	if (str && str[0] == '>')
		ft_putstr_fd("minishell: syntax error near unexpected token `>'\n", 2);
	if (str && str[0] == '|')
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	if (str && str[0] == '<')
		ft_putstr_fd("minishell: syntax error near unexpected token `<'\n", 2);
}
