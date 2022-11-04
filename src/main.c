/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:34:58 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/21 17:18:579 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_mini g_mini;

void	ft_init_pipefd(int nbr_of_pipes)
{
	int	i;
	int	*fds;

	i = 0;
	g_mini.pipefd = malloc(sizeof(int *) * nbr_of_pipes + 1);
	if (!g_mini.pipefd)
	{
		printf("malloc failed.\n");
		exit_program(1);
	}
	while (nbr_of_pipes > 0)
	{
		fds = malloc(sizeof(int) * 2);
		if (!fds)
		{
			printf("malloc failed.\n");
			exit_program(1);
		}
		fds[0] = 0;
		fds[1] = 1;
		// printf("fds[0:1]: %d, %d\n", fds[0], fds[1]);
		g_mini.pipefd[i] = fds;
		// printf("pipefd[%d][0]/[%d][1]: %d/%d\n", i, i, g_mini.pipefd[i][0], g_mini.pipefd[i][1]);
		nbr_of_pipes--;
		i++;
	}
}

/*
	We need to execute the commands simultaneously, so create pipes before forking.
	pipe command stores an int[2] in the given parameter. We store it in "int *pipefd[i]";

*/
void	ft_set_pipes(void)
{
	int	nbr_of_pipes;
	int	pipe_check;
	int	i;

	i = 0;
	pipe_check = 0;
	nbr_of_pipes = ft_lstsize_cmds(g_mini.cmds) - 1; // We need in total n_cmds - 1 pipes
	g_mini.nbr_of_pipes = nbr_of_pipes;
	printf("nbr of pipes: %d\n", nbr_of_pipes);
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
		printf("pipefd: %d\n", g_mini.pipefd[i][0]); // printing reading end of pipe.
		printf("pipefd: %d\n", g_mini.pipefd[i][1]); // printing writing end of pipe.
		nbr_of_pipes--;
		i++;
	}
}

/*
j == 0 input
j == 1 output
*/
void	ft_open_file(char *file_name, int *fd, int j, int open_flag)
{
	int file_check;

	if (j == 0)
	{
		file_check = access(file_name, R_OK | F_OK);
		if (file_check != 0)
		{
			printf("bash: %s: No such file or directory\n", file_name);
			exit_program(1);
		}
		*fd = open(file_name, O_RDWR, 0777);
		if (*fd == -1)
		{
			printf("bash: %s: Permission denied\n", file_name);
			exit_program(1);
		}
	}
	else if (j == 1 && open_flag == 0)
	{
		*fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0777);
		if (*fd == -1)
		{
			printf("bash: %s: Permission denied\n", file_name);
			exit_program(1);
		}
	}
	else if (j == 1 && open_flag == 1)
	{
		*fd = open(file_name, O_CREAT | O_RDWR | O_APPEND, 0777);
		if (*fd == -1)
		{
			printf("bash: %s: Permission denied\n", file_name);
			exit_program(1);
		}
	}
}

void	ft_set_files(void)
{
	t_cmd	*cmd_iterator;
	int		i;

	i = 0;
	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		if (cmd_iterator->input_file != NULL)
			ft_open_file(cmd_iterator->input_file, &cmd_iterator->fd_in, 0, -1);
		else if (cmd_iterator->output_file != NULL)
			ft_open_file(cmd_iterator->output_file, &cmd_iterator->fd_out, 1, cmd_iterator->open_flag);
		i++;
	}
}

/*
* creates individual processes for each command;
*/
void	ft_test(void)
{
	int		i;
	int		pid;
	t_cmd	*cmd_iterator;

	i = 0;
	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		pid = fork();
		if (pid == -1)
		{
			printf("Forking failed.\n");
			perror("Forking: ");
			exit_program(1);
		}
		if (pid == 0)
			ft_execute_process(cmd_iterator, i);
		else
		{
			i++;
			cmd_iterator = cmd_iterator->next;
		}
	}
	waitpid(0, NULL, 0);
	// ft_close_all_fds();
}

void	ft_execute(void)
{
	ft_set_pipes();
	ft_set_files();

}

/*
* copies content of file_1 into file_2;
* file_1 and file_2 are outpufiles[0]/[i]
*/
void	ft_copy_content(char *file_1, char *file_2, int open_flag)
{
	int 	fd_file_1;
	int 	fd_file_2;
	char	*temp;

	fd_file_1 = open(file_1, O_CREAT | O_RDWR, 0777);
	if (fd_file_1 == -1)
	{
		printf("ft_copy_content:error fd_file_1\n");
		close(fd_file_1);
		exit_program(1);
	}
	if (open_flag == 0)
	{
		fd_file_2 = open(file_2, O_CREAT | O_RDWR | O_TRUNC, 0777);
		if (fd_file_2 == -1)
		{
			printf("ft_copy_content:fd_file_2\n");
			close(fd_file_2);
			exit_program(1);
		}
	}
	else if (open_flag == 1)
	{
		fd_file_2 = open(file_2, O_CREAT | O_RDWR | O_APPEND, 0777);
		if (fd_file_2 == -1)
		{
			printf("ft_copy_content:error fd_file_2\n");
			close(fd_file_2);
			exit_program(1);
		}
	}
	while (1)
	{
		temp = get_next_line(fd_file_1);
		if (temp)
		{
			write(fd_file_2, temp, ft_strlen(temp));
			free (temp);
		}
		else
		{
			free (temp);
			break ;
		}
	}
	close(fd_file_1);
	close(fd_file_2);
}

/******************************************************************************************/

/*
* displays a prompt and reads in user input;
* adds input to the history for later acces by user;
* checks for open quotes, backslash and semicolon which 
* are all not allowed in this project
*/
int ft_get_input(void)
{
	//int input_ckeck;
	char	*prompt;

	prompt = "42shell > ";
	g_mini.raw_input = readline(prompt);
	if (!g_mini.raw_input)
		exit_shell_quit(0);
	if (ft_str_only_space(g_mini.raw_input) != 1 && g_mini.raw_input)
		add_history(g_mini.raw_input);
	if (ft_check_quotes(g_mini.raw_input) == 1)
	{
		printf("quotes not closed\n");
		free(g_mini.raw_input);
		exit_program(1);
	}
	if (ft_check_backslash(g_mini.raw_input) == 1)
	{
		printf("backslash forbidden\n");
		free(g_mini.raw_input);
		exit_program(1);
	}
	if (ft_check_semicolon(g_mini.raw_input) == 1)
	{
		printf("semicolon forbidden\n");
		free(g_mini.raw_input);
		exit_program(1);
	}
	return (0);
}

/*
* When launching the shell, we don't launch it with any input()
* so argv should be empty. If it isn't we need to return an error.
*/
int main (int argc, char **argv, char **env)
{
	int input_check;
	(void)argv;
	(void)argc;

	// if (argc != 1)
	// {
	// 	printf("too many commands only ./minishell\n");
	// 	exit_program(EXIT_FAILURE);
	// }
	ft_init_minishell(&g_mini, env);
	while (1)
	{	
		ft_handle_sigint();
		input_check = ft_get_input();
		if (input_check == 2)
			printf("invalid syntax\n");
		else
		{
			if (ft_check_input_validity(g_mini.raw_input) == 0)
				continue ;
			else
			{
				while (ft_dollar_sign(g_mini.raw_input) != 0)
					ft_env_vars(g_mini.raw_input);
				ft_parsing(g_mini.raw_input);
				ft_interpret();
				ft_execute();
				ft_free();
			}
		}
	}
	//ft_env(&g_mini.dup_env); jus testing if env works

	///free_all(g_mini);
	printf("\n");
	return (0);
}
