/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:48 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/17 15:32:24 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	manage_files(t_exec *cmd)
{
	int		outfile_fd[1024];
	int		infile_fd[1024];
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	while (cmd->docs[j].str)
	{
		if (cmd->docs[j].type == OUTFILE)
		{
			outfile_fd[i] = open(cmd->docs[j].str, O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
			dup2(outfile_fd[i], STDOUT_FILENO);
			close(outfile_fd[i]);
			i++;
		}
		else if (cmd->docs[j].type == INFILE)
		{
			infile_fd[k] = open(cmd->docs[j].str, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
			dup2(infile_fd[k], STDIN_FILENO);
			close(infile_fd[k]);
			k++;
		}
		else if (cmd->docs[j].type == HEREDOC)
		{
			infile_fd[k] = open(cmd->docs[j].str, O_RDWR | S_IWUSR | S_IRUSR);
			dup2(infile_fd[k], STDIN_FILENO);
			close(infile_fd[k]);
			k++;
		}
		else if (cmd->docs[j].type == APPEND)
		{
			outfile_fd[i] = open(cmd->docs[j].str, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | O_APPEND);
			dup2(outfile_fd[i], STDOUT_FILENO);
			close(outfile_fd[i]);
			i++;
		}
		j++;
	}
}

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