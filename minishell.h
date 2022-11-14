/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 19:09:48 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/10 16:14:14:09 by pmeising         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// libft
# include "libft/libft.h"

// pre defined libraries
# include <curses.h> /*    */
# include <termcap.h> /* for terminal settings    */
# include <errno.h> /* for exit errors   */
# include <signal.h> /* for signal funcs.  */
# include <dirent.h> /*    */
# include <sys/wait.h> /*    */
# include <sys/types.h> /*    */
# include <sys/stat.h> /*    */
# include <sys/ioctl.h> /*    */
# include <termios.h> /*    */
# include <fcntl.h>
// # include <readline/readline.h>
// # include <readline/history.h>

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NEED TO REMOVE/ADJUST THE PATH HERE.

# include </Users/bde-carv/goinfre/.brew/opt/readline/include/readline/readline.h>
# include </Users/bde-carv/goinfre/.brew/opt/readline/include/readline/history.h>

// macros

# define BLANK_SPACE " \t\n\v\f\r" //\t=tab,\n=newline,\v=vertical tab,
									//\f=form feed, \r=carriage return*/
// structs


/*
* struct for commands. commands are separated by pipes;
* a command consists of individual tokens like ls, -la, cat, -e...
* (=smaller execution unit of a command);
*/
typedef struct	s_cmd
{
	t_list		*toks;
	char		*command_path; // "/bin/usr/echo" executable file to cmds like echo
	char		**arguments; // contains cmd name and its options and args;
	char		*input_file; // default NULL
	char		*output_file; // default NULL
	char		*HEREDOC_DELIM;
	char		*heredoc_temp;
	int			fd_in; // reuse in/-output files, just need to rewrite the in/output files at end of execution.
	int			fd_out;
	int			open_flag; // 0 stands for overwrite, 1 for add. default = -1
	int			is_built_in; // flag for calling builtin function, default = 0;
	int			is_prgrm; // 0 default
	int			is_heredoc; // initialized to 0.
	struct s_cmd	*next;
	struct s_cmd	*previous;
}				t_cmd;

/*
* global struct that stores all relevant data
* exit_status: stores the exit status of the last cmd
* input_history: double linked list, each node stores a previous cmd
* ast: the abstract syntax tree that contains the cmd_tables of each cmd
* termcaps: contains temrinal settings derived from termcaps.h 
* copy_env: copy of the environment variables
* curr_cmd_table: pointer to the current executing cmd_table
*/
typedef struct	s_mini
{
	//int exit_status;
	t_list		*dup_env;
	t_cmd		*cmds;
	char		**env;
	char		*raw_input;
	int			nbr_heredocs;
	int			fdin;
	int			fdout;
	int			exit;
	int			**pipefd;
	int			nbr_of_pipes;
	int			exit_status;
	//t_cmd_table	*curr_cmd_table;
	
}				t_mini;

// /*
// * struct that saves the destiny of the directed content
// * and also saves the type of redirection (<,>,>>,<<);
// */
// typedef struct s_redir
// {
// 	char *to_where;
// 	char *type[2];
// }				t_cmd;


// global variable
extern t_mini	g_mini; /* extern = global variable */

// main.c
int ft_get_input(void);
void exit_program(int status);
int ft_is_tok_delim(char c);

// init_minishell.c
void	ft_init_minishell(t_mini *g_mini, char **env);
void ft_oldpwd(void);
void raise_shvl(void);
void ft_update_env_list(char *env_name, char *new_value, t_list *dup_env);
char *replace_env_cont(char **curr_content, char *env_name, char *new_value);

// signals.c

void ft_handle_sigint(void);
void ft_sigint(int sig);
void	exit_shell_quit(int sig);

// env_utils.c
int ft_env_exist(char *str, t_list **dup_env);
int ft_del_env_cont(char *str, t_list **dup_env);
void ft_create_env(char *str, t_list **dup_env);
char *ft_get_env_cont(char *env_name);
void ft_copy_env(t_mini *g_mini, char **env);

// ft_env.c
int ft_env(t_list **dup_env);

// validity_checks.c
int ft_space_between_char(char *raw_input, char c);
int ft_check_backslash(char *raw_input);
int ft_check_semicolon(char *raw_input);
int ft_str_only_space(char *raw_input);
char *ft_strstr_quotes(char *raw_input, char *illegal_str);

// validity_checks_1.c
int ft_is_char_front(char *raw_input, char c);
int	ft_is_char_end(char *raw_input, char c);
int ft_illegal_sequence(char *raw_input, char *sequence);
int	ft_wrong_tokens(char *raw_input);
int ft_contains_not_supported(char *raw_input, char *illegal_str);

// validity_checks_2.c
int ft_not_supported(char *raw_input);
int ft_check_input_validity(char *raw_input);

// validity_utils.c
int ft_skip_quotes(char *raw_input, int pos);
int ft_skip_spaces(char *raw_input, int pos);
int ft_check_quotes(char *raw_input);

// ft_lst_funcs.c
t_cmd	*ft_lstnew_cmds(void);
t_cmd	*ft_lstlast_cmds(t_cmd *lst);
void	ft_lstadd_back_cmds(t_cmd **lst, t_cmd *new);
int		ft_lstsize_cmds(t_cmd *lst);

// ft_delim_funcs.c
int ft_is_tok_delim(char c);
int	ft_is_cmd_delim(char c);

// parsing.c
void ft_parsing(char *raw_input);
int	ft_get_redir_tok(t_cmd *cmd, char *raw_input, int pos);
char *ft_get_token(char *raw_input, int pos);
int	ft_get_token_pos(char *raw_input, int pos);

// ft_print_funcs.c
void	ft_print_list(t_cmd *cmds);
void	ft_print_cmds(t_cmd *cmd);

// check_builtin.c
int ft_is_built_in(char *cmd_name);
int ft_is_cd(char *str);
int ft_is_exit(char *cmd_name);
int ft_is_unset(char *str);
int ft_is_pwd(char *str);

// check_builtin2.c
int ft_is_export(char *str);
int ft_is_echo(char *str);
int ft_is_env(char *cmd_name);

// ft_free_funcs.c
void ft_free_lst_cont(t_list *cmd_iterator);
void ft_free();
void exit_program(int status);

// ft_interpret.c
char	*ft_find_path(void);
char	**ft_find_paths(char *path);
int		ft_find_command(t_cmd *cmd, t_list *iterator);
void	ft_store_arguments(t_cmd *cmd, t_list *toks);

// parsing_utils.c
int		ft_get_token_pos(char *raw_input, int pos);
char	*ft_get_token(char *raw_input, int pos);
void	ft_parsing(char *raw_input);
void	ft_skip_var(char *raw_input, char *var_name, int start, int pos);

// ft_env_vars_funcs.c
char	*ft_extract_content(char *var_name);
void	ft_insert(char *raw_input, char *dup_var_cont, int pos, int start);
void	ft_put_env_in_input(char *raw_input, int pos);
void	ft_env_vars(char *raw_input);
int		ft_dollar_sign(void);

//test
// ft_parsing_utils.c
int		ft_find_command(t_cmd *cmd, t_list *iterator);
char	**ft_find_paths(char *path);
char	*ft_find_path(void);
void	ft_store_arguments(t_cmd *cmd, t_list *toks);
void	ft_interpret(t_cmd *cmd_iterator);
void	ft_remove_quotes(char *content);

// ft_built_in_exec.c
void ft_pwd_exec(void);
void ft_env_exec(void);
void ft_exit_exec(t_list *toks);
void ft_unset_exec(t_list *toks);
void ft_delete_env(t_list *dup_env, int i); // in utils

// ft_built_in_exec_2.c
void	ft_export_exec(t_list *toks);
void	ft_print_sorted_env(t_list *dup_env);
void	ft_cd_exec(t_list *toks);
void	ft_cd_two_dots(void);
void	ft_cd_empty(void);

// pipe_funcs.c
void	ft_set_pipes(void);
void	ft_init_pipefd(int nbr_of_pipes);

// file_funcs.c
void	ft_set_files(void);
void	ft_open_file(char *file_name, int *fd, int j, int open_flag);




void	ft_copy_content(char *file_1, char *file_2, int open_flag);

void	ft_execute_built_in(t_cmd *cmd, t_list *toks);
void	ft_cd_empty(void);
int		ft_is_echo(char *str);
void	ft_execute_process(t_cmd *cmd_iterator, int i);
void	ft_close_fds(int in, int out, int nbr);
void	ft_free_input(void);
void	ft_overwrite_env(void);

// utils to comply with 25 lines
void	ft_free_chars(char *cur_cwd, char *new_cwd);
void	ft_check(char *);
#endif