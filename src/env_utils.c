/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 19:38:47 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/07 15:42:01 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* finds an env.var in our list of env.vars
* returns 1 if match found or 0 if NO match found
*/
int	ft_env_exist(char *str, t_list **dup_env)
{
	t_list	*tmp;
	int		len;

	tmp = *dup_env;
	len = ft_strlen(str);
	if (!str || !dup_env)
	{
		exit_program(EXIT_FAILURE);
	}
	while (tmp)
	{
		if (ft_strncmp(str, tmp->content, len) == 0)
		{
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

/*
* checks the list on env.vars for matching name and then
* replaces the whole string (=name+content) with just the name
*/
int	ft_del_env_cont(char *name, t_list **dup_env)
{
	t_list	*iterator;
	int		len;

	iterator = *dup_env;
	len = ft_strlen(name);
	if (!name || !dup_env)
	{
		exit_program(EXIT_FAILURE);
	}
	while (iterator)
	{
		if (ft_strncmp(name, iterator->content, len) == 0)
		{
			iterator->content = name;
			break ;
		}
		iterator = iterator->next;
	}
	return (1);
}

/*
* creates a new environment variable and adds it into our env list;
*/
void ft_create_env(char *str, t_list **dup_env)
{
	t_list	*new_env;

	new_env = ft_lstnew(str);
	if (!new_env)
	{
		printf("ft_create_env:init_minishell:new_env not created\n");
		exit_program(1);
	}
	ft_lstadd_front(dup_env, new_env);
}

/*
* extracts the content of a node/env.var in our dup_env list; 
*/
char *ft_get_env_cont(char *env_name)
{
	t_list	*env_list;
	char	*curr_value;
	char	*value;
	int		len;

	env_list = g_mini.dup_env;
	len = ft_strlen(env_name);
	while (env_list)
	{
		curr_value = (char *)env_list->content;
		if (ft_strncmp(env_name, curr_value, len) == 0)
		{
			value = ft_strdup(&(curr_value[len + 1]));
			if (!value)
			{
				printf("ft_get_env_cont:init_minishell:no value copied\n");
				free(value);
				exit_program(1);
			}
			return (value);
		}
		env_list = env_list->next;
	}
	return (NULL);
}

/*
* copy environment variables into a list, for later use in export;
*/
void	ft_copy_env(t_mini *g_mini, char **env)
{
	int	i;
	// t_list	*iter;

	i = 0;
	if (!env)
		exit_program(2);
	while (env[i])
	{
		ft_lstadd_back(&g_mini->dup_env, ft_lstnew(env[i]));
		i++;
	}
	// iter = g_mini->dup_env;
	// while (iter)
	// {
	// 	printf("content of t_list node: %s\n", iter->content);
	// 	iter = iter->next;
	// }
}
