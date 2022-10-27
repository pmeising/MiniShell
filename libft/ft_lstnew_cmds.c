/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_cmds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 17:50:24 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/20 18:31:00 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(cmd));
	if (!new)
	{
		return (NULL);
	}
	new->content = content;
	new->next = NULL;
	new->toks = NULL;
	new->redirs = NULL;
	new->arguments
	return (new);
}
