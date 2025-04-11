/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:48 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/11 09:32:28 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipes(int *pipefd, int pipe_nb)
{
	int	i;

	i = 0;
	while (i < pipe_nb * 2)
	{
		close(pipefd[i]);
		i++;
	}
}

int	open_pipes(int *pipefd, int pipe_nb)
{
	int	i;

	i = 0;
	while (i < pipe_nb)
	{
		if (pipe(pipefd + i * 2) == -1)
		{
			perror("pipe");
			return (0);
		}
		i++;
	}
	return (1);
}

void	dup_fd(int fd1, int fd2, t_exec *cmd)
{
	if (dup2(fd1, fd2) == -1)
	{
		if (cmd)
		{
			do_dishes(get_sink(NULL));
			do_dishes(get_sink(cmd->bin));
		}
		perror("dup");
		exit(EXIT_FAILURE);
	}
}

void	dup_pipes(t_exec **cmds, int *pipefd, int cur_cmd, int cur_pipe)
{
	int	temp_file_fd;

	if (cmds[cur_cmd + 1])
		dup_fd(pipefd[cur_pipe + 1], STDOUT_FILENO, cmds[cur_cmd]);
	if (cur_cmd)
		dup_fd(pipefd[cur_pipe - 2], STDIN_FILENO, cmds[cur_cmd]);
	if (!cmds[cur_cmd + 1] && cmds[cur_cmd]->here_doc)
	{
		temp_file_fd = open(EXEC_TMP_FILE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
		dup_fd(temp_file_fd, STDOUT_FILENO, cmds[cur_cmd]);
		close(temp_file_fd);
	}
}
