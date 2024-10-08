/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:43:39 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/18 14:34:54 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./colors.h"
# include "./libft/libft.h"     // libft functions
# include <dirent.h>            // for directory handling
# include <errno.h>             // for errors specification
# include <fcntl.h>             // for open sytem call and others system calls
# include <readline/history.h>  // readline GNU library
# include <readline/readline.h> // readline GNU library
# include <signal.h>            // for kill and signal handling
# include <stdbool.h>           // for boolean vars
# include <stdio.h>             // for printf
# include <stdlib.h>            // for malloc and free
# include <sys/stat.h>          // for file and dir info
# include <sys/types.h>         //
# include <sys/wait.h>

// for wating child process to terminate execution
# include <unistd.h> // for system calls

# define COLOR BBLU
// # define DEFAULT_PATH
// "/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin:."

# define MAX_LONG "9223372036854775808"
# define WHITESPACES " \t\v\n"
# define SYNTERRR 2
# define AMPIGOUS 1

// MACROS for signals

# define SIG_INT_HANDL 1
# define SIG_INT_QUI_IGN 2
# define SIG_INT_QUI_DFL 3
# define SIG_QUI_IGN 4

// MACROS for parsing

# define TOKEN 0
# define PIPE 1
# define RED_OUT 2
# define RED_IN 3
# define APP 4
# define HER_DOC 5

typedef struct s_env
{
	// char				*value;
	char				*env_value;
	char				*env_key;
	struct s_env		*next;
}						t_env;

typedef struct s_command
{
	int					type;
	// int					in_file;
	// int					out_file;
	int					syntxerr;
	int					quoted;
	char				*value;
	char				**args;
	struct s_command	*next;
}						t_command;

typedef struct s_data
{
	int					i;
	int					j;
	int					ac;
	int					flag;
	int					redirect;
	int					exit_status;
	t_env				*env;
	char				**av;
	char				*str1;
	char				*str2;
	t_command			*list;
	t_command			*head;
	char				*prompt;
	char				*expanded;
	char				**old_pwd;
	char				**shell_lvl;
	char				*new_command;
	char				*trimed_line;
	char				**current_pwd;
	char				*unquoted_line;
	char				*special_chars;
	char				**envirenment;
	t_command			*list_cmd;
	t_command			*_tokens_list;
	t_command			*rdrct_head;
	int					ignore_sig;
	long				k;
	int					error_num;
	bool				syntax_error;
}						t_data;

typedef struct s_token
{
	int					i;
	// just a normal index for reading the input character by character
	int					prev_type;
	// this variable for tracking the type of the previous node
	int					index;
	// to point to the location of the next token
	int					type;
	// for the type of the token PIPE REDER ...
	char				*value;
	// token value if it's a pipe the value is "|"
}						t_token;

typedef struct s_pipex
{
	char				**name_file;
	int					pipe[2];
	int					w;
	int					i;
	int					j;
	int					k;
	int					q;
	pid_t				r;
	pid_t				pid1;
	pid_t				pid2;
	pid_t				a[2];
	pid_t				pid;
	int					*fd;
	int					*tb;
	int					**strs;
	int					arr[16];
	int					end[2];
	int					status;
	int					status_1;
	int					indixe;
	int					outfile;
	int					infile;
	int					infile_here_doc;
	int					save1;
	int					save2;
	int					count_pipe;
	int					count_read_out_in;
	int					count_here_doc;
	int					flag;
	int					b;
	char				*line;
	t_command			*cur;
	char				*s;
	char				*s1;
	char				*s2;
	int					*end_1;
	int					*pipe_t;
	int					save_in;
	int					d;
	int					quoted;
	t_command			*c;
}						t_pipex;

typedef struct s_path
{
	int					i;
	char				*path;
	char				**mypaths;
	char				*part_path;
	char				*path_from_envp;
}						t_path;

extern int	g_exit_stat; // for use this global var from all files

// print functions

void					print_minishell(void);
void					print_prompt(void);
void					print_type(int type);
void					print_args(char **args);
void					print_list(t_command *table);

// errors

// void					put_error(char *str);

// buit-in commands:

int						is_builtin_cmd(t_data *data, t_command *command,
							int flag);
void					get_old_current_pwd(t_data *data);
int						change_dir(t_data *data, t_env *env, char *path);
int						chdir_home_prev(t_data *data, t_env *env, char **args);
int						cd(t_data *data, char **args);
void					echo_it(char **cmd, int i);
bool					check_echo_options(char *cmd);
int						echo(char **cmd);
char					*get_word_(char *line, char *del);
t_env					*get_env_ele_ptr(t_env *env, char *env_val);
char					**env_to_array_(t_env *env);
int						ft_is_str_digit(char *str);
int						env(t_env *env);
int						check_exit_overflow(char *str, int exit_num);
void					check_exit(t_data *data, t_command *command);
void					exit_(t_data *data, t_command *command, int flag);
void					print_sorted_env(t_env *env);
int						valid_identifier(char *str);
char					**get_exp_splited(char *str, char del);
void					export__cnt(t_data *data, t_env *env_ptr, char *arg);
int						export(t_data *data, t_command *cmd, t_env *env);
void					free_env_ptr(t_env **env_ptr);
void					export_var_app(t_data *data, t_env *env_ptr,
							char **splited);
int						env_c_len(t_env *env_);
void					print_export_env(t_env **env, int env_len);
t_env					**sort_env(t_env **env_, int env_len);
int						unset(t_data *data, char **env_var);
void					del_node(t_data *data, t_env *env_var);
void					del_one(t_env **env, t_env *env_var);
void					pwd(t_data *data);
void					print_array(char **array);
void					panic(char *error_str, int exit_stat);
void					check_synt_err(char *str);
void					put_error(char *str);

// utiles for linked list:

void					add_back_list(t_command **lst, t_command *new);
t_command				*new_node(int type, char *value);
void					clear_list(t_command **lst);
t_command				*lstlastcmd(t_command *lst);

// parsing functions

int						parse_command(t_data *data, char *command);
int						get_args_size(t_command *list);
t_command				*redirect_list(t_data *data, t_command **redirect_head);
void					get_redirect_node(t_data *data);
int						exec_command(t_data *data, t_command *commands_list);
char					*get_env_element(t_data *data, char *env_var);
t_command				*syntax_error(t_data *data, t_command *list_command,
							t_command *head, char *str);

t_env					*get_alternative_env(t_data *data);
int						parser_continue(t_data *data);

// tokenizer functions

t_command				*tokenzer_command(char *command_line);
char					*duplicate_word(char *command_line, int *i, int j);

// expander functions

t_command				*expander_command(t_data *data, t_command *list);
char					**ft_split_str(const char *s, char *del);
void					what_quote(t_command *list, char *arg);
char					*get_var(char *env_var, int *i);
char					*unquote_arg(t_command *list, char *arg, int j, int k);
char					*get_word(char *argument, int *i);
int						get_expanded(t_data *data, char *argument, int *i);
char					*expand_digits(t_data *data, char *argument, int *i);
void					expand_(t_data *data, char *argument, int *i);
char					*expand_vars__(t_data *data, char *argument);
char					*_get_quoted___word(char *arg, int *i);
char					*expand_vars(t_data *data, t_command *list,
							char *argument);
size_t					ft_len_arr(char **arr);
char					**ft_arr_join(char **arr1, char **arr2);
int						ambigous_red(char *red_file);
int						is_ambiguous(t_command *list);
int						is_empty(char *str);
void					set_error(t_data *data, int err_num, char *str,
							t_command **cmd);
char					**split_argument(t_command *list, int i);
int						get_cmd_if_empty(t_command *list);
char					**split_and_join(char **args, char *exp_args);
int						get_real_len(char **arr);
int						exist_exp_cmd(char *cmd, char *arg);
int						is_all_quotes(char *str);
// mini utiles

void					init_minishell(t_data *data, int ac, char **av,
							char **env);
void					clear_env(t_env **env);
int						get_token_type(char *token);
char					*get_prompt(void);
void					print_list(t_command *table);
char					*read_input(void);

// signals hanling

void					handle_signals(int i);
int						check_exit_status(int status);

// envirenement utiles

void					clear_env(t_env **env);
char					**env_to_array_(t_env *env);
void					add_back(t_env **lst, t_env *new);
t_env					*lstnew(char *env_value, char *env_key);
t_env					*creat_env(t_data *data, char **env);
void					clear_all(t_data *data);
void					free_env_ptr(t_env **env_ptr);

// lexer functions

int						check_unqoted(char *line);
char					*lexer_command(t_data *data, char *line);

// clean functions
t_command				*free_node(t_command **node);
void					clear_list(t_command **lst);
void					free_array(char **array);
void					free_int_array(int **array);
void					free_command(t_command *cmd);

// execution

void					ft_count_pipe(t_command *list, t_pipex *p);
void					ft_count_read_out_in(t_command *node, t_pipex *p);
void					open_file(t_command *node, t_pipex *p);
void					ft_count_here_doc(t_command *node, t_pipex *p);
char					*function(char **env);
char					*slash(char *mycmdargs);
char					*without_slash(t_data *data, char **env,
							char *mycmdargs);
char					*search_path(t_data *data, char *mycmdargs, char **env);
void					ft_error(t_data *data, char **av);
pid_t					fork_pipe(t_data *data, t_command *node1, char **env,
							t_pipex *p);
void					open_here_doc(t_data *data, t_command *node,
							t_pipex *pipex);
void					here_doc(t_data *data, t_command *node, t_pipex *pipex);
void					ft_excute(t_data *data, char **av, char **env);
void					ft_error_2(void);
void					ft_pipe(t_data *data, t_command *node1, char **ev,
							t_pipex *p);
int						ft_strcmp(char *s1, char *s2);
int						func(t_data *data, t_command *list);
char					*strjoin1(char *s1, char *s2);
void					infile(t_data *data, t_command *node1, char **env,
							t_pipex *p);
void					outfile(t_data *data, t_command *node1, char **env,
							t_pipex *p);
void					pipe_heredoc(t_data *data, t_command *node1, char **env,
							t_pipex *p);
int						if_is_buil(t_command *command);
void					excut_butlin(t_data *data, t_command *node1, char **env,
							t_pipex *p);
void					sig_handler(int signal);
void					skip_two(t_data *data, char **ev, t_pipex *p);
int						check(t_command *node);
int						check_exit_status(int status);
void					free_int_array(int **array);
void					skip_pipe(t_pipex *p);
void					here_doc_error(char **av);
void					file_info_2(char **av, int *flag,
							struct stat file_start);
void					file_info(char **av, int *flag);
int						handle_direct(char **av);
void					exec_built_in(t_pipex *pipex, t_data *data,
							t_command *list);
void					child_process(t_data *data, t_command *node1,
							char **env, t_pipex *p);
char					**get_env_add_ele(char **env);
void					shlvl_update(t_data *data);
void					out_app_in_(t_command *cur, t_pipex *p);
int						in_out_err(t_pipex *p);

#endif
