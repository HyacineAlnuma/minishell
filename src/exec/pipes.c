/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:48 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/18 11:37:24 by halnuma          ###   ########.fr       */
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

void	open_pipes(int *pipefd, int pipe_nb)
{
	int	i;

	i = 0;
	while (i < pipe_nb)
	{
		if (pipe(pipefd + i * 2) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	dup_pipes(t_exec **cmds, int *pipefd, int cur_cmd, int cur_pipe)
{
	int	temp_file_fd;

	if (cmds[cur_cmd + 1])
		dup2(pipefd[cur_pipe + 1], STDOUT_FILENO);
	if (cur_cmd)
		dup2(pipefd[cur_pipe - 2], STDIN_FILENO);
	if (!cmds[cur_cmd + 1] && cmds[cur_cmd]->here_doc)
	{
		temp_file_fd = open(HD_TEMP_FILE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
		dup2(temp_file_fd, STDOUT_FILENO);
		close(temp_file_fd);
	}
	if (!cmds[cur_cmd + 1] && cmds[cur_cmd]->here_doc > 2)
	{
		dup2(cmds[cur_cmd]->here_doc, STDOUT_FILENO);
		close(cmds[cur_cmd]->here_doc);
	}
}
