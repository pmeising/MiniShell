/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:40:53 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/14 19:28:59 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_open_file_2(char *file_name, int *fd, int j, int open_flag)
{
	if (j == 1 && open_flag == 0)
	{
		*fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0777);
		if (*fd == -1)
		{
			printf("42_shell: %s: Permission denied\n", file_name);
			g_mini.exit_status = 1;
		}
	}
	else if (j == 1 && open_flag == 1)
	{
		*fd = open(file_name, O_CREAT | O_RDWR | O_APPEND, 0777);
		if (*fd == -1)
		{
			printf("42_shell: %s: Permission denied\n", file_name);
			g_mini.exit_status = 1;
		}
	}
}

/*
	Checks with access() if the file_name exist;
	if yes, opens and connects it to the fd_in/fd_out
	filedescriptor in the cmd-struct;
	j == 0 inputfile/write
	j == 1 outputfile/read
*/
void	ft_open_file(char *file_name, int *fd, int j, int open_flag)
{
	int	file_check;

	if (j == 0)
	{
		file_check = access(file_name, F_OK);
		if (file_check != 0)
		{
			printf("42_minishell: %s: No such file or directory\n", file_name);
			g_mini.exit_status = 1;
		}
		*fd = open(file_name, O_RDWR, 0777);
		if (*fd == -1)
		{
			printf("42_minishell: %s: Permission denied\n", file_name);
			g_mini.exit_status = 1;
		}
	}
	else
		ft_open_file_2(file_name, fd, j, open_flag);
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
	t_cmd	*cmd;
	int		i;

	i = 0;
	cmd = g_mini.cmds;
	while (cmd)
	{
		if (cmd->input_file == NULL && i != 0)
			cmd->fd_in = (i * 2) + 1;
		if (cmd->output_file == NULL && i < g_mini.nbr_of_pipes)
			cmd->fd_out = (i * 2) + 4;
		i++;
		cmd = cmd->next;
	}
	cmd = g_mini.cmds;
	while (cmd)
	{
		if (cmd->input_file != NULL)
			ft_open_file(cmd->input_file, &cmd->fd_in, 0, -1);
		if (cmd->output_file != NULL)
			ft_open_file(cmd->output_file, &cmd->fd_out, 1, cmd->open_flag);
		cmd = cmd->next;
	}
}
