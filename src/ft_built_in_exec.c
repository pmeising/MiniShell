/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_built_in_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 17:51:01 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/27 19:53:42 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* execution function for builtin pwd;
* prints out the current working directory;
* pwd is obtained by calling getcwd();
*/
void	ft_pwd_exec(void)
{
	char	*cur_path;

	cur_path = malloc(sizeof(char) * 1024);
	if (!cur_path)
	{
		perror("malloc :");
		free(cur_path);
		exit(EXIT_FAILURE);
	}
	if (getcwd(cur_path, 1024))
		printf("%s\n", cur_path);
	if (cur_path)
		free(cur_path);
	cur_path = NULL;
}

/*
* execute function for builtin command env;
* prints out the list of environment variables;
*/
void	ft_env_exec(void)
{
	t_list	*iterator;

	iterator = g_mini.dup_env;
	while (iterator)
	{
		if (iterator->content)
			printf("%s\n", iterator->content);
		iterator = iterator->next;
	}
}

void	ft_exit_exec(t_list *toks)
{
	if (toks->next)
		printf("error: no additional parameters for exit allowed\n");
}

void	ft_echo_exec(t_cmd *iterator)
{
	t_list	*toks_iterator;
	int		n_flag;

	n_flag = 0;
	if (!iterator->toks->next)
	{
		printf("\n");
		return ;
	}
	toks_iterator = iterator->toks->next;
	if (ft_strncmp(toks_iterator->content, "-n", 2) == 0)
	{
		n_flag = 1;
		toks_iterator = toks_iterator->next;
	}
	while (toks_iterator)
	{
		ft_putstr_fd(toks_iterator->content, 1);
		ft_putstr_fd(" ", 1);
		toks_iterator = toks_iterator->next;
	}
	if (n_flag != 1)
		printf("\n");
}

void	ft_execute_built_in(t_cmd *cmd, t_list *toks)
{
	(void)cmd;
	if (ft_is_pwd(toks->content))
		ft_pwd_exec();
	if (ft_is_env(toks->content))
		ft_env_exec();
	if (ft_is_unset(toks->content))
		ft_unset_exec(toks);
	if (ft_is_exit(toks->content))
		ft_exit_exec(toks);
	if (ft_is_export(toks->content))
		ft_export_exec(toks);
	if (ft_is_cd(toks->content))
		ft_cd_exec(toks);
	if (ft_is_echo(toks->content))
		ft_echo_exec(cmd);
}
