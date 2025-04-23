/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:28:35 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/23 13:59:34 by secros           ###   ########.fr       */
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

int	expand_cmd(t_hd_utils *hd_utils, char **formatted, char **cmd)
{
	*formatted = isolate_cmd(hd_utils);
	if (!*formatted)
		return (0);
	*formatted = exec_hd(hd_utils, *cmd);
	if (!formatted || !*formatted)
		return (0);
	return (1);
}

char	*parse_hd(char *str, t_list **env, char **envp, t_sink **bin)
{
	size_t		i[2];
	char		*f;
	char		*cmd;
	t_hd_utils	hd_utils;
	int			synthax;

	i[0] = -1;
	i[1] = 0;
	f = NULL;
	cmd = NULL;
	while (str && str[++i[0]])
	{
		synthax = synthax_expand(str, i[0]);
		if (synthax == 1)
		{
			hd_utils = (t_hd_utils){f, str, &i[1], &i[0], &cmd, env, envp, bin};
			if (!expand_cmd(&hd_utils, &f, &cmd))
				return (NULL);
		}
		else if (!synthax)
			return (NULL);
	}
	if (!last_concat(&f, &str[i[1]]))
		return (NULL);
	return (f);
}

int	format_here_doc(char *str, t_list **env, char **envp, t_sink **bin)
{
	char	*formatted;
	int		hd_fd;
	int		f_len;

	if (!envp)
		return (-1);
	fill_dishwasher(envp, free, bin);
	formatted = parse_hd(str, env, envp, bin);
	if (!formatted)
		return (-1);
	hd_fd = open(HD_TEMP_FILE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	if (hd_fd != -1)
	{
		f_len = ft_strlen(formatted);
		write(hd_fd, formatted, f_len);
		close(hd_fd);
	}
	else
		perror("file error");
	hand_wash(formatted, bin);
	return (HEREDOC);
}

void	do_heredoc(t_doc *docs, char quote, t_sink **bin, t_list **env)
{
	char	*str;
	char	*heredoc;
	int		doc_fd;
	size_t	i;

	str = docs->str;
	i = 0;
	heredoc = handle_env(get_heredoc(bin, str), env, bin);
	docs->str = heredoc;
	if (quote == 0)
	{
		docs->str = handle_env(heredoc, env, bin);
		docs->type = format_here_doc(docs->str, env, lst_to_tab(env), bin);
		return ;
	}
	doc_fd = open(HD_TEMP_FILE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	if (doc_fd == -1)
		return ;
	write(doc_fd, heredoc, ft_strlen(heredoc));
	close(doc_fd);
}
