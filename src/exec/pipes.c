/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:48 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/28 10:35:40 by halnuma          ###   ########.fr       */
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
			// do_dishes(get_sink(NULL));
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	dup_fd(int fd1, int fd2)
{
	if (dup2(fd1, fd2) == -1)
	{
		// do_dishes(get_sink(NULL));
		perror("dup");
		exit(EXIT_FAILURE);
	}
}

void	dup_pipes(t_exec **cmds, int *pipefd, int cur_cmd, int cur_pipe)
{
	int	temp_file_fd;

	if (cmds[cur_cmd + 1])
		dup_fd(pipefd[cur_pipe + 1], STDOUT_FILENO);
	if (cur_cmd)
		dup_fd(pipefd[cur_pipe - 2], STDIN_FILENO);
	if (!cmds[cur_cmd + 1] && cmds[cur_cmd]->here_doc)
	{
		temp_file_fd = open(EXEC_TMP_FILE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
		dup_fd(temp_file_fd, STDOUT_FILENO);
		close(temp_file_fd);
	}
}
