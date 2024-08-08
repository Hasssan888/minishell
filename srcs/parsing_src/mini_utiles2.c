/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utiles2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:42:13 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/08 10:22:55 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

t_command	*lstlastcmd(t_command *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

char	*get_env_element(t_data *data, char *env_var)
{
	t_env	*env;

	env = data->env;
	if (!env_var)
		return (NULL);
	while (env != NULL)
	{
		if (ft_strcmp(env->env_value, env_var) == 0)
			return (ft_strdup(env->env_key));
		env = env->next;
	}
	return (ft_strdup(""));
}

void	env_maker(char *env, t_env **head)
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

t_env	*creat_env(t_data *data, char **env)
{
	int		i;
	t_env	*head;

	i = 0;
	head = NULL;
	if (!env || !*env)
		return (get_alternative_env(data));
	else
	{
		i = -1;
		while (env[++i] != NULL)
			env_maker(env[i], &head);
	}
	return (head);
}
