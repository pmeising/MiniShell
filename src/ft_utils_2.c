/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 19:22:21 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/27 19:54:16 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_fork_1(t_cmd *cmd_iterator, int *pid, int i)
{
	if (cmd_iterator->toks && cmd_iterator->toks->content && \
		(ft_is_export(cmd_iterator->toks->content) == 1 || \
		ft_is_cd(cmd_iterator->toks->content) == 1 || \
		ft_is_unset(cmd_iterator->toks->content) == 1) && \
		g_mini.nbr_of_pipes == 0)
	{
		ft_execute_built_in(cmd_iterator, cmd_iterator->toks);
		ft_overwrite_env();
		g_mini.special_flag++;
	}
	else
	{
		pid[i] = fork();
		if (pid[i] == -1)
		{
			perror("Fork: ");
			exit_program(1);
		}
		if (pid[i] == 0)
			ft_execute_process(cmd_iterator, i, pid);
	}
}

void	ft_fork_2(int *pid)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < g_mini.nbr_of_pipes + 1)
	{
		(waitpid(pid[i], &status, 0));
		if (status == 6)
			g_mini.exit_status = 0;
		else
		{
			if (WIFSIGNALED(status))
				g_mini.exit_status = 130;
			else
				g_mini.exit_status = status / 256;
		}
		i++;
	}
}

void	ft_check_malloc_1(int *pid)
{
	if (!pid)
	{
		perror("calloc: ");
		exit_program(1);
	}
}

/*
*	creates individual processes for each command;
*	1 == true;
*/
void	ft_fork(void)
{
	int		i;
	int		*pid;
	t_cmd	*cmd_iterator;

	i = 0;
	pid = ft_calloc(sizeof(int *), g_mini.nbr_of_pipes + 1);
	ft_check_malloc_1(pid);
	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		ft_fork_1(cmd_iterator, pid, i);
		i++;
		cmd_iterator = cmd_iterator->next;
	}
	ft_close_fds(-1, -1, -1);
	ft_fork_2(pid);
	free (pid);
	if (g_mini.exit_status != 0 && g_mini.exit == 1)
	{
		ft_free_fds();
		exit(g_mini.exit_status);
	}
}

void	ft_execute(void)
{
	ft_set_pipes();
	ft_set_files();
	if (g_mini.exit != 7)
		ft_fork();
}
