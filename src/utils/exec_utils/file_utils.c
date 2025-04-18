/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:34:38 by secros            #+#    #+#             */
/*   Updated: 2025/04/18 17:35:02 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	manage_outfile(t_exec *cmd, int *outfile_fd, int i, int j)
{
	if (cmd->docs[j]->type == OUTFILE)
	{
		outfile_fd[i] = open(
				cmd->docs[j]->str, O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR
				);
		if (outfile_fd[i] == -1)
		{
			print_error(cmd->docs[j]->str, NULL, "Permission denied");
			return (0);
		}
		dup_fd(outfile_fd[i], STDOUT_FILENO, cmd);
		close(outfile_fd[i]);
		i++;
	}
	return (1);
}

int	manage_append(t_exec *cmd, int *outfile_fd, int i, int j)
{
	if (cmd->docs[j]->type == APPEND)
	{
		outfile_fd[i] = open(
				cmd->docs[j]->str, O_RDWR | O_APPEND \
				| O_CREAT, S_IWUSR | S_IRUSR
				);
		if (outfile_fd[i] == -1)
		{
			print_error(cmd->docs[j]->str, NULL, "Permission denied");
			return (0);
		}
		dup_fd(outfile_fd[i], STDOUT_FILENO, cmd);
		close(outfile_fd[i]);
		i++;
	}
	return (1);
}

int	manage_infile(t_exec *cmd, int *infile_fd, int k, int j)
{
	if (cmd->docs[j]->type == INFILE)
	{
		infile_fd[k] = open(
				cmd->docs[j]->str, O_RDWR | S_IWUSR | S_IRUSR
				);
		if (infile_fd[k] == -1)
		{
			print_error("no such file or directory", cmd->docs[j]->str, NULL);
			return (0);
		}
		dup_fd(infile_fd[k], STDIN_FILENO, cmd);
		close(infile_fd[k]);
		k++;
	}
	return (1);
}

int	manage_here_doc(t_exec *cmd, int *infile_fd, int k, int j)
{
	if (cmd->docs[j]->type >= HEREDOC)
	{
		infile_fd[k] = open(HD_TEMP_FILE, O_RDONLY);
		if (infile_fd[k] == -1)
		{
			print_error("no such file or directory", cmd->docs[j]->str, NULL);
			return (0);
		}
		dup_fd(infile_fd[k], STDIN_FILENO, cmd);
		close(infile_fd[k]);
	}
	return (1);
}
