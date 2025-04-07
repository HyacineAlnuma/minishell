/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:02:19 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/07 14:09:40 by halnuma          ###   ########.fr       */
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
