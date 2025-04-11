/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:10:18 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/11 16:17:02 by halnuma          ###   ########.fr       */
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

char	*exec_hd(t_hd_utils *hd_utils, char *cmd)
{
	int		k;
	t_exec	**cmds;
	char	*buffer;

	buffer = NULL;
	cmds = parsing(cmd, hd_utils->env, hd_utils->bin);
	if (!cmds)
	{
		free(cmd);
		return (NULL);
	}
	free(cmd);
	k = -1;
	while (cmds[++k])
		cmds[k]->here_doc = 1;
	exec(cmds, hd_utils->env, hd_utils->envp);
	buffer = retrieve_result();
	if (!buffer)
		return (NULL);
	hd_utils->formatted = ft_strappend(hd_utils->formatted, buffer);
	if (!hd_utils->formatted)
	{
		perror("malloc error");
		return (NULL);
	}
	if (*buffer || buffer[0] == '\n')
		free(buffer);
	return (hd_utils->formatted);
}

t_hd_utils	*parse_and_dup(t_hd_utils *hd_utils, size_t j)
{
	hd_utils->formatted = ft_strappend(
			hd_utils->formatted, &hd_utils->str[*hd_utils->begin_part]
			);
	if (!hd_utils->formatted)
	{
		perror("malloc error");
		return (NULL);
	}
	*hd_utils->cmd = ft_strndup(
			&hd_utils->str[*hd_utils->i + 2], (j - (*hd_utils->i + 2))
			);
	if (!hd_utils->cmd)
	{
		perror("malloc error");
		return (NULL);
	}
	return (hd_utils);
}
