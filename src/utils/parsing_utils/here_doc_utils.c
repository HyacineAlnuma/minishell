/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:02:19 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/14 12:57:04 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_file_name(char *str)
{
	int	i;

	i = 0;
	while (str[i] && EXEC_TMP_FILE[i])
	{
		if (str[i] != EXEC_TMP_FILE[i])
			return (str);
		i++;
	}
	return (&str[i + 1]);
}

char	*format_buffer(char *buffer)
{
	if (buffer)
		buffer = remove_file_name(buffer);
	if (ft_strlen(buffer) == 0)
	{
		buffer = (char *)malloc(sizeof(char) * 1);
		buffer[0] = '\0';
	}
	return (buffer);
}

t_hd_utils	*parse_and_dup(t_hd_utils *hd_utils, size_t j)
{
	hd_utils->formatted = fill_dishwasher(ft_strjoin(
				hd_utils->formatted, &hd_utils->str[*hd_utils->begin_part]
				), free, hd_utils->bin);
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

int	last_concat(char **formatted, char *str)
{
	*formatted = ft_strjoin(*formatted, str);
	if (!*formatted)
	{
		perror("malloc error");
		return (0);
	}
	return (1);
}
