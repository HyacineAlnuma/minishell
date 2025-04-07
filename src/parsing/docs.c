/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   docs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:56:31 by secros            #+#    #+#             */
/*   Updated: 2025/04/07 14:02:10 by secros           ###   ########.fr       */
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
		if (str && !ft_strncmp(str, eof, ft_strlen(eof)))
			break ;
		f_str = fill_dishwasher(ft_strjoin(f_str, str), free, &bin);
		f_str = fill_dishwasher(ft_strjoin(f_str, "\n"), free, &bin);
	}
	return (f_str);
}

void	do_heredoc(t_doc *docs, char quote, t_sink *bin, t_list **env)
{
	char	*str;
	char	*heredoc;
	int		doc_fd;
	size_t	i;
	
	str = docs->str;
	i = 0;
	heredoc = get_heredoc(bin, str);
	docs->str = heredoc;
	if (quote == 0)
	{
		docs->type = format_here_doc(docs->str, env, lst_to_tab(env), &bin); //Can send null
		return ;
	}
	doc_fd = open(HD_TEMP_FILE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	if (doc_fd == -1)
		return ;
	write(doc_fd, heredoc, ft_strlen(heredoc));
	close(doc_fd);
	doc_fd = open(HD_TEMP_FILE, O_RDONLY);
	if (doc_fd == -1)
		return ;
	docs->type = doc_fd;
}

int	heredoc_quotes(t_list *lst)
{
	while (lst && lst->content != NULL)
	{
		if (ft_strchr(lst->content, '"') || ft_strchr(lst->content, '\''))
			return (1);
		lst = lst->next;
	}
	return (0);
}

t_doc	polish_doc(t_list **lst, t_sink *bin, t_list **env)
{
	t_doc		document;
	char		i;

	i = 0;
	document.type = find_type((char *)(*lst)->content);
	while (lst && i < 2)
	{
		(*lst) = (*lst)->next;
		i++;
	}
	if (i != 2)
	{
		document.type = -2;
		return (document);
	}
	i = heredoc_quotes(*lst);
	merge_tokens(*lst, bin);
	document.str = (*lst)->content;
	if (document.type == HEREDOC)
		do_heredoc(&document, i, bin, env);
	*lst = (*lst)->next;
	return (document);
}


/* t_doc	polish_doc(t_list **lst, t_sink *bin, t_list **env)
{
	t_doc	current;

	current.type = find_type((char *)(*lst)->content);
	ft_printf("\n--%d--\n", current.type);
	(*lst) = (*lst)->next->next;
	merge_tokens(*lst, bin);
	current.str = (*lst)->content;
	ft_printf("\n--%s--\n", current.str);
	if (current.type == HEREDOC)
		do_heredoc(&current, bin, env);
	*lst = (*lst)->next;
	return (current);
} */

void	*alloc_docs(t_list *lst, t_sink *bin)
{
	t_doc	**docs;
	size_t	i;
	size_t	j;

	j = 0;
	i = lst_count_char(lst, '<') + lst_count_char(lst, '>');
	docs = new_plate(sizeof(t_doc *) * (i + 1), &bin);
	if (!docs)
		return (NULL);
	ft_bzero(docs, sizeof(t_doc *) * (i + 1));
	while (j < i)
	{
		docs[j] = new_plate(sizeof(t_doc), &bin);
		if (!docs)
			return (NULL);
		j++;
	}
	return (docs);
}

t_doc	**create_docs(t_list **head, t_list *lst, t_sink *bin, t_list **env)
{
	t_doc	**docs;
	t_list	*prev;
	size_t	i;

	i = 0;
	docs = alloc_docs(*head, bin);
	if (!docs)
		return (NULL);
	prev = NULL;
	while (lst)
	{
		if (lst->content && (!ft_strncmp(lst->content, "<", 1) \
		|| !ft_strncmp(lst->content, ">", 1)))
		{
			*docs[i++] = polish_doc(&lst, bin, env);
			if (prev)
				prev->next = lst;
			else
				*head = lst;
		}
		prev = lst;
		if (lst)
			lst = lst->next;
	}
	return (docs);
}
