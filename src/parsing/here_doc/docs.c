/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   docs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:56:31 by secros            #+#    #+#             */
/*   Updated: 2025/04/11 16:24:14 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// add an error check if invalid redirection like strlen > 2 or bad redirection
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
	ft_printf("hell: syntax error near unexpected token `%c'\n", str[ft_strlen(str) - 1]);
	return (-1);
}

void	do_heredoc(t_doc **docs, char quote, t_sink *bin, t_list **env)
{
	char	*str;
	char	*heredoc;
	int		doc_fd;
	size_t	i;
	// t_sink	*bin_hd;

	str = (*docs)->str;
	i = 0;
	heredoc = handle_env(get_heredoc(bin, str), env, bin);
	(*docs)->str = heredoc;
	if (quote == 0)
	{
		(*docs)->type = format_here_doc((*docs)->str, env, lst_to_tab(env), &bin); //Can send null
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
	(*docs)->type = doc_fd;
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

int	polish_doc(t_list **lst, t_sink *bin, t_list **env, t_doc *document)
{
	char		i;

	i = 0;
	document->type = find_type((char *)(*lst)->content);
	if (document->type == -1)
		return (1);
	while (*lst && i < 2)
	{
		(*lst) = (*lst)->next;
		i++;
	}
	if (i != 2 || !(*lst))
	{
		ft_printf("hell: syntax error near unexpected token `\\n'\n");
		return (1);
	}
	i = heredoc_quotes(*lst);
	merge_tokens(*lst, bin);
	document->str = (*lst)->content;
	if (document->type == HEREDOC)
		do_heredoc(&document, i, bin, env);
	if (document->type <= 0)
		return (1);
	*lst = (*lst)->next;
	return (0);
}


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

void	relink_lst(t_list **prev, t_list **head, t_list **lst)
{
	if (prev)
		(*prev)->next = *lst;
	else
		*head = *lst;
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
			if (polish_doc(&lst, bin, env, &(*docs[i++])))
				return (NULL);
			relink_lst(&prev, head, &lst);
		}
		prev = lst;
		if (lst)
			lst = lst->next;
	}
	return (docs);
}
