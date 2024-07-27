/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:17:53 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/27 13:09:03 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	data->expanded = 0;
	free(cwd);
}

int ft_env_lenght(t_env *env)
{
	int env_len = 0;
	while (env != NULL)
	{
		env_len++;
		env = env->next;
	}
	return (env_len);
}

char **env_to_array_(t_env *env, int *len)
{
	// env_ptr1 = env;
	int i = 0;
	int env_len = ft_env_lenght(env);
	char **array = malloc((env_len + 1) *sizeof(char *));
	if (!array || !env)
		return NULL;
	// env_ptr1 = env;
	while (env != NULL)
	{
		if (!env->env_key)
			array[i++] = ft_strjoin(ft_strdup(env->env_value), ft_strdup(""));
		else if (!env->env_key[0])
			array[i++] = ft_strjoin(ft_strdup(env->env_value), ft_strdup("="));
		else
			array[i++] = ft_strjoin(ft_strjoin(ft_strdup(env->env_value), ft_strdup("=")), ft_strdup(env->env_key));
		env = env->next;
	}
	array[i] = NULL;
	*len = env_len;
	return (array);
}

// char **sort_array(char **array, int arr_len)
// {
// 	int i = 0;
// 	int j = 0;
	
// 	while (i < arr_len - 1)
// 	{
// 		j = 0;
// 		while (j < arr_len - i - 1)
// 		{
// 			if (ft_strcmp(array[j], array[j + 1]) > 0)
// 			{
// 				char *temp = array[j];
// 				array[j] = array[j + 1];
// 				array[j + 1] = temp;
// 			}
// 			j++;
// 		}
// 		i++;
// 	}
// 	array[arr_len] = NULL;
// 	return (array);
// }


int env_c_len(t_env *env_)
{
	int len = 0;
	while(env_ != NULL)
	{
		len++;
		env_ = env_->next;
	}
	return (len);
}

t_env **sort_env(t_env **env_, int env_len)
{
	int i = 0;
	int j = 0;
	
	while (i < env_len - 1)
	{
		j = 0;
		while (j < env_len - i - 1)
		{
			if (ft_strcmp(env_[j]->env_value, env_[j + 1]->env_value) > 0)
			{
				t_env *temp = env_[j];
				env_[j] = env_[j + 1];
				env_[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	// env_[env_len] = NULL;
	return (env_);
}


void print_export_env(t_env **env, int env_len)
{
	int i = 0;
	while(i < env_len)
	{
		if (!env[i]->env_key)
			printf("%s\n", env[i]->env_value);
		else if (env[i]->env_key != NULL)
			printf("%s=\"%s\"\n", env[i]->env_value, env[i]->env_key);
		i++;
	}
}

void print_sorted_env(t_env *env)
{
	t_env *env_ = env;
	int env_len = env_c_len(env_);
	int i = 0;
	t_env **env__ = malloc((env_len + 1) * sizeof(t_env *));
	while (i < env_len)
	{
		env__[i] = env_;
		env_ = env_->next;
		i++;
	}
	env__[i] = NULL;
	env__ = sort_env(env__, env_len);
	print_export_env(env__, env_len);
	free(env__);
}
	// char **sorted_arr = env_to_array_(env, &env_len);
	// if (data != NULL && data->envirenment != NULL)
	// {
	// 	free_array(data->envirenment);
	// 	data->envirenment = NULL;
	// }
	// data->envirenment = sorted_arr;
	// sorted_arr = sort_array(sorted_arr, env_len);
	// env__ = creat_env(sorted_arr);
	// print_export_env(env__);
	// if (env__ != NULL)
	// 	clear_env(&env__);

int valid_identifier(char *str)
{
	int i = 0;
	printf("%s\n", str);
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while(str[++i] && str[i] != '=')
	{
		if (str[i] == '+')
			break;
		else if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
	}
	if (str[i] && str[i] == '+' && str[i + 1] != '=')
		return (0);
	return (1);
}

int	export(t_command *cmd, t_env *env)
{
	int i = 0;
	if (!cmd->args[1])
	{
		print_sorted_env(env);
		data->exit_status = 0;
		return (0);
	}
	while(cmd->args[++i] != NULL)
	{
		if (!valid_identifier(cmd->args[i]))
		{
			ft_perror ("minishell: export: not a valid identifier\n");
			data->exit_status = 1;
			continue;
		}
		char **splited = ft_split(cmd->args[i], '=');
		data->str1 = ft_strchr(cmd->args[i], '=');
		char *trimed = ft_strtrim(splited[0], "+");
		t_env *env_ptr = get_env_ele_ptr(trimed);
		if (env_ptr != NULL && splited != NULL && splited[0] != NULL && data->str1 != NULL)
		{
			if (env_ptr->env_key != NULL && splited[0][ft_strlen(splited[0]) - 1] == '+')
			{
				if (splited[1] != NULL)
					env_ptr->env_key = ft_strjoin(env_ptr->env_key, ft_strdup(splited[1]));
				else
				{
					if (env_ptr->env_key != NULL)
						free(env_ptr->env_key);
					env_ptr->env_key = ft_strdup("");
				}
			}
			else
			{
				if (env_ptr->env_key != NULL)
					free(env_ptr->env_key);
				if (splited[1] != NULL)
					env_ptr->env_key = ft_strdup(splited[1]);
				else if (data->str1 != NULL)
					env_ptr->env_key = ft_strdup("");
				else
					env_ptr->env_key = NULL;
			}
		}
		else if (!env_ptr && splited[0][ft_strlen(splited[0]) - 1] == '+')
		{
			if (splited[1] != NULL)
				add_back(&data->env, lstnew(ft_strdup(trimed), ft_strdup(splited[1])));
			else
				add_back(&data->env, lstnew(ft_strdup(trimed), ft_strdup("")));
		}

		else
			add_back(&data->env, lstnew(ft_strdup(cmd->args[i]), NULL));
		free_array(splited);
		free(trimed);
	}

	return (0);
}

void	del_one(t_env **env, t_env *env_var)
{
	t_env	*current;

	current = env_var->next;
	free(env_var->env_value);
	free(env_var);
	*env = current;
}

void	del_node(t_env **env, t_env *env_var)
{
	t_env	*current;
	t_env	*previous;

	if (!env || !*env || !env_var)
		return ;
	if (*env == env_var)
	{
		del_one(env, env_var);
		return ;
	}
	previous = *env;
	current = (*env)->next;
	while (current)
	{
		if (current == env_var)
		{
			previous->next = current->next;
			free(current->env_value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

int	unset(char *env_var, t_env *envirenement)
{
	t_env	*env_ptr;

	if (!env_var)
		return (0);
	env_ptr = get_env_ele_ptr(env_var);
	if (!env_ptr)
		return (1);
	del_node(&envirenement, env_ptr);
	data->expanded = 0;
	return (0);
}
