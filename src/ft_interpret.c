/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_interpret.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 18:33:38 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/26 19:56:01 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* X_OK = is executable rights granted;
* F_OK = zugriffsrights granted;
*/
int	ft_is_program(t_cmd *cmd, char *prog_name)
{
	int		i;

	i = 2;
	if (prog_name[0] == '.' && prog_name[1] == '/')
	{
		if (ft_is_prog_help_1(cmd, prog_name) == 1)
			return (1);
		else
			return (0);
	}
	else if (prog_name[0] == '.' && prog_name[1] == '.' && prog_name[2] == '/')
	{
		ft_is_prog_help_2(cmd, prog_name, i);
		if (access(cmd->command_path, F_OK | X_OK) == 0)
			return (1);
		else
			ft_free_str(&cmd->command_path);
	}
	return (0);
}

/*
* helper function for freeing paths;
*/
void	ft_free_path_at_i(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		if (paths[i])
		{
			free(paths[i]);
			paths[i] = NULL;
		}
		i++;
	}
	if (paths)
	{
		free (paths);
		paths = NULL;
	}
}

/*
* compares each token of a cmd with the paths for linux commands or 
* our own builtins;
* appends the token name to each available path, then checks with access
* if the path exists;
* path = the content of env variable PATH;
* paths = the single paths contained in PATH splitted into individual
* strings pointed to by a double pointer;
* access permission modes: F_OK = the existence test;
* X_OK = execute/search permission;
* *cmd is the command list, *iterator is the toks list;
*/
int	ft_find_command(t_cmd *cmd, t_list *tok)
{
	char	*path;
	char	**paths;
	int		j;

	j = 0;
	path = ft_find_path();
	paths = ft_find_paths(path);
	if (!paths)
		return (ft_find_command_1(cmd, tok, paths));
	ft_remove_quotes(tok->content);
	if (access(tok->content, F_OK | X_OK) == 0)
		return (ft_find_command_2(cmd, tok, paths));
	if (ft_is_built_in(tok->content) == 1)
		cmd->is_built_in = 1;
	if (cmd->is_built_in == 0)
		j = ft_find_command_3(cmd, tok, paths);
	ft_free_path_at_i(paths);
	if (!cmd->command_path)
		g_mini.exit_status = 127;
	return (j);
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
		perror("calloc: ");
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
		if (ft_find_command(cmd_iterator, tok_iterator) == 0 && \
			cmd_iterator->is_built_in == 0)
		{
			ft_store_arguments(cmd_iterator, tok_iterator);
			break ;
		}
		if (!cmd_iterator->command_path && cmd_iterator->is_built_in == 0)
		{
			g_mini.exit_status = 127;
			g_mini.exit = 1;
			printf("42shell: %s: command not found\n", \
			cmd_iterator->toks->content);
			break ;
		}
		tok_iterator = tok_iterator->next;
	}
}
