# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: secros <secros@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/18 09:48:16 by halnuma           #+#    #+#              #
#    Updated: 2025/04/18 17:38:03 by secros           ###   ########.fr        #
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
P_HEREDOC		= $(P_PARSING)here_doc/
P_EXEC			= $(P_SRC)exec/
P_UTILS			= $(P_SRC)utils/
P_UTILS_E		= $(P_UTILS)exec_utils/
P_UTILS_P		= $(P_UTILS)parsing_utils/
P_INC			= includes/
P_LIB			= libft/
P_OBJ			= obj/

# ------ FILES ------ #

MAIN			= main

PARSING			= parsing 				parse_env 			\
	 			tokenization 			create_exec			

HEREDOC 		= fork_here_doc			here_doc_2			\
				here_doc				docs

EXEC			= builtins				builtins_2 			\
				check_cmd				exec				\
				files					pipes				\
				unset					export			
						

UTILS			= handle_signal			print_ascii			\
				lst_utils				debug
						
				
UTILS_E			= builtins_utils		builtins_utils_2	\
				exec_utils				exec_utils_2		\
				file_utils

UTILS_P			= init_hd_struct		parsing_utils		\
				parsing_utils_2			here_doc_utils		\
				env_utils				env_utils2			\
				parsing_utils_3

HDR_SRC			= libft					minishell			\
				color					bt_malloc	

SRC_MAIN		= $(addprefix $(P_SRC), $(addsuffix .c, $(MAIN)))
SRC_PARSING		= $(addprefix $(P_PARSING), $(addsuffix .c, $(PARSING)))
SRC_HEREDOC		= $(addprefix $(P_HEREDOC), $(addsuffix .c, $(HEREDOC)))
SRC_EXEC		= $(addprefix $(P_EXEC), $(addsuffix .c, $(EXEC)))
SRC_UTILS		= $(addprefix $(P_UTILS), $(addsuffix .c, $(UTILS)))
SRC_UTILS_E		= $(addprefix $(P_UTILS_E), $(addsuffix .c, $(UTILS_E)))
SRC_UTILS_P		= $(addprefix $(P_UTILS_P), $(addsuffix .c, $(UTILS_P)))
SRC_ALL			= $(SRC_MAIN) $(SRC_EXEC) $(SRC_HEREDOC) $(SRC_PARSING) \
				$(SRC_UTILS) $(SRC_UTILS_E) $(SRC_UTILS_P)

OBJ_MAIN		= $(addprefix $(P_OBJ), $(addsuffix .o, $(MAIN)))
OBJ_PARSING		= $(addprefix $(P_OBJ), $(addsuffix .o, $(PARSING)))
OBJ_HEREDOC		= $(addprefix $(P_OBJ), $(addsuffix .o, $(HEREDOC)))
OBJ_EXEC		= $(addprefix $(P_OBJ), $(addsuffix .o, $(EXEC)))
OBJ_UTILS		= $(addprefix $(P_OBJ), $(addsuffix .o, $(UTILS)))
OBJ_UTILS_E		= $(addprefix $(P_OBJ), $(addsuffix .o, $(UTILS_E)))
OBJ_UTILS_P		= $(addprefix $(P_OBJ), $(addsuffix .o, $(UTILS_P)))
OBJ_ALL			= $(OBJ_MAIN) $(OBJ_PARSING) $(OBJ_HEREDOC) $(OBJ_EXEC) \
				$(OBJ_UTILS) $(OBJ_UTILS_E) $(OBJ_UTILS_P)

HEADERS			= $(addprefix $(P_INC), $(addsuffix .h, $(HDR_SRC)))
LIBFT			= $(P_LIB)libft.a

# ------ RULES ------ #

all: 			libft $(NAME)

$(NAME): 		$(OBJ_ALL) Makefile $(HEADERS) $(LIBFT)
				@$(CC) $(CFLAGS) -I $(P_INC) $(OBJ_ALL) $(LIBFT) -lreadline -o $@
				@echo "$(_GREEN)$(_BOLD)=> $(NAME) compiled!$(_END)"

$(P_OBJ):
				@mkdir -p $(P_OBJ)

$(P_OBJ)%.o:	$(P_SRC)%.c Makefile $(HEADERS) $(LIBFT) | $(P_OBJ)
				@echo "$(_YELLOW)Compiling $<...$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

$(P_OBJ)%.o:	$(P_PARSING)%.c Makefile $(HEADERS) $(LIBFT) | $(P_OBJ)
				@echo "$(_YELLOW)Compiling $<...$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

$(P_OBJ)%.o:	$(P_HEREDOC)%.c Makefile $(HEADERS) $(LIBFT) | $(P_OBJ)
				@echo "$(_YELLOW)Compiling $<...$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

$(P_OBJ)%.o:	$(P_EXEC)%.c Makefile $(HEADERS) $(LIBFT) | $(P_OBJ)
				@echo "$(_YELLOW)Compiling $<...$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

$(P_OBJ)%.o:	$(P_UTILS)%.c Makefile $(HEADERS) $(LIBFT) | $(P_OBJ)
				@echo "$(_YELLOW)Compiling $<...$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

$(P_OBJ)%.o:	$(P_UTILS_E)%.c Makefile $(HEADERS) $(LIBFT) | $(P_OBJ)
				@echo "$(_YELLOW)Compiling $<...$(_END)"
				@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@

$(P_OBJ)%.o:	$(P_UTILS_P)%.c Makefile $(HEADERS) $(LIBFT) | $(P_OBJ)
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
