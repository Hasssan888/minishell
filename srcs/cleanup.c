/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:27:48 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/09 14:46:54 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	free_array(char **array)
{
	int	i;

	i = -1;
	if (!array)
		return ;
	while (array != NULL && array[++i] != NULL && array[i][0] != '\0')
		free(array[i]);
	free(array);
}
void	clear_list(t_command **lst)
{
	t_command	*node;
	t_command	*ptr;

	if (!lst)
		return ;
	node = *lst;
	while (node)
	{
		ptr = node->next;
		if (node->value != NULL)
			free(node->value);
		if (node->args != NULL)
			free_array(node->args);
		free(node);
		node = ptr;
	}
	*lst = NULL;
}

t_command	*free_node(t_command *_tokens_list)
{
	t_command	*ptr;

	if (!_tokens_list)
		return (NULL);
	ptr = _tokens_list->next;
	free(_tokens_list->value);
	free(_tokens_list);
	_tokens_list = NULL;
	return (ptr);
}
