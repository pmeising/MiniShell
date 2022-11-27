/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/28 13:06:54 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/27 15:28:38 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen_gnl(char *str)
{
	size_t	len;

	len = 0;
	while (str[len] != '\0')
	{
		len++;
	}
	return (len);
}

char	*ft_strchr_gnl(char *str, int c)
{
	int	i;

	i = 0;
	if (!str)
	{
		return (NULL);
	}
	while (str[i] != '\0')
	{
		if (str[i] == (char)c)
		{
			return (&str[i]);
		}
		i++;
	}
	return (NULL);
}

char	*ft_strjoin_gnl(char *line_a, char *line_b)
{
	int		i;
	int		p;
	char	*joins;

	i = -1;
	p = 0;
	if (line_a == 0)
	{
		line_a = (char *)malloc(sizeof(char) * 1);
		line_a[0] = '\0';
	}
	if (!line_a || !line_b)
		return (NULL);
	joins = malloc(sizeof(char) * (ft_strlen_gnl(line_a) + \
			ft_strlen_gnl(line_b) + 1));
	if (!joins)
		return (NULL);
	if (line_a)
		while (line_a[++i] != '\0')
			joins[i] = line_a[i];
	while (line_b[p] != '\0')
		joins[i++] = line_b[p++];
	joins[i] = '\0';
	free(line_a);
	return (joins);
}
