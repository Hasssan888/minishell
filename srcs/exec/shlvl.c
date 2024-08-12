/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 11:49:23 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/12 11:49:39 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	**get_env_add_ele(char **env)
{
	int	i;

	i = -1;
	while (env && env[++i])
	{
		if (!ft_strncmp(env[i], "SHLVL", ft_strlen("SHLVL")))
			return (&env[i]);
	}
	return (NULL);
}

void	shlvl_update(t_data *data)
{
	int		a;
	char	**ptr;
	t_env	*env_ptr;

	ptr = NULL;
	env_ptr = NULL;
	env_ptr = get_env_ele_ptr(data->env, "SHLVL");
	if (env_ptr && env_ptr->env_key)
	{
		a = ft_atoi(env_ptr->env_key) + 1;
		ptr = get_env_add_ele(data->envirenment);
		if (ptr && *ptr)
		{
			free(*ptr);
			*ptr = ft_strjoin(ft_strdup("SHLVL="), ft_itoa(a));
		}
	}
}
