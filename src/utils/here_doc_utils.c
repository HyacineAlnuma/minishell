/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:02:19 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/11 14:22:20 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_hd_utils(t_hd_utils *hd_utils, char *f, char *str, size_t *i)
{
	hd_utils->formatted = f;
	hd_utils->str = str;
	hd_utils->i = i;
}

void	init_hd_utils_2(t_hd_utils *hd, size_t *b, char **cmd, t_list **env)
{
	hd->begin_part = b;
	hd->cmd = cmd;
	hd->env = env;
}

void	init_hd_utils_3(t_hd_utils *hd, char *f, char **envp, t_sink **bin)
{
	hd->formatted = f;
	hd->envp = envp;
	hd->bin = bin;
}

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

enum e_doc	find_type(char *str)
{
	if (!strncmp(str, "<\0", 2))
		return (INFILE);
	else if (!strncmp(str, "<<\0", 3))
		return (HEREDOC);
	else if (!strncmp(str, ">\0", 2))
		return (OUTFILE);
	else if (!strncmp(str, ">>\0", 3))
		return (APPEND);
	ft_printf("hell: syntax error near unexpected token `%c'\n", \
str[ft_strlen(str) - 1]);
	return (-1);
}
