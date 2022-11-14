/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:28:28 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/14 18:44:47 by bde-carv         ###   ########.fr       */
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
	while (i < g_mini.nbr_of_pipes)
	{
		close(g_mini.pipefd[i][0]);
		close(g_mini.pipefd[i][1]);
		free(g_mini.pipefd[i]);
		i++;
	}
	free (g_mini.pipefd);
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
		ft_free_lst_cont(cmd_iterator->toks);
		if (cmd_iterator->heredoc_temp && cmd_iterator->input_file)
		{
			if (ft_strncmp(cmd_iterator->heredoc_temp, cmd_iterator->input_file, ft_strlen(cmd_iterator->heredoc_temp)) == 0)
				unlink(cmd_iterator->heredoc_temp);
		}
		if (cmd_iterator->command_path) 
			free(cmd_iterator->command_path);
		if (cmd_iterator->input_file)
			free(cmd_iterator->input_file);
		if (cmd_iterator->output_file)
			free(cmd_iterator->output_file);
		if (cmd_iterator->HEREDOC_DELIM)
			free(cmd_iterator->HEREDOC_DELIM);
		if (cmd_iterator->arguments)
			free (cmd_iterator->arguments);
		temp = cmd_iterator->next;
		free (cmd_iterator);
		cmd_iterator = temp;
	}
	free (g_mini.raw_input);
	g_mini.cmds = NULL;
	g_mini.nbr_heredocs = 0;
	ft_free_fds();
	if (g_mini.exit_status == 2 || g_mini.exit == 1)
	{
		ft_free_lst_cont(g_mini.dup_env);
		exit(g_mini.exit_status);	
	}
}

/*
	Walks through a list and frees its content.
*/
void ft_free_lst_cont(t_list *iterator)
{
	t_list	*temp;

	while (iterator)
	{
		temp = iterator->next;
		free (iterator);
		iterator = temp;
	}
}

/*
	Organizes the exit calls of the program.
	If 1, keep minishell running,
	if 2 terminate minishell.
*/
void exit_program(int status)
{
	g_mini.exit = 1;
	g_mini.exit_status = status;
	if (status == 2)
	{
		ft_free_input();
		exit (EXIT_FAILURE);
	}
	else
		ft_free_input();
}
