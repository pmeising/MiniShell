/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlst_new.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 17:39:58 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/16 17:51:42 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
* creates a new node in a double linked list;
*/
t_dlist	*ft_dlst_new(void *data)
{
	t_dlist	*new_node;

	new_node = malloc(sizeof(t_dlist));
	if (!new_node)
		return (0);
	new_node->data = data;
	new_node->next = 0;
	new_node->prev = 0;
	return (new_node);
}
