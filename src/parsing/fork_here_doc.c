/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_here_doc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 10:52:29 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/09 11:19:36 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*readline_hd(char *f_str, char *eof, t_sink *bin)
{
	int		i;
	char	*str;

	i = 1;
	str = NULL;
	while (1)
	{
		free(str);
		str = readline("heredoc>");
		if (!str)
		{
			if (!g_sigint_flag)
				printf("minishell: warning: here-document at line %d \
					delimited by end-of-file (wanted `%s')\n", i, eof);
			return (free(str), f_str);
		}
		if (str && !ft_strncmp(str, eof, ft_strlen(eof) - 1))
			return (free(str), f_str);
		f_str = fill_dishwasher(ft_strjoin(f_str, str), free, &bin);
		f_str = fill_dishwasher(ft_strjoin(f_str, "\n"), free, &bin);
		i++;
	}
}

void	process_hd(int *pipefd, char *eof, t_sink *bin)
{
	int		f_len;
	char	*f_str;

	f_str = NULL;
	close(pipefd[0]);
	signal(SIGINT, sig_handler_hd);
	g_sigint_flag = 0;
	f_str = readline_hd(f_str, eof, bin);
	dup_fd(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	if (!g_sigint_flag)
	{
		f_len = ft_strlen(f_str);
		write(STDOUT_FILENO, f_str, f_len);
	}
	g_sigint_flag = 1;
	do_dishes(get_sink(&bin));
	exit(EXIT_SUCCESS);
}

char	*read_pipe(int *pipefd, char *f_str, t_sink *bin)
{
	char	char_buf[2];

	ft_bzero(char_buf, 2);
	while (read(pipefd[0], char_buf, 1))
	{
		f_str = fill_dishwasher(ft_strjoin(f_str, char_buf), free, &bin);
		if (!f_str)
		{
			perror("malloc error");
			close(pipefd[0]);
			return (NULL);
		}
	}
	return (f_str);
}

char	*get_heredoc(t_sink *bin, char *eof)
{
	pid_t	pid;
	int		pipefd[2];
	char	*f_str;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (NULL);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (NULL);
	}
	else if (pid == 0)
		process_hd(pipefd, eof, bin);
	close(pipefd[1]);
	f_str = NULL;
	f_str = read_pipe(pipefd, f_str, bin);
	close(pipefd[0]);
	return (f_str);
}
