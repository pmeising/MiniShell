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

/*
* creates individual processes for each command;
*/
void	ft_test(void)
{
	int		i;
	int		j;
	int		status;
	int		*pid;
	t_cmd	*cmd_iterator;

	i = 0;
	pid = malloc(sizeof(int) * g_mini.nbr_of_pipes + 1);
	if (!pid)
	{
		printf("ft_test: malloc error\n");
		exit_program(2);
	}
	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		pid[i] = fork();
		if (pid[i] == -1)
		{
			printf("Forking failed.\n");
			perror("Forking: ");
			exit_program(1);
		}
		if (pid[i] == 0)
		{
			printf("Hello from child process.\n");
			ft_execute_process(cmd_iterator, i);
		}
		else
		{
			i++;
			cmd_iterator = cmd_iterator->next;
		}
	}
	j = 0;
	while ((waitpid(pid[j], &status, 1) || waitpid(pid[j], &status, 0)) && i > 0)
	{
		// if (WIFSIGNALED(status))
		i--;
		j++;
	}
	// sleep(1);
	// waitpid(-1, NULL, 0);
	printf("Main.\n");
	ft_close_fds(-1, -1, -1);
}

void	ft_execute(void)
{
	ft_set_pipes();
	ft_set_files();
	ft_print_cmds(g_mini.cmds);
	ft_test();
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
	char	*temp;

	prompt = "42shell > ";
	temp = readline(prompt);
	if (temp)
		g_mini.raw_input = ft_strdup(temp);
	free (temp);
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
