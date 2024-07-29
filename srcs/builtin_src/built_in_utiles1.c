/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:31:33 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/29 09:34:33 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	*get_word_(char *line, char *del)
{
	int		i;
	char	*word;

	i = 0;
	if (!line)
		return (NULL);
	while (line[i] && !ft_strchr(del, line[i]))
		i++;
	word = malloc((i + 1) * sizeof(char));
	ft_strlcpy(word, line, i + 1);
	return (word);
}

t_env	*get_env_ele_ptr(char *env_val)
{
	t_env	*env;

	env = data->env;
	while (env != NULL)
	{
		if (ft_strncmp(env->env_value, env_val, ft_strlen(env_val)) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	print_array(char **array)
{
	int	i;

	i = 0;
	if (!array || !array[0])
		return ;
	while (array != NULL && array[i] != NULL)
		printf("%s\n", array[i++]);
}
