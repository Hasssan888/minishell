/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:17:53 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/08 12:14:19 by aelkheta         ###   ########.fr       */
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
		printf("env_value: %s\n", env->value);
		if (ft_strncmp(env->value, env_val, ft_strlen(env_val)) == 0)
			return (env);	
		env = env->next;
	}
	return (NULL);
}
int	export(t_command *cmd, t_env *envir)
{
	if (!cmd->args[1])
		env(envir);
	else
	{
		char 	*str = ft_strchr(cmd->args[1], '=');
		char 	*word = get_word_(cmd->args[1], "+=");
		// if (str[i])
		t_env 	*env_var = get_env_ele_ptr(word);
		
		printf("str = %s\n", str);
		printf("word = %s\n", word);
		// printf("env = %s\n", env_var->value);

		free(word);
		
		if (!str)
			return (0);
		else if (env_var != NULL && str[-1] == '=')
		{
			if (env_var->value != NULL)
				free(env_var->value);
			env_var->value = ft_strdup(cmd->args[1]);
		}
		else if (env_var != NULL && str[-1] == '+')
		{
			// printf("%s\n", env_var->value);
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

int unset()
{
    return (0);
}