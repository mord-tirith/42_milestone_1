/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 07:26:56 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/28 10:56:43 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# ifndef FD_MAX
#  define FD_MAX 1024
# endif

# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>

char	*get_next_line(int fd);

char	*ft_strchr(char *str, char chr);
char	*ft_join_free(char *s1, char *s2);
char	*ft_handle_line(char *buffer, char *new_line);
char	*ft_main_loop(char *buffer, char *new_line, int fd, ssize_t *bytes);
void	ft_carriage(char *buffer, size_t amount);

#endif
