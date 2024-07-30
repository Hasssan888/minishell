/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:52:20 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/30 11:18:01 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libraries/minishell.h"

char	*expand_digits(char *argument, int *i)
{
	if (argument[*i + 1] && ft_isdigit(argument[*i + 1])) // && ft_strlen(&argument[1]) > 2
	{
		*i += 2;
		int j = *i;
		if (!argument[j])
			return (data->expanded);
		while(argument[++j] && argument[j] != '$');
		data->expanded = ft_strjoin(data->expanded, duplicate_word(argument, i, j));
		*i = j;
		// *i += ft_strlen(&argument[2]);
	}
	// free(argument);
	return (data->expanded);
}

char *expand_vars__(char *argument)
{
	int i = 0;
	while(argument[i])
	{
		if (argument[i] == '$' && (ft_isalnum(argument[i + 1]) || ft_strchr("?_", argument[i + 1])))
		{
			if (!argument[i + 1])
				return (free(argument), ft_strdup("$"));
			// else if (argument[i + 1] == '"' || argument[i + 1] == '\'')
			// 	i++;
			else if (argument[i + 1] == '?')
			{
				i += 2;
				data->expanded = ft_strjoin(data->expanded,
						ft_itoa(data->exit_status));
			}
			else if (!ft_isdigit(argument[i + 1]))
			{
				get_expanded(argument, &i);
			}
			else if (ft_isdigit(argument[i + 1]))
				data->expanded = expand_digits(argument, &i);
		}
		else
		{
			data->str1 = get_word(argument, &i);
			data->expanded = ft_strjoin(data->expanded, data->str1);
		}
	}
	if (!data->expanded[0])
	{
		free(data->expanded);
		data->expanded = NULL;
	}
	return (data->expanded);
}

char *_get_quoted___word(char *arg, int *i)
{
	int j = *i;
	char *_quoted_word = NULL;
	char quote = 0;
	if (arg[j] == '$' && (arg[j + 1] == '\'' || arg[j + 1] == '"'))
		*i += 1;
	if (arg[j] == '\'' || arg[j] == '"')
	// {
		quote = arg[j++];
		// j++;
	// }
	// printf("quote: %c", quote);
	while(arg[j] && arg[j] != quote)
		j++;
	if (arg[j] && arg[j] == quote)
		j++;
	_quoted_word = ft_calloc(j - *i + 1, sizeof(char));
	ft_strlcpy(_quoted_word, &arg[*i], j - *i + 1);
	*i = j;
	return (_quoted_word);
}

char	*expand_vars(char *argument, int i)
{
	data->expanded = ft_strdup("");
	while (argument[i])
	{
		char *word = _get_quoted___word(argument, &i);
		// printf("word: %s %d\n", word, i);
		if (word != NULL && word[0] != '\'')
		{
			expand_vars__(word);
			free(word);
		}
		else
		{
			// data->str1 = get_word(argument, &i);
			data->expanded = ft_strjoin(data->expanded, word);
		}
	}
	free(argument);
	return (data->expanded);
}

size_t ft_len_arr(char **arr)
{
	size_t len = -1;
	if (!arr)
		return (0);
	while(arr[++len] != NULL);
	return (len);
}

char **ft_arr_join(char **arr1, char **arr2)
{
	int i = 0;
	int j = 0;

	if (!arr1 && !arr2)
		return (NULL);
	int len1 = ft_len_arr(arr1);
	int len2 = ft_len_arr(arr2);
	char **joined = malloc((len1 + len2 + 1) * sizeof(char *));

	while(arr1 != NULL && arr1[j] != NULL)
		joined[i++] = ft_strdup(arr1[j++]);
	j = 0;
	while(arr2 != NULL && arr2[j] != NULL)
		joined[i++] = ft_strdup(arr2[j++]);
	joined[i] = NULL;
	free_array(arr1);
	free_array(arr2);
	return (joined);
}

int ambigous_red(char *red_file)
{
	int i = 0;
	if (!red_file) // || !red_file[0]
		return (1);
	while (red_file != NULL && red_file[i] && ft_strchr(" \t", red_file[i]))
		i++;
	while(red_file != NULL && red_file[i])
	{
		char *str = ft_strchr(" \t", red_file[i]);
		if (str != NULL)
		{
			while(red_file[i] && ft_strchr(" \t", red_file[i]))
				i++;
			if (red_file[i] && !ft_strchr(" \t", red_file[i]))		
				return (1);
		}
		i++;
	}
	return 0;
}

// void open_files(t_command *heredoc)
// {
// 	while(heredoc != NULL)
// 	{
// 		if (heredoc->args[0] != NULL)
// 		{
// 			printf("%s\n", heredoc->args[0]);
// 			open(heredoc->args[0], O_TRUNC | O_CREAT | O_RDWR, 0744);
// 		}
// 		heredoc = heredoc->next;
// 	}
// }

int is_ambigous(t_command *list)
{
	if (!list->quoted && (list->type == RED_OUT || list->type == RED_IN))
	{
		// printf("%s\n", list->args[0]);
		if (ambigous_red(list->args[0]))
		{
			// open_files(data->head);
			clear_list(&data->head);
			ft_perror("ambiguous redirect\n");
			data->syntax_error = 0;
			return (0);
		}
	}
	return (1);
}

int is_empty(char *str)
{
	int i = 0;
	if (!str)
		return (1);
	while(str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (!str[i])
		return 1;
	return 0;
}

char **split_args_(char **exp_args, int i)
{
	// int i = -1;
	char **args = NULL;
	char **splited = NULL;

	if (!exp_args)
		return (NULL);
	while(exp_args[i] != NULL)
	{
		if (is_empty(exp_args[i]))
		{
			i++;
			continue;
		}
		splited = ft_split_str(exp_args[i], " \t\v");
		// free(list->value);
		// list->value = ft_strdup(splited[0]);
		if (splited != NULL && splited[0] != NULL && splited[1] != NULL)
			args = ft_arr_join(args, splited);
		// free_array(args);
		// {
			// list->args = args;
		// }
		i++;
	}
	free_array(exp_args);
	return (args);
}


int split_expanded(t_command *list)
{
	int i = 0;
	
	if (is_empty(list->value))
	{
		while(list->args[i] != NULL && is_empty(list->args[i]))
			i++;
		if (!list->args[i])
		{
			printf("[%s] [%d]\n", list->value, i);
			clear_list(&list);
			return (0);
		}
		free(list->value);
		char **splited = ft_split_str(list->args[i], " \t\v\n");
		list->value = ft_strdup(splited[0]);
		free_array(splited);
		printf("[%s] [%d]\n", list->value, i);
	}
	if (!list->quoted && list->value != NULL && list->value[0])
	{
		list->args = split_args_(list->args, i);
	}
	// {
	// }
	// while()
	return 1;
}

// int _expander__extended(t_command *list)
// {
	
// 	return (1);
// }

int	expander_extended(t_command *list)
{
	while (list->value != NULL && list->args != NULL && list->args[data->i] != NULL)
	{
		if (list->quoted != 1 && list->type != HER_DOC)
			list->args[data->i] = expand_vars(list->args[data->i], 0);
		if (list->type != HER_DOC)
			list->args[data->i] = unquote_arg(list, list->args[data->i], 0, 0);
		if (data->syntax_error)
		{
			clear_list(&data->head);
			ft_perror("syntax error\n");
			data->syntax_error = 0;
			return (0);
		}
		data->i++;
	}
	list->value = expand_vars(list->value, 0);
	list->value = unquote_arg(list, list->value, 0, 0);
	
	if (!split_expanded(list) || !is_ambigous(list))
		return (0);
	// list = list->next;
	return (1);
}


t_command	*expander_command(t_command *list)
{
	data->head = list;
	if (!list)
		return (NULL);
	if (list != NULL && list->type == PIPE)
	{
		ft_perror("syntax error\n");
		clear_list(&data->head);
		return (NULL);
	}
	while (list != NULL)
	{
		data->i = 0;
		list->quoted = 0;
		if (list->type == -1)
		{
			ft_perror("syntax error\n");
			clear_list(&data->head);
			return (NULL);
		}
		if (!expander_extended(list))
			return (NULL);
		list = list->next;
	}
	return (data->head);
}
