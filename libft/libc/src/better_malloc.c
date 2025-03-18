/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   better_malloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:09:12 by secros            #+#    #+#             */
/*   Updated: 2025/03/18 13:10:14 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_mallocs.h"
#include <stdlib.h>

void clear_garbage(t_garb **head)
{
	t_garb	*tmp;
	t_garb	*tmp_next;

	tmp = *head;
	while (tmp)
	{
		tmp_next = tmp->next;
		if(tmp->f)
			tmp->f(tmp->content);
		free(tmp);
		tmp = tmp_next;
	}
	*head = NULL;
}

void	ft_free(void *pt, t_garb **head)
{
	t_garb	*prev;
	t_garb	*tmp;

	if (!head || !*head)
		return (free(pt));
	tmp = *head;
	prev = NULL;
	while (tmp)
	{
		if (tmp->content == pt)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*head = tmp->next;
			if (tmp->f)
				tmp->f(tmp->content);
			return (free(tmp));
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

static void	addback_garb(t_garb **lst, t_garb *new)
{
	t_garb	*pt;

	if (!*lst)
		*lst = new;
	else
	{
		pt = (*lst);
		while (pt->next)
			pt = pt->next;
		pt->next = new;
	}
}

void	*add_garbage(void *pt, void (*free_pt)(void *), t_garb **head)
{
	t_garb	*new_garb;

	if (!pt)
	{
		clear_garbage(head);
		return (NULL);
	}
	new_garb = malloc(sizeof(t_garb));
	if (!new_garb)
	{
		clear_garbage(head);
		free_pt(pt);
		return (NULL);
	}
	new_garb->content = pt;
	new_garb->f = free_pt;
	new_garb->next = NULL;
	addback_garb(head, new_garb);
	return(pt);
}

void	*ft_malloc(size_t size, t_garb **bin)
{
	void	*pt;
	t_garb	*new;

	pt = malloc(size);
	if (!pt)
	{
		if (bin)
			clear_garbage(bin);
		return (NULL);
	}
	if (bin)
	{
		new = malloc(sizeof(t_garb));
		if (!new)
		{
			free(pt);
			clear_garbage(bin);
			return (NULL);
		}
		new->content = pt;
		new->f = free;
		new->next = NULL;
		addback_garb(bin, new);
	}
	return (pt);
}
