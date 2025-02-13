/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:02:33 by secros            #+#    #+#             */
/*   Updated: 2025/02/13 10:55:13 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <linux/limits.h>
# include <signal.h>

# define CHARSET " \t\n\v\f\r"

# define ASCII1 " /$$      /$$           /$$                                   "
# define ASCII2 "                  /$$                     /$$      /$$ /$$    "
# define ASCII3 "       /$$           /$$                 /$$ /$$ /$$\n"
# define ASCII4 "| $$  /$ | $$          | $$                                   "
# define ASCII5 "                 | $$                    | $$$    /$$$|__/    "
# define ASCII6 "      |__/          | $$                | $$| $$| $$\n"
# define ASCII7 "| $$ /$$$| $$  /$$$$$$ | $$  /$$$$$$$  /$$$$$$  /$$$$$$/$$$$  "
# define ASCII8 " /$$$$$$        /$$$$$$    /$$$$$$       | $$$$  /$$$$ /$$ /$$"
# define ASCII9 "$$$$$  /$$  /$$$$$$$| $$$$$$$   /$$$$$$ | $$| $$| $$\n"
# define ASCII10 "| $$/$$ $$ $$ /$$__  $$| $$"
# define ASCII11 " /$$_____/ /$$__  $$| $$_  $$_  $$ "
# define ASCII12 "/$$__  $$      |_  $$_/   /$$__"
# define ASCII13 "  $$      | $$ $$/$$ $$| $$| $$"
# define ASCII14 "__  $$| $$ /$$_____/| $$__  $$ /$$__  $$| $$| $$| $$\n"
# define ASCII15 "| $$$$_  $$$$| $$$$$$$$|"
# define ASCII16 " $$| $$      | $$  \\ $$| $$ \\ $$ \\ "
# define ASCII17 "$$| $$$$$$$$        | $$    "
# define ASCII18 "| $$  \\ $$      | $$  $$$| $$| $$"
# define ASCII19 "| $$  \\ $$| $$|  $$$$$$ | $$  \\ $$| $$$$$$$$| $$| $$|__/\n"
# define ASCII20 "| $$$/ \\  $$$| $$_____/| "
# define ASCII21 "$$| $$      | $$  | $$| $$ | $$ | $$"
# define ASCII22 "| $$_____/        | $$ /$$| $"
# define ASCII23 "$  | $$      | $$\\  $ | $$| $$| "
# define ASCII24 "$$  | $$| $$ \\____  $$| $$  | $$| $$_____/| $$| $$    \n"
# define ASCII25 "| $$/   \\  $$|  $$$$$$$| "
# define ASCII26 "$$|  $$$$$$$|  $$$$$$/| $$ | $$ | $$"
# define ASCII27 "|  $$$$$$$        |  $$$$/|  "
# define ASCII28 "$$$$$$/      | $$ \\/  | $$| $$| "
# define ASCII29 "$$  | $$| $$ /$$$$$$$/| $$  | $$|  $$$$$$$| $$| $$ /$$\n"
# define ASCII30 "|__/     \\__/ \\_______/|_"
# define ASCII31 "_/ \\_______/ \\______/ |__/ |__/ |"
# define ASCII32 "__/ \\_______/         \\___/ "
# define ASCII33 "  \\______/       |__/     |__/|"
# define ASCII34 "__/|__/  |__/|__/|_______/ |__"
# define ASCII35 "/  |__/ \\_______/|__/|__/|__/\n"

// Text Background Colors (40 - 47)
#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

// Bright Text Background Colors (100 - 107)
#define BG_BRIGHT_BLACK   "\033[100m"
#define BG_BRIGHT_RED     "\033[101m"
#define BG_BRIGHT_GREEN   "\033[102m"
#define BG_BRIGHT_YELLOW  "\033[103m"
#define BG_BRIGHT_BLUE    "\033[104m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN    "\033[106m"
#define BG_BRIGHT_WHITE   "\033[107m"

// Reset color (reset to default)
#define RESET   "\033[0m"

// Text Colors (30 - 37)
#define FG_BLACK   "\033[30m"
#define FG_RED     "\033[31m"
#define FG_GREEN   "\033[0;32m"
#define FG_YELLOW  "\033[33m"
#define FG_BLUE    "\033[34m"
#define FG_MAGENTA "\033[35m"
#define FG_CYAN    "\033[36m"
#define FG_WHITE   "\033[37m"

// Bright Text Colors (90 - 97)
#define FG_BRIGHT_BLACK   "\033[90m"
#define FG_BRIGHT_RED     "\033[91m"
#define FG_BRIGHT_GREEN   "\033[92m"
#define FG_BRIGHT_YELLOW  "\033[93m"
#define FG_BRIGHT_BLUE    "\033[94m"
#define FG_BRIGHT_MAGENTA "\033[95m"
#define FG_BRIGHT_CYAN    "\033[96m"
#define FG_BRIGHT_WHITE   "\033[97m"

typedef struct s_exec
{
	char	*cmd;
	char	**opt;
	char	*infile;
	char	*outfile;
}	t_exec;

//utils
void	free_the_mallocs(void **pt);
void	print_ascii(void);

//parsing
char	*handle_env(char *str, t_list **env);
t_exec	**create_struct(char ***tab, size_t count);
t_list	**lst_env(char **envp);
char	*find_node(t_list **env, char *var_env);

//exec
void	exec(t_exec **cmds, t_list **env, char **envp);

#endif