/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:27:50 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/18 12:55:49 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd(t_exec *cmd, t_list **env, char **envp)
{
	char	*exe;
	char	*path;
	char	pwd[PATH_MAX];

	if (!check_cmd(cmd->cmd))
	{
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": command not found.\n", 2);
		exit(CMD_NOT_FOUND);
	}
	else if (!exec_builtins(cmd, env))
	{
		if (!ft_strcmp(cmd->cmd, "./"))
		{
			getcwd(pwd, sizeof(pwd));
			cmd->cmd[0] = '/';
			path = pwd;
		}
		else
			path = "/bin/";
		exe = ft_strjoin(path, cmd->cmd);
		execve(exe, cmd->opt, envp);
	}
}

void	exec_parent_builtins(t_exec **cmds, t_exec *cmd, t_list **env, pid_t *pid)
{
	if (!ft_strncmp(cmd->cmd, "cd", 3))
		cd(cmd, env);
	if (!ft_strncmp(cmd->cmd, "exit", 5))
		exit_program(cmds, env, pid);
	if (!ft_strncmp(cmd->cmd, "export", 7))
		export(cmd, env);
	if (!ft_strncmp(cmd->cmd, "unset", 6))
		unset(cmd, env);
}

void	exec_process(t_fork *f, t_list **env, char **envp)
{
	// f->pid[f->cur_cmd] = fork();
	f->cmds[f->cur_cmd]->pid = fork();
	// if (f->pid[f->cur_cmd] == -1)
	if (f->cmds[f->cur_cmd]->pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	// else if (f->pid[f->cur_cmd] == 0)
	else if (f->cmds[f->cur_cmd]->pid == 0)
	{
		dup_pipes(f->cmds, f->pipefd, f->cur_cmd, f->cur_pipe);
		manage_files(f->cmds[f->cur_cmd]);
		close_pipes(f->pipefd, f->pipe_nb);
		exec_cmd(f->cmd, env, envp);
	}
}

// int	init_processes(t_exec **cmds, char **envp, t_list **env, int pipe_nb, pid_t *pid, int *pipefd)
// {
// 	int		cur_cmd;
// 	int		cur_pipe;
// 	t_fork	*fork_info;

// 	cur_cmd = 0;
// 	cur_pipe = 0;
// 	while (cmds[cur_cmd])
// 	{
// 		fork_info = ft_calloc(sizeof(t_fork), 1);
// 		if (!fork_info)
// 			return (0);
// 		init_fork(fork_info, cmds, pipe_nb, cur_pipe);
// 		init_fork_bis(fork_info, pipefd, cmds[cur_cmd], cur_cmd, pid);
// 		exec_process(fork_info, env, envp);
// 		cur_cmd++;
// 		cur_pipe += 2;
// 	}
// 	return (cur_cmd);
// }

int	exec_cmds(t_exec **cmds, char **envp, t_list **env, int pipe_nb, pid_t *pid)
{
	int		cur_cmd;
	int		cur_pipe;
	int		pipefd[1024];
	t_fork	*fork_info;

	cur_cmd = 0;
	cur_pipe = 0;
	open_pipes(pipefd, pipe_nb);
	while (cmds[cur_cmd])
	{
		fork_info = ft_calloc(sizeof(t_fork), 1);
		if (!fork_info)
			return (0);
		init_fork(fork_info, cmds, pipe_nb, cur_pipe);
		init_fork_bis(fork_info, pipefd, cmds[cur_cmd], cur_cmd, pid);
		exec_process(fork_info, env, envp);
		cur_cmd++;
		cur_pipe += 2;
	}
	//cur_cmd = init_processes(cmds, envp, env, pipe_nb, pid, pipefd);
	exec_parent_builtins(cmds, cmds[--cur_cmd], env, pid);
	close_pipes(pipefd, pipe_nb);
	wait_all_pid(pid, pipe_nb);
	unlink(HD_TEMP_FILE);
	// free_all(env, cmds, pid);
	return (1);
}

void	exec(t_exec **cmds, t_list **env, char **envp)
{
	int		pipe_nb;
	pid_t	*pid;

	pipe_nb = ft_tablen((char **)cmds) - 1;
	pid = malloc(sizeof(int) * (pipe_nb + 1));
	exec_cmds(cmds, envp, env, pipe_nb, pid);
}
