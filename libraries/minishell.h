/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:43:39 by aelkheta          #+#    #+#             */
/*   Updated: 2024/08/04 19:41:13 by aelkheta         ###   ########.fr       */
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
# include <sys/types.h>         //
# include <sys/wait.h>
#include <sys/stat.h>
// for wating child process to terminate execution
# include <unistd.h> // for system calls
// # include "../exec/main.h"

#define COLOR BBLU
# define DEFAULT_PATH "/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin:."

typedef enum s_types
{
	CMD,
	RED_OUT,
	RED_IN,
	PIPE,
	LIST,
	BACK,
	ARG,
	OR_OP,
	AND_OP,
	FLE,
	APP,
	HER_DOC
}						t_types;

// # define	CMD 			0
// # define EXEC 			1
// # define LIST 			5
// # define BACK 			6
// # define ARG 			7
// # define OR_OP 			8
// # define AND_OP 			9
// # define FLE				10

# define WHITESPACES 	" \t\v\n"
# define SYNTERRR 		2
# define AMPIGOUS 		1



# define TOKEN 			0
# define PIPE 			1
# define RED_OUT 		2
# define RED_IN 		3
# define APP 			4
# define HER_DOC 		5

// #define  sigact;

typedef struct sigaction sigact;

typedef struct s_env
{
	// char				*value;
	char				*env_key;
	char				*env_value;
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
	int 				redirect;
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
	char 				*unquoted_line;
	char				*special_chars;
	char				**envirenment;
	t_command			*list_command;
	t_command			*_tokens_list;
	t_command			*rdrct_head;
	int					ignore_sig;
	bool 				syntax_error;
}						t_data;

typedef struct s_token
{
	int					i;				// just a normal index for reading the input character by character
	int 				prev_type; 		// this variable for tracking the type of the previous node
	int 				index;     		// to point to the location of the next token
	int 				type;      		// for the type of the token PIPE REDER ...
	char 				*value;   		// token value if it's a pipe the value is "|"
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
	int					count_pipe;
	int					count_read_in;
	int					count_read_out;
	int					count_here_doc;
	int					flag;
	int					b;
	char				*line;
	t_command			*cur;
	char 				*s;
	int					*end_1;
	int					*pipe_t;
}						t_pipex;

typedef struct s_path
{
	int					i;
	char				*path;
	char				**mypaths;
	char				*part_path;
	char				*path_from_envp;
}						t_path;

// extern t_data *g_data; // for use this global var from all files

extern int g_exit_stat; // for use this global var from all files
extern int flag;


// # include <termios.h>
// # include <sys/stat.h>

void					print_minishell(void);
void					print_prompt(void);
void					print_type(int type);
void					print_args(char **args);
void					print_list(t_command *table);

// buit-in commands:

int						cd(t_data *data, char **args);
char					*get_prompt(void);
void					pwd(void);
int						env(t_env *env);
int						echo(char **cmd);
int						is_builtin_cmd(t_data *data, t_command *command);
char					*get_cmd_path(t_data *data, char *cmd_);
char					*get_word_(char *line, char *del);
t_env					*get_env_ele_ptr(t_data *data, char *env_val);
void					print_array(char **array);
int						export(t_data *data, t_command *cmd, t_env *env);
int						unset(t_data *data, char **env_var, t_env *envirenement);
t_env					**sort_env(t_env **env_, int env_len);
int					 	valid_identifier(char *str);
void					print_sorted_env(t_env *env);
void					print_export_env(t_env **env, int env_len);
int					 	env_c_len(t_env *env_);

// general purpose utiles

// char 			*skip_command(char *command);

// utiles for linked list:

void					add_back_list(t_command **lst, t_command *new);
t_command				*new_node(int type, char *value);
void					clear_list(t_command **lst);

// parsing functions

int						parse_command(t_data *data, char *command);
int						get_args_size(t_command *list);
t_command				*redirect_list(t_data *data, t_command **redirect_head);
void					get_redirect_node(t_data *data);

// tokenizer functions
t_command				*tokenzer_command(char *command_line);
char					*duplicate_word(char *command_line, int *i, int j);

// print functions

void					print_minishell(void);
void					print_prompt(void);
void					ft_perror(char *message);

// signals hanling

// void					child_sig_handler(int signo);
// void 				ign_sig_child();
void					handle_signals(int i);
int 					check_exit_status(int status);

// envirenement utiles

void 					clear_env(t_env **env);
char					**env_to_array_(t_env *env);
void					add_back(t_env **lst, t_env *new);
t_env					*lstnew(char *env_value, char *env_key);
t_env					*creat_env(t_data *data, char **env);
void					clear_all(t_data *data);


// parsing utiles

// int 					ft_strisalnum(char *str);
int						exec_command(t_data *data, t_command *commands_list);
// char					*ft_strnstr_l(const char *big, const char *little,
// 							size_t len);
char					*get_env_element(t_data *data, char *env_var);

// lexer functions

int						check_unqoted(char *line);
char					*lexer_command(t_data *data, char *line);

// expander functions

char					*get_var(char *env_var, int *i);
char					*unquote_arg(t_command *list, char *arg, int j, int k);
// char					*ft_strnstr_l(const char *big, const char *little,
// 							size_t len);
char					*get_word(char *argument, int *i);
char					*expand_vars(t_data *data, char *argument, int i);
t_command				*expander_command(t_data *data, t_command *list);
int						get_expanded(t_data *data, char *argument, int *i);
char					**ft_split_str(const char *s, char *del);

// clean functions
t_command				*free_node(t_command **node);
void					clear_list(t_command **lst);
void					free_array(char **array);
void 					free_int_array(int **array);

// execution

void					ft_count_pipe(t_command *list, t_pipex *p);
void					ft_count_read_out(t_command *node, t_pipex *p);
void					ft_count_read_in(t_command *node, t_pipex *p);
void					open_outfile(t_command *node, t_pipex *p);
void					open_infile(t_command *node, t_pipex *p);
void					ft_count_here_doc(t_command *node, t_pipex *p);
char					*function(char **env);
char					*slash(char *mycmdargs);
char					*without_slash(char **env, char *mycmdargs);
char					*search_path(char *mycmdargs, char **env);
void					ft_error(char **av);
pid_t					fork_pipe(t_data *data, t_command *node1, char **env, t_pipex *p);
void					open_here_doc(t_data *data, t_command *node, t_pipex *pipex);
void					here_doc(t_data *data, t_command *node, t_pipex *pipex);
void					ft_excute(char **av, char **env);
void					ft_error_2(void);
void					ft_pipe(t_data *data, t_command *node1, char **ev, t_pipex *p);
int						ft_strcmp(char *s1, char *s2);
int						func(t_data *data, t_command *list);
char					*strjoin1(char *s1, char *s2);
void   					infile(t_data *data, t_command *node1, char **env, t_pipex *p);
void					outfile(t_data *data, t_command *node1, char **env, t_pipex *p);
void					pipe_heredoc(t_data *data, t_command *node1, char **env, t_pipex *p);
int						if_is_buil(t_command *command);
void					excut_butlin(t_data *data, t_command *node1, char **env);
void					sig_handler(int signal);
void					skip_two(t_data *data, char **ev, t_pipex *p);
int						check(t_command *node);
int						check_exit_status(int status);
void					free_int_array(int **array);
void					skip_pipe(t_pipex *p);
void					here_doc_error(char **av);
int						file_info(char **av);
int						file_info_2(char **av);
void					handle_direct(char **av);
#endif
