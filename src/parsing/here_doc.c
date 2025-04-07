/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:28:35 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/07 14:11:02 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*isolate_cmd(t_hd_utils *hd_utils)
{
	size_t	j;

	j = *hd_utils->i;
	while (hd_utils->str[++j])
	{
		if (hd_utils->str[j] == ')')
			break ;
	}
	if (j == ft_strlen(hd_utils->str))
	{
		perror("parse error");
		return (0);
	}
	hd_utils->str[*hd_utils->i] = '\0';
	hd_utils = parse_and_dup(hd_utils, j);
	if (!hd_utils)
		return (NULL);
	*hd_utils->i = j + 1;
	*hd_utils->begin_part = *hd_utils->i;
	return (hd_utils->formatted);
}

int	last_concat(char **formatted, char *str)
{
	*formatted = ft_strappend(*formatted, str);
	if (!*formatted)
	{
		perror("malloc error");
		return (0);
	}
	return (1);
}

int	expand_cmd(t_hd_utils *hd_utils, char **formatted, char **cmd)
{
	(void)cmd;
	*formatted = isolate_cmd(hd_utils);
	if (!*formatted)
		return (0);
	*formatted = exec_hd(hd_utils, *cmd);
	if (!*formatted)
		return (0);
	return (1);
}

char	*parse_hd(char *str, t_list **env, char **envp, t_sink **bin)
{
	size_t		i;
	size_t		begin_part;
	char		*formatted;
	char		*cmd;
	t_hd_utils	hd_utils;

	i = -1;
	begin_part = 0;
	formatted = NULL;
	cmd = NULL;
	while (str && str[++i])
	{
		if (str[i] == '$' && str[i + 1] == '(')
		{
			init_hd_utils(&hd_utils, formatted, str, &i);
			init_hd_utils_2(&hd_utils, &begin_part, &cmd, env);
			init_hd_utils_3(&hd_utils, formatted, envp, bin);
			if (!expand_cmd(&hd_utils, &formatted, &cmd))
				return (NULL);
		}
	}
	if (!last_concat(&formatted, &str[begin_part]))
		return (NULL);
	return (formatted);
}

int	format_here_doc(char *str, t_list **env, char **envp, t_sink **bin)
{
	char	*formatted;
	int		hd_fd;
	int		f_len;

	formatted = parse_hd(str, env, envp, bin);
	if (!formatted)
	{
		free(envp);
		return (0);
	}
	hd_fd = open(HD_TEMP_FILE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	f_len = ft_strlen(formatted);
	write(hd_fd, formatted, f_len);
	close(hd_fd);
	hd_fd = open(HD_TEMP_FILE, O_RDONLY);
	free(formatted);
	free(envp);
	return (hd_fd);
}
