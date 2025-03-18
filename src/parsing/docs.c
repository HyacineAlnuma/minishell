/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   docs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:56:31 by secros            #+#    #+#             */
/*   Updated: 2025/03/18 14:24:24 by secros           ###   ########.fr       */
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

t_doc	polish_doc(t_list **lst, t_list *tmp, t_garb *bin)
{
	t_doc	current;
	t_list	*to_free;

	current.type = find_type((char *)(*lst)->content);
	(*lst) = (*lst)->next->next;
	merge_tokens(*lst, bin);
	current.str = (*lst)->content;
	to_free = *lst;
	*lst = (*lst)->next;
	tmp->next = *lst;
	return (current);
}

t_doc	**create_docs(t_list *lst, t_garb *bin)
{
	t_doc	**docs;
	t_list	*tmp;
	int		i;

	i = lst_count_char(lst, '<') + lst_count_char(lst, '>');
	docs = ft_malloc(sizeof(t_doc *) * (i + 1), &bin);
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
			docs[i] = ft_malloc(sizeof(t_doc), &bin);
			*docs[i++] = polish_doc(&lst, tmp, bin);
		}
		if (!lst)
			break ;
		tmp = lst;
		lst = lst->next;
	}
	return (docs);
}
