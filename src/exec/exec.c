/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:27:50 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/23 09:38:38 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_bin(t_exec *cmd, t_list **env, char **envp)
{
	char	*exe;
	char	*path;
	char	pwd[PATH_MAX];

	if (!ft_strcmp(cmd->cmd, "./"))
	{
		getcwd(pwd, sizeof(pwd));
		cmd->cmd[0] = '/';
		path = pwd;
		exe = fill_dishwasher(ft_strjoin(path, cmd->cmd), free, cmd->bin);
	}
	else if (!ft_strncmp(cmd->cmd, "/", 1) || !ft_strncmp(cmd->cmd, "../", 3))
		exe = cmd->cmd;
	else
		exe = cmd->path;
	envp = fill_dishwasher(lst_to_tab(env), free, cmd->bin);
	if (!exe || !envp)
	{
		perror("Malloc error");
		clean_exit(cmd->bin, EXIT_FAILURE);
	}
	execve(exe, cmd->opt, envp);
	print_error(cmd->cmd + 2, NULL, "command not found");
	clean_exit(cmd->bin, EXIT_FAILURE);
}

void	exec_cmd(t_exec *cmd, t_list **env, char **envp)
{
	int	check_cmd_val;

	check_cmd_val = check_cmd(cmd, env);
	if (!cmd->cmd || check_cmd_val != 1)
	{
		if (cmd->cmd && !check_cmd_val)
			print_error(cmd->cmd, NULL, "command not found.");
		if (cmd->cmd && check_cmd_val == 3)
			print_error(cmd->cmd, NULL, ": Is a directory.");
		if (cmd->cmd && check_cmd_val == 4)
			perror("Malloc Error");
		clean_exit(cmd->bin, CMD_NOT_FOUND);
	}
	else if (!exec_builtins(cmd, env))
		exec_bin(cmd, env, envp);
}

void	exec_process(t_fork *f, t_list **env, char **envp)
{
	int	file_error;

	f->cmds[f->cur_cmd]->pid = fork();
	if (f->cmds[f->cur_cmd]->pid == -1)
	{
		perror("fork");
		clean_exit(f->cmds[f->cur_cmd]->bin, CMD_NOT_FOUND);
	}
	else if (f->cmds[f->cur_cmd]->pid == 0)
	{
		signal(SIGPIPE, SIG_IGN);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		dup_pipes(f->cmds, f->pipefd, f->cur_cmd, f->cur_pipe);
		file_error = manage_files(f->cmds[f->cur_cmd]);
		close_pipes(f->pipefd, f->pipe_nb);
		if (!file_error)
			clean_exit(f->cmd->bin, EXIT_FAILURE);
		if (f->cmds[f->cur_cmd]->cmd)
			exec_cmd(f->cmd, env, envp);
		else
			clean_exit(f->cmds[f->cur_cmd]->bin, EXIT_SUCCESS);
	}
}

void	launch_forks(t_exec **cmds, t_list **env, char **envp, int pipe_nb)
{
	int		cur_cmd;
	int		cur_pipe;
	int		pipefd[MAX_PIPE];
	t_fork	fork_info;

	cur_cmd = 0;
	cur_pipe = 0;
	if (!open_pipes(pipefd, pipe_nb))
		return ;
	while (cmds[cur_cmd])
	{
		init_fork(&fork_info, cmds, pipe_nb, cur_pipe);
		init_fork_bis(&fork_info, pipefd, cmds[cur_cmd], cur_cmd);
		exec_process(&fork_info, env, envp);
		cur_cmd++;
		cur_pipe += 2;
	}
	close_pipes(pipefd, pipe_nb);
	wait_all_pid(cmds, pipe_nb);
	cur_cmd--;
	if (!cur_cmd && cmds[cur_cmd]->cmd)
		exec_parent_builtins(cmds[cur_cmd], env);
	close_temp_file(cmds);
}

void	exec(t_exec **cmds, t_list **env, char **envp)
{
	int		pipe_nb;

	if (!cmds || !*cmds || !(*cmds)->cmd || !*(*cmds)->cmd)
		return ;
	pipe_nb = ft_tablen((char **)cmds) - 1;
	if (pipe_nb == 0)
		cmds[0]->nb_cmd = 1;
	if (pipe_nb > MAX_PIPE / 2)
		return (print_error(NULL, NULL, "too many commands"));
	launch_forks(cmds, env, envp, pipe_nb);
}
