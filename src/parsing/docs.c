/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   docs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:56:31 by secros            #+#    #+#             */
/*   Updated: 2025/03/28 12:31:23 by halnuma          ###   ########.fr       */
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

	f_str = NULL;
	str = NULL;
	while (1)
	{
		str = fill_dishwasher(readline("heredoc>"), free, &bin);
		if (str && !ft_strncmp(str, eof, ft_strlen(eof) - 1))
			break ;
		f_str = fill_dishwasher(ft_strjoin(f_str, str), free, &bin);
		f_str = fill_dishwasher(ft_strjoin(f_str, "\n"), free, &bin);

	}
	return (f_str);
}

void	do_heredoc(t_doc *docs, t_sink *bin, t_list **env)
{
	char	*str;
	char	*heredoc;
	size_t	i;
	
	str = docs->str;
	i = 0;
	heredoc = get_heredoc(bin, str);
	docs->str = heredoc;
	// ft_printf("%s\n", docs->str);
	docs->type = format_here_doc(docs->str, env, lst_to_tab(env));
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
