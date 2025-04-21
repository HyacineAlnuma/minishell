/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_here_doc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 10:52:29 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/21 10:35:38 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	end_heredoc(char *str, char **f_str, char *eof, t_sink **bin)
{
	if (str && !ft_strncmp(str, eof, ft_strlen(eof) + 1))
		return (free(str), 1);
	*f_str = fill_dishwasher(ft_strjoin(*f_str, str), free, bin);
	if (!*f_str)
		return (free(str), 2);
	*f_str = fill_dishwasher(ft_strjoin(*f_str, "\n"), free, bin);
	return (0);
}

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
			return (free(str), empty_str(f_str, &bin));
		}
		if (end_heredoc(str, &f_str, eof, &bin))
			return (empty_str(f_str, &bin));
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
	if (!f_str)
	{
		do_dishes(get_sink(&bin));
		exit(EXIT_SUCCESS);
	}
	dup_fd(pipefd[1], STDOUT_FILENO, NULL);
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
