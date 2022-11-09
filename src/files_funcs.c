/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:40:53 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/09 19:58:13 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* j == 0 inputfile
* j == 1 outputfile
* checks with access() if the file_name exist;
* if yes, opens and connects it to the fd_in/fd_out
* filedescriptor in the cmd-struct;
*/
void	ft_open_file(char *file_name, int *fd, int j, int open_flag)
{
	int file_check;

	if (j == 0)
	{
		file_check = access(file_name, F_OK);
		if (file_check != 0)
		{
			printf("minishell: %s: No such file or directory\n", file_name);
			g_mini.exit_status = 1;
		}
		*fd = open(file_name, O_RDWR, 0777);
		if (*fd == -1)
		{
			printf("minishell: %s: Permission denied\n", file_name);
			g_mini.exit_status = 1;
		}
	}
	else if (j == 1 && open_flag == 0)
	{
		*fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0777);
		if (*fd == -1)
		{
			printf("minishell: %s: Permission denied\n", file_name); // could be fused together ?
			g_mini.exit_status = 1;
		}
	}
	else if (j == 1 && open_flag == 1)
	{
		*fd = open(file_name, O_CREAT | O_RDWR | O_APPEND, 0777);
		if (*fd == -1)
		{
			printf("minishell: %s: Permission denied\n", file_name);
			g_mini.exit_status = 1;
		}
	}
	if (g_mini.exit_status != 0)
		g_mini.exit = 1;
}

/*
* checks if we have input or output files (redirects) in
* a command;
* if yes, calls ft_open_file to open the respective file;
*/
void	ft_set_files(void)
{
	t_cmd	*cmd_iterator;
	int		i;

	i = 0;
	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		printf("i: %d\n", i);
		if (cmd_iterator->input_file == NULL && i != 0)
			cmd_iterator->fd_in = (i * 2) + 1;
		if (cmd_iterator->output_file == NULL && i < g_mini.nbr_of_pipes)
			cmd_iterator->fd_out = (i * 2) + 4;
		i++;
		cmd_iterator = cmd_iterator->next;
	}
	i = 0;
	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		if (cmd_iterator->input_file != NULL)
			ft_open_file(cmd_iterator->input_file, &cmd_iterator->fd_in, 0, -1);
		if (cmd_iterator->output_file != NULL)
			ft_open_file(cmd_iterator->output_file, &cmd_iterator->fd_out, 1, cmd_iterator->open_flag);
		i++;
		cmd_iterator = cmd_iterator->next;
	}
}
