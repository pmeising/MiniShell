/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_interpret.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 18:33:38 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/31 17:23:381 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
*	Here we find the correct variable from the "env" command and store it in a 
*	single string.
*/
char	*ft_find_path(void)
{
	char	*str;
	t_list	*env_iterator;
	
	env_iterator = g_mini.dup_env;
	while (env_iterator->content)
	{
		str = ft_strnstr(env_iterator->content, "PATH=", 5);
		if (str != NULL)
			break ;
		env_iterator = env_iterator->next;
	}
	return (env_iterator->content);
}

/*
*	In this function we split the string into several directories by the de-
*	limiter ":". This is then stored in a 2D array of strings.
*/
char	**ft_find_paths(char *path)
{
	char	**paths;
	char	*path_temp;
	int		i;

	i = 0;
	paths = ft_split(path, ':');
	while (paths[i] != NULL)
	{
		path_temp = ft_strjoin(paths[i], "/");
		free(paths[i]);
		paths[i] = path_temp;
		i++;
	}
	return (paths);
}

/*
* compares each token of a cmd with the paths for linux commands or 
* our own builtins;
* appends the token name to each available path, then checks with access
* if the path exists;
* path = the content of env variable PATH;
* paths = the single paths contained in PATH splitted into individual
* strings pointed to by a double pointer;
* access permission modes: F_OK = the existence test; X_OK = execute/search permission;
* *cmd is the command list, *iterator is the toks list;
*/
int ft_find_command(t_cmd *cmd, t_list *iterator)
{
	char	*path;
	char	**paths;
	char	*temp_str;
	int		i;

	i = 0;
	path = ft_find_path();
	paths = ft_find_paths(path);
	// DELETE QUOTES ETC. --> we'll test if execve can handle them.
	ft_remove_quotes(iterator->content);
	while (paths[i])
	{
		temp_str = ft_strjoin(paths[i], iterator->content);
		if (ft_is_built_in(iterator->content) == 1)
			cmd->is_built_in = 1;
		else if (access(temp_str, F_OK | X_OK) == 0)
		{
			cmd->command_path = temp_str;
			break ;
		}
		i++;
	}
	if (paths[i] == NULL)
	{
		free(temp_str);
		return (1);
	}
	return (0);
}

/*
* is called if a command was found in the toks of a cmd struct;
* stores the following toks in char**arguments;
*/
void	ft_store_arguments(t_cmd *cmd, t_list *toks)
{
	int	i;

	i = 0;
	cmd->arguments = ft_calloc(10000, sizeof(char));
	if (!cmd->arguments)
		printf("calloc error ft_store_args\n");
	while (toks)
	{
		ft_remove_quotes(toks->content);
		cmd->arguments[i] = toks->content;
		toks = toks->next;
		i++;
	}
	cmd->arguments[i] = NULL;
}
/* *********************************/

void ft_cd_empty(void)
{
	int	check;
	
	printf("hello\n");
	check = chdir(getenv("HOME"));
	printf("check; %d\n", check);
}


// t_list	*sort_list(t_list* lst, int (*cmp)(int, int))
// {
// 	int swap;
// 	t_list *tmp;

// 	tmp = lst;
// 	while(lst->next != 0)
// 	{
// 		if ((*cmp)(lst->data, lst->next->data) == 0)
// 		{
// 			swap = lst->data;
// 			lst->data = lst->next->data;
// 			lst->next->data = swap;
// 			lst = tmp;
// 		}
// 		else
// 		{
// 			lst = lst->next;
// 		}
// 	}
// 	lst = tmp;
// 	return (lst);
// }

void	ft_print_sorted_env(t_list *dup_env)
{  // swap content version

	t_list	*iterator;
	char	*temp_cont;

	iterator = dup_env;
	while (iterator && iterator->next)
	{
		if (strcmp(iterator->content, iterator->next->content) > 0 /*&& iterator == *dup_env*/)
		{
			temp_cont = iterator->content;
			iterator->content = iterator->next->content;
			iterator->next->content = temp_cont;
			iterator = dup_env;
		}
		else
			iterator = iterator->next;
	}
	iterator = dup_env;
	while (iterator)
	{
		printf("%s\n", iterator->content);
		iterator = iterator->next;
	}
}

void	ft_export_exec(t_list *toks)
{
	if (!toks->next)
		ft_print_sorted_env(g_mini.dup_env);
	else
	{
		
	}
		ft_lstadd_back(&g_mini.dup_env, ft_lstnew(toks->next->content));
}

void ft_execute_built_in(t_cmd *cmd, t_list *toks)
{
	(void)cmd;

	printf("Entered the built_in execution.\n");
	if (ft_is_pwd(toks->content))
		ft_pwd_exec();
	if (ft_is_env(toks->content))
		ft_env_exec();
	if (ft_is_unset(toks->content))
		ft_unset_exec(toks);
	if (ft_is_exit(toks->content))
		ft_exit_exec(toks);
	if (ft_is_export(toks->content))
		ft_export_exec(toks);


	
	if (ft_is_cd(toks->content)) /*&& !cmd->arguments[1]*/ // cant be checked because of immedeate abort after calling
	{
		printf("cd_empty.\n");
		ft_cd_empty();
	}
	// else if (ft_is_cd(toks->content) && cmd->arguments[1] == '.' && !cmd->arguments[2])
	// {
	// 	ft_cd_dot()
	// }
	// else if (ft_is_cd(toks->content) && cmd->arguments[1] == '.' && !cmd->arguments[2] == '.')
	// {
	// 	ft_cd_two_dots();
	// }
	// else if (ft_is_cd(toks->content) && cmd->arguments[1])
	// {
	// 	ft_cd_file(cmd->arguments[1]);
	// }
}

/* ***************************** */
/*
* gos through the individual tokens of each cmd and searches 
* for a command (e.g echo, export..);
* if command was found it stores the arguments of the command
* (= the tokens following) in the cmd structs char** arguments;
*/
void	ft_interpret(void)
{
	t_cmd	*cmd_iterator;
	t_list	*tok_iterator;

	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		tok_iterator = cmd_iterator->toks;
		while (tok_iterator)
		{
			if (ft_find_command(cmd_iterator, tok_iterator) == 0)
			{
				ft_store_arguments(cmd_iterator, tok_iterator);
				// if (cmd_iterator->is_built_in == 1)
				// {
				// 	ft_execute_built_in(cmd_iterator, tok_iterator);
				// }
				break ;
			}
			if (!cmd_iterator->command_path && cmd_iterator->is_built_in == 0)
			{
				printf("minishell: %s: command not found\n", \
						cmd_iterator->toks->content);
				break ;
			}
			tok_iterator = tok_iterator->next;
		}
		cmd_iterator = cmd_iterator->next;
	}
	ft_print_cmds(g_mini.cmds);
}
