/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:31:33 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/20 18:38:23 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

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

t_env	*sort_list(t_env *env)
{
	t_env	*env_ptr1;
	int		i;
	int		j;
	int		list_len;
	char	**sorted_array;
	char	*temp;

	i = 0;
	j = 0;
	env_ptr1 = env;
	list_len = 0;
	while (env_ptr1 != NULL)
	{
		list_len++;
		env_ptr1 = env_ptr1->next;
	}
	sorted_array = malloc((list_len + 1) * sizeof(char *));
	env_ptr1 = env;
	while (env_ptr1 != NULL)
	{
		sorted_array[i++] = env_ptr1->env_value;
		env_ptr1 = env_ptr1->next;
	}
	i = 0;
	while (i < list_len - 1)
	{
		j = 0;
		while (j < list_len - i - 1)
		{
			if (ft_strcmp(sorted_array[j], sorted_array[j + 1]) > 0)
			{
				temp = sorted_array[j];
				sorted_array[j] = sorted_array[j + 1];
				sorted_array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	sorted_array[list_len] = NULL;
	print_array(sorted_array);
	free(sorted_array);
	return (env);
}
