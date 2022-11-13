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

int	ft_is_special_built(char *arguments)
{
	if (ft_is_export(arguments) == 1 || ft_is_cd(arguments) == 1 || ft_is_unset(arguments) == 1)
		return (1);
	return (0);
}

/*
*	creates individual processes for each command;
*/
void	ft_test(void)
{
	int		i;
	int		*pid;
	int		status;
	t_cmd	*cmd_iterator;

	i = 0;
	pid = ft_calloc(sizeof(int), g_mini.nbr_of_pipes + 1);
	if (!pid)
	{
		printf("ft_test: calloc error\n");
		exit_program(1);
	}
	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		// printf("in while.\n");
		if (cmd_iterator->toks && cmd_iterator->toks->content && (ft_is_export(cmd_iterator->toks->content) == 1 || ft_is_cd(cmd_iterator->toks->content) == 1 || \
			ft_is_unset(cmd_iterator->toks->content) == 1) && g_mini.nbr_of_pipes == 0) // 1 == true //  || ft_is_cd(cmd_iterator->arguments[0]) == 1 || ft_is_unset(cmd_iterator->arguments[0]) == 1)
		{
			// printf("\n\n\nentered ft_is_special_built\n\n\n");
			ft_execute_built_in(cmd_iterator, cmd_iterator->toks);
		}
		else
		{
			// printf("before fork.\n");
			// ft_print_cmds(cmd_iterator);
			pid[i] = fork();
			if (pid[i] == -1)
			{
				printf("Forking failed.\n");
				perror("Forking: ");
				exit_program(1);
			}
			if (pid[i] == 0)
			{
				// printf("Hello from child process.\n");
				ft_execute_process(cmd_iterator, i);
			}
		}
		i++;
		cmd_iterator = cmd_iterator->next;
	}
	// printf("Main.\n");
	i = 0;
	ft_close_fds(-1, -1, -1);
	while (i < g_mini.nbr_of_pipes + 1)
	{
		(waitpid(pid[i], &status, 0));
		// printf("status: %d\n", status);
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
	free (pid);
	// printf("ft_test: exit stat: %d\n", g_mini.exit_status);
	if (g_mini.exit_status != 0 && g_mini.exit == 1)
		exit(g_mini.exit_status);
}

void	ft_execute(void)
{
	ft_set_pipes();
	ft_set_files();
	// ft_print_cmds(g_mini.cmds);
	// if (g_mini.exit != 1)
	ft_test();
	// else
	// {
	// 	ft_close_fds(-1, -1, -1);
	// 	exit_program(g_mini.exit_status);
	// }
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
	if (!g_mini.raw_input)
		exit_shell_quit(0);
	free (temp);
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
				ft_dollar_sign();
				// while (ft_dollar_sign(g_mini.raw_input) != 0)
				// {
				// 	printf("RAW INPUT.................................: %s\n", g_mini.raw_input);
				// 	ft_env_vars(g_mini.raw_input);
				// }
				ft_parsing(g_mini.raw_input);
				ft_execute();
				ft_free_input();
			}
		}
	}
	//ft_env(&g_mini.dup_env); jus testing if env works

	///free_all(g_mini);
	//ft_free();
	return (0);
}
