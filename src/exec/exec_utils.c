/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:29:37 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/17 15:46:30 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_fork(t_fork *fork_info, t_exec **cmds, int pipe_nb, int cur_pipe)
{
	fork_info->cmds = cmds;
	fork_info->pipe_nb = pipe_nb;
	fork_info->cur_pipe = cur_pipe;
}

void	init_fork_bis(t_fork *f, int *pfd, t_exec *cmd, int cur_cmd, int *pid)
{
	f->cmd = cmd;
	f->pipefd = pfd;
	f->cur_cmd = cur_cmd;
	f->pid = pid;
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

void	wait_all_pid(pid_t *pid, int pipe_nb)
{
	int		i;
	int		status;
	// int		t;

	i = 0;
	status = 0;
	while (i < (pipe_nb + 1))
	{
		waitpid(pid[i], &status, 0);
		last_status_code(status, 0);
		i++;
	}
	// t = last_status_code(0, 1);
	// printf("status:%d\n", t);
}

void	free_all(t_list **env, t_exec **cmds, int *pid)
{
	int		i;
	int		j;

	ft_lstclear(env, NULL);
	i = 0;
	free(pid);
	i = 0;
	while (cmds[i])
	{
		//free(cmds[i]->cmd);
		j = 0;
		while (cmds[i]->opt[j])
		{
			free(cmds[i]->opt[j]);
			j++;
		}
		free(cmds[i]->opt);
		i++;
	}
	free(cmds);
}

int	check_cmd(char *cmd)
{
	char	*paths;
	char	*path;
	int		i;
	struct stat fs;

	if (check_builtins(cmd))
		return (1);
	if (stat(cmd, &fs) == 0 && (fs.st_mode & S_IXUSR) && S_ISREG(fs.st_mode))
		return (1);
	i = 0;
	paths = getenv("PATH");
	paths = ft_strjoin(paths, ":");
	path = paths;
	while (paths[i])
	{
		if (paths[i] == ':')
		{
			paths[i] = '\0';
			path = ft_strjoin(path, "/");
			path = ft_strjoin(path, cmd);
			if (!access(path, F_OK))
				return (1);
			path = &paths[i + 1];
		}
		i++;
	}
	return (0);
}