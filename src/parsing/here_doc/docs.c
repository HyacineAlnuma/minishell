/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   docs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:56:31 by secros            #+#    #+#             */
/*   Updated: 2025/04/18 17:35:13 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	skip_to_file(t_list **lst)
{
	int	i;

	i = 0;
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
	return (0);
}

int	polish_doc(t_list **lst, t_sink *bin, t_list **env, t_doc *document)
{
	char		i;

	i = 0;
	document->type = find_type((char *)(*lst)->content);
	if (document->type == -1)
		return (1);
	if (skip_to_file(lst))
		return (1);
	i = heredoc_quotes(*lst);
	if (merge_tokens(*lst, bin) == 2)
		return (1);
	document->str = (*lst)->content;
	if (document->type == HEREDOC)
		do_heredoc(document, i, bin, env);
	if (document->type < 0)
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
