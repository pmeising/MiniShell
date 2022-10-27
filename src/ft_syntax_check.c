/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 19:29:13 by pmeising          #+#    #+#             */
/*   Updated: 2022/10/25 19:35:11 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

// The program receives 4 arguments. Like so: "./pipex file1 cmd1 cmd2 file2"
// This syntax check function tries to open the arguments 1 and 4 which are 
// file path names. If the open fails (because the file doesn't exist) the 
// program throws an error.
// For the arguments 2 and 3, the expected input is a Shell command like "ls"
// or "cat" etc. Here we need to make sure, that this command actually
// represents a valid command with valid flags and syntaxes.
// The finding of the binary executables is done in here. If the binary does
// not exist, it means, that the command is not valid. 
// l.28: chmod access all. 0777 is an octal
// l.140:  // Stores the PATH variable of env
// l.141: // Stores each directory in a 2D array
// l. 142:  // Limits the amount of calls to locate the binary to the amount
// of cmds in the function call arguments.

void	ft_files_check(t_prgrm *vars)
{
	vars->file_2 = open(vars->argv[vars->argc - 1], O_CREAT | O_RDWR | O_TRUNC,
			0777);
	if (access(vars->argv[1], F_OK) == -1)
		ft_error(vars, 4);
	vars->file_1 = open(vars->argv[1], O_RDWR);
	if (vars->file_1 == -1 && vars->file_2 == -1)
	{
		ft_helper_0(vars);
		vars->file_1_nok = 1;
		vars->file_2_nok = 1;
	}
	else if (vars->file_1 == -1)
	{
		ft_printf("%s: ", vars->argv[1]);
		ft_printf("No such file or directory\n");
		vars->file_1_nok = 1;
	}
	else if (vars->file_2 == -1)
	{
		ft_printf("%s: ", vars->argv[vars->argc - 1]);
		ft_printf("No such file or directory\n");
		vars->file_2_nok = 1;
	}
	if (vars->file_1 == -1 || vars->file_2 == -1)
		vars->eflag = 1;
}

// Here I find the correct variable from the "env" command and store it in a 
// single string.

char	*ft_find_path(t_prgrm *vars)
{
	int		i;
	char	*str;

	i = 0;
	while (vars->envp[i] != NULL)
	{
		str = ft_strnstr(vars->envp[i], "PATH=", 5);
		if (str != NULL)
			break ;
		i++;
	}
	return (vars->envp[i]);
}

// In this function I split the string into several directories by the de-
// limiter ":". This is then stored in a 2D array of strings.

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

// Here I make a "fake" file, by adding the cmd1 / cmd2 to the end of the
// file path. This way I then check if it is accessible using the access
// function. If it is, I found the correct directory, if it isn't, we have
// to keep iterating through the 2D array of strings.

char	*ft_locate_binaries(t_prgrm *vars, char *cmd, char **paths, int j)
{
	int		i;
	char	*temp;

	i = 0;
	ft_helper_1(vars, cmd, j);
	if (access(vars->main_commands[0], F_OK | X_OK) == 0)
		return (ft_strdup(vars->main_commands[0]));
	else
	{
		while (paths[i] != NULL)
		{
			temp = ft_strjoin(paths[i], vars->main_commands[0]);
			if (access(temp, F_OK | X_OK) == 0)
				return (temp);
			free (temp);
			i++;
		}
	}
	ft_printf("%s: ", vars->argv[j + 2]);
	if (ft_strchr(vars->argv[j + 2], '/') == NULL)
		ft_printf("command not found\n");
	else
		ft_printf("No such file or directory\n");
	vars->eflag = 2;
	return (NULL);
}

void	ft_syntax_check_bonus(t_prgrm *vars)
{
	char	*path;
	char	**paths;
	int		i;

	i = 0;
	if (vars->argc < 5)
		ft_error(vars, 5);
	ft_files_check(vars);
	vars->cmd_paths = malloc(10000);
	path = ft_find_path(vars);
	paths = ft_find_paths(path);
	while ((i + 1) <= vars->argc - 3)
	{
		vars->cmd_paths[i] = ft_locate_binaries(vars, vars->argv[i + 2],
				paths, i);
		if (vars->eflag == 1)
			free(vars->cmd_paths[i]);
		vars->cmd_args_ptr[i] = vars->main_commands;
		i++;
	}
	vars->cmd_paths[i] = NULL;
	ft_helper_2(vars, paths);
}
