/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:27:50 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/28 13:41:00 by halnuma          ###   ########.fr       */
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
	}
	else if (!ft_strncmp(cmd->cmd, "/", 1) || !ft_strncmp(cmd->cmd, "../", 3))
		path = "\0";
	else
		path = "/bin/";
	exe = ft_strjoin(path, cmd->cmd);
	envp = lst_to_tab(env);
	execve(exe, cmd->opt, envp);
}

void	exec_cmd(t_exec *cmd, t_list **env, char **envp)
{
	if (!check_cmd(cmd->cmd))
	{
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": command not found.\n", 2);
		do_dishes(get_sink(NULL));
		exit(CMD_NOT_FOUND);
	}
	else if (!exec_builtins(cmd, env))
		exec_bin(cmd, env, envp);
}

void	exec_process(t_fork *f, t_list **env, char **envp)
{
	f->cmds[f->cur_cmd]->pid = fork();
	if (f->cmds[f->cur_cmd]->pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (f->cmds[f->cur_cmd]->pid == 0)
	{
		dup_pipes(f->cmds, f->pipefd, f->cur_cmd, f->cur_pipe);
		manage_files(f->cmds[f->cur_cmd]);
		close_pipes(f->pipefd, f->pipe_nb);
		exec_cmd(f->cmd, env, envp);
	}
}

void	close_temp_file(t_exec **cmds)
{
	int	i;
	int	j;

	i = 0;
	while (cmds[i])
	{
		if (cmds[i]->here_doc)
		{
			i++;
			continue ;
		}
		j = 0;
		while (cmds[i]->docs[j])
		{
			if (cmds[i]->docs[j]->type >= HEREDOC)
			{
				close(cmds[i]->docs[j]->type);
				unlink(HD_TEMP_FILE);
			}
			j++;
		}
		i++;
	}
}

int	exec_cmds(t_exec **cmds, char **envp, t_list **env, int pipe_nb)
{
	int		cur_cmd;
	int		cur_pipe;
	int		pipefd[1024];
	t_fork	fork_info;

	cur_cmd = 0;
	cur_pipe = 0;
	open_pipes(pipefd, pipe_nb);
	while (cmds[cur_cmd])
	{
		init_fork(&fork_info, cmds, pipe_nb, cur_pipe);
		init_fork_bis(&fork_info, pipefd, cmds[cur_cmd], cur_cmd);
		exec_process(&fork_info, env, envp);
		cur_cmd++;
		cur_pipe += 2;
	}
	cur_cmd--;
	exec_parent_builtins(cmds[cur_cmd], env, cur_cmd);
	close_pipes(pipefd, pipe_nb);
	wait_all_pid(cmds, pipe_nb);
	close_temp_file(cmds);
	return (1);
}

void	exec(t_exec **cmds, t_list **env, char **envp)
{
	int		pipe_nb;

	pipe_nb = ft_tablen((char **)cmds) - 1;
	exec_cmds(cmds, envp, env, pipe_nb);
}
