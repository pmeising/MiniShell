/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 15:35:06 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/27 20:57:19 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// libft
# include "libft/libft.h"
// libraries
# include <curses.h> /*    */
# include <termcap.h> /* for terminal settings    */
# include <errno.h> /* for exit errors   */
# include <signal.h> /* for signal funcs.  */
# include <dirent.h> /*    */
# include <sys/wait.h> /*    */
# include <sys/types.h> /*    */
# include <sys/stat.h> /*    */
# include <sys/ioctl.h> /*    */
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

// # include </Users/bde-carv/goinfre/.brew/opt/readline/include/readline/readline.h>
// # include </Users/bde-carv/goinfre/.brew/opt/readline/include/readline/history.h>

// macros: \t=tab,\n=newline,\v=vertical tab,\f=form feed, \r=carriage return
# define BLANK_SPACE " \t\n\v\f\r"

/*
* struct for commands. commands are separated by pipes;
* a command consists of individual tokens like ls, -la, cat, -e...
* (=smaller execution unit of a command);
* command_path: "/bin/usr/echo" executable file to cmds like echo
* arguments: contains cmd name and its options and args;
* open_flag: 0 stands for overwrite, 1 for add. default = -1;
* fd_in: in/-output files, just need to 
* rewrite the in/output files at end of execution;
* is_built_in: flag for calling builtin function, default = 0;
*/
typedef struct s_cmd
{
	t_list			*toks;
	char			*command_path;
	char			**arguments;
	char			*input_file;
	char			*output_file;
	char			*heredoc_delim;
	char			*heredoc_temp;
	int				fd_in;
	int				fd_out;
	int				open_flag;
	int				is_built_in;
	int				is_prgrm;
	int				is_heredoc;
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
typedef struct s_mini
{
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
	int			special_flag;
}				t_mini;

// global variable
extern t_mini	g_mini;

// main.c
int		ft_is_special_built(char *arguments);
int		ft_get_input(void);
void	ft_main_1(void);
// ft_built_in_exec.c
void	ft_pwd_exec(void);
void	ft_env_exec(void);
void	ft_exit_exec(t_list *toks);
void	ft_echo_exec(t_cmd *iterator);
void	ft_execute_built_in(t_cmd *cmd, t_list *toks);
// ft_cd_exec.c
void	ft_cd_exec_helper_1(char *content);
void	ft_cd_empty(void);
void	ft_cd_two_dots(void);
void	ft_cd_exec(t_list *toks);
// check_builtin.c
int		ft_is_built_in(char *cmd_name);
int		ft_is_cd(char *str);
int		ft_is_exit(char *cmd_name);
int		ft_is_unset(char *str);
int		ft_is_pwd(char *str);
// check_builtin2.c
int		ft_is_export(char *str);
int		ft_is_echo(char *str);
int		ft_is_env(char *cmd_name);
// ft_delim_funcs.c
int		ft_is_tok_delim(char c);
int		ft_is_cmd_delim(char c);
// ft_env_input.c
void	ft_bad_sub(char *raw_input, int *pos);
void	ft_insert(char *raw_input, char *dup_var_cont, int pos, int start);
void	help_put_env_in_input(char **raw_input, \
		char**var_name, int *start, int *pos);
void	ft_put_env_in_input(char *raw_input, int pos);
// env_utils.c
int		ft_env_exist(char *str, t_list **dup_env);
int		ft_del_env_cont(char *str, t_list **dup_env);
void	ft_create_env(char *str, t_list **dup_env);
char	*ft_get_env_cont(char *env_name);
void	ft_copy_env(t_mini *g_mini, char **env);
// ft_env_vars_funcs.c
void	ft_replace_dollar_question_mark(char *raw_input, int pos);
void	ft_env_vars(char *raw_input);
void	ft_dollar_sign(void);
// ft_env_vars_funcs_2.c
void	ft_quotes(char *raw_input, int *single_quotes, \
					int *double_quotes, int *pos);
char	*ft_check_doll(char *raw_input, int *pos);
char	*ft_extract_content(char *var_name);
// ft_execute_process.c
void	ft_free_double_char(char **str);
void	ft_overwrite_env(void);
void	ft_execute_process_2(t_cmd *cmd_iterator, int *pid);
void	ft_execute_process(t_cmd *cmd_iterator, int i, int *pid);
// ft_free_funcs.c
void	ft_free_fds(void);
void	ft_free_helper(t_cmd *cmd_iterator);
void	ft_free_helper_1(void);
void	ft_free_input(void);
void	ft_free_input_built_in(void);
void	ft_free_lst_cont(t_list *cmd_iterator);
void	exit_program(int status);
// init_minishell.c
void	ft_init_minishell(t_mini *g_mini, char **env);
void	ft_oldpwd(void);
void	raise_shvl(void);
void	ft_update_env_list(char *env_name, char *new_value, t_list *dup_env);
char	*replace_env_cont(char **curr_content, char *env_name, char *new_value);
// ft_interpret.c
int		ft_is_program(t_cmd *cmd, char *prog_name);
void	ft_free_path_at_i(char **paths);
int		ft_find_command(t_cmd *cmd, t_list *tok);
void	ft_store_arguments(t_cmd *cmd, t_list *toks);
void	ft_interpret(t_cmd *cmd_iterator);
// ft_interpret_2.c
int		ft_find_command_3(t_cmd *cmd, t_list *tok, char **paths);
int		ft_find_command_5(char **paths, char *temp, int i);
// ft_interpret_1.c
char	*ft_find_path(void);
char	**ft_find_paths(char *path);
void	ft_cut_path(t_list *toks);
int		ft_find_command_1(t_cmd *cmd, t_list *tok, char **paths);
int		ft_find_command_2(t_cmd *cmd, t_list *tok, char **paths);
// ft_lst_funcs.c
t_cmd	*ft_lstnew_cmds(void);
t_cmd	*ft_lstlast_cmds(t_cmd *lst);
void	ft_lstadd_back_cmds(t_cmd **lst, t_cmd *new);
int		ft_lstsize_cmds(t_cmd *lst);
// ft_parsing_4.c
void	ft_remove_quotes_1(char *content, int k);
void	ft_remove_quotes(char *content);
void	ft_skip_var(char *raw_input, char *var_name, int start, int pos);
void	ft_find_exit_1(t_cmd *cmd);
// ft_parsing_3.c
void	ft_read_heredoc_2(t_cmd *cmd, int fd_input_file);
void	ft_read_heredoc(t_cmd *cmd);
int		ft_get_redir_tok_1(t_cmd *cmd, char *raw_input, int *pos, int *i);
void	ft_get_redir_tok_2(t_cmd *cmd, char *raw_input, int *pos);
void	ft_get_redir_tok_3(t_cmd *cmd, char *raw_input, int *pos);
// ft_parsing_2.c
int		ft_get_token_pos(char *raw_input, int pos);
char	*ft_get_token_1(char *raw_input, int pos);
char	*ft_get_token(char *raw_input, int pos);
void	ft_1(void);
void	ft_2(void);
// ft_parsing_1.c
int		ft_get_redir_tok(t_cmd *cmd, char *raw_input, int pos);
int		ft_is_only_digit(char *str);
void	ft_find_exit(void);
int		ft_parsing_1(t_cmd *cmd, char *raw_input, int *pos);
void	ft_parsing(char *raw_input);
// ft_print_funcs.c
void	ft_print_list(t_cmd *cmds);
void	ft_print_args(t_cmd *iterator);
void	ft_print_cmds(t_cmd *cmd);
// signals.c
void	ft_handle_sigint(void);
void	ft_sigint(int sig);
void	exit_shell_quit(int sig);
// ft_utils.c
void	ft_del_sus_quotes(void);
void	ft_put_prompt(int sig);
void	ft_check_malloc_int(int *fd);
void	ft_check_malloc(char *str);
void	ft_free_chars(char *cur_cwd, char *new_cwd);
// ft_utils_2.c
void	ft_fork_1(t_cmd *cmd_iterator, int *pid, int i);
void	ft_fork_2(int *pid);
void	ft_check_malloc_1(int *pid);
void	ft_fork(void);
void	ft_execute(void);
// ft_env.c
int		ft_env(t_list **dup_env);
// validity_utils.c
int		ft_skip_quotes(char *raw_input, int pos);
int		ft_skip_spaces(char *raw_input, int pos);
void	ft_check_q_help(char *raw_input, int *i, int *single_q, int *double_q);
int		ft_check_quotes(char *raw_input);
// validity_checks.c
int		ft_space_between_char(char *raw_input, char c);
int		ft_check_backslash(char *raw_input);
int		ft_check_semicolon(char *raw_input);
int		ft_str_only_space(char *raw_input);
char	*ft_strstr_quotes(char *raw_input, char *illegal_str);
// validity_checks_2.c
int		ft_not_supported(char *raw_input);
int		ft_check_input_validity(char *raw_input);
int		ft_qsb(void);
int		ft_contains_not_supported(char *raw_input, char *illegal_str);
// validity_checks_1.c
int		ft_is_only_redir(char *raw_input, char c);
int		ft_is_char_front(char *raw_input, char c);
int		ft_is_char_end(char *raw_input, char c);
int		ft_illegal_sequence(char *raw_input, char *sequence);
int		ft_wrong_tokens(char *raw_input);
// ft_built_in_exec.c
void	ft_pwd_exec(void);
void	ft_env_exec(void);
void	ft_exit_exec(t_list *toks);
void	ft_echo_exec(t_cmd *iterator);
void	ft_execute_built_in(t_cmd *cmd, t_list *toks);
// ft_export_exec.c
void	ft_export_exec_2(char **env_name, char **env_cont);
void	ft_export_exec(t_list *toks);
void	ft_export_else(t_list *iterator);
void	ft_print_sorted_env(t_list *dup_env);
void	ft_export_loop(t_list *iterator, char **env_name, char **env_cont);
//ft_unset_exec.c
void	ft_unset_help(t_list *env_iterator, t_list *tok_it);
void	ft_delete_env(t_list *dup_env, int i);
void	ft_unset_exec(t_list *toks);
// ft_cd_exec.c
void	ft_cd_exec(t_list *toks);
void	ft_cd_two_dots(void);
void	ft_cd_empty(void);
void	ft_cd_exec_helper_1(char *content);
// env_input.c
void	help_put_env_in_input(char **raw_input, \
		char**var_name, int *start, int *pos);
void	ft_put_env_in_input(char *raw_input, int pos);
void	ft_insert(char *raw_input, char *dup_var_cont, int pos, int start);
void	ft_bad_sub(char *raw_input, int *pos);
// interpret_utils.c
void	ft_cut_path(t_list *toks);
char	*ft_find_path(void);
char	**ft_find_paths(char *path);
// ft_is_prog.c
int		ft_is_program(t_cmd *cmd, char *prog_name);
int		ft_is_prog_help_1(t_cmd *cmd, char *prog_name);
void	ft_is_prog_help_2(t_cmd *cmd, char *prog_name, int i);
void	ft_free_str(char **str);
// pipe_funcs.c
void	ft_close_pipes(int in, int out);
void	ft_close_fds(int in, int out, int nbr);
void	ft_set_pipes(void);
void	ft_init_pipefd(int nbr_of_pipes);
// file_funcs.c
void	ft_open_file_2(char *file_name, int *fd, int j, int open_flag);
void	ft_open_file(char *file_name, int *fd, int j, int open_flag);
void	ft_set_files(void);

#endif