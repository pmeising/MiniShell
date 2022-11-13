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
* X_OK = is executable rights granted;
* F_OK = zugriffsrights granted;
*/
int ft_is_program(t_cmd *cmd, char *prog_name)
{
	char	*cwd;
	char	*name;
	int		len;
	int		i;

	cwd = malloc(sizeof(char) * 1024);
	i = 2;
	if (prog_name[0] == '.' && prog_name[1] == '/')
	{
		name = ft_strdup(&prog_name[1]); // free in the end.
		cmd->command_path = ft_strjoin(getcwd(cwd, 1024), name);
		free (name);
		free (cwd);
		if (access(cmd->command_path, F_OK | X_OK) == 0)
			return (1);
		else
			cmd->command_path = NULL;
	}
	else if (prog_name[0] == '.' && prog_name[1] == '.' && prog_name[2] == '/')
	{ // /Users/bde-carv/Core/ shelltest/src
		name = ft_strdup(prog_name); // .. /../ bin/ls
		while (name[0] == '.')
		{
			cwd = malloc(sizeof(char) * 1024);
			getcwd(cwd, 1024);
			len = ft_strlen(cwd);
			while (cwd[len] != '/')
				len--;
			cwd[len] = '\0';
			chdir(cwd);
			free (cwd);
			free (name);
			name = ft_strdup(&prog_name[i]);
			i = i + 3;
		}
		cwd = malloc(sizeof(char) * 1024);
		cmd->command_path = ft_strjoin(getcwd(cwd, 1024), name);
		free (cwd);
		if (access(cmd->command_path, F_OK | X_OK) == 0)
			return (1);
		else
			cmd->command_path = NULL;
	}
	return (0);
}

/*
* cuts out . and / from program name
*/
// char	*ft_cut_prgrm(char *content)
// {
// 	int		len;
// 	char	*new;
// 	int		i;
// 	int		j;

// 	i = 1;
// 	j = 0;
// 	len = ft_strlen(content);
// 	new = malloc(sizeof(char) * len);
// 	while (content[i])
// 	{
// 		new[j] = content[i];
// 		i++;
// 		j++;
// 	}
// 	return (new);
// }

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
	char	*temp_2;
	int		i;

	i = 0;
	path = ft_find_path();
	paths = ft_find_paths(path);
	ft_remove_quotes(iterator->content);
	while (paths[i])
	{
		temp_str = ft_strjoin(paths[i], iterator->content);
		if (ft_is_built_in(iterator->content) == 1)
			cmd->is_built_in = 1;
		temp_2 = ft_strjoin(paths[i], "test");
		if (cmd->is_built_in == 1 && (access(temp_2, F_OK | X_OK) == 0))
		{
			cmd->arguments = ft_calloc(1000, sizeof(char));
			if (!cmd->arguments)
				printf("calloc error  ft_find_command\n");
			cmd->arguments[0] = "test";
			cmd->arguments[1] = NULL;
			cmd->command_path = ft_strjoin(paths[i], "test");
			cmd->is_built_in = 1;
			free (temp_2);
			free (temp_str);
			break ;
		}
		else if (ft_is_program(cmd, iterator->content) == 1)
		{
			free (temp_2);
			break ;
		} 
		else if (access(temp_str, F_OK | X_OK) == 0 && cmd->is_built_in == 0)
		{
			cmd->command_path = temp_str;
			free (temp_2);
			break ;
		}
		free (temp_2);
		i++;
	}
	if (paths[i] == NULL)
	{
		free(temp_str);
		return (1);
	}
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free (paths);
	if (!cmd->command_path)
		g_mini.exit_status = 127;
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
	{
		printf("calloc error ft_store_args\n");
		exit_program(1);
	}
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


void	ft_echo_exec(t_cmd *iterator)
{
	t_list	*toks_iterator;
	int		n_flag;

	n_flag = 0;
	if (!iterator->toks->next)
	{
		printf("\n");
		exit_program(1); // back to 42shell;
	}
	toks_iterator = iterator->toks->next;
	if (ft_strncmp(toks_iterator->content, "-n", 2) == 0)
	{
		n_flag = 1;
		toks_iterator = toks_iterator->next;
	}
	while(toks_iterator)
	{
		ft_putstr_fd(toks_iterator->content, 1);
		ft_putstr_fd(" ", 1);
		toks_iterator = toks_iterator->next;
	}
	if (n_flag != 1)
		printf("\n");
	
}

void ft_execute_built_in(t_cmd *cmd, t_list *toks)
{
	(void)cmd;

	// printf("Entered the built_in execution.\n");
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
	if (ft_is_cd(toks->content))
		ft_cd_exec(toks);
	if (ft_is_echo(toks->content))
		ft_echo_exec(cmd);
}

/* ***************************** */
/*
* gos through the individual tokens of each cmd and searches 
* for a command (e.g echo, export..);
* if command was found it stores the command name itself and
* the arguments of the command (= the tokens following) in
* the cmd structs char** arguments;
*/
void	ft_interpret(t_cmd *cmd_iterator)
{
	t_list	*tok_iterator;

	tok_iterator = cmd_iterator->toks;
	while (tok_iterator)
	{
		if (ft_find_command(cmd_iterator, tok_iterator) == 0 && cmd_iterator->is_built_in == 0)
		{
			ft_store_arguments(cmd_iterator, tok_iterator);
			break ;
		}
		if (!cmd_iterator->command_path && cmd_iterator->is_built_in == 0)
		{
			g_mini.exit_status = 127;
			g_mini.exit = 1;
			printf("42_minishell: %s: command not found\n", cmd_iterator->toks->content);
			//printf("42_minishell: %s: command not found\n", g_mini.raw_input);
			// printf("42_minishell: %d: command not found\n", g_mini.exit_status);


			// if (g_mini.exit_status == 127 && ft_strlen(g_mini.raw_input) < 2)
			// 	dprintf(2, "42_minishell: %d: command not found\n", g_mini.exit_status);
			// else
			// 	printf("42_minishell: %s: command not found\n", cmd_iterator->toks->content);
		}
		tok_iterator = tok_iterator->next;
	}
	// if (g_mini.exit == 1)
	// 	exit_program(1);
}
