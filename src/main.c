/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:34:58 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/25 19:41:54 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_mini	g_mini;

int	ft_is_special_built(char *arguments)
{
	if (ft_is_export(arguments) == 1 || ft_is_cd(arguments) == 1 || \
		ft_is_unset(arguments) == 1)
		return (1);
	return (0);
}

/*
* displays a prompt and reads in user input;
* adds input to the history for later acces by user;
* checks for open quotes, backslash and semicolon which 
* are all not allowed in this project
*/
int	ft_get_input(void)
{
	char	*prompt;
	char	*temp;

	prompt = "42shell > ";
	temp = readline(prompt);
	if (temp)
		g_mini.raw_input = ft_strdup(temp);
	if (temp == NULL)
	{
		ft_free_lst_cont(g_mini.dup_env);
		if (g_mini.raw_input)
			free (g_mini.raw_input);
		g_mini.raw_input = NULL;
		exit(EXIT_SUCCESS);
	}
	free (temp);
	temp = NULL;
	if (ft_str_only_space(g_mini.raw_input) != 1 && g_mini.raw_input)
		add_history(g_mini.raw_input);
	return (0);
}

void	ft_main_1(void)
{
	if (ft_check_input_validity(g_mini.raw_input) == 1)
	{
		ft_del_sus_quotes();
		ft_dollar_sign();
		ft_parsing(g_mini.raw_input);
		if (g_mini.exit == 0)
			ft_execute();
	}
	ft_free_input();
}

/*
* When launching the shell, we don't launch it with any input()
* so argv should be empty. If it isn't we need to return an error.
*/
int	main(int argc, char **argv, char **env)
{
	int	input_check;

	(void)argv;
	(void)argc;
	if (argc != 1)
	{
		printf("too many commands only ./minishell\n");
		exit_program(EXIT_FAILURE);
	}
	ft_init_minishell(&g_mini, env);
	while (1)
	{
		signal(SIGINT, ft_put_prompt);
		signal(SIGQUIT, SIG_IGN);
		input_check = ft_get_input();
		if (input_check == 2)
			printf("invalid syntax\n");
		else
			ft_main_1();
	}
	return (0);
}
