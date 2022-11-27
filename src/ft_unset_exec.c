/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 17:11:29 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/25 17:13:57 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (iterator->next->content)
	{
		free(iterator->next->content);
		iterator->next->content = NULL;
	}
	if (iterator->next)
	{
		free (iterator->next);
		iterator->next = NULL;
	}
	iterator->next = next;
}

void	ft_unset_help(t_list *env_iterator, t_list *tok_it)
{
	int	i;

	i = 0;
	while (env_iterator)
	{
		if (ft_strncmp(env_iterator->content, tok_it->content, \
			(int)ft_strlen(tok_it->content)) == 0)
		{
			ft_delete_env(g_mini.dup_env, i);
			break ;
		}
		i++;
		if (env_iterator->next)
			env_iterator = env_iterator->next;
		else
			break ;
	}
}

/*
* execution function for unset command;
* compares the current token with every
* env-variable until match found;
* uses ft_delete_env() to delete/unset the var;
*/
void	ft_unset_exec(t_list *toks)
{
	t_list	*env_iterator;
	t_list	*tok_it;

	if (!toks->next)
		return ;
	tok_it = toks->next;
	while (tok_it)
	{
		env_iterator = g_mini.dup_env;
		ft_unset_help(env_iterator, tok_it);
		tok_it = tok_it->next;
	}
}
