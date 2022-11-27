/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:28:28 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/27 20:52:22 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
	Walks through the int array ant frees
	each position.
	Also freeing array pointer.
*/
void	ft_free_fds(void)
{
	int	i;

	i = 0;
	while (g_mini.pipefd && i < g_mini.nbr_of_pipes)
	{
		if (g_mini.pipefd)
		{
			close(g_mini.pipefd[i][0]);
			close(g_mini.pipefd[i][1]);
			free(g_mini.pipefd[i]);
		}
		i++;
	}
	if (g_mini.nbr_of_pipes > 0)
		free (g_mini.pipefd);
}

void	ft_free_helper(t_cmd *cmd_iterator)
{
	ft_free_lst_cont(cmd_iterator->toks);
	if (cmd_iterator->heredoc_temp && cmd_iterator->input_file)
	{
		if (ft_strncmp(cmd_iterator->heredoc_temp, \
			cmd_iterator->input_file, \
			ft_strlen(cmd_iterator->heredoc_temp)) == 0)
			unlink(cmd_iterator->heredoc_temp);
	}
	if (cmd_iterator->command_path)
		ft_free_str(&cmd_iterator->command_path);
	if (cmd_iterator->input_file)
		ft_free_str(&cmd_iterator->input_file);
	if (cmd_iterator->output_file)
		ft_free_str(&cmd_iterator->output_file);
	if (cmd_iterator->heredoc_delim)
		ft_free_str(&cmd_iterator->heredoc_delim);
	if (cmd_iterator->arguments)
		ft_free_str(cmd_iterator->arguments);
}

/*
	Frees each and every malloced location on the heap.
	Only frees everything at exit of minishell, keeps env
	if minishell was not terminated yet.
*/
void	ft_free_input(void)
{
	t_cmd	*cmd_iterator;
	t_cmd	*temp;

	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		ft_free_helper(cmd_iterator);
		temp = cmd_iterator->next;
		free (cmd_iterator);
		cmd_iterator = temp;
	}
	ft_free_helper_1();
	if (g_mini.exit_status == 2 || g_mini.exit == 1)
	{
		if (g_mini.dup_env)
			ft_free_lst_cont(g_mini.dup_env);
		if (g_mini.special_flag > 0)
			free (g_mini.env);
		exit(g_mini.exit_status);
	}
	g_mini.exit = 0;
}

/*
	Walks through a list and frees its content.
*/
void	ft_free_lst_cont(t_list *iterator)
{
	t_list	*temp;

	while (iterator)
	{
		temp = iterator->next;
		if (iterator->content != NULL)
		{
			free (iterator->content);
			iterator->content = NULL;
		}
		if (iterator)
			free (iterator);
		iterator = temp;
	}
	iterator = NULL;
}

/*
	Organizes the exit calls of the program.
	If 1, keep minishell running,
	if 2 terminate minishell.
*/
void	exit_program(int status)
{
	g_mini.exit = 1;
	g_mini.exit_status = status;
	if (status == 2)
	{
		printf("exit_program\n");
		ft_free_input();
		exit (EXIT_FAILURE);
	}
	else
		ft_free_input();
}
