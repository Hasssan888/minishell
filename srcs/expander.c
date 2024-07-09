/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:52:20 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/09 18:50:55 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*get_var(char *env_var, int *i)
{
	int		k;
	char	*env_val;

	k = 0;
	if (!env_var || !env_var[0])
		return (NULL);
	while (env_var[k] && env_var[k] != ' ' && env_var[k] != '$'
		&& env_var[k] != ')')
		k++;
	*i += k;
	env_val = malloc((k + 1) * sizeof(char));
	ft_strlcpy(env_val, env_var, k + 1);
	return (env_val);
}

char	*unquote_arg(t_command *list, char *arg, int j, int k)
{
	char	quote;
	char	*argument;

	argument = ft_calloc(ft_strlen(arg) + 1, sizeof(char));
	while (arg[j])
	{
		quote = arg[j];
		if (arg[j] == '\'' || arg[j] == '"')
		{
			quote = arg[j++];
			while (arg[j] && arg[j] != quote)
				argument[k++] = arg[j++];
			j++;
		}
		else
			argument[k++] = arg[j++];
		if (arg[0] == quote && quote == '\'')
			list->quoted = 1;
		else if (arg[0] == quote && quote == '"')
			list->quoted = 2;
	}
	free(arg);
	return (argument);
}

char	*ft_strnstr_l(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (*little == '\0')
		return ((char *)big);
	while (big[i] && i < len)
	{
		j = 0;
		while (big[i + j] == little[j] && little[j] && (i + j) < len)
			j++;
		if (little[j] == '\0')
			return ((char *)&big[i + j]);
		i++;
	}
	return (NULL);
}

char	*get_word(char *argument, int *i)
{
	char	*str;
	int		j;

	str = NULL;
	j = *i;
	while (argument[*i] && argument[*i] != '$')
		(*i)++;
	str = malloc((*i - j + 1) * sizeof(char));
	ft_strlcpy(str, &argument[j], (*i - j + 1));
	return (str);
}
int get_expanded(char *argument, int *i)
{
	data->str1 = get_var(&argument[++(*i)], i);
	data->str2 = get_env_element(data->str1);
	if (!data->str2)
		return (0);
	data->expanded = ft_strjoin(data->expanded, data->str2);
	free(data->str2);
	return (1);
}

char	*expand_vars(char *argument, int i)
{
	data->expanded = "";
	while (argument[i])
	{
		if (argument[i] == '$')
		{
			if (!argument[i + 1])
			{
				free(argument);
				return (ft_strdup("$"));
			}
			else if (argument[i + 1] == '?')
			{
				i += 2;
				data->expanded = ft_strjoin(data->expanded, ft_strdup("$?"));
				// continue ;
			}
			else if (!get_expanded(argument, &i))
				return (NULL);
		}
		else
		{
			data->str1 = get_word(argument, &i);
			data->expanded = ft_strjoin(data->expanded, data->str1);
			free(data->str1);
		}
	}
	free(argument);
	return (data->expanded);
}

t_command	*expander_command(t_command *list)
{
	int			i;
	t_command	*head;

	head = list;
	if (list != NULL && (list->type == PIPE || list->type == OR_OP))
	{
		printf("syntax error\n");
		clear_list(&head);
		return (NULL);
	}
	while (list != NULL)
	{
		i = 0;
		list->quoted = 0;
		if (list->type == -1)
		{
			printf("syntax error\n");
			clear_list(&head);
			return (NULL);
		}
		while (list->args != NULL && list->args[i] != NULL)
		{
			list->value = unquote_arg(list, list->value, 0, 0);
			list->args[i] = unquote_arg(list, list->args[i], 0, 0);
			if (list->quoted != 1)
				list->args[i] = expand_vars(list->args[i], 0);
			if (data->syntax_error)
			{
				clear_list(&head);
				printf("syntax error\n");
				data->syntax_error = 0;
				return (NULL);
			}
			i++;
		}
		list = list->next;
	}
	return (head);
}
