.PHONY: all clean fclean re libft

# ------ COLORS ------

_END			= \033[0m
_GREY			= \033[0;30m
_RED			= \033[0;31m
_GREEN			= \033[0;32m
_YELLOW			= \033[0;33m
_BLUE			= \033[0;34m
_PURPLE			= \033[0;35m
_CYAN			= \033[0;36m
_BOLD			= \e[1m

# ------ VARIABLES ------

NAME			= minishell
CC				= cc
AR				= ar -rcs
CFLAGS			= -Wall -Wextra -Werror -lreadline -g3

# ------ PATHS ------

P_SRC 			= src/
P_PARSING		= $(P_SRC)parsing/
P_EXEC			= $(P_SRC)exec/
P_INC			= includes/
P_LIB			= libft/

# ------ FILES ------

MAIN			= main parsing print_ascii parse_env create_struct utils

PARSING			= 

EXEC			= 

HDR_SRC			= libft				minishell

SRC_MAIN		= $(addprefix $(P_SRC), $(addsuffix .c, $(MAIN)))
SRC_PARSING		= $(addprefix $(P_PARSING), $(addsuffix .c, $(PARSING)))
SRC_EXEC		= $(addprefix $(P_EXEC), $(addsuffix .c, $(EXEC)))
SRC_ALL			= $(SRC_MAIN) $(SRC_UTILS)

HEADERS			= $(addprefix $(P_INC), $(addsuffix .h, $(HDR_SRC)))
LIBFT			= $(P_LIB)libft.a

# ------ RULES ------

all: 			libft $(NAME)

$(NAME): 		$(SRC_ALL) Makefile $(HEADERS) $(LIBFT)
				@$(CC) $(CFLAGS) -I $(P_INC) $(SRC_ALL) $(LIBFT) -o $@
				@echo "$(_YELLOW)Compiling $(SRC_ALL)$(_END)"
				@echo "$(_GREEN)$(_BOLD)$(NAME) compiled!$(_END)"

libft:		
				@$(MAKE) -C $(P_LIB) --no-print-directory

# ------ BASIC RULES ------

clean: 
				@rm -rf $(P_OBJ)
				@$(MAKE) -C $(P_LIB) clean --no-print-directory
				@echo "$(_CYAN)$(NAME) cleaned!$(_END)"

fclean:
				@$(MAKE) clean --no-print-directory
				@$(MAKE) -C $(P_LIB) fclean --no-print-directory
				@rm -rf $(LIBFT)
				@rm -rf $(NAME)
				@echo "$(_CYAN)$(NAME) full cleaned!$(_END)"

re:
				@$(MAKE) fclean --no-print-directory
				@$(MAKE) all --no-print-directory
