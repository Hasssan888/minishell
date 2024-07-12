/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utiles1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 09:09:06 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/12 09:40:32 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	add_back_list(t_command **lst, t_command *new)
{
	t_command	*temp;

	if (!lst || !new)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		temp = *lst;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

t_command	*new_node(int type, char *value)
{
	t_command	*new_node;

	new_node = (t_command *)malloc(sizeof(t_command));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

void	add_back(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		temp = *lst;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

t_env	*lstnew(char *content)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->value = content;
	new_node->next = NULL;
	return (new_node);
}

t_env	*creat_env(char **env)
{
	t_env	*head;
	t_env	*new;
	int		i;

	i = -1;
	head = NULL;
	data->old_pwd = NULL;
	data->current_pwd = NULL;
	while (env[++i] != NULL)
	{
		new = lstnew(ft_strdup(env[i]));
		if (!strncmp(new->value, "PWD", 3))
			data->current_pwd = &(new->value);
		else if (!strncmp(new->value, "OLDPWD", 6))
			data->old_pwd = &(new->value);
		add_back(&head, new);
	}
	return (head);
}
