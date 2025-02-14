/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:02:33 by secros            #+#    #+#             */
/*   Updated: 2025/02/14 11:05:05 by secros           ###   ########.fr       */
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

typedef struct s_exec
{
	char	*cmd;
	char	**opt;
	char	*infile;
	char	*outfile;
	int		append;
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