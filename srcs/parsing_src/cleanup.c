/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:27:48 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/04 15:40:03 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i] != NULL)
		free(array[i++]);
	free(array);
	array = NULL;
}

void	clear_list(t_command **lst)
{
	t_command	*node;
	t_command	*ptr;

	if (!lst || !*lst)
		return ;
	node = *lst;
	while (node != NULL)
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

t_command	*free_node(t_command **node)
{
	t_command	*ptr;

	if (!*node)
		return (NULL);
	ptr = (*node)->next;
	free((*node)->value);
	free(*node);
	*node = NULL;
	return (ptr);
}
void	clear_all(t_data *data)
{
	free(data->prompt);
	free(data->new_command);
	if (data->envirenment != NULL)
		free_array(data->envirenment);
	clear_env(&data->env);
}
