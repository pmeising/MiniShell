/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:39:14 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/27 19:54:33 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_double_char(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i])
		{
			free (str[i]);
			str[i] = NULL;
		}
		i++;
	}
	free (str);
}

/*
* updates the env list after using unset or export;
*/
void	ft_overwrite_env(void)
{
	t_list	*iterator;
	int		i;

	i = 0;
	iterator = g_mini.dup_env;
	if (g_mini.special_flag > 0 && g_mini.env)
	{
		free(g_mini.env);
		g_mini.env = NULL;
	}
	g_mini.env = malloc(sizeof(char) * 10000 * ft_lstsize(iterator));
	while (iterator)
	{
		g_mini.env[i] = iterator->content;
		iterator = iterator->next;
		i++;
	}
	g_mini.env[i] = NULL;
}

void	ft_execute_process_2(t_cmd *cmd_iterator, int *pid)
{
	if (g_mini.exit == 1)
	{
		if (g_mini.nbr_heredocs > 0)
		{
			ft_close_fds(-1, -1, -1);
			unlink(cmd_iterator->heredoc_temp);
		}
		ft_close_fds(-1, -1, -1);
		free (pid);
		ft_free_input();
	}
	if (cmd_iterator->heredoc_temp && \
		ft_strcmp(cmd_iterator->input_file, cmd_iterator->heredoc_temp) != 0)
		unlink(cmd_iterator->heredoc_temp);
}

/*
* i = cmd counter;
* unink: deletes a file if no process has it open
*/
void	ft_execute_process(t_cmd *cmd_iterator, int i, int *pid)
{
	ft_interpret(cmd_iterator);
	ft_execute_process_2(cmd_iterator, pid);
	if (cmd_iterator->input_file != NULL)
		dup2(cmd_iterator->fd_in, STDIN_FILENO);
	else if (cmd_iterator->input_file == NULL && i > 0)
		dup2(g_mini.pipefd[i - 1][0], STDIN_FILENO);
	if (cmd_iterator->output_file != NULL)
		dup2(cmd_iterator->fd_out, STDOUT_FILENO);
	else if (cmd_iterator->output_file == NULL && i < g_mini.nbr_of_pipes)
		dup2(g_mini.pipefd[i][1], STDOUT_FILENO);
	ft_close_fds(cmd_iterator->fd_in, cmd_iterator->fd_out, i);
	if (cmd_iterator->is_built_in == 1)
	{
		ft_execute_built_in(cmd_iterator, cmd_iterator->toks);
		free (pid);
		ft_free_input_built_in();
	}
	execve(cmd_iterator->command_path, cmd_iterator->arguments, g_mini.env);
	dup2(1, STDOUT_FILENO);
	perror("42shell: ");
	exit (127);
}
