/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_built_in_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 17:51:01 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/14 19:35:51 by bde-carv         ###   ########.fr       */
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
	free(cur_path);
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
		printf("%s\n", iterator->content);
		iterator = iterator->next;
	}
}

void	ft_delete_env(t_list *dup_env, int i)
{
	t_list	*iterator;
	t_list	*next;

	iterator = dup_env;
	if (!iterator)
		printf("nothing to delete\n");
	while (i > 1 && iterator)
	{
		iterator = iterator->next;
		i--;
	}
	next = iterator->next->next;
	free (iterator->next);
	iterator->next = next;
}

void	ft_unset_exec(t_list *toks)
{
	int		len;
	int		i;
	t_list	*env_iterator;
	t_list	*tok_it;

	if (!toks->next)
		return ;
	tok_it = toks->next;
	while (tok_it)
	{
		env_iterator = g_mini.dup_env;
		len = (int)ft_strlen(tok_it->content);
		i = 0;
		while (env_iterator)
		{
			if (ft_strncmp(env_iterator->content, tok_it->content, len) == 0)
			{
				ft_delete_env(g_mini.dup_env, i);
				break ;
			}
			i++;
			if (env_iterator->next)
				env_iterator = env_iterator->next;
		}
		tok_it = tok_it->next;
	}
}

void	ft_exit_exec(t_list *toks)
{
	if (toks->next)
		printf("error: no additional parameters for exit allowed\n");
}
