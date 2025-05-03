#############
# VARIABLES #
#############

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
OBJS = ft_isalpha.o ft_isdigit.o ft_isalnum.o ft_isascii.o ft_isprint.o ft_strlen.o \
	   ft_memset.o ft_bzero.o ft_memcpy.o ft_memmove.o ft_strlcpy.o ft_strlcat.o \
	   ft_toupper.o ft_tolower.o ft_strchr.o ft_strrchr.o ft_strncmp.o ft_memchr.o \
	   ft_memcmp.o ft_strnstr.o ft_atoi.o ft_calloc.o ft_strdup.o ft_substr.o \
	   ft_strjoin.o ft_strtrim.o ft_split.o ft_itoa.o ft_strmapi.o ft_striteri.o \
	   ft_putchar_fd.o ft_putstr_fd.o ft_putendl_fd.o ft_putnbr_fd.o
BONUS_OBJS = ft_lstnew_bonus.o ft_lstadd_front_bonus.o ft_lstsize_bonus.o ft_lstlast_bonus.o \
			ft_lstadd_back_bonus.o 	ft_lstdelone_bonus.o ft_lstclear_bonus.o ft_lstiter_bonus.o \
			ft_lstmap_bonus.o
SRCS = $(OBS:.o=.c)
B_SRCS = $(BONUS_OBJS:.o=.c)

MAKEFLAGS += --no-print-directory

PINK := $(shell tput setaf 200)
CYAN := $(shell tput setaf 87)
RESET := $(shell tput sgr0)

#########
# RULES #
#########

all: $(NAME)

$(NAME): $(OBJS)
	@make banner
	@ar rcs $(NAME) $(OBJS)

bonus: $(BONUS_OBJS) $(NAME)
	@ar rcs $(NAME) $(BONUS_OBJS)

$(OBJS):%.o: %.c
	@$(CC) $(CFLAGS) -c $<

$(BONUS_OBJS):%.o: %.c
	@$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus

banner:
	@echo "${PINK}";
	@echo "░  ░░░░░░░░        ░░       ░░░        ░░        ░";
	@echo "▒  ▒▒▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒  ▒▒▒▒  ▒▒  ▒▒▒▒▒▒▒▒▒▒▒  ▒▒▒▒";
	@echo "▓  ▓▓▓▓▓▓▓▓▓▓▓  ▓▓▓▓▓       ▓▓▓      ▓▓▓▓▓▓▓  ▓▓▓▓";
	@echo "█  ███████████  █████  ████  ██  ███████████  ████";
	@echo "█        ██        ██       ███  ███████████  ████";
	@echo "                                       ${CYAN}by thenriqu${RESET}";
