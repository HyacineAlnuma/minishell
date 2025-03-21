/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:40:10 by secros            #+#    #+#             */
/*   Updated: 2025/03/21 13:45:21 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bt_malloc.h"

static void	addback_plate(t_sink **lst, t_sink *new)
{
	t_sink	*pt;

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

void	*fill_dishwasher(void *pt, void (*free_pt)(void *), t_sink **head)
{
	t_sink	*new_garb;

	if (!pt)
	{
		do_dishes(head);
		return (NULL);
	}
	new_garb = malloc(sizeof(t_sink));
	if (!new_garb)
	{
		do_dishes(head);
		free_pt(pt);
		return (NULL);
	}
	new_garb->content = pt;
	new_garb->f = free_pt;
	new_garb->next = NULL;
	addback_plate(head, new_garb);
	return (pt);
}

t_sink	**get_sink(t_sink **bin)
{
	static t_sink	*pt;
	t_sink			*tmp;

	if (bin && !pt)
		pt = *bin;
	if (bin && pt)
	{
		tmp = *bin;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = pt;
		pt = *bin;
	}
	return (&pt);
}

void	*new_plate(size_t size, t_sink **bin)
{
	void	*pt;
	t_sink	*new;

	pt = malloc(size);
	if (!pt)
	{
		if (bin)
			do_dishes(bin);
		return (NULL);
	}
	if (bin)
	{
		new = malloc(sizeof(t_sink));
		if (!new)
		{
			free(pt);
			do_dishes(bin);
			return (NULL);
		}
		new->content = pt;
		new->f = free;
		new->next = NULL;
		addback_plate(bin, new);
	}
	return (pt);
}
