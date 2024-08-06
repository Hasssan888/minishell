/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utiles1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 09:09:06 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/06 10:21:02 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

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
	new_node->args = NULL;
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

t_env	*lstnew(char *env_value, char *env_key)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->env_value = env_value;
	new_node->env_key = env_key;
	new_node->next = NULL;
	return (new_node);
}

t_env	*get_alternative_env(t_data *data)
{
	t_env	*head;
	t_env	*new;
	char	*cwd;

	head = NULL;
	new = NULL;
	cwd = getcwd(NULL, 0);
	new = lstnew(ft_strdup("PWD"), ft_strdup(cwd));
	data->current_pwd = &(new->env_value);
	add_back(&head, new);
	new = lstnew(ft_strdup("OLDPWD"), ft_strdup(cwd));
	data->old_pwd = &(new->env_value);
	add_back(&head, new);
	free(cwd);
	return (head);
}
