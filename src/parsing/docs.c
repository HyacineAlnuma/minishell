/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   docs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:56:31 by secros            #+#    #+#             */
/*   Updated: 2025/04/08 11:21:14 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// add an error check if invalid redirection like strlen > 2 or bad redirection
enum e_doc	find_type(char *str)
{
	if (!strncmp(str, "<\0", 2))
		return (INFILE);
	else if (!strcmp(str, "<<\0"))
		return (HEREDOC);
	else if (!strncmp(str, ">\0", 2))
		return (OUTFILE);
	else if (!strcmp(str, ">>\0"))
		return (APPEND);
	else
		return (-1);
}

char	*get_heredoc(t_sink *bin, char *eof)
{
	char	*str;
	char	*f_str;
	pid_t	pid;
	int		pipefd[2];
	int		i;
	char	*char_buf;
	int		f_len;
	int		error_flag;

	f_str = NULL;
	str = NULL;
	i = 0;
	while (i < 2)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (NULL);
	}
	else if (pid == 0)
	{
		signal(SIGINT, sig_handler_hd);
		i = 1;
		error_flag = 0;
		while (1)
		{
			// str = fill_dishwasher(readline("heredoc>"), free, &bin);
			free(str);
			str = readline("heredoc>");
			if (!str)
			{
				close(pipefd[0]);
				dup_fd(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
				printf("minishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n", i, eof);
				error_flag = 1;
				break ;
			}
			if (str && !ft_strncmp(str, eof, ft_strlen(eof) - 1))
				break ;
			f_str = fill_dishwasher(ft_strjoin(f_str, str), free, &bin);
			f_str = fill_dishwasher(ft_strjoin(f_str, "\n"), free, &bin);
			i++;
		}
		if (!error_flag)
		{
			close(pipefd[0]);
			dup_fd(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
		}
		f_len = ft_strlen(f_str);
		write(STDOUT_FILENO, f_str, f_len);
		free(str);
		do_dishes(get_sink(&bin));
		exit(EXIT_SUCCESS);
	}
	close(pipefd[1]);
	char_buf = ft_calloc(sizeof(char), 2);
	if (!char_buf)
	{
		perror("malloc error");
		return (NULL);
	}
	f_str = NULL;
	waitpid(pid, NULL, 0);
	while (read(pipefd[0], char_buf, 1))
	{
		f_str = ft_strappend(f_str, char_buf);
		if (!f_str)
		{
			perror("malloc error");
			return (NULL);
		}
	}
	free(char_buf);
	close(pipefd[0]);
	return (f_str);
}

void	do_heredoc(t_doc *docs, t_sink *bin, t_list **env)
{
	char	*str;
	char	*heredoc;
	size_t	i;

	str = docs->str;
	i = 0;
	heredoc = fill_dishwasher(get_heredoc(bin, str), free, &bin);
	docs->str = heredoc;
	docs->type = format_here_doc(docs->str, env, lst_to_tab(env), &bin);
}
/* 
char	buff[1024];
ft_printf("%d\n", hd_fd);
i = read(hd_fd, buff, 1024);
buff[i] = '\0';
ft_printf("%s\n", buff); */

t_doc	polish_doc(t_list **lst, t_list *tmp, t_sink *bin, t_list **env)
{
	t_doc	current;

	current.type = find_type((char *)(*lst)->content);
	(*lst) = (*lst)->next->next;
	merge_tokens(*lst, bin);
	current.str = (*lst)->content;
	if (current.type == HEREDOC)
		do_heredoc(&current, bin, env);
	*lst = (*lst)->next;
	tmp->next = *lst;
	return (current);
}

t_doc	**create_docs(t_list *lst, t_sink *bin, t_list **env)
{
	t_doc	**docs;
	t_list	*tmp;
	int		i;

	i = lst_count_char(lst, '<') + lst_count_char(lst, '>');
	docs = new_plate(sizeof(t_doc *) * (i + 1), &bin);
	ft_bzero(docs, sizeof(t_doc *) * (i + 1));
	if (!docs)
		return (NULL);
	tmp = lst;
	i = 0;
	while (lst)
	{
		if (lst->content && (!ft_strncmp(lst->content, "<", 1) \
		|| !ft_strncmp(lst->content, ">", 1)))
		{
			docs[i] = new_plate(sizeof(t_doc), &bin);
			*docs[i++] = polish_doc(&lst, tmp, bin, env);
		}
		if (!lst)
			break ;
		tmp = lst;
		lst = lst->next;
	}
	return (docs);
}
