/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:02:33 by secros            #+#    #+#             */
/*   Updated: 2025/03/28 15:30:31 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "color.h"
# include "bt_malloc.h"

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
# define EXEC_TMP_FILE "tmp/exec_temp.txt"
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
	OUTFILE,
	APPEND,
	HEREDOC,
};

typedef struct s_vect
{
	size_t	x;
	size_t	y;
}	t_vect;

typedef struct s_doc
{
	char		*str;
	// char		*str_type;
	int			type;
}	t_doc;

typedef struct s_exec
{
	pid_t	pid;
	char	*cmd;
	char	**opt;
	int		here_doc;
	t_doc	**docs;
	t_sink	**bin;
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

//utils
void	free_the_mallocs(void *pt);
void	print_ascii(void);
int		lst_count_char(t_list *tokens, char c);
void	clear_to(t_list	*start, t_list *end);
int		is_space(char c);
int		is_redir(char c);
void	sig_handler(int signum);
char	**lst_to_tab(t_list **lst);

//parsing
t_exec	**parsing(char *str, t_list **env, t_sink **bin);
char	*handle_env(char *str, t_list **env, t_sink *bin);
t_list	*create_token_list(char *str, t_sink *bin);
t_list	**lst_env(char **envp);
char	*find_node(t_list **env, char *var_env);
char	*remove_quote(char *str, t_sink *bin);
char	*synthax_quote(char *str);
int		merge_tokens(t_list *tokens, t_sink *bin);
char	*find_user_in_pwd(void);

t_list	**cut_instruction(t_list *tokens, int count);
t_doc 	**create_docs(t_list *lst, t_sink *bin, t_list **env);
int		env_handling(t_list *tokens, t_list **env, t_sink *bin);
int		compare(char *str, char *str_ref);
void	merge_all(t_list *lst, t_sink *bin);
void	ft_lst_hand_wash_if(t_list **begin_list, void *data_ref, int (*cmp)(), t_sink *bin);
int		format_here_doc(char *str, t_list **env, char **envp, t_sink **bin);

//exec
void	exec(t_exec **cmds, t_list **env, char **envp);
void	exit_program(t_exec *cmd);
void	echo(t_exec *cmd);
void	cd(t_exec *cmd, t_list **env);
void	pwd(t_exec *cmd);
void	unset(t_exec *cmd, t_list **env);
void	export(t_exec *cmd, t_list **env);
void	init_fork(t_fork *fork_info, t_exec **cmds, int pipe_nb, int cur_pipe);
void	init_fork_bis(t_fork *f, int *pfd, t_exec *cmd, int cur_cmd);
void	manage_files(t_exec *cmd);
void	close_pipes(int *pipefd, int pipe_nb);
void	open_pipes(int *pipefd, int pipe_nb);
void	dup_pipes(t_exec **cmds, int *pipefd, int cur_cmd, int cur_pipe);
char	*get_previous_pwd(t_list **env);
void	print_exp_env(t_list **env);
int		check_cmd(char *cmd);
int		check_builtins(char *cmd);
int		exec_builtins(t_exec *cmd, t_list **env);
void	wait_all_pid(t_exec **cmds, int pipe_nb);
void	exec_parent_builtins(t_exec *cmd, t_list **env, int cur_cmd);
void	dup_fd(int fd1, int fd2);

#endif