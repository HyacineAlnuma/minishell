# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: halnuma <halnuma@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/18 08:45:55 by halnuma           #+#    #+#              #
#    Updated: 2025/01/08 12:38:06 by halnuma          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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

NAME			= libft.a
CC				= cc
AR				= ar -rcs
CFLAGS			= -Wall -Wextra -Werror

# ------ PATHS ------

P_OBJ 			= obj/
P_PRINTF 		= ft_printf/
P_UTILS			= $(P_PRINTF)utils/
P_INC			= includes/
P_LIB			= libft/
P_GNL			= gnl/

# ------ FILES ------

MAIN			= ft_printf	

UTILS			= ft_printchar				ft_printstr			\
				ft_printnbr					ft_printptr			\
				ft_printhex

GNL				= get_next_line				get_next_line_utils

SRC_MAIN		= $(addprefix $(P_PRINTF), $(addsuffix .c, $(MAIN)))
OBJ_MAIN 		= $(addprefix $(P_OBJ), $(addsuffix .o, $(MAIN)))
SRC_UTILS		= $(addprefix $(P_UTILS), $(addsuffix .c, $(UTILS)))
OBJ_UTILS 		= $(addprefix $(P_OBJ), $(addsuffix .o, $(UTILS)))
SRC_GNL			= $(addprefix $(P_GNL), $(addsuffix .c, $(GNL)))
OBJ_GNL	 		= $(addprefix $(P_OBJ), $(addsuffix .o, $(GNL)))

SRC_ALL			= $(SRC_MAIN) $(SRC_UTILS) $(SRC_GNL)
OBJ_ALL 		= $(OBJ_MAIN) $(OBJ_UTILS) $(OBJ_GNL)
LIBFT			= $(P_LIB)libft.a
HEADERS			= $(P_INC)libft.h 			$(P_INC)ft_printf.h \
				$(P_INC)get_next_line.h

# ------ RULES ------

all: 			libft $(NAME)

$(NAME): 		$(OBJ_ALL) Makefile $(P_INC) $(LIBFT)
				@cp $(LIBFT) $(NAME)
				@$(AR) $(NAME) $(OBJ_ALL)
				@echo -n "$(_CYAN)$(_BOLD)]$(_END)"
				@echo "$(_GREEN)$(_BOLD) => Libft compiled!$(_END)"

$(P_OBJ):
				@mkdir -p $(P_OBJ)

$(P_OBJ)%.o:	$(P_PRINTF)%.c Makefile $(HEADERS) | $(P_OBJ)
				@echo -n "$(_YELLOW)=$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

$(P_OBJ)%.o:	$(P_UTILS)%.c Makefile $(HEADERS) | $(P_OBJ)
				@echo -n "$(_YELLOW)=$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

$(P_OBJ)%.o:	$(P_GNL)%.c Makefile $(HEADERS) | $(P_OBJ)
				@echo -n "$(_YELLOW)=$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

libft:		
				@echo -n "$(_CYAN)$(_BOLD)[$(_END)"
				@$(MAKE) -C $(P_LIB) --no-print-directory

# ------ BASIC RULES ------

clean: 
				@rm -rf $(P_OBJ)
				@$(MAKE) -C $(P_LIB) clean --no-print-directory
				@echo "$(_CYAN)Libft cleaned!$(_END)"

fclean:
#				@$(MAKE) clean --no-print-directory
				@$(MAKE) -C $(P_LIB) fclean --no-print-directory
				@rm -rf $(NAME)
				@echo "$(_CYAN)Libft full cleaned!$(_END)"

re:
				@$(MAKE) fclean --no-print-directory
				@$(MAKE) all --no-print-directory
				@echo "$(_CYAN)libft rebuilt!$(_END)"
