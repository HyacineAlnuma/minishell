/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:02:33 by secros            #+#    #+#             */
/*   Updated: 2025/03/17 15:54:51 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "color.h"

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
# define HD_TEMP_FILE "tmp/here_doc_temp.txt"
# define CMD_NOT_FOUND 127

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


enum e_here_bool
{
	FALSE,
	TRUE,
	TMPDOC,
};

enum e_doc
{
	INFILE,
	HEREDOC,
	OUTFILE,
	APPEND,
};

typedef struct s_garb
{
	void	*obj;
	void	(*f)(void *);
} t_garb;


typedef struct s_doc
{
	char		*str;
	char		*str_type;
	enum e_doc	type;
}	t_doc;

typedef struct s_exec
{
	char	*cmd;
	char	**opt;
	int		here_doc;
	t_doc	*docs;
}	t_exec;

typedef struct s_fork
{
	t_exec	**cmds;
	t_exec	*cmd;
	pid_t	*pid;
	int		*pipefd;
	int		pipe_nb;
	int		cur_cmd;
	int		cur_pipe;
}	t_fork;

extern sig_atomic_t g_sigint_flag;

//garbage collector
void 	clear_garbage(t_list **head);
void	*add_garbage(void *pt, void (*free_pt)(void *), t_list **head);

//utils
void	free_the_mallocs(void *pt);
void	print_ascii(void);
int		lst_count_char(t_list *tokens, char c);
void	clear_to(t_list	*start, t_list *end);
int		is_space(char c);
int		is_redir(char c);
void	sig_handler(int signum);

//parsing
t_exec	**parsing(char *str, t_list **env, t_list **bin);
char	*handle_env(char *str, t_list **env);
t_list	*create_token_list(char *str);
t_list	**lst_env(char **envp);
char	*find_node(t_list **env, char *var_env);
char	*remove_quote(char *str);
char	*synthax_quote(char *str);
int		merge_tokens(t_list *tokens);
char	*find_user_in_pwd(void);

//exec
void	exec(t_exec **cmds, t_list **env, char **envp);
void	exit_program(t_exec **cmds, t_list **env, int *pid);
void	echo(t_exec *cmd);
void	cd(t_exec *cmd, t_list **env);
void	pwd(void);
void	unset(t_exec *cmd, t_list **env);
void	export(t_exec *cmd, t_list **env);
void	init_fork(t_fork *fork_info, t_exec **cmds, int pipe_nb, int cur_pipe);
void	init_fork_bis(t_fork *f, int *pfd, t_exec *cmd, int cur_cmd, int *pid);
void	manage_files(t_exec *cmd);
void	close_pipes(int *pipefd, int pipe_nb);
void	open_pipes(int *pipefd, int pipe_nb);
void	dup_pipes(t_exec **cmds, int *pipefd, int cur_cmd, int cur_pipe);
char	*get_previous_pwd(t_list **env);
void	print_exp_env(t_list **env);
void	free_all(t_list **env, t_exec **cmds, int *pid);
int		check_cmd(char *cmd);
int		check_builtins(char *cmd);
int		exec_builtins(t_exec *cmd, t_list **env);
void	wait_all_pid(pid_t *pid, int pipe_nb);

#endif