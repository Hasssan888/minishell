/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utiles1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 09:09:06 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/31 13:20:42 by aelkheta         ###   ########.fr       */
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

t_env	*get_alternative_env(void)
{
	t_env	*head;
	t_env	*new;
	char	*cwd;

	head = NULL;
	new = NULL;
	new = lstnew(ft_strdup("PATH"), ft_strdup(DEFAULT_PATH));
	add_back(&head, new);
	cwd = getcwd(NULL, 0);
	new = lstnew(ft_strdup("PWD"), ft_strdup(cwd));
	g_data->current_pwd = &(new->env_value);
	add_back(&head, new);
	new = lstnew(ft_strdup("OLDPWD"), ft_strdup(cwd));
	g_data->old_pwd = &(new->env_value);
	add_back(&head, new);
	free(cwd);
	return (head);
}

void env_maker(char *env, t_env **head)
{
	t_env	*new;
	char	**splited;
	
	new = NULL;
	splited = NULL;
	splited = ft_split(env, '=');
	if (!splited)
		return ;
	if (splited[0] != NULL && splited[1] != NULL)
		new = lstnew(ft_strdup(splited[0]), ft_strdup(splited[1]));
	else if (splited[0] != NULL && !splited[1] && ft_strchr(env, '='))
		new = lstnew((ft_strdup(splited[0])), ft_strdup(""));
	else
		new = lstnew(ft_strdup(splited[0]), NULL);
	free_array(splited);
	add_back(head, new);
}

t_env	*creat_env(char **env)
{
	int		i;
	t_env	*head;

	i = 0;
	head = NULL;
	if (!env || !*env)
		return (get_alternative_env());
	else
	{
		i = -1;
		while (env[++i] != NULL)
			env_maker(env[i], &head);
	}
	return (head);
}
