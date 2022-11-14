/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:39:14 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/12 16:38:114 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_overwrite_env(void)
{
	t_list	*iterator;
	int		i;

	i = 0;
	iterator = g_mini.dup_env;
	g_mini.env = malloc(sizeof(char) * 10000 * ft_lstsize(iterator));
	while (iterator)
	{
		g_mini.env[i] = iterator->content;
		iterator = iterator->next;
		i++;
	}
}

void	ft_close_fds(int in, int out, int nbr)
{
	int		i;
	t_cmd	*iterator;

	(void)nbr;
	i = 0;
	iterator = g_mini.cmds;
	while (iterator)
	{
		if (iterator->input_file && iterator->fd_in != in)
			close(iterator->fd_in);
		if (iterator->output_file && iterator->fd_out != out)
			close(iterator->fd_out);
		if (iterator->heredoc_temp)
			unlink(iterator->heredoc_temp);
		iterator = iterator->next;
	}
	if (g_mini.nbr_of_pipes > 0)
	{
		while (i < g_mini.nbr_of_pipes && g_mini.pipefd[i])
		{
			if (g_mini.pipefd[i][0] != in)
				close(g_mini.pipefd[i][0]);
			if (g_mini.pipefd[i][1] != out)
				close(g_mini.pipefd[i][1]);
			i++;
		}
	}
}

/*
	i = cmd counter;
	unink: deletes a file if no process has it open
*/
void	ft_execute_process(t_cmd *cmd_iterator, int i)
{
	ft_interpret(cmd_iterator);
	if (g_mini.exit == 1)
	{
		if (g_mini.nbr_heredocs > 0)
		{
			ft_close_fds(-1, -1, -1);
			unlink(cmd_iterator->heredoc_temp);
		}
		ft_close_fds(-1, -1, -1);
		exit(g_mini.exit_status);
	}
	if (cmd_iterator->heredoc_temp && \
		ft_strcmp(cmd_iterator->input_file, cmd_iterator->heredoc_temp) != 0)
		unlink(cmd_iterator->heredoc_temp);
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
		exit(0);
	}
	execve(cmd_iterator->command_path, cmd_iterator->arguments, g_mini.env);
	dup2(1, STDOUT_FILENO);
}

/*
* initializes the **pipefd in the g_mini struct;
* each *int in **pipefd is initialized with two ints
* that have the default values of 0 at [0] and 1 at [1];
* 
*/
void	ft_init_pipefd(int nbr_of_pipes)
{
	int	i;
	int	*fds;

	i = 0;
	g_mini.pipefd = malloc(sizeof(int *) * nbr_of_pipes + 1);
	if (!g_mini.pipefd)
	{
		printf("ft_init_pipe: malloc failed.\n");
		exit_program(1);
	}
	while (nbr_of_pipes > 0)
	{
		fds = malloc(sizeof(int) * 2);
		if (!fds)
		{
			printf("ft_init_pipe: malloc failed.\n");
			exit_program(1);
		}
		fds[0] = 0;
		fds[1] = 1;
		g_mini.pipefd[i] = fds;
		nbr_of_pipes--;
		i++;
	}
}

/*
* We need to execute the commands simultaneously, so create pipes before forking.
* pipe command stores an int[2] in the given parameter. We store it in "int *pipefd[i]";
* counts number of pipes;
* initializes **pipefd in the g_mini struct;
* pipe(): creates the pipe and puts the corresponding
* fd into the **pipefd[i]
* We need in total n_cmds - 1 pipes
*/
void	ft_set_pipes(void)
{
	int	nbr_of_pipes;
	int	pipe_check;
	int	i;

	i = 0;
	pipe_check = 0;
	nbr_of_pipes = ft_lstsize_cmds(g_mini.cmds) - 1;
	g_mini.nbr_of_pipes = nbr_of_pipes;
	ft_init_pipefd(nbr_of_pipes);
	while (nbr_of_pipes > 0)
	{
		pipe_check = pipe(g_mini.pipefd[i]);
		if (pipe_check == -1)
		{
			printf("Pipe couldn't be opened.\n");
			perror("pipe: ");
			exit_program(1);
		}
		nbr_of_pipes--;
		i++;
	}
}
