# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: secros <secros@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/18 09:48:16 by halnuma           #+#    #+#              #
#    Updated: 2025/03/21 14:48:43 by secros           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re libft

# ------ COLORS ------ #

_END			= \033[0m
_GREY			= \033[0;30m
_RED			= \033[0;31m
_GREEN			= \033[0;32m
_YELLOW			= \033[0;33m
_BLUE			= \033[0;34m
_PURPLE			= \033[0;35m
_CYAN			= \033[0;36m
_BOLD			= \e[1m

# ------ VARIABLES ------ #

NAME			= minishell
CC				= cc
CFLAGS			= -Wall -Wextra -Werror -g3

# ------ PATHS ------ #

P_SRC 			= src/
P_PARSING		= $(P_SRC)parsing/
P_EXEC			= $(P_SRC)exec/
P_UTILS			= $(P_SRC)utils/
P_INC			= includes/
P_LIB			= libft/
P_OBJ			= obj/

# ------ FILES ------ #

MAIN			=	 main

PARSING			= parsing 				parse_env 			\
	 			tokenization 			docs				\
				unclosed				here_doc			\
				create_exec


EXEC			= builtins				builtins_bis		\
				builtins_utils			exec				\
				exec_utils				pipes				\
				files					check_cmd

UTILS			= handle_signal			print_ascii			\
				env_utils				parsing_utils		\
				parsing_utils2


HDR_SRC			= libft					minishell			\
				color		

SRC_MAIN		= $(addprefix $(P_SRC), $(addsuffix .c, $(MAIN)))
SRC_PARSING		= $(addprefix $(P_PARSING), $(addsuffix .c, $(PARSING)))
SRC_EXEC		= $(addprefix $(P_EXEC), $(addsuffix .c, $(EXEC)))
SRC_UTILS		= $(addprefix $(P_UTILS), $(addsuffix .c, $(UTILS)))
SRC_ALL			= $(SRC_MAIN) $(SRC_EXEC) $(SRC_PARSING) $(SRC_UTILS)

OBJ_MAIN		= $(addprefix $(P_OBJ), $(addsuffix .o, $(MAIN)))
OBJ_PARSING		= $(addprefix $(P_OBJ), $(addsuffix .o, $(PARSING)))
OBJ_EXEC		= $(addprefix $(P_OBJ), $(addsuffix .o, $(EXEC)))
OBJ_UTILS		= $(addprefix $(P_OBJ), $(addsuffix .o, $(UTILS)))
OBJ_ALL			= $(OBJ_MAIN) $(OBJ_PARSING) $(OBJ_EXEC) $(OBJ_UTILS)


HEADERS			= $(addprefix $(P_INC), $(addsuffix .h, $(HDR_SRC)))
LIBFT			= $(P_LIB)libft.a

# ------ RULES ------ #

all: 			libft $(NAME)

$(NAME): 		$(OBJ_ALL) Makefile $(HEADERS) $(LIBFT)
				@$(CC) $(CFLAGS) -I $(P_INC) $(OBJ_ALL) $(LIBFT) -lreadline -o $@
				@echo "$(_GREEN)$(_BOLD)=> $(NAME) compiled!$(_END)"

$(P_OBJ):
				@mkdir -p $(P_OBJ)

$(P_OBJ)%.o:	$(P_SRC)%.c Makefile $(HEADERS) | $(P_OBJ)
				@echo "$(_YELLOW)Compiling $<...$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

$(P_OBJ)%.o:	$(P_PARSING)%.c Makefile $(HEADERS) | $(P_OBJ)
				@echo "$(_YELLOW)Compiling $<...$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

$(P_OBJ)%.o:	$(P_EXEC)%.c Makefile $(HEADERS) | $(P_OBJ)
				@echo "$(_YELLOW)Compiling $<...$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

$(P_OBJ)%.o:	$(P_UTILS)%.c Makefile $(HEADERS) | $(P_OBJ)
				@echo "$(_YELLOW)Compiling $<...$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

libft:		
				@$(MAKE) -C $(P_LIB) --no-print-directory

# ------ BASIC RULES ------ #

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
