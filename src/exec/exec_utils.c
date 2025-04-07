/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:29:37 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/07 14:37:44 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_fork(t_fork *fork_info, t_exec **cmds, int pipe_nb, int cur_pipe)
{
	fork_info->cmds = cmds;
	fork_info->pipe_nb = pipe_nb;
	fork_info->cur_pipe = cur_pipe;
}

void	init_fork_bis(t_fork *f, int *pfd, t_exec *cmd, int cur_cmd)
{
	f->cmd = cmd;
	f->pipefd = pfd;
	f->cur_cmd = cur_cmd;
}

int	last_status_code(int status, int instruction)
{
	static int	status_code;

	if (!instruction && WIFEXITED(status))
		status_code = WEXITSTATUS(status);
	else
		return (status_code);
	return (0);
}

void	wait_all_pid(t_exec **cmds, int pipe_nb)
{
	int		i;
	int		status;

	i = 0;
	status = 0;
	while (i < (pipe_nb + 1))
	{
		waitpid(cmds[i]->pid, &status, 0);
		last_status_code(status, 0);
		i++;
	}
}

void	exec_parent_builtins(t_exec *cmd, t_list **env)
{
	if (!ft_strncmp(cmd->cmd, "cd", 3) && !cur_cmd)
		cd(cmd, env);
	if (!ft_strncmp(cmd->cmd, "exit", 5))
		exit_program(cmd);
	if (!ft_strncmp(cmd->cmd, "export", 7))
		export(cmd, env);
	if (!ft_strncmp(cmd->cmd, "unset", 6))
		unset(cmd, env);
}
