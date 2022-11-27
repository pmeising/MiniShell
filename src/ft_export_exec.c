/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:03:38 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/27 19:07:54 by pmeising         ###   ########.fr       */
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

void	ft_export_exec_2(char **env_name, char **env_cont)
{
	free(*env_cont);
	free(*env_name);
	env_cont = NULL;
	env_name = NULL;
}

void	ft_export_loop(t_list *iterator, char **env_name, char **env_cont)
{
	while (iterator)
	{
		if (ft_isalpha(iterator->content[0]) == 0)
			printf("42shell: export: `%s': not a valid identifier\n", \
				iterator->content);
		else if (ft_env_exist(*env_name, &g_mini.dup_env) == 0)
			ft_lstadd_back(&g_mini.dup_env, \
			ft_lstnew(ft_strdup(iterator->content)));
		else if (ft_env_exist(*env_name, &g_mini.dup_env) == 1)
			ft_update_env_list(*env_name, *env_cont, g_mini.dup_env);
		if (iterator != NULL && iterator->next != NULL)
			iterator = iterator->next;
		else
			break ;
	}
}

void	ft_export_else(t_list *iterator)
{
	char	*env_name;
	char	*env_cont;
	int		i;
	int		j;

	i = 0;
	j = 0;
	env_name = malloc(sizeof(char) * ft_strlen(iterator->content) + 2);
	ft_check_malloc(env_name);
	env_cont = malloc(sizeof(char) * ft_strlen(iterator->content) + 2);
	ft_check_malloc(env_cont);
	while (iterator->content[i] != '=' && iterator->content[i])
	{
		env_name[i] = iterator->content[i];
		i++;
	}
	env_name[i] = '\0';
	if (iterator->content[i] != '\0')
		i++;
	while (iterator->content[i])
		env_cont[j++] = iterator->content[i++];
	env_cont[j] = '\0';
	if (iterator)
		ft_export_loop(iterator, &env_name, &env_cont);
	ft_export_exec_2(&env_name, &env_cont);
}

void	ft_export_exec(t_list *toks)
{
	t_list	*iterator;

	iterator = NULL;
	if (!toks->next)
		ft_print_sorted_env(g_mini.dup_env);
	else
	{
		iterator = toks->next;
		ft_export_else(iterator);
	}
}
