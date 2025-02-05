.PHONY: all clean fclean re bonus
# ----- COLORS -----

_END			= \033[0m
_GREY			= \033[0;30m
_RED			= \033[0;31m
_GREEN			= \033[0;32m
_YELLOW			= \033[0;33m
_BLUE			= \033[0;34m
_PURPLE			= \033[0;35m
_CYAN			= \033[0;36m

# ----- VARIABLES -----

NAME			= libft.a
CC 				= cc
CFLAGS 			= -Wall -Wextra -Werror

# ----- PATHS -----

P_INC			= ../includes/
P_SRC			= src/
P_OBJ 			= ../obj/

# ----- FILES -----

MAIN 			= ft_atoi 				ft_bzero				\
        		ft_calloc				ft_isalnum				\
				ft_isalpha				ft_isascii				\
				ft_isdigit				ft_isprint				\
				ft_memchr				ft_memcmp				\
				ft_memcpy				ft_memmove				\
				ft_memset				ft_strchr				\
				ft_strdup				ft_strlcat				\
				ft_strlcpy				ft_strlen				\
				ft_strncmp				ft_strnstr				\
				ft_strrchr				ft_tolower				\
				ft_toupper				ft_substr				\
				ft_substr				ft_strjoin				\
				ft_strtrim				ft_split				\
				ft_itoa					ft_strmapi				\
				ft_striteri				ft_putchar_fd			\
				ft_putstr_fd			ft_putendl_fd			\
				ft_putnbr_fd			ft_free_split

BONUS 			= ft_lstnew_bonus		ft_lstadd_front_bonus	\
				ft_lstsize_bonus		ft_lstlast_bonus		\
				ft_lstadd_back_bonus	ft_lstdelone_bonus		\
				ft_lstclear_bonus		ft_lstiter_bonus		\
				ft_lstmap_bonus			ft_lst_remove_if		

# SRCALL 			= $(MAIN) $(BONUS)
# OBJ 			= $(MAIN:.c=.o)
# OBJALL 			= $(SRCALL:.c=.o)
# HEADER 			= $(P_INC)libft.h

SRC_MAIN		= $(addprefix $(P_SRC), $(addsuffix .c, $(MAIN)))
OBJ_MAIN 		= $(addprefix $(P_OBJ), $(addsuffix .o, $(MAIN)))
SRC_BONUS		= $(addprefix $(P_SRC), $(addsuffix .c, $(BONUS)))
OBJ_BONUS		= $(addprefix $(P_OBJ), $(addsuffix .o, $(BONUS)))

SRC_ALL			= $(SRC_MAIN) $(SRC_BONUS)
OBJ_ALL 		= $(OBJ_MAIN) $(OBJ_BONUS)
HEADERS			= $(P_INC)libft.h

# ----- RULES -----

all: 			$(NAME) 

$(NAME): 		$(OBJ_ALL) Makefile $(HEADER)
				@ar -rcs $(NAME) $(OBJ_ALL)
#				@echo "$(_GREEN)libft compiled!$(_END)"

$(P_OBJ):
				@mkdir -p $(P_OBJ)

$(P_OBJ)%.o : 	$(P_SRC)%.c Makefile $(HEADER) | $(P_OBJ)
				@echo -n "$(_YELLOW)=$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

# ----- BASIC RULES -----

clean: 
				@rm -rf $(P_OBJ)

fclean:
				@$(MAKE) clean
				@rm -rf libft.a


re:
				@$(MAKE) fclean
				@$(MAKE) all


