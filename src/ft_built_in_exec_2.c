/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_built_in_exec_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:03:38 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/33 16:103:13 by pmeising         ###   ########.fr       */
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
		printf("declare -x %s\n", iterator->content);
		iterator = iterator->next;
	}
}

/*
* export execution function;
* prints out sorted env list when export 
* without args is called;
* adds new env-vars to the envlist or 
* replaces the content if already existent;
*/
void	ft_export_exec(t_list *toks)
{
	t_list	*iterator;
	char	*env_name;
	char	*env_cont;
	int		i;
	int		j;

	if (!toks->next)
		ft_print_sorted_env(g_mini.dup_env);
	else
	{
		iterator = toks->next;
		env_name = malloc(sizeof(char) * ft_strlen(iterator->content) + 2);
		env_cont = malloc(sizeof(char) * ft_strlen(iterator->content) + 2);
		i = 0;
		j = 0;
		while (iterator->content[i] != '=' && iterator->content[i]) // 20.11
		{
			env_name[i] = iterator->content[i]; 
			i++;
		}
		env_name[i] = '\0';
		if (iterator->content[i] != '\0')
			i++; // fur ohne =
		while (iterator->content[i]) // 20.11
		{
			env_cont[j] = iterator->content[i];
			i++;
			j++;
		}
		env_cont[j] = '\0';
		if (iterator)
		{
			while (iterator)
			{
				if (ft_isalpha(iterator->content[0]) == 0)
					printf("bash: export: `%s´: not a valid identifier\n", \
						iterator->content);
				else if (ft_env_exist(env_name, &g_mini.dup_env) == 0)
					ft_lstadd_back(&g_mini.dup_env, ft_lstnew(ft_strdup(env_name)));
				else if (ft_env_exist(env_name, &g_mini.dup_env) == 1)
					ft_update_env_list(env_name, env_cont, g_mini.dup_env);
				if (iterator != NULL && iterator->next != NULL)
					iterator = iterator->next;
				else
					break ;
			}
		}
		free(env_cont);
		free(env_name);
		env_cont = NULL;
		env_name = NULL;
	}
}

void	ft_cd_empty(void)
{
	int		check;
	char	*cwd;

	cwd = malloc(sizeof(char) * 2048);
	if (!cwd)
	{
		printf("ft_cd_empty: malloc error\n");
		exit(EXIT_FAILURE);
	}
	cwd = getcwd(cwd, 1024);
	ft_update_env_list("OLDPWD", cwd, g_mini.dup_env);
	free(cwd);
	check = chdir(getenv("HOME"));
	if (check < 0)
		perror("chdir: ");
	cwd = malloc(sizeof(char) * 1024);
	if (!cwd)
	{
		printf("ft_cd_empty: malloc error\n");
		exit(EXIT_FAILURE);
	}
	cwd = getcwd(cwd, 1024);
	ft_update_env_list("PWD", cwd, g_mini.dup_env);
	free(cwd);
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
	ft_check_malloc(cur_cwd);
	cur_cwd = getcwd(cur_cwd, 1024);
	ft_update_env_list("OLDPWD", cur_cwd, g_mini.dup_env);
	len = ft_strlen(cur_cwd);
	while (cur_cwd[len] != '/')
		len--;
	cur_cwd[len] = '\0';
	new_cwd = malloc(sizeof(char) * len + 1);
	ft_check_malloc(new_cwd);
	while (cur_cwd[j])
	{
		new_cwd[j] = cur_cwd[j];
		j++;
	}
	new_cwd[j] = '\0';
	chdir(new_cwd);
	ft_update_env_list("PWD", new_cwd, g_mini.dup_env);
	ft_free_chars(cur_cwd, new_cwd);
}

void	ft_cd_exec(t_list *toks)
{
	char	*cwd;
	int		i;

	i = 0;
	cwd = malloc(sizeof(char) * 1024);
	ft_check_malloc(cwd);
	cwd = getcwd(cwd, 1024);
	ft_check_malloc(cwd);
	ft_update_env_list("OLDPWD", cwd, g_mini.dup_env);
	free (cwd);
	if (!toks->next)
		ft_cd_empty();
	else if (toks->next->content[0] == '.' && toks->next->content[1] == '.')
	{
		while (i < (int)ft_strlen(toks->next->content) && toks->next->content[i] == '.' \
			&& toks->next->content[i + 1] == '.')
		{
			ft_cd_two_dots();
			i = i + 3;
		}
	}
	else if (toks->next)
		ft_cd_exec_helper_1(toks->next->content);
}
