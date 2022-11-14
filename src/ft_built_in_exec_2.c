/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_built_in_exec_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:03:38 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/14 18:44:40 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* sorts the env variables based on ascii value;
* based on sortlist function from ex02;
*/
void	ft_print_sorted_env(t_list *dup_env)
{

	t_list	*iterator;
	char	*temp_cont;

	iterator = dup_env;
	while (iterator && iterator->next)
	{
		if (strcmp(iterator->content, iterator->next->content) > 0)
		{
			temp_cont = iterator->content;
			iterator->content = iterator->next->content;
			iterator->next->content = temp_cont;
			iterator = dup_env;
		}
		else
			iterator = iterator->next;
	}
	iterator = dup_env;
	while (iterator)
	{
		printf("%s\n", iterator->content);
		iterator = iterator->next;
	}
}

void	ft_export_exec(t_list *toks)
{
	t_list	*iterator;

	if (!toks->next)
		ft_print_sorted_env(g_mini.dup_env);
	iterator = toks->next;
	if (iterator)
	{
		while (iterator)
		{
			if (ft_isalpha(iterator->content[0]) == 0)
				printf("bash: export: `%s´: not a valid identifier\n", iterator->content);
			else
				ft_lstadd_back(&g_mini.dup_env, ft_lstnew(iterator->content));
			if (iterator != NULL && iterator->next != NULL)
				iterator = iterator->next;
			else
				break;
		}
	}
}

void ft_cd_empty(void)
{
	int	check;

	check = chdir(getenv("HOME"));
}

/*
	built in execution function for "cd ..";
*/
void	ft_cd_two_dots(void)
{
	char	*cur_cwd;
	char	*new_cwd;
	int		len;
	int		j;

	j = 0;
	cur_cwd = malloc(sizeof(char) * 2048);
	if (!cur_cwd)
		perror("malloc :");
	cur_cwd = getcwd(cur_cwd, 1024);
	len = ft_strlen(cur_cwd);
	while (cur_cwd[len] != '/')
		len--;
	cur_cwd[len] = '\0';
	new_cwd = malloc(sizeof(char) * len + 1);
	if (!new_cwd)
		perror("malloc :");
	while (cur_cwd[j])
	{
		new_cwd[j] = cur_cwd[j];
		j++;
	}
	new_cwd[j] = '\0';
	free (cur_cwd);
	chdir(new_cwd);
	free (new_cwd);
}

void ft_cd_exec(t_list *toks)
{
	int 	p;
	int		check;
	char	*cwd;

	p = 3;
	cwd = malloc(sizeof(char) * 1024);
	if(!cwd)
	{
		perror("malloc :");
		free (cwd);
		exit_program(1);
	}
	cwd = getcwd(cwd, 1024);
	if (!cwd)
	{
		perror("getcwd :");
		free (cwd);
		exit_program(1);
	}
	ft_update_env_list("OLDPWD", cwd, g_mini.dup_env);
	free (cwd);
	if (!toks->next)
		ft_cd_empty();
	else if (toks->next->content[0] == '.' && (ft_is_space(toks->next->content[1]) == 1 || toks->next->content[1] == '\0'))
		(void)p;
	else if (toks->next->content[0] == '.' && toks->next->content[1] == '.')
		ft_cd_two_dots();
	else if (toks->next)
	{
		check = chdir(toks->next->content);
		if (check == -1)
			printf("42_shell: cd: %s: No such file or directory\n", toks->next->content);
	}
}
