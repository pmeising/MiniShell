/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 20:30:19 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/27 17:53:00 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

# define BUFFER_SIZE 100

char	*get_next_line(int fd);
char	*read_line(int fd, char *line_a);
char	*ft_strjoin_gnl(char *line_a, char *line_b);
char	*ft_strchr_gnl(char *str, int c);
size_t	ft_strlen_gnl(char *str);
char	*output(char *line_a);
char	*new_line(char *line_a);

#endif