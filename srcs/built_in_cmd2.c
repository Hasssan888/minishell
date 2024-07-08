/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:17:53 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/08 20:20:19 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../libraries/minishell.h"

void	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
}

char *get_word_(char *line, char *del)
{
	int i = 0;
	if (!line)
		return (NULL);
	while (line[i] && !ft_strchr(del, line[i]))
		i++;
	char *word = malloc((i + 1) * sizeof(char));
	ft_strlcpy(word, line, i + 1);
	return (word);
}

t_env *get_env_ele_ptr(char *env_val)
{
	t_env *env = data->env;
	while(env != NULL)
	{
		if (ft_strncmp(env->value, env_val, ft_strlen(env_val)) == 0)
			return (env);	
		env = env->next;
	}
	return (NULL);
}

void print_array(char **array)
{
	int i = 0;
	while(array != NULL && array[i] != NULL)
		printf("%s\n", array[i++]);
}

t_env *sort_list(t_env *env) 
{
    t_env *env_ptr1 = env;
    int i = 0, j = 0;
    int list_len = 0;

    while (env_ptr1 != NULL) 
	{
        list_len++;
        env_ptr1 = env_ptr1->next;
    }
    char **sorted_array = malloc((list_len + 1) * sizeof(char *));
    env_ptr1 = env;
	while (env_ptr1 != NULL) 
	{
        sorted_array[i++] = env_ptr1->value;
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
                char *temp = sorted_array[j];
                sorted_array[j] = sorted_array[j + 1];
                sorted_array[j + 1] = temp;
            }
			j++;
		}
		i++;
	}
	sorted_array[list_len] = NULL;
    print_array(sorted_array);

    return env;
}

int	export(t_command *cmd, t_env *envir)
{
	if (!cmd->args[1])
		sort_list(envir);
	else
	{
		char 	*str = ft_strchr(cmd->args[1], '=');
		char 	*word = get_word_(cmd->args[1], "+=");
		t_env 	*env_var = get_env_ele_ptr(word);
		
		free(word);
		if (!str)
			return (0);
		else if (env_var != NULL && str[0] == '=' && str[-1] != '+')
		{
			if (env_var->value != NULL)
				free(env_var->value);
			env_var->value = ft_strdup(cmd->args[1]);
		}
		else if (env_var != NULL && str[-1] == '+')
		{
			char *strr = ft_strdup(&str[1]);
			char *val = ft_strjoin(env_var->value, strr);
			free(env_var->value);
			free(strr);
			env_var->value = val;
		}
		else
			add_back(&data->env, lstnew(ft_strdup(cmd->args[1])));
	}
	return (0);
}

void del_node(t_env *env, t_env *env_var)
{
	if (!env || !env_var)
		return ;
	else if (env == env_var)
	{
		t_env *ptr = env_var->next;
		free(env_var);
		env = ptr;
	}
	// else if(!env_var->next)
	// 	;
	// else
	// {
		
	// }
}

int unset(char *env_var, t_env *envirenement)
{
	t_env *env_ptr = get_env_ele_ptr(env_var);
	if (!env_ptr)
		return (1);
	del_node(envirenement ,env_ptr);

	// printf("%s\n", env_ptr->value);
    return (0);
}