/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:21:35 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/18 17:34:55 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	manage_outfiles(t_exec *cmd, int *outfile_fd, int i, int j)
{
	if (!manage_outfile(cmd, outfile_fd, i, j))
		return (0);
	if (!manage_append(cmd, outfile_fd, i, j))
		return (0);
	return (1);
}

int	manage_infiles(t_exec *cmd, int *outfile_fd, int i, int j)
{
	if (!manage_infile(cmd, outfile_fd, i, j))
		return (0);
	if (!manage_here_doc(cmd, outfile_fd, i, j))
		return (0);
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
		if (!cmd->docs[j]->str)
			j++;
		else
		{
			if (!manage_infiles(cmd, infile_fd, k, j))
				return (0);
			if (!manage_outfiles(cmd, outfile_fd, i, j))
				return (0);
			j++;
		}
	}
	return (1);
}
