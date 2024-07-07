/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:17:53 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/07 17:36:34 by aelkheta         ###   ########.fr       */
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

// void modify_env_var(char *var, )
// {
	
// }


// int join_str(char **env, char **cmd)
// {
// 	int len;
// 	char **new_env;
	
// 	len = -1;
// 	while(env[++len] != NULL);
// 	new_env = malloc((len + 1) * sizeof(char *));
// 	if (!new_env)
// 		return (0);
// 	len = -1;
// 	while(env[++len])
// 	{
// 		// new_env[len] = malloc((env[len] + 1) * sizeof(char));
// 		new_env[len] = ft_strdup(env[len]);
// 	}
// 	new_env[len] = ft_strdup(cmd[1]);
// 	new_env[++len] = NULL;
// 	return (1);
// }

char *get_word_(char *line, int del)
{
	int i = 0;
	if (!line)
		return (NULL);
	while (line[i] && line[i] != del)
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
		if (ft_strcmp(env->value, env_val) == 0)
			return (env);	
		env = env->next;
	}
	return (NULL);
}
int	export(t_command *cmd, t_env *envir)
{
	// int	i;

	// i = 0;
	if (!cmd->args[1])
		env(envir);
	else
	{
		char 	*str = ft_strchr(cmd->args[1], '=');
		char 	*word = get_word_(cmd->args[1], '=');
		t_env 	*env_var = get_env_ele_ptr(word);
		free(word);
		// if (str != NULL && str[-1] == '+')
		// {
		// char *word = get_word(cmd->args[1], '=');
		// char *env_var = get_env_ele_ptr(word);
		// free(word);
		
		if (str != NULL && env_var != NULL)
		{
			free(env_var->value);
			env_var->value = ft_strdup(&str[1]);
			// add_back(&data->env, lstnew(ft_strdup(cmd->args[1])));
		}
		else if (str != NULL)
			add_back(&data->env, lstnew(ft_strdup(cmd->args[1])));
	}
		// printf("%s\n", cmd->value);
	// if (cmd[1] == NULL)
	// {
	// 	env(data->env);
	// 	return (0);
	// }
	// while (env[++i] != NULL)
	// {
		// if (ft_strncmp(exprt_var, env[i], ft_strlen(env[i])) == 0)
		// 	modify_env_var(env);
	// }
	// join_str(data->env, cmd);
	// printf("%s\n", env[i]);
	return (0);
}

int unset()
{
    return (0);
}