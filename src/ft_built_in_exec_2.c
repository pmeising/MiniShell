/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_built_in_exec_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:03:38 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/05 17:47:15 by bde-carv         ###   ########.fr       */
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
		if (strcmp(iterator->content, iterator->next->content) > 0 /*&& iterator == *dup_env*/)
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
	if (!toks->next)
		ft_print_sorted_env(g_mini.dup_env);
	else
	{
		if (ft_isalpha(toks->next->content[0]) == 0)
			printf("bash: export: `%s´: not a valid identifier\n", toks->next->content);
		else
		{
			printf("Adding variable.\n");
			ft_lstadd_back(&g_mini.dup_env, ft_lstnew(toks->next->content));
		}
	}
}

void ft_cd_empty(void)
{
	int	check;
	
	printf("hello\n");
	check = chdir(getenv("HOME"));
	printf("check; %d\n", check);
}

/*
* built in execution function for "cd ..";
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
		printf("cd 2 dots: malloc error\n");
	cur_cwd = getcwd(cur_cwd, 1024);
	len = ft_strlen(cur_cwd);
	while (cur_cwd[len] != '/')
		len--;
	cur_cwd[len] = '\0';
	new_cwd = malloc(sizeof(char) * len);
	if (!new_cwd)
		printf("cd 2 dots: malloc error\n");
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
		printf("malloc error ft_cd_exec\n");
		free (cwd);
		exit_program(1);
	}
	cwd = getcwd(cwd, 1024);
	if (!cwd)
	{
		printf("getcwd error ft_cd_exec\n");
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
			printf("minishell: cd: %s: No such file or directory\n", toks->next->content);
	}
}
