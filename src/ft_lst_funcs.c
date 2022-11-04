/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 18:42:27 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/04 19:38:59 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_lstadd_back_cmds(t_cmd **lst, t_cmd *new)
{
	t_cmd	*last;

	if (*lst != NULL)
	{
		last = ft_lstlast_cmds(*lst);
		last->next = new;
		new->previous = last;
	}
	else
	{
		new->previous = NULL;
		new->next = NULL;
		*lst = new;
	}
}

t_cmd	*ft_lstlast_cmds(t_cmd *lst)
{
	if (lst == NULL)
	{
		return (0);
	}
	while (lst != NULL)
	{
		if (lst->next == NULL)
		{
			return (lst);
		}
		lst = lst->next;
	}
	return (lst);
}

int	ft_lstsize_cmds(t_cmd *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

/*
*	This is the initiation of a t_cmd node, which forms part of the
*	second level of abstraction. In this list, we store the commands
*	themselves. Each command node contains the following variables:
*	char	*command_name->	name of command (i.e. echo, cat, ls)
*	char	**arguments	->	arguments this command executes with 
*							(i.e. echo, -n, -e, -la).
*	void	*next		->	needed for walking through the list.
*	int		fd_in		->	file descriptor of the input file;
*							initialized to STDIN = 0.
*	int		fd_out		->	file descriptor of the output file;
*							initialized to STDOUT = 1.
*	int		*open_flag	->	int which indicates, whether the file
*							is to be opened in append mode (1) or truncate
*							mode (0); initialized to -1.
* creates and initializes a new command node;
*/
t_cmd	*ft_lstnew_cmds(void)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
	{
		return (NULL);
	}
	new->toks = NULL;
	new->command_path = NULL;
	new->arguments = NULL;
	new->input_file = NULL;
	new->output_file = NULL;
	new->HEREDOC_DELIM = NULL;
	new->fd_in = 0;
	new->fd_out = 1;
	new->open_flag = -1;
	new->next = NULL;
	new->previous = NULL;
	new->is_built_in = 0;
	return (new);
}
