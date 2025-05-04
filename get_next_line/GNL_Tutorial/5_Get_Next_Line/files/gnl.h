#ifndef GNL_H
# define GNL_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);

size_t	ft_strlen(char *str);
char	*ft_join_free(char *line, char *buffer);
ssize_t	ft_locate_nl(char *str);
void	ft_memmove_nl(char *str);
char	*ft_safe_free(char *line, char *buffer);

#endif
