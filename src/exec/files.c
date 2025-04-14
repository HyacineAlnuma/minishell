/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:21:35 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/14 12:35:17 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	manage_outfile(t_exec *cmd, int *outfile_fd, int i, int j)
{
	if (cmd->docs[j]->type == OUTFILE)
	{
		outfile_fd[i] = open(
				cmd->docs[j]->str, O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR
				);
		if (outfile_fd[i] == -1)
			return ((void) close(outfile_fd[i]));
		dup_fd(outfile_fd[i], STDOUT_FILENO, cmd);
		close(outfile_fd[i]);
		i++;
	}
	else if (cmd->docs[j]->type == APPEND)
	{
		outfile_fd[i] = open(
				cmd->docs[j]->str, O_RDWR | O_APPEND \
				| O_CREAT, S_IWUSR | S_IRUSR
				);
		if (outfile_fd[i] == -1)
			return ((void) close(outfile_fd[i]));
		dup_fd(outfile_fd[i], STDOUT_FILENO, cmd);
		close(outfile_fd[i]);
		i++;
	}
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
			ft_putstr_fd("minishell: no such file or directory:", 2);
			ft_putendl_fd(cmd->docs[j]->str, 2);
			return (0);
		}
		dup_fd(infile_fd[k], STDIN_FILENO, cmd);
		close(infile_fd[k]);
		k++;
	}
	else if (cmd->docs[j]->type >= HEREDOC)
	{
		dup_fd(cmd->docs[j]->type, STDIN_FILENO, cmd);
		close(cmd->docs[j]->type);
	}
	return (1);
}

int	manage_files(t_exec *cmd)
{
	int		outfile_fd[1024];
	int		infile_fd[1024];
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	while (cmd->docs[j])
	{
		if (!manage_infile(cmd, infile_fd, k, j))
			return (0);
		manage_outfile(cmd, outfile_fd, i, j);
		j++;
	}
	return (1);
}
