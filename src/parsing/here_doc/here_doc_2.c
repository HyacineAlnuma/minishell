/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:10:18 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/14 11:41:20 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_from_temp_file(char *char_buf)
{
	int		temp_file_fd;
	char	*buffer;

	buffer = NULL;
	temp_file_fd = open(EXEC_TMP_FILE, O_RDONLY);
	if (!temp_file_fd || temp_file_fd == -1)
	{
		perror("file error");
		return (0);
	}
	while (read(temp_file_fd, char_buf, 1))
	{
		buffer = ft_strappend(buffer, char_buf);
		if (!buffer)
		{
			perror("malloc error");
			return (NULL);
		}
	}
	buffer = format_buffer(buffer);
	close(temp_file_fd);
	unlink(EXEC_TMP_FILE);
	return (buffer);
}

char	*remove_nl(char *buffer)
{
	int	g;

	g = -1;
	while (buffer[++g])
	{
		if (buffer[g] == '\n' && !buffer[g + 1])
			buffer[g] = '\0';
	}
	return (buffer);
}

char	*retrieve_result(void)
{
	char	*char_buf;
	char	*buffer;

	char_buf = ft_calloc(sizeof(char), 2);
	if (!char_buf)
	{
		perror("malloc error");
		return (NULL);
	}
	buffer = read_from_temp_file(char_buf);
	if (!buffer)
	{
		free(char_buf);
		return (NULL);
	}
	if (!ft_strncmp(buffer, "\n\0", 2))
	{
		free(char_buf);
		return (buffer);
	}
	buffer = remove_nl(buffer);
	free(char_buf);
	return (buffer);
}

int	microshell(char *cmd, t_hd_utils *hd_utils)
{
	t_exec	**cmds;
	t_sink	*tmp_bin;
	t_sink	*tmp;
	int		k;

	tmp_bin = NULL;
	cmds = parsing(cmd, hd_utils->env, &tmp_bin);
	if (!cmds)
	{
		free(hd_utils->formatted);
		free(cmd);
		do_dishes(&tmp_bin);
		return (0);
	}
	free(cmd);
	k = -1;
	while (cmds[++k])
		cmds[k]->here_doc = 1;
	tmp = tmp_bin;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = *(hd_utils)->bin;
	exec(cmds, hd_utils->env, hd_utils->envp);
	tmp->next = NULL;
	do_dishes(&tmp_bin);
	return (1);
}

char	*exec_hd(t_hd_utils *hd_utils, char *cmd)
{
	char	*buffer;

	buffer = NULL;
	if (!microshell(cmd, hd_utils))
		return (NULL);
	buffer = retrieve_result();
	if (!buffer)
		return (NULL);
	hd_utils->formatted = fill_dishwasher(ft_strjoin(hd_utils->formatted, buffer), free, hd_utils->bin);
	if (!hd_utils->formatted)
	{
		perror("malloc error");
		return (NULL);
	}
	if (*buffer || buffer[0] == '\0')
		free(buffer);
	return (hd_utils->formatted);
}
