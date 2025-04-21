/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:02:33 by secros            #+#    #+#             */
/*   Updated: 2025/04/21 11:23:40 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "color.h"
# include "bt_malloc.h"
# include "ascii.h"

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
# define MAX_PIPE 1024

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
	int			type;
}	t_doc;

typedef struct s_exec
{
	pid_t	pid;
	char	*cmd;
	char	**opt;
	int		here_doc;
	char	*path;
	t_doc	**docs;
	t_sink	**bin;
	int		nb_cmd;
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

typedef struct s_hd_utils
{
	char	*formatted;
	char	*str;
	size_t	*begin_part;
	size_t	*i;
	char	**cmd;
	t_list	**env;
	char	**envp;
	t_sink	**bin;
}	t_hd_utils;

extern sig_atomic_t	g_sigint_flag;

//--------- UTILS ---------//

void		free_the_mallocs(void *pt);
void		print_ascii(void);
int			lst_count_char(t_list *tokens, char c);
void		clear_to(t_list	*start, t_list *end);
int			is_space(char c);
int			is_redir(char c);
void		sig_handler(int signum);
void		sig_handler_hd(int signum);
char		**lst_to_tab(t_list **lst);
char		*remove_file_name(char *str);
void		init_hd_utils(t_hd_utils *hd_utils, char *f, char *str, size_t *i);
void		init_hd_utils_2(t_hd_utils *hd, \
size_t *b, char **cmd, t_list **env);
int			all_digit(char *str);
void		print_error(char *cmd, char *arg, char *message);

void		init_hd_utils_3(t_hd_utils *hd, char *f, char **envp, t_sink **bin);
char		**convert_lst_in_tab(t_list *lst, t_sink *bin);
void		skip_space(char *str, size_t *i);
void		*add_empty(t_list **lst);
enum e_doc	find_type(char *str);
void		do_heredoc(t_doc *docs, char quote, t_sink *bin, t_list **env);
int			last_concat(char **formatted, char *str);
void		clean_exit(t_sink **bin, int exit_code);
int			check_exit_arg(t_exec *cmd);
char		*format_buffer(char *buffer);
int			exit_hd(char *cmd, t_hd_utils *hd_utils, t_sink	**tmp_bin);
void		relink_lst(t_list **prev, t_list **head, t_list **lst);
int			manage_outfile(t_exec *cmd, int *outfile_fd, int i, int j);
int			manage_append(t_exec *cmd, int *outfile_fd, int i, int j);
int			manage_infile(t_exec *cmd, int *infile_fd, int k, int j);
int			manage_here_doc(t_exec *cmd, int *infile_fd, int k, int j);
t_list		**get_alpha_env(t_list **env);
void		print_quoted_env(char *var_env);

//--------- PARSING ---------//

t_exec		**parsing(char *str, t_list **env, t_sink **bin);
char		*handle_env(char *str, t_list **env, t_sink *bin);
t_list		*create_token_list(char *str, t_sink **bin);
t_list		**lst_env(char **envp);
char		*find_node(t_list **env, char *var_env);
char		*remove_quote(char *str, t_sink *bin);
char		*synthax_quote(char *str);
int			merge_tokens(t_list *tokens, t_sink *bin);
char		*find_user_in_pwd(t_list **env);
char		*exec_hd(t_hd_utils *hd_utils, char *cmd);
t_hd_utils	*parse_and_dup(t_hd_utils *hd_utils, size_t j);
char		*get_heredoc(t_sink *bin, char *eof);
int			last_status_code(int status, int instruction);
t_list		**cut_instruction(t_list *tokens, int count);
t_doc		**create_docs(t_list **head, t_list *lst, \
t_sink *bin, t_list **env);
int			synthax_expand(char *str, int i);
char		*empty_str(char *f_str, t_sink **bin);

int			env_handling(t_list *tokens, t_list **env, t_sink *bin);
int			compare(char *str, char *str_ref);
int			merge_all(t_list *lst, t_sink *bin);
void		ft_lst_hand_wash_if(t_list **begin_list, \
void *data_ref, int (*cmp)(), t_sink *bin);

int			format_here_doc(char *str, t_list **env, \
char **envp, t_sink **bin);

//--------- EXEC ---------//

void		exec(t_exec **cmds, t_list **env, char **envp);
void		exit_program(t_exec *cmd, int is_parent);
void		echo(t_exec *cmd);
void		cd(t_exec *cmd, t_list **env);
void		pwd(t_exec *cmd	);
int			unset(t_exec *cmd, t_list **env, int index);
void		export(t_exec *cmd, t_list **env, int is_parent);
void		init_fork(t_fork *fork_info, t_exec **cmds, \
int pipe_nb, int cur_pipe);

void		init_fork_bis(t_fork *f, int *pfd, t_exec *cmd, int cur_cmd);
int			manage_files(t_exec *cmd);
void		close_pipes(int *pipefd, int pipe_nb);
int			open_pipes(int *pipefd, int pipe_nb);
void		dup_pipes(t_exec **cmds, int *pipefd, int cur_cmd, int cur_pipe);
char		*get_previous_pwd(t_list **env);
void		print_exp_env(t_list **env);
int			check_cmd(t_exec *cmd);
int			check_builtins(char *cmd);
int			exec_builtins(t_exec *cmd, t_list **env);
void		wait_all_pid(t_exec **cmds, int pipe_nb);
void		exec_parent_builtins(t_exec *cmd, t_list **env);
void		dup_fd(int fd1, int fd2, t_exec *cmd);
void		close_temp_file(t_exec **cmds);

#endif